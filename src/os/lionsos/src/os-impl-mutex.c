#include <libmicrokitco.h>

#include "os-shared-mutex.h"
#include "os-shared-idmap.h"
#include "os-impl-mutex.h"
#include "osapi-error.h"
#include "os-lionsos.h"

OS_impl_mutex_internal_record_t OS_impl_mutex_table[OS_MAX_TASKS];

int32 OS_LionsOS_MutexAPI_Impl_Init(void)
{
    for (int i = 0; i < OS_MAX_TASKS; i++)
    {
        // Signal all semaphores immediately so the initial take call succeeds
        microkit_cothread_semaphore_init(&OS_impl_mutex_table[i].semaphore);
        microkit_cothread_semaphore_signal(&OS_impl_mutex_table[i].semaphore);
    }

    return OS_SUCCESS;
}

int32 OS_MutSemCreate_Impl(const OS_object_token_t *token, uint32 options)
{
    // Already initialised
    return OS_SUCCESS;
}

int32 OS_MutSemDelete_Impl(const OS_object_token_t *token)
{
    OS_impl_mutex_internal_record_t *impl = OS_OBJECT_TABLE_GET(OS_impl_mutex_table, *token);
    bool empty = microkit_cothread_semaphore_is_queue_empty(&impl->semaphore);

    if (!empty)
    {
        OS_DEBUG("Error: Mutex destruction request received when waiters are still present\n");
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;
}

int32 OS_MutSemGive_Impl(const OS_object_token_t *token)
{
    OS_impl_mutex_internal_record_t *impl = OS_OBJECT_TABLE_GET(OS_impl_mutex_table, *token);
    microkit_cothread_semaphore_signal(&impl->semaphore);
    return OS_SUCCESS;
}

int32 OS_MutSemTake_Impl(const OS_object_token_t *token)
{
    OS_impl_mutex_internal_record_t *impl = OS_OBJECT_TABLE_GET(OS_impl_mutex_table, *token);
    microkit_cothread_semaphore_wait(&impl->semaphore);
    return OS_SUCCESS;
}

int32 OS_MutSemGetInfo_Impl(const OS_object_token_t *token, OS_mut_sem_prop_t *mut_prop)
{
    return OS_SUCCESS;
}
