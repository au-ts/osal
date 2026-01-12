#include "common_types.h"
#include "os-lionsos.h"
#include "os-shared-queue.h"
#include "osapi-error.h"

int32 OS_LionsOS_QueueAPI_Impl_Init(void)
{
    return OS_SUCCESS;
}

int32 OS_QueueCreate_Impl(const OS_object_token_t *token, uint32 flags)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_QueueDelete_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_QueueGet_Impl(const OS_object_token_t *token, void *data, size_t size, size_t *size_copied, int32 timeout)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_QueuePut_Impl(const OS_object_token_t *token, const void *data, size_t size, uint32 flags)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_QueueGetInfo_Impl(const OS_object_token_t *token, OS_queue_prop_t *queue_prop)
{
    return OS_ERR_NOT_IMPLEMENTED;
}
