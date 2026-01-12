#include <sel4/sel4.h>
#include <libmicrokitco.h>

#include "os-shared-globaldefs.h"
#include "os-shared-common.h"
#include "osapi-error.h"
#include "osapi-idmap.h"
#include "os-lionsos.h"

int32 OS_API_Impl_Init(osal_objtype_t idtype)
{
    int32 return_code;

    switch (idtype)
    {
        case OS_OBJECT_TYPE_UNDEFINED:
            return_code = OS_SUCCESS;
            break;
        case OS_OBJECT_TYPE_OS_TASK:
            return_code = OS_LionsOS_TaskAPI_Impl_Init();
            break;
        case OS_OBJECT_TYPE_OS_QUEUE:
            return_code = OS_LionsOS_QueueAPI_Impl_Init();
            break;
        case OS_OBJECT_TYPE_OS_BINSEM:
            return_code = OS_LionsOS_BinSemAPI_Impl_Init();
            break;
        case OS_OBJECT_TYPE_OS_COUNTSEM:
            return_code = OS_LionsOS_CountSemAPI_Impl_Init();
            break;
        case OS_OBJECT_TYPE_OS_MUTEX:
            return_code = OS_LionsOS_MutexAPI_Impl_Init();
            break;
        case OS_OBJECT_TYPE_OS_MODULE:
            return_code = OS_LionsOS_ModuleAPI_Impl_Init();
            break;
        case OS_OBJECT_TYPE_OS_TIMEBASE:
            return_code = OS_LionsOS_TimeBaseAPI_Impl_Init();
            break;
        case OS_OBJECT_TYPE_OS_STREAM:
            return_code = OS_LionsOS_StreamAPI_Impl_Init();
            break;
        case OS_OBJECT_TYPE_OS_DIR:
            return_code = OS_LionsOS_DirAPI_Impl_Init();
            break;
        case OS_OBJECT_TYPE_OS_FILESYS:
            return_code = OS_LionsOS_FileSysAPI_Impl_Init();
            break;
        default:
            /* Do nothing and note success */
            return_code = OS_SUCCESS;
            break;
    }

    return return_code;
}

void OS_IdleLoop_Impl()
{
    OS_DEBUG("Entering idle loop\n");

    while (true)
    {
        microkit_cothread_yield();
    }
}

void OS_ApplicationShutdown_Impl()
{
    OS_DEBUG("Application shutdown requested\n");
    seL4_DebugHalt();
}
