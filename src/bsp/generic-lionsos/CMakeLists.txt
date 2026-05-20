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
    "${LIONSOS}/dep/libmicrokitco"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/lwip_include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../os/lionsos/inc"
)

target_compile_options(osal_generic-lionsos_impl PRIVATE
    -std=gnu99
    -Wno-pedantic
    -Wno-unused-function
)

target_link_libraries(osal_public_api INTERFACE
    "${BUILD_DIR}/tcp.o"
    "${BUILD_DIR}/libmicrokitco_cfs.a"
)

# This BSP only works with "lionsos" OS layer
# Confirming this reduces risk of accidental misconfiguration
set_property(TARGET osal_generic-lionsos_impl PROPERTY OSAL_EXPECTED_OSTYPE "lionsos")
