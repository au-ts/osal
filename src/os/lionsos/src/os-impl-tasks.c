#include <unistd.h>
#include <stdio.h>

#include "libmicrokitco.h"
#include "os-impl-loader.h"
#include "os-shared-globaldefs.h"
#include "os-shared-task.h"
#include "os-shared-module.h"
#include "os-shared-idmap.h"
#include "os-impl-tasks.h"
#include "osapi-error.h"
#include "osapi-task.h"

OS_impl_task_internal_record_t OS_impl_task_table[OS_MAX_TASKS];

static void OS_Cothread_Entry_Point(void)
{
    osal_id_t id = (osal_id_t) (uintptr_t) microkit_cothread_my_arg();
    OS_TaskEntryPoint(id);
}

int32 OS_LionsOS_TaskAPI_Impl_Init(void)
{
    memset(OS_impl_task_table, 0, sizeof(OS_impl_task_table));
    return OS_SUCCESS;
}

int32 OS_TaskCreate_Impl(const OS_object_token_t *token, uint32 flags)
{
    OS_impl_task_internal_record_t *impl = OS_OBJECT_TABLE_GET(OS_impl_task_table, *token);
    OS_task_internal_record_t *task = OS_OBJECT_TABLE_GET(OS_task_table, *token);

    int32 return_code = OS_ERROR;

    /* Tasks prefixed with "PD_" are assumed to belong to a separate protection domain
     * If that's the case, OS_TaskCreate will just start the PD at its defined entrypoint */
    if (strncmp(task->task_name, "PD_", 3) == 0)
    {
        for (int i = 0; i < OS_MAX_MODULES; i++)
        {
            if (strcmp(task->task_name, OS_module_table[i].module_name) == 0)
            {
                uintptr_t entrypoint = elf_getEntryPoint(&OS_impl_module_table[i].elf);
                microkit_pd_restart(OS_impl_module_table[i].pd, entrypoint);

                impl->pd = OS_impl_module_table[i].pd;
                impl->osal_id = OS_ObjectIdFromToken(token);

                return_code = OS_SUCCESS;
                break;
            }
        }
    }

    /* Assume all other tasks are cothreads inside cFS */
    else if (task->stack_size > COTHREAD_STACK_SIZE)
    {
        return_code = OS_ERR_OPERATION_NOT_SUPPORTED;
    }
    else
    {
        /* The coroutine does not start executing until we yield
         * cFS needs to do further setup and bookkeeping first, so just create the cothread
         * and leave it suspended for now */
        impl->osal_id = OS_ObjectIdFromToken(token);
        impl->cothread = microkit_cothread_spawn(OS_Cothread_Entry_Point, (void*) (uintptr_t) impl->osal_id);
        return_code = impl->cothread != LIBMICROKITCO_NULL_HANDLE ? OS_SUCCESS : OS_ERR_NO_FREE_IDS;
    }

    return return_code;
}

int32 OS_TaskDelete_Impl(const OS_object_token_t *token)
{
    OS_impl_task_internal_record_t *impl = OS_OBJECT_TABLE_GET(OS_impl_task_table, *token);
    OS_task_internal_record_t *task = OS_OBJECT_TABLE_GET(OS_task_table, *token);

    if (strncmp(task->task_name, "PD_", 3) == 0)
    {
        microkit_pd_stop(impl->pd);
    }
    else
    {
        OS_DEBUG("Ignoring task deletion request for cothread with OSAL task ID %u\n", impl->osal_id);
    }

    return OS_SUCCESS;
}

int32 OS_TaskDetach_Impl(const OS_object_token_t *token)
{
    /* Detaching a task means asking the operating system
     * to automatically clean up its resources once the task
     * finishes, without waiting for a task to join the finished task */
    return OS_SUCCESS;
}

void OS_TaskExit_Impl()
{
    OS_DEBUG("Task exit requested\n");
    microkit_cothread_ref_t self = microkit_cothread_my_handle();
    microkit_cothread_destroy(self);
}

int32 OS_TaskDelay_Impl(uint32 millisecond)
{
    int err = usleep(millisecond);
    return err == 0 ? OS_SUCCESS : OS_ERROR;
}

int32 OS_TaskSetPriority_Impl(const OS_object_token_t *token, osal_priority_t new_priority)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_TaskMatch_Impl(const OS_object_token_t *token)
{
    microkit_cothread_ref_t self = microkit_cothread_my_handle();
    OS_impl_task_internal_record_t *impl = OS_OBJECT_TABLE_GET(OS_impl_task_table, *token);
    return self == impl->cothread ? OS_SUCCESS : OS_ERROR;
}

int32 OS_TaskRegister_Impl(osal_id_t global_task_id)
{
    /* Nothing to notify */
    return OS_SUCCESS;
}

osal_id_t OS_TaskGetId_Impl(void)
{
    microkit_cothread_ref_t self = microkit_cothread_my_handle();

    for (int i = 0; i < OS_MAX_TASKS; i++)
    {
        if (OS_impl_task_table[i].cothread == self)
        {
            return OS_impl_task_table[i].osal_id;
        }
    }

    return OS_OBJECT_ID_UNDEFINED;
}

int32 OS_TaskGetInfo_Impl(const OS_object_token_t *token, OS_task_prop_t *task_prop)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_TaskValidateSystemData_Impl(const void *sysdata, size_t sysdata_size)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

bool OS_TaskIdMatchSystemData_Impl(void *ref, const OS_object_token_t *token, const OS_common_record_t *obj)
{
    return false;
}

int32 OS_Posix_InternalTaskCreate_Impl(void *pthr, osal_priority_t priority, osal_stackptr_t stackptr,
                                       size_t stacksz, void * entry, void *entry_arg) {
    /* Needed in os-impl-console.c */
    return OS_SUCCESS;
}
