#include <pthread.h>

#include "common_types.h"
#include "os-shared-globaldefs.h"
#include "osapi-error.h"
#include "osapi-idmap.h"
#include "os-impl-idmap.h"

OS_impl_objtype_lock_t *const OS_impl_objtype_lock_table[OS_OBJECT_TYPE_USER] = {
    [OS_OBJECT_TYPE_UNDEFINED] = NULL,
    [OS_OBJECT_TYPE_OS_TASK] = &((OS_impl_objtype_lock_t) {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER}),
    [OS_OBJECT_TYPE_OS_QUEUE] = &((OS_impl_objtype_lock_t) {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER}),
    [OS_OBJECT_TYPE_OS_COUNTSEM] = &((OS_impl_objtype_lock_t) {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER}),
    [OS_OBJECT_TYPE_OS_BINSEM] = &((OS_impl_objtype_lock_t) {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER}),
    [OS_OBJECT_TYPE_OS_MUTEX] = &((OS_impl_objtype_lock_t) {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER}),
    [OS_OBJECT_TYPE_OS_STREAM] = &((OS_impl_objtype_lock_t) {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER}),
    [OS_OBJECT_TYPE_OS_DIR] = &((OS_impl_objtype_lock_t) {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER}),
    [OS_OBJECT_TYPE_OS_TIMEBASE] = &((OS_impl_objtype_lock_t) {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER}),
    [OS_OBJECT_TYPE_OS_TIMECB] = &((OS_impl_objtype_lock_t) {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER}),
    [OS_OBJECT_TYPE_OS_MODULE] = &((OS_impl_objtype_lock_t) {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER}),
    [OS_OBJECT_TYPE_OS_FILESYS] = &((OS_impl_objtype_lock_t) {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER}),
    [OS_OBJECT_TYPE_OS_CONSOLE] = &((OS_impl_objtype_lock_t) {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER})
};

void OS_Lock_Global_Impl(osal_objtype_t idtype)
{
    OS_impl_objtype_lock_t *impl;
    int ret;

    impl = OS_impl_objtype_lock_table[idtype];

    if (impl != NULL)
    {
        ret = pthread_mutex_lock(&impl->mutex);

        if (ret != 0)
        {
            OS_DEBUG("pthread_mutex_lock(&impl->mutex): %s", strerror(ret));
        }
    }
}

void OS_Unlock_Global_Impl(osal_objtype_t idtype)
{
    OS_impl_objtype_lock_t *impl;
    int ret;

    impl = OS_impl_objtype_lock_table[idtype];

    if (impl != NULL)
    {
        /* Notify any waiting threads that the state _may_ have changed */
        ret = pthread_cond_broadcast(&impl->cond);
        if (ret != 0)
        {
            OS_DEBUG("pthread_cond_broadcast(&impl->cond): %s", strerror(ret));
            /* unexpected but keep going (not critical) */
        }

        ret = pthread_mutex_unlock(&impl->mutex);
        if (ret != 0)
        {
            OS_DEBUG("pthread_mutex_unlock(&impl->mutex): %s", strerror(ret));
        }
    }
}

void OS_WaitForStateChange_Impl(osal_objtype_t objtype, uint32 attempts)
{
    OS_DEBUG("Waiting for state change on object %x (%u attempts)", objtype, attempts);
}

int32 OS_LionsOS_TableMutex_Init(osal_objtype_t idtype)
{
    return OS_SUCCESS;
}
