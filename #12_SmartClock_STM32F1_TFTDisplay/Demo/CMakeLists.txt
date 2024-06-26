# STM32 Minimal CMake project for C/C++ projects
cmake_minimum_required(VERSION 3.12)
###############################################################################
# Set project name and source code folder location
project(rtos)
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Project)

option(DUMP_ASM "Create full assembly of final executable" OFF)

# Set microcontroller information
set(MCU_FAMILY STM32F1xx)
set(MCU_FAMILY_DRIVER STM32F10x)
set(MCU_MODEL STM32F10X_MD)
set(CPU_PARAMETERS
    -mcpu=cortex-m3
    -mthumb)

set(STARTUP_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/Core/startup_stm32f103xb.s)
set(MCU_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/Core/STM32F103C8Tx_FLASH.ld)

###############################################################################
set(EXECUTABLE ${CMAKE_PROJECT_NAME})

enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

###############################################################################
set(STM32CUBEMX_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY_DRIVER}_StdPeriph_Driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/${MCU_FAMILY}/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/include
)

set(PROJECT_INCLUDE_DIRECTORIES 
    ${CMAKE_CURRENT_SOURCE_DIR}/Project/
    ${CMAKE_CURRENT_SOURCE_DIR}/Project/Main
    ${CMAKE_CURRENT_SOURCE_DIR}/Project/DriverCore
    ${CMAKE_CURRENT_SOURCE_DIR}/Project/Devices
    ${CMAKE_CURRENT_SOURCE_DIR}/Project/Devices/ft
    ${CMAKE_CURRENT_SOURCE_DIR}/Project/Devices/buzzer
    ${CMAKE_CURRENT_SOURCE_DIR}/Project/Devices/button
    ${CMAKE_CURRENT_SOURCE_DIR}/Project/Devices/rs232
    ${CMAKE_CURRENT_SOURCE_DIR}/Project/Devices/joy
    ${CMAKE_CURRENT_SOURCE_DIR}/Project/Devices/tft
    ${CMAKE_CURRENT_SOURCE_DIR}/Project/Devices/tft/bmp
    ${CMAKE_CURRENT_SOURCE_DIR}/Project/Devices/tft/fonts
	${CMAKE_CURRENT_SOURCE_DIR}/Project/App
    ${CMAKE_CURRENT_SOURCE_DIR}/Project/App/gui
    ${CMAKE_CURRENT_SOURCE_DIR}/Project/App/gui/tetris
)

file(GLOB_RECURSE STM32CUBEMX_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY_DRIVER}_StdPeriph_Driver/src/misc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY_DRIVER}_StdPeriph_Driver/src/stm32f10x_rcc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY_DRIVER}_StdPeriph_Driver/src/stm32f10x_tim.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY_DRIVER}_StdPeriph_Driver/src/stm32f10x_usart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY_DRIVER}_StdPeriph_Driver/src/stm32f10x_spi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY_DRIVER}_StdPeriph_Driver/src/stm32f10x_dma.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY_DRIVER}_StdPeriph_Driver/src/stm32f10x_gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY_DRIVER}_StdPeriph_Driver/src/stm32f10x_adc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/*.c
)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    ${PROJECT_DIR}/*.cpp
    ${PROJECT_DIR}/*.c
)

add_executable(${EXECUTABLE}
    ${STM32CUBEMX_SOURCES} 
    ${PROJECT_SOURCES}
    ${STARTUP_SCRIPT})

target_compile_definitions(${EXECUTABLE} PRIVATE
    #$<$<CONFIG:Debug>:DEBUG>
    ${MCU_MODEL}
    USE_STDPERIPH_DRIVER)

target_include_directories(${EXECUTABLE} SYSTEM PRIVATE
    ${STM32CUBEMX_INCLUDE_DIRECTORIES})

target_include_directories(${EXECUTABLE} PRIVATE
    ${PROJECT_INCLUDE_DIRECTORIES})
###############################################################################
#-Wold-style-cast
#-Wuseless-cast
target_compile_options(${EXECUTABLE} PRIVATE
${CPU_PARAMETERS}
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wdouble-promotion
    -Wformat=2 -Wformat-truncation
    -Wundef
    -fno-common
    -Wno-unused-parameter
    $<$<COMPILE_LANGUAGE:CXX>:
        -Wno-volatile
        -Wsuggest-override>
    $<$<CONFIG:Debug>:-Og -g3 -ggdb>
    $<$<CONFIG:Release>:-Og -g0>)

target_link_options(${EXECUTABLE} PRIVATE
    -T${MCU_LINKER_SCRIPT}
    ${CPU_PARAMETERS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    --specs=nosys.specs
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -Wl,--end-group
    -Wl,--print-memory-usage)

# The last command can take a couple of seconds on larger project, usefull for debugging
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.bin)

if (${DUMP_ASM})
    add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
        COMMAND ${CMAKE_OBJDUMP} -D $<TARGET_FILE:${EXECUTABLE}> > ${EXECUTABLE}.s)
endif()
