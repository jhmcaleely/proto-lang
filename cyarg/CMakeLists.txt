# PICO SDK defaults to 3.13 minimum
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

if(PICO_BOARD)
# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

set(PICO_USE_FASTEST_SUPPORTED_CLOCK 1)
endif()

# The PICO SDK will also need CXX & ASM toolchains to assemble the firmware image
project(cyarg VERSION 0.1.0 LANGUAGES C CXX ASM)


if (PICO_BOARD)
# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()
endif()

add_executable(cyarg 
    chunk.c
    main.c
    memory.c
    debug.c
    value.c
    routine.c
    vm.c
    ast.c
    parser.c
    compiler.c
    scanner.c
    object.c
    table.c
    native.c
    builtin.c
    channel.c
    yargtype.c
    platform_hal.c
    ws2812_native.c)

if (PICO_BOARD)
target_sources(cyarg
    PRIVATE
      files_littlefs.c
      pico_lfs_hal.c
        ../hostyarg/littlefs/lfs.c
        ../hostyarg/littlefs/lfs_util.c)
else()
target_sources(cyarg
    PRIVATE
      files_stdlib.c)
endif()

add_compile_definitions(NAN_BOXING)

if (HOST_TYPE STREQUAL "Interactive")
add_compile_definitions(DEBUG_TRACE_EXECUTION)
add_compile_definitions(DEBUG_AST_PARSE)
add_compile_definitions(DEBUG_STRESS_GC)
endif()
#add_compile_definitions(DEBUG_LOG_GC)

if (PICO_BOARD)
set(PICO_USE_FASTEST_SUPPORTED_CLOCK 1)

add_compile_definitions(CYARG_PICO_TARGET)
endif()


if (PICO_BOARD)
# Setup the firmware image and details of the executable it will host


pico_set_program_name(cyarg "cyarg")
pico_set_program_version(cyarg "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(cyarg 1)
pico_enable_stdio_usb(cyarg 1)

# Add the standard library to the build
target_link_libraries(cyarg
        hardware_pio
        pico_stdlib
        pico_multicore
        pico_sync)

# Add the standard include files to the build
target_include_directories(cyarg PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(cyarg)
endif()