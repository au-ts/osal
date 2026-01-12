#include <microkit.h>
#include <unistd.h>

#include "common_types.h"
#include "os-shared-console.h"
#include "os-shared-idmap.h"
#include "osapi-error.h"

#define OS_CONSOLE_ASYNC false

typedef struct
{
    int out_fd;
} OS_impl_console_internal_record_t;

/* Tables where the OS object information is stored */
OS_impl_console_internal_record_t OS_impl_console_table[OS_MAX_CONSOLES];

int32 OS_ConsoleCreate_Impl(const OS_object_token_t *token)
{
    OS_impl_console_internal_record_t *local;
    OS_console_internal_record_t *console;
    int32 return_code;

    return_code = OS_ERR_NOT_IMPLEMENTED;
    local = OS_OBJECT_TABLE_GET(OS_impl_console_table, *token);
    console = OS_OBJECT_TABLE_GET(OS_console_table, *token);

    if (OS_ObjectIndexFromToken(token) == 0 && !console->IsAsync)
    {
        return_code = OS_SUCCESS;
        local->out_fd = STDOUT_FILENO;
    }

    return return_code;
}

void OS_ConsoleWakeup_Impl(const OS_object_token_t *token)
{
    OS_DEBUG("Console wakeup requested with token %p\n", token);
}
