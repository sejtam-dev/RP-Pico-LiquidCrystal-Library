cmake_minimum_required(VERSION 3.12)

message("==== Test ====")
message("=")
    
set(TEST_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
message("= Root Directory: ${TEST_ROOT_DIR}")
message("=")

set(TEST_INCLUDES_DIR ${TEST_ROOT_DIR}/include)
file(GLOB TEST_INCLUDES "${TEST_INCLUDES_DIR}/*.h" "${TEST_INCLUDES_DIR}/*.hpp")

message("= Headers Directory: ${TEST_INCLUDES_DIR}")

# Source
set(TEST_SOURCE_DIR ${TEST_ROOT_DIR}/src)
file(GLOB TEST_SOURCES "${TEST_SOURCE_DIR}/.c" "${TEST_SOURCE_DIR}/*.cpp")

message("= Source Directory: ${TEST_SOURCE_DIR}")
message("=")

# Libraries
set(TEST_INCLUDE_DIRS)
set(TEST_LIBRARIES)

list(APPEND TEST_LIBRARIES pico_stdlib)

#list(APPEND TEST_LIBRARIES hardware_pwm)
#list(APPEND TEST_LIBRARIES hardware_flash)
#list(APPEND TEST_LIBRARIES hardware_sync)

# Create Executable File
include_directories(${TEST_INCLUDES_DIR})
add_executable(${PROJECT_NAME}_TEST ${TEST_SOURCES})

# Link Libraries
target_include_directories(${PROJECT_NAME}_TEST PUBLIC ${TEST_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}_TEST PRIVATE ${TEST_LIBRARIES})

#pico_enable_stdio_usb(${PROJECT_NAME}_TEST 1)
pico_enable_stdio_uart(${PROJECT_NAME}_TEST 0)

pico_add_extra_outputs(${PROJECT_NAME}_TEST)