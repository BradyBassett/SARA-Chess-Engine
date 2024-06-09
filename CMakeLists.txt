make_minimum_required(VERSION 3.14)
project(chess-engine-api)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/Include)
include_directories(${PROJECT_SOURCE_DIR}/Src)

# Collect all source files, excluding main.cpp
file(GLOB_RECURSE SOURCES "Src/[!main]*.cpp")

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} "Src/main.cpp")

# Collect all test source files
file(GLOB_RECURSE TEST_SOURCES "Tests/*.cpp")

# Add test executable
add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES} ${SOURCES})

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# Link GoogleTest to the test executable
target_link_libraries(${PROJECT_NAME}_tests gtest gtest_main)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)