#include "common_types.h"
#include "os-shared-mutex.h"
#include "osapi-error.h"

int32 OS_LionsOS_MutexAPI_Impl_Init(void)
{
    return OS_SUCCESS;
}

int32 OS_MutSemCreate_Impl(const OS_object_token_t *token, uint32 options)
{
    return OS_SUCCESS;
}

int32 OS_MutSemDelete_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_MutSemGive_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_MutSemTake_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_MutSemGetInfo_Impl(const OS_object_token_t *token, OS_mut_sem_prop_t *mut_prop)
{
    return OS_ERR_NOT_IMPLEMENTED;
}
