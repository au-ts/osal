#include <lions/posix/posix.h>
#include <sddf/timer/config.h>
#include <sddf/timer/client.h>
#include <sddf/timer/protocol.h>
#include <lions/fs/config.h>
#include <lions/fs/helpers.h>
#include <lions/fs/protocol.h>
#include <sddf/network/config.h>
#include <sddf/network/queue.h>
#include <sddf/network/lib_sddf_lwip.h>
#include <sddf/serial/config.h>
#include <sddf/serial/queue.h>
#include <sel4/syscalls.h>
#include <libmicrokitco.h>
#include <microkit.h>
#include <sys/auxv.h>
#include <stdarg.h>

#include "bsp-impl.h"

extern libc_socket_config_t socket_config;

__attribute__((__section__(".timer_client_config"))) timer_client_config_t timer_config;
__attribute__((__section__(".serial_client_config"))) serial_client_config_t serial_config;
__attribute__((__section__(".net_client_config"))) net_client_config_t net_config;
__attribute__((__section__(".lib_sddf_lwip_config"))) lib_sddf_lwip_config_t lib_sddf_lwip_config;
__attribute__((__section__(".fs_client_config"))) fs_client_config_t fs_config;

fs_queue_t *fs_command_queue;
fs_queue_t *fs_completion_queue;
char *fs_share;

net_queue_handle_t net_tx_handle;
net_queue_handle_t net_rx_handle;

serial_queue_handle_t serial_tx_queue_handle;
serial_queue_handle_t serial_rx_queue_handle;

static bool fs_enabled;
static bool serial_rx_enabled;
static bool net_enabled;
static bool net_dhcp_acked;

static char morecore_area[0x100000 /* 1 MiB */];

static void OS_BSP_DHCP_Success(char *ip)
{
    BSP_DEBUG("DHCP negotiation finished, IP address for %s is %s\n", microkit_name, ip);
    net_dhcp_acked = true;
}

static long sys_exit(va_list ap)
{
    BSP_DEBUG("cFS will now halt\n");

    while (true)
    {
        seL4_DebugHalt();
    }

    return -1;
}

static long sys_exit_group(va_list ap)
{
    return sys_exit(ap);
}

static long sys_pselect6(va_list ap)
{
    int n = va_arg(ap, int);
    fd_set *rfds = va_arg(ap, void*);
    fd_set *wfds = va_arg(ap, void*);
    fd_set *efds = va_arg(ap, void*);
    struct timeval *tv = va_arg(ap, void*);
    sigset_t *sm = va_arg(ap, void*);

    (void) tv;
    (void) sm;

    long count = 0;

    for (int i = 0; i < n; i++)
    {
        count += (rfds != NULL && FD_ISSET(i, rfds));
        count += (wfds != NULL && FD_ISSET(i, wfds));
        count += (efds != NULL && FD_ISSET(i, efds));
    }

    /* pselect6(2) is expected to return the number of ready FDs */
    return count;
}

void OS_BSP_Initialize(void)
{
    assert(serial_config_check_magic(&serial_config));
    assert(timer_config_check_magic(&timer_config));
    fs_enabled = fs_config_check_magic(&fs_config);
    net_enabled = net_config_check_magic(&net_config);
    net_dhcp_acked = false;
    serial_rx_enabled = (serial_config.rx.queue.vaddr != NULL);

    serial_queue_init(
        &serial_tx_queue_handle,
        serial_config.tx.queue.vaddr,
        serial_config.tx.data.size,
        serial_config.tx.data.vaddr
    );

    // LionsOS does not define the exit or exit_group syscalls
    // so we need to provide stub implementations or the system
    // will go into an infinite loop
    libc_define_syscall(__NR_exit, sys_exit);
    libc_define_syscall(__NR_exit_group, sys_exit_group);
    libc_define_syscall(__NR_pselect6, sys_pselect6);
    libc_init(&socket_config, morecore_area, sizeof(morecore_area));

    BSP_DEBUG("fs_enabled: %s\n", fs_enabled ? "true" : "false");
    BSP_DEBUG("net_enabled: %s\n", net_enabled ? "true" : "false");
    BSP_DEBUG("serial_rx_enabled: %s\n", serial_rx_enabled ? "true" : "false");

    if (net_enabled)
    {
        net_queue_init(
            &net_rx_handle,
            net_config.rx.free_queue.vaddr,
            net_config.rx.active_queue.vaddr,
            net_config.rx.num_buffers
        );

        net_queue_init(
            &net_tx_handle,
            net_config.tx.free_queue.vaddr,
            net_config.tx.active_queue.vaddr,
            net_config.tx.num_buffers
        );

        net_buffers_init(&net_tx_handle, 0);

        sddf_lwip_init(
            &lib_sddf_lwip_config,
            &net_config,
            &timer_config,
            net_rx_handle,
            net_tx_handle,
            NULL,
            printf,
            OS_BSP_DHCP_Success,
            NULL,
            NULL,
            NULL
        );

        sddf_lwip_maybe_notify();
        sddf_timer_set_timeout(timer_config.driver_id, 100 * NS_IN_MS);

        BSP_DEBUG("Waiting for DHCP...\n");

        while (!net_dhcp_acked)
        {
            microkit_cothread_yield();
        }
    }

    if (fs_enabled)
    {
        fs_set_blocking_wait(microkit_cothread_wait_on_channel);
        fs_command_queue = fs_config.server.command_queue.vaddr;
        fs_completion_queue = fs_config.server.completion_queue.vaddr;
        fs_share = fs_config.server.share.vaddr;

        fs_cmpl_t completion;
        int err = fs_command_blocking(&completion, (fs_cmd_t) {.type = FS_CMD_INITIALISE});

        if (!err && completion.status == FS_STATUS_SUCCESS)
        {
            BSP_DEBUG("Filesystem mounted\n");
        }
        else
        {
            BSP_DEBUG(
                "Could not mount filesystem, error %d with status %s\n",
                err,
                fs_status_to_str(completion.status)
            );
        }
    }

    if (serial_rx_enabled)
    {
        serial_queue_init(
            &serial_rx_queue_handle,
            serial_config.rx.queue.vaddr,
            serial_config.rx.data.size,
            serial_config.rx.data.vaddr
        );
    }

    OS_Application_Startup();
    OS_Application_Run();
}

void OS_BSP_Handle_Notifications(microkit_channel channel)
{
    if (channel == net_config.rx.id)
    {
        BSP_DEBUG("Received notification from network RX\n");

        sddf_lwip_process_rx();

        if (net_enabled && net_dhcp_acked)
        {
            microkit_cothread_yield();
        }
    }
    else if (channel == net_config.tx.id)
    {
        BSP_DEBUG("Received notification from network TX?\n");
    }
    else if (channel == timer_config.driver_id)
    {
        if (net_enabled) {
            sddf_lwip_process_rx();
            sddf_lwip_process_timeout();
            sddf_timer_set_timeout(timer_config.driver_id, 100 * NS_IN_MS);
        }
    }
    else if (channel == serial_config.rx.id)
    {
        BSP_DEBUG("Received notification from serial RX?\n");
    }
    else if (channel == serial_config.tx.id)
    {
        BSP_DEBUG("Received notification from serial TX?\n");
    }
    else if (channel == fs_config.server.id)
    {
        fs_process_completions(NULL);
    }

    if (net_enabled) {
        sddf_lwip_maybe_notify();
    }
}
