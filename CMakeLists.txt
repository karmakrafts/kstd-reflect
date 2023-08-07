cmake_minimum_required(VERSION 3.18)
project(kstd-reflect LANGUAGES C CXX)

option(KSTD_REFLECT_BUILD_TESTS "Build unit tests for kstd-reflect" OFF)
set(KSTD_REFLECT_MODULE ON) # Let other modules know we are present!
add_compile_definitions(KSTD_REFLECT_MODULE)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;")
include(cmx)
cmx_include_scripts()

if(${KSTD_REFLECT_BUILD_TESTS})
    cmx_add_tests(kstd-reflect-tests "${CMAKE_SOURCE_DIR}/test")
	target_include_directories(kstd-reflect-tests PRIVATE "${CMAKE_SOURCE_DIR}/include")
    cmx_include_fmt(kstd-reflect-tests PUBLIC)
    cmx_include_kstd_core(kstd-reflect-tests PUBLIC)
endif()
