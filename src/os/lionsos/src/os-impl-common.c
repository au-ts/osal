#include <sel4/sel4.h>
#include <libmicrokitco.h>

#include "os-shared-globaldefs.h"
#include "os-shared-common.h"
#include "osapi-error.h"
#include "osapi-idmap.h"
#include "os-lionsos.h"
#include "os-posix.h"

#include <time.h>

POSIX_GlobalVars_t POSIX_GlobalVars = {0};

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
            return_code = OS_Posix_QueueAPI_Impl_Init();
            break;
        case OS_OBJECT_TYPE_OS_BINSEM:
            return_code = OS_LionsOS_BinSemAPI_Impl_Init();
            break;
        case OS_OBJECT_TYPE_OS_COUNTSEM:
            return_code = OS_Posix_CountSemAPI_Impl_Init();
            break;
        case OS_OBJECT_TYPE_OS_MUTEX:
            return_code = OS_LionsOS_MutexAPI_Impl_Init();
            break;
        case OS_OBJECT_TYPE_OS_MODULE:
            return_code = OS_Posix_ModuleAPI_Impl_Init();
            break;
        case OS_OBJECT_TYPE_OS_TIMEBASE:
            return_code = OS_LionsOS_TimeBaseAPI_Impl_Init();
            break;
        case OS_OBJECT_TYPE_OS_STREAM:
            return_code = OS_Posix_StreamAPI_Impl_Init();
            break;
        case OS_OBJECT_TYPE_OS_DIR:
            return_code = OS_Posix_DirAPI_Impl_Init();
            break;
        case OS_OBJECT_TYPE_OS_FILESYS:
            return_code = OS_Posix_FileSysAPI_Impl_Init();
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

// Needed for POSIX implementation of counting semaphores and queues
void OS_Posix_CompAbsDelayTime(uint32 msecs, struct timespec *tm)
{
    clock_gettime(CLOCK_REALTIME, tm);

    /* add the delay to the current time */
    tm->tv_sec += (time_t)(msecs / 1000);
    /* convert residue ( msecs )  to nanoseconds */
    tm->tv_nsec += (msecs % 1000) * 1000000L;

    if (tm->tv_nsec >= 1000000000L)
    {
        tm->tv_nsec -= 1000000000L;
        tm->tv_sec++;
    }
}
