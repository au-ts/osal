#include <sddf/timer/client.h>
#include <sddf/timer/config.h>
#include <sddf/timer/protocol.h>

#include "libmicrokitco.h"
#include "os-shared-idmap.h"
#include "os-shared-binsem.h"
#include "os-impl-binsem.h"
#include "os-lionsos.h"

/* Tables where the OS object information is stored */
OS_impl_binsem_internal_record_t OS_impl_bin_sem_table[OS_MAX_BIN_SEMAPHORES];

/* Access to the sDDF timer channel */
extern timer_client_config_t timer_config;

void OS_BinSemTimedWait_Callback(void)
{
    uint64_t now = sddf_timer_time_now(timer_config.driver_id);

    for (int i = 0; i < OS_MAX_BIN_SEMAPHORES; i++)
    {
        if (now > OS_impl_bin_sem_table[i].expiry && !OS_impl_bin_sem_table->expired)
        {
            OS_impl_bin_sem_table[i].expired = true;
            microkit_cothread_semaphore_signal(&OS_impl_bin_sem_table[i].semaphore);
        }
    }
}

int32 OS_LionsOS_BinSemAPI_Impl_Init(void)
{
    for (int i = 0; i < OS_MAX_BIN_SEMAPHORES; i++)
    {
        microkit_cothread_semaphore_init(&OS_impl_bin_sem_table[i].semaphore);
        OS_impl_bin_sem_table[i].expiry = (uint64_t) 0xFFFFFFFFFFFFFFFF;
        OS_impl_bin_sem_table[i].expired = false;
    }

    return OS_SUCCESS;
}

int32 OS_BinSemCreate_Impl(const OS_object_token_t *token, uint32 sem_initial_value, uint32 options)
{
    // Nothing to do
    return OS_SUCCESS;
}

int32 OS_BinSemDelete_Impl(const OS_object_token_t *token)
{
    OS_impl_binsem_internal_record_t *impl = OS_OBJECT_TABLE_GET(OS_impl_bin_sem_table, *token);
    bool empty = microkit_cothread_semaphore_is_queue_empty(&impl->semaphore);

    if (!empty)
    {
        OS_DEBUG("Error: Binary semaphore destruction request received when waiters are still present\n");
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;
}

int32 OS_BinSemGive_Impl(const OS_object_token_t *token)
{
    OS_impl_binsem_internal_record_t *impl = OS_OBJECT_TABLE_GET(OS_impl_bin_sem_table, *token);
    microkit_cothread_semaphore_signal(&impl->semaphore);
    return OS_SUCCESS;
}

int32 OS_BinSemFlush_Impl(const OS_object_token_t *token)
{
    OS_impl_binsem_internal_record_t *impl = OS_OBJECT_TABLE_GET(OS_impl_bin_sem_table, *token);

    // Mark this as unexpired so all non-timeout waits succeed
    impl->expired = false;

    // Everyone needs to be woken up, so keep doing this until the queue is empty
    while (!microkit_cothread_semaphore_is_queue_empty(&impl->semaphore))
    {
        microkit_cothread_semaphore_signal(&impl->semaphore);
    }

    return OS_SUCCESS;
}

int32 OS_BinSemTake_Impl(const OS_object_token_t *token)
{
    OS_impl_binsem_internal_record_t *impl = OS_OBJECT_TABLE_GET(OS_impl_bin_sem_table, *token);

    do
    {
        microkit_cothread_semaphore_wait(&impl->semaphore);
    } while (impl->expired);

    return OS_SUCCESS;
}

int32 OS_BinSemTimedWait_Impl(const OS_object_token_t *token, uint32 msecs)
{
    OS_impl_binsem_internal_record_t *impl = OS_OBJECT_TABLE_GET(OS_impl_bin_sem_table, *token);
    uint64_t now = sddf_timer_time_now(timer_config.driver_id);

    impl->expiry = now + msecs * NS_IN_MS;
    sddf_timer_set_timeout(timer_config.driver_id, msecs * NS_IN_MS);

    // Suspend this cothread for now
    microkit_cothread_semaphore_wait(&impl->semaphore);

    // If we woke up and a timeout occurred
    if (impl->expired)
    {
        impl->expired = false;
        return OS_SEM_TIMEOUT;
    }

    return OS_SUCCESS;
}

int32 OS_BinSemGetInfo_Impl(const OS_object_token_t *token, OS_bin_sem_prop_t *props)
{
    // libmicrokitco has no API for obtaining the current value of a semaphore
    return OS_ERR_NOT_IMPLEMENTED;
}
