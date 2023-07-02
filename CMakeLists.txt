cmake_minimum_required(VERSION 3.18)
project(kstd-reflect LANGUAGES C CXX)

option(KSTD_REFLECT_BUILD_TESTS "Build unit tests for kstd-reflect" ON)
set(KSTD_REFLECT_MODULE ON) # Let other modules know we are present!
add_compile_definitions(KSTD_REFLECT_MODULE)

if(NOT DEFINED CMAKE_CXX_STANDARD)
	message(STATUS "No C++ version specified, defaulting to C++20")
	set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;")
include(Platform)
include(CommonLibraries)
include_scripts()

if(${KSTD_REFLECT_BUILD_TESTS})
    file(GLOB_RECURSE KSTD_REFLECT_TEST_SOURCE_FILES "${CMAKE_SOURCE_DIR}/test/*.cpp")
    add_executable(kstd-reflect-tests ${KSTD_REFLECT_TEST_SOURCE_FILES})
    target_include_directories(kstd-reflect-tests PUBLIC "${CMAKE_SOURCE_DIR}/kstd-core/include")
    target_include_directories(kstd-reflect-tests PUBLIC "${CMAKE_SOURCE_DIR}/include")
    target_include_gtest(kstd-reflect-tests)
    target_include_fmt(kstd-reflect-tests)
endif()
