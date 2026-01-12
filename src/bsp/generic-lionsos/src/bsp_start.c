#include <sel4/sel4.h>
#include <microkit.h>
#include <libmicrokitco.h>

#include "bsp_libc.h"
#include "bsp-impl.h"

// 64k of stack space
#define COTHREAD_STACK_SIZE 0x10000

static co_control_t co_control;
static char co_stack[COTHREAD_STACK_SIZE];

void init(void)
{
    microkit_cothread_init(
        &co_control,
        COTHREAD_STACK_SIZE,
        (stack_ptrs_arg_array_t) { (uintptr_t) co_stack }
    );

    microkit_cothread_ref_t cont = microkit_cothread_spawn(OS_BSP_Initialize, NULL);

    if (cont == LIBMICROKITCO_NULL_HANDLE)
    {
        /* Can't use any libc functionality to panic, so crash and burn */
        microkit_dbg_puts("Cannot initialise BSP cothread");
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
