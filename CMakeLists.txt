cmake_minimum_required(VERSION 3.20.0)

# Specify the C standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_definitions(ZB_ZCL_SUPPORT_CLUSTER_ELECTRICAL_MEASUREMENT=1)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project("Smart Meter Zigbee")

FILE(GLOB app_sources src/*.c)
FILE(GLOB libsml_sources libsml/sml/src/*.c)
list(REMOVE_ITEM libsml_sources "${CMAKE_CURRENT_SOURCE_DIR}/libsml/sml/src/sml_transport.c")

target_compile_options(app PRIVATE -Wall -Wextra)
target_sources(app PRIVATE ${app_sources} ${libsml_sources})
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc ${CMAKE_CURRENT_SOURCE_DIR}/libsml/sml/include)
target_compile_definitions(app PRIVATE SML_NO_UUID_LIB=1)
