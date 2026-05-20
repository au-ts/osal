#ifndef OS_IMPL_BINSEM_H
#define OS_IMPL_BINSEM_H

#include <libmicrokitco.h>

#include "osconfig.h"

typedef struct
{
    microkit_cothread_sem_t semaphore;
    volatile uint64_t expiry;
    volatile bool expired;
} OS_impl_binsem_internal_record_t;

void OS_BinSemTimedWait_Callback(void);

extern OS_impl_binsem_internal_record_t OS_impl_bin_sem_table[OS_MAX_BIN_SEMAPHORES];

#endif /* OS_IMPL_BINSEM_H */
