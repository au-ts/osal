######################################################################
#
# CMAKE build recipe for the LionsOS Board Support Package (BSP)
#
######################################################################

add_library(osal_generic-lionsos_impl OBJECT
    src/bsp_start.c
    src/bsp_console.c
    src/bsp_libc.c
)

target_include_directories(osal_generic-lionsos_impl PRIVATE
    ${LIONSOS}/dep/libmicrokitco
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lwip_include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(osal_generic-lionsos_impl PRIVATE
    -std=gnu99
    -Wno-pedantic
    -Wno-unused-function
)

set(LIONSOS_LIBC_TCP_OBJECT "${CMAKE_CURRENT_BINARY_DIR}/../../../../../tcp.o")
cmake_path(ABSOLUTE_PATH LIONSOS_LIBC_TCP_OBJECT NORMALIZE OUTPUT_VARIABLE LIONSOS_LIBC_TCP_OBJECT)
set(MICROKIT_COROUTINE_CLIENT_LIBRARY "${CMAKE_CURRENT_BINARY_DIR}/../../../../../libmicrokitco_client.a")
cmake_path(ABSOLUTE_PATH MICROKIT_COROUTINE_CLIENT_LIBRARY NORMALIZE OUTPUT_VARIABLE MICROKIT_COROUTINE_CLIENT_LIBRARY)

target_link_libraries(osal_public_api INTERFACE
    "${LIONSOS_LIBC_TCP_OBJECT}"
    "${MICROKIT_COROUTINE_CLIENT_LIBRARY}"
)

# This BSP only works with "lionsos" OS layer
# Confirming this reduces risk of accidental misconfiguration
set_property(TARGET osal_generic-lionsos_impl PROPERTY OSAL_EXPECTED_OSTYPE "lionsos")
