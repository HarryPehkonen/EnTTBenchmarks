cmake_minimum_required(VERSION 3.10)
project(EnTTBenchmarks)
set(CMAKE_CXX_STANDARD 17)

# Use FetchContent with extra logging
include(FetchContent)
set(FETCHCONTENT_QUIET OFF)  # Show download progress
set(FETCHCONTENT_UPDATES_DISCONNECTED OFF)  # Always check for updates

# Fetch EnTT library
FetchContent_Declare(
    entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG        v3.14.0  # Use the latest stable version
    GIT_PROGRESS   TRUE
)

# Fetch Google Benchmark
FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG        v1.8.3
    GIT_PROGRESS   TRUE
)

# Configure benchmark options
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark testing")
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "Disable benchmark installation")

# Make the downloaded content available
FetchContent_MakeAvailable(entt benchmark)

# Add the executable
add_executable(EnTTBenchmarks main.cpp)

# Include directories
target_include_directories(EnTTBenchmarks PRIVATE ${entt_SOURCE_DIR}/src)

# Link with Google Benchmark
target_link_libraries(EnTTBenchmarks PRIVATE benchmark::benchmark benchmark::benchmark_main)

# Add optimization flags for Release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(EnTTBenchmarks PRIVATE -O3)
endif()

### make_minimum_required(VERSION 3.10)
### project(EnTTBenchmarks)
### 
### set(CMAKE_CXX_STANDARD 17)
### 
### # Use FetchContent with extra logging
### include(FetchContent)
### set(FETCHCONTENT_QUIET OFF)  # Show download progress
### set(FETCHCONTENT_UPDATES_DISCONNECTED OFF)  # Always check for updates
### 
### # Fetch EnTT library
### FetchContent_Declare(
###     entt
###     GIT_REPOSITORY https://github.com/skypjack/entt.git
###     GIT_TAG        v3.14.0  # Use the latest stable version
###     GIT_PROGRESS   TRUE
### )
### 
### FetchContent_GetProperties(entt)
### if(NOT entt_POPULATED)
###     message(STATUS "Downloading EnTT...")
###     FetchContent_Populate(entt)
###     message(STATUS "EnTT download complete")
### endif()
### 
### # Make the downloaded content available
### FetchContent_MakeAvailable(entt)
### 
### add_executable(EnTTBenchmarks main.cpp)
### target_include_directories(EnTTBenchmarks PRIVATE ${entt_SOURCE_DIR}/src)

