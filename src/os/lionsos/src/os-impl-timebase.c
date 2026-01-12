#include "common_types.h"
#include "os-shared-timebase.h"
#include "os-shared-common.h"
#include "osapi-error.h"

int32 OS_LionsOS_TimeBaseAPI_Impl_Init(void)
{
    /* tick every 100ms */
    OS_SharedGlobalVars.TicksPerSecond = 10;
    OS_SharedGlobalVars.MicroSecPerTick = 100000;
    return OS_SUCCESS;
}

void OS_TimeBaseLock_Impl(const OS_object_token_t *token)
{
    /* return OS_ERR_NOT_IMPLEMENTED; */
}

void OS_TimeBaseUnlock_Impl(const OS_object_token_t *token)
{
    /* return OS_ERR_NOT_IMPLEMENTED; */
}

int32 OS_TimeBaseCreate_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_TimeBaseSet_Impl(const OS_object_token_t *token, uint32 start_time, uint32 interval_time)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_TimeBaseDelete_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_TimeBaseGetInfo_Impl(const OS_object_token_t *token, OS_timebase_prop_t *timer_prop)
{
    return OS_ERR_NOT_IMPLEMENTED;
}
