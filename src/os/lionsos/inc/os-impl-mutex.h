#ifndef OS_IMPL_MUTEX_H
#define OS_IMPL_MUTEX_H

#include <libmicrokitco.h>

#include "osconfig.h"

/* Mutexes */
typedef struct
{
    microkit_cothread_sem_t semaphore;
} OS_impl_mutex_internal_record_t;

/* Tables where the OS object information is stored */
extern OS_impl_mutex_internal_record_t OS_impl_mutex_table[OS_MAX_TASKS];

#endif /* OS_IMPL_MUTEX_H */
