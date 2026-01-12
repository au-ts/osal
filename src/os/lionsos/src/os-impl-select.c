#include "common_types.h"
#include "os-shared-select.h"
#include "osapi-error.h"

int32 OS_SelectSingle_Impl(const OS_object_token_t *token, uint32 *SelectFlags, OS_time_t abs_timeout)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_SelectMultiple_Impl(OS_FdSet *ReadSet, OS_FdSet *WriteSet, OS_time_t abs_timeout)
{
    return OS_ERR_NOT_IMPLEMENTED;
}
