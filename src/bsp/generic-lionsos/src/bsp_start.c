#include <sel4/sel4.h>
#include <microkit.h>
#include <libmicrokitco.h>

#include "bsp_libc.h"
#include "bsp-impl.h"

extern timer_client_config_t timer_config;

static co_control_t co_control;

__attribute__((aligned(0x1000))) static char co_stacks[LIBMICROKITCO_MAX_COTHREADS - 1][COTHREAD_STACK_SIZE];

void init(void)
{
    stack_ptrs_arg_array_t stacks;

    for (int i = 0; i < LIBMICROKITCO_MAX_COTHREADS - 1; i++)
    {
        stacks[i] = (uintptr_t) &co_stacks[i];
    }

    microkit_cothread_init(&co_control, COTHREAD_STACK_SIZE, stacks);
    microkit_cothread_ref_t cont = microkit_cothread_spawn(OS_BSP_Initialize, NULL);

    if (cont == LIBMICROKITCO_NULL_HANDLE)
    {
        /* Can't use any libc functionality to panic, so crash and burn */
        microkit_dbg_puts("Cannot initialise BSP cothread\n");
        seL4_DebugHalt();
    }

    microkit_cothread_yield();
}

void notified(microkit_channel channel)
{
    OS_BSP_Handle_Notifications(channel);
    microkit_cothread_recv_ntfn(channel);
}

void OS_BSP_Lock_Impl(void)
{
    // Intentionally a no-op
    // BSP_DEBUG("BSP locking is a no-op\n");
}

void OS_BSP_Unlock_Impl(void)
{
    // Intentionally a no-op
    // BSP_DEBUG("BSP unlocking is a no-op\n");
}
