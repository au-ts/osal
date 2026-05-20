######################################################################
#
# CMAKE build recipe for LionsOS OSAL implementation
#
######################################################################

# This CMake script generates targets specific to the LionsOS implementation

# The basic set of POSIX files which are always built
set(POSIX_BASE_SRCLIST
    # ../posix/src/os-impl-binsem.c
    # ../posix/src/os-impl-common.c
    ../posix/src/os-impl-console.c
    ../posix/src/os-impl-condvar.c
    ../posix/src/os-impl-countsem.c
    ../posix/src/os-impl-dirs.c
    ../posix/src/os-impl-errors.c
    ../posix/src/os-impl-files.c
    ../posix/src/os-impl-filesys.c
    ../posix/src/os-impl-heap.c
    ../posix/src/os-impl-idmap.c
    # ../posix/src/os-impl-mutex.c
    ../posix/src/os-impl-queues.c
    # ../posix/src/os-impl-tasks.c
    # ../posix/src/os-impl-timebase.c
)

# Use portable blocks for basic I/O
set(POSIX_IMPL_SRCLIST
    ../portable/os-impl-posix-gettime.c
    ../portable/os-impl-posix-gettime-monotonic.c
    ../portable/os-impl-console-bsp.c
    ../portable/os-impl-bsd-select.c
    ../portable/os-impl-posix-io.c
    ../portable/os-impl-posix-files.c
    ../portable/os-impl-posix-dirs.c
    ../portable/os-impl-no-file-allocate.c
)

# Special LionsOS overrides
set(LIONSOS_IMPL_SRCLIST
    src/os-impl-binsem.c
    src/os-impl-common.c
    src/os-impl-tasks.c
    src/os-impl-mutex.c
    src/os-impl-timebase.c
)

if (OSAL_CONFIG_INCLUDE_SHELL)
    message (FATAL_ERROR "LionsOS OSAL implementation does not support a shell")
else ()
    list(APPEND POSIX_IMPL_SRCLIST
       ../portable/os-impl-no-shell.c
    )
endif ()

if (OSAL_CONFIG_RWLOCK)
    message (FATAL_ERROR "LionsOS OSAL implementation does not support RW locks")
else ()
    list(APPEND POSIX_IMPL_SRCLIST
       ../portable/os-impl-no-rwlock.c
    )
endif ()

# If some form of module loading is configured,
# then build the module loader
if (OSAL_CONFIG_INCLUDE_DYNAMIC_LOADER)
    message (FATAL_ERROR "LionsOS OSAL implementation does not support dynamic loading")
else ()
    list(APPEND POSIX_IMPL_SRCLIST
        ../posix/src/os-impl-no-module.c
        ../portable/os-impl-no-loader.c
        ../portable/os-impl-no-symtab.c
    )
endif ()

if (OSAL_CONFIG_INCLUDE_NETWORK)
    message (FATAL_ERROR "LionsOS OSAL implementation does not support networking")
else()
    list(APPEND POSIX_IMPL_SRCLIST
        ../portable/os-impl-no-network-gethostname.c    # non-implemented version of gethostname API
        ../portable/os-impl-no-network-gethostid.c      # non-implemented version of gethostid API
        ../portable/os-impl-no-sockets.c    # non-implemented versions of all socket APIs
    )
endif ()

# Defines an OBJECT target named "osal_lionsos_impl" with selected source files
add_library(osal_lionsos_impl OBJECT
    ${POSIX_BASE_SRCLIST}
    ${POSIX_IMPL_SRCLIST}
    ${LIONSOS_IMPL_SRCLIST}
)

target_include_directories(osal_lionsos_impl PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../posix/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../bsp/generic-lionsos/src
    "${LIONSOS}/dep/libmicrokitco"
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
    _POSIX_OS_
    _LIONSOS_OS_
)
