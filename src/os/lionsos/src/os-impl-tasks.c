#include <stdio.h>

#include "os-shared-globaldefs.h"
#include "os-shared-task.h"
#include "os-impl-tasks.h"
#include "os-lionsos.h"
#include "osapi-error.h"

OS_impl_task_internal_record_t OS_impl_task_table[OS_MAX_TASKS];

int32 OS_LionsOS_TaskAPI_Impl_Init(void)
{
    memset(OS_impl_task_table, 0, sizeof(OS_impl_task_table));
    return OS_SUCCESS;
}

int32 OS_TaskCreate_Impl(const OS_object_token_t *token, uint32 flags)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_TaskDelete_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_TaskDetach_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

void OS_TaskExit_Impl()
{
    OS_DEBUG("Task exit requested\n");
}

int32 OS_TaskDelay_Impl(uint32 milli_second)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_TaskSetPriority_Impl(const OS_object_token_t *token, osal_priority_t new_priority)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_TaskMatch_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_TaskRegister_Impl(osal_id_t global_task_id)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

osal_id_t OS_TaskGetId_Impl(void)
{
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
