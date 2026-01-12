#include "common_types.h"
#include "os-shared-filesys.h"
#include "osapi-error.h"

int32 OS_LionsOS_FileSysAPI_Impl_Init(void)
{
    return OS_SUCCESS;
}

int32 OS_FileSysStartVolume_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_FileSysStopVolume_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_FileSysFormatVolume_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_FileSysMountVolume_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_FileSysUnmountVolume_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_FileSysStatVolume_Impl(const OS_object_token_t *token, OS_statvfs_t *result)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_FileSysCheckVolume_Impl(const OS_object_token_t *token, bool repair)
{
    return OS_ERR_NOT_IMPLEMENTED;
}
