#ifndef OS_IMPL_LOADER_H
#define OS_IMPL_LOADER_H

#include <elf/elf.h>

#include "osconfig.h"

typedef struct
{
    elf_t elf;
    int pd;
} OS_impl_module_internal_record_t;

extern OS_impl_module_internal_record_t OS_impl_module_table[OS_MAX_MODULES];

#endif /* OS_IMPL_LOADER_H */
