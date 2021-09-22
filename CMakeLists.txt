cmake_minimum_required(VERSION 3.4)
project(searchString VERSION 0.1.0)

include("cmake/CPM.cmake")
include("cmake/searchString.cmake")
include("cmake/googletest.cmake")
include("cmake/cxxopts.cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

set(searchStringLib_SOURCES "")
file(GLOB_RECURSE searchStringLib_SOURCES   CONFIGURE_DEPENDS ./lib/**.cpp)
file(GLOB_RECURSE searchStringLib_SOURCES_C CONFIGURE_DEPENDS ./lib/**.c)
list(APPEND searchStringLib_SOURCES ${searchStringLib_SOURCES_C})
add_library(searchStringLib STATIC ${searchStringLib_SOURCES})
target_link_libraries(searchStringLib PUBLIC SentenceValidationLib)
set_property(TARGET searchStringLib PROPERTY CXX_STANDARD 17)
target_include_directories(searchStringLib PUBLIC include)

set(searchString_SOURCES "")
list(APPEND searchString_SOURCES
	"src/main.cpp"
)
add_executable(searchString)
target_sources(searchString PRIVATE ${searchString_SOURCES})
target_link_libraries(searchString PRIVATE cxxopts)
target_link_libraries(searchString PRIVATE searchStringLib)
set_property(TARGET searchString PROPERTY CXX_STANDARD 17)
target_include_directories(searchString PRIVATE include)


# testing
enable_testing()
include(GoogleTest)
file(GLOB TEST_FILES_CPP "${CMAKE_CURRENT_LIST_DIR}/test/*.cpp")
file(GLOB TEST_FILES_CX  "${CMAKE_CURRENT_LIST_DIR}/test/*.cx")
file(GLOB TEST_FILES     "${CMAKE_CURRENT_LIST_DIR}/test/*.c")
list(APPEND TEST_FILES ${TEST_FILES_CPP} ${TEST_FILES_CX})
foreach (test_file IN LISTS TEST_FILES)
    get_filename_component(filenamewe ${test_file} NAME_WE)
    string(CONCAT execname "test_" ${filenamewe})
    add_executable(${execname} ${test_file})
    set_property(TARGET ${execname} PROPERTY CXX_STANDARD 17)
    target_link_libraries(${execname} gtest_main)
    target_link_libraries(${execname} searchStringLib)
    gtest_discover_tests(${execname})
endforeach()

