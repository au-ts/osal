#ifndef OS_LIONSOS_H
#define OS_LIONSOS_H

#include "common_types.h"

int32 OS_LionsOS_TaskAPI_Impl_Init(void);
int32 OS_LionsOS_QueueAPI_Impl_Init(void);
int32 OS_LionsOS_BinSemAPI_Impl_Init(void);
int32 OS_LionsOS_CountSemAPI_Impl_Init(void);
int32 OS_LionsOS_MutexAPI_Impl_Init(void);
int32 OS_LionsOS_CondVarAPI_Impl_Init(void);
int32 OS_LionsOS_ModuleAPI_Impl_Init(void);
int32 OS_LionsOS_TimeBaseAPI_Impl_Init(void);
int32 OS_LionsOS_StreamAPI_Impl_Init(void);
int32 OS_LionsOS_DirAPI_Impl_Init(void);
int32 OS_LionsOS_FileSysAPI_Impl_Init(void);

#endif /* OS_LIONSOS_H */
