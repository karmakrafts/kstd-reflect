cmake_minimum_required(VERSION 3.18)
project(kstd-reflect LANGUAGES C CXX)

option(KSTD_REFLECT_BUILD_TESTS "Build unit tests for kstd-reflect" OFF)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;")
include(cmx)
cmx_include_scripts()

cmx_add_library(kstd-reflect INTERFACE)
cmx_include_kstd_core(kstd-reflect INTERFACE)
target_include_directories(kstd-reflect INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

if (${KSTD_REFLECT_BUILD_TESTS})
    cmx_add_tests(kstd-reflect-tests "${CMAKE_CURRENT_SOURCE_DIR}/test")
    target_link_libraries(kstd-reflect-tests INTERFACE kstd-reflect)
	add_dependencies(kstd-reflect-tests kstd-reflect)
endif ()
