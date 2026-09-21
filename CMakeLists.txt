cmake_minimum_required(VERSION 3.16)
project(FDM_FTCS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default to a debug build; release has to be asked for explicitly.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

add_executable(ftcs src/main.cpp)

target_include_directories(ftcs PRIVATE src)

target_compile_options(ftcs PRIVATE -Wall -Wextra)

# Address/UB sanitizers: catches out-of-bounds vector writes, which is the
# failure mode this kind of code hits most. On by default in Debug.
option(ENABLE_SANITIZERS "Build with address and UB sanitizers" ON)
if(ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(ftcs PRIVATE -fsanitize=address,undefined -g)
    target_link_options(ftcs PRIVATE -fsanitize=address,undefined)
endif()
