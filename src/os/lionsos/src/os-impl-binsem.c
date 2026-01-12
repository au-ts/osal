#include "common_types.h"
#include "os-shared-binsem.h"

int32 OS_LionsOS_BinSemAPI_Impl_Init(void)
{
    return OS_SUCCESS;
}

int32 OS_BinSemCreate_Impl(const OS_object_token_t *token, uint32 sem_initial_value, uint32 options)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_BinSemDelete_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_BinSemGive_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_BinSemFlush_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_BinSemTake_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_BinSemTimedWait_Impl(const OS_object_token_t *token, uint32 msecs)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_BinSemGetInfo_Impl(const OS_object_token_t *token, OS_bin_sem_prop_t *bin_prop)
{
    return OS_ERR_NOT_IMPLEMENTED;
}
