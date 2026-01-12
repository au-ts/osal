#include "common_types.h"
#include "os-shared-countsem.h"
#include "osapi-error.h"

int32 OS_LionsOS_CountSemAPI_Impl_Init(void)
{
    return OS_SUCCESS;
}

int32 OS_CountSemCreate_Impl(const OS_object_token_t *token, uint32 sem_initial_value, uint32 options)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_CountSemDelete_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_CountSemGive_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_CountSemTake_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_CountSemTimedWait_Impl(const OS_object_token_t *token, uint32 msecs)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_CountSemGetInfo_Impl(const OS_object_token_t *token, OS_count_sem_prop_t *count_prop)
{
    return OS_ERR_NOT_IMPLEMENTED;
}
