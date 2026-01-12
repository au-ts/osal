#ifndef OS_IMPL_IDMAP_H
#define OS_IMPL_IDMAP_H

#include <pthread.h>

#include "osapi-idmap.h"

typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
} OS_impl_objtype_lock_t;

extern OS_impl_objtype_lock_t *const OS_impl_objtype_lock_table[OS_OBJECT_TYPE_USER];

#endif /* OS_IMPL_IDMAP_H */
