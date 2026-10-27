cmake_minimum_required(VERSION 3.30)

project(struct_reordering VERSION 0.1.0 LANGUAGES CXX)

add_library(struct_reordering INTERFACE)
add_library(struct_reordering::struct_reordering ALIAS struct_reordering)

target_include_directories(struct_reordering INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_compile_features(struct_reordering INTERFACE cxx_std_26)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 16)
    target_compile_options(struct_reordering INTERFACE -freflection)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)
option(STRUCT_REORDERING_BUILD_TESTS "Build struct_reordering tests" ${PROJECT_IS_TOP_LEVEL})

if(STRUCT_REORDERING_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
