######################################################################
#
# CMAKE build recipe for LionsOS OSAL implementation
#
######################################################################

# This CMake script generates targets specific to the LionsOS implementation

# The basic set of files which are always built
set(LIONSOS_BASE_SRCLIST
    src/os-impl-binsem.c
    src/os-impl-common.c
    src/os-impl-console.c
    src/os-impl-countsem.c
    src/os-impl-errors.c
    src/os-impl-files.c
    src/os-impl-filesys.c
    src/os-impl-dirs.c
    src/os-impl-heap.c
    src/os-impl-idmap.c
    src/os-impl-mutex.c
    src/os-impl-queues.c
    src/os-impl-tasks.c
    src/os-impl-timebase.c
    src/os-impl-gettime.c
    src/os-impl-select.c
)

# Use portable blocks for basic I/O
set(LIONSOS_IMPL_SRCLIST
#     ../portable/os-impl-posix-gettime.c
    ../portable/os-impl-console-bsp.c
#     ../portable/os-impl-bsd-select.c
#     ../portable/os-impl-posix-io.c
#     ../portable/os-impl-posix-files.c
#     ../portable/os-impl-posix-dirs.c
    ../portable/os-impl-no-condvar.c
)

if (OSAL_CONFIG_INCLUDE_SHELL)
    # list(APPEND LIONSOS_IMPL_SRCLIST
    #    src/os-impl-shell.c
    # )

    message (FATAL_ERROR "LionsOS OSAL implementation does not support a shell")
else ()
    list(APPEND LIONSOS_IMPL_SRCLIST
       ../portable/os-impl-no-shell.c
    )
endif ()

# If some form of module loading is configured,
# then build the module loader
if (OSAL_CONFIG_INCLUDE_DYNAMIC_LOADER)
    # list(APPEND LIONSOS_IMPL_SRCLIST
    #     src/os-impl-loader.c
    #     src/os-impl-symtab.c
    # )

    message (FATAL_ERROR "LionsOS OSAL implementation does not support dynamic loading")
else ()
    list(APPEND LIONSOS_IMPL_SRCLIST
        src/os-impl-no-module.c
        ../portable/os-impl-no-loader.c
        ../portable/os-impl-no-symtab.c
    )
endif ()

if (OSAL_CONFIG_INCLUDE_NETWORK)
    # list(APPEND LIONSOS_IMPL_SRCLIST
    #     src/os-impl-network.c
    #     ../portable/os-impl-bsd-sockets.c   # Use BSD socket layer implementation
    #     src/os-impl-sockets.c               # Additional LionsOS-specific code to handle socket flags
    # )
    message (FATAL_ERROR "LionsOS OSAL implementation does not support networking")
else()
    list(APPEND LIONSOS_IMPL_SRCLIST
        ../portable/os-impl-no-network.c    # non-implemented versions of all network APIs
        ../portable/os-impl-no-sockets.c    # non-implemented versions of all socket APIs
    )
endif ()

# Defines an OBJECT target named "osal_lionsos_impl" with selected source files
add_library(osal_lionsos_impl OBJECT
    ${LIONSOS_BASE_SRCLIST}
    ${LIONSOS_IMPL_SRCLIST}
)

target_include_directories(osal_lionsos_impl PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    "${LIONSOS}/dep/libmicrokitco"
    "/Users/larrabyte/Projects/tor/cfs/osal/src/bsp/generic-lionsos/src"
)

target_compile_options(osal_lionsos_impl PRIVATE
    -std=gnu99
    -Wno-pedantic
    -Wno-unused-function
)

if (CMAKE_SYSTEM_VERSION VERSION_LESS 7.0)
    target_compile_definitions(osal_lionsos_impl PRIVATE
        OSAL_LIONSOS_COMPATIBILITY
    )
endif ()

target_compile_definitions(osal_public_api INTERFACE
    _LIONSOS_OS_
)
