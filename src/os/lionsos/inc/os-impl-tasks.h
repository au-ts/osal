#ifndef OS_IMPL_TASKS_H
#define OS_IMPL_TASKS_H

#include "libmicrokitco.h"
#include "osapi-task.h"
#include "osconfig.h"

/*tasks */
typedef struct
{
    osal_id_t osal_id;

    union {
        microkit_cothread_ref_t cothread;
        int pd;
    };
} OS_impl_task_internal_record_t;

/* Tables where the OS object information is stored */
extern OS_impl_task_internal_record_t OS_impl_task_table[OS_MAX_TASKS];

int32 OS_Posix_InternalTaskCreate_Impl(void *pthr, osal_priority_t priority, osal_stackptr_t stackptr, size_t stacksz, void * entry, void *entry_arg);

#endif /* OS_IMPL_TASKS_H */
