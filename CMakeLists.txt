cmake_minimum_required(VERSION 3.10)

project(ds C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O2")

include_directories(structures)
file(GLOB SOURCE_FILES "*.c")

add_executable(ds ${SOURCE_FILES})

set_target_properties(ds PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
