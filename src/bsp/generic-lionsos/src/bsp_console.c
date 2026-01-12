#include <unistd.h>
#include <microkit.h>

#include "common_types.h"

void OS_BSP_ConsoleOutput_Impl(const char *Str, size_t DataLen)
{
    write(STDOUT_FILENO, Str, DataLen);
}

void OS_BSP_ConsoleSetMode_Impl(uint32 ModeBits)
{
    /* no-op on LionsOS */
}
