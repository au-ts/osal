
#include "common_types.h"
#include "os-shared-dir.h"
#include "osapi-error.h"

int32 OS_LionsOS_DirAPI_Impl_Init(void)
{
    return OS_SUCCESS;
}

int32 OS_DirCreate_Impl(const char *local_path, uint32 access)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_DirOpen_Impl(const OS_object_token_t *token, const char *local_path)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_DirClose_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_DirRead_Impl(const OS_object_token_t *token, os_dirent_t *dirent)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_DirRewind_Impl(const OS_object_token_t *token)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_DirRemove_Impl(const char *local_path)
{
    return OS_ERR_NOT_IMPLEMENTED;
}
