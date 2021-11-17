cmake_minimum_required(VERSION 3.10)
project(StringFinder VERSION 0.1.0)

include("cmake/CPM.cmake")
include("cmake/dlib.cmake")
include("cmake/googletest.cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

set(PEPARSE_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/thirdparty/pe-parse/pe-parser-library")
list(APPEND PEPARSERLIB_SOURCEFILES
  ${PEPARSE_SOURCE_DIR}/src/buffer.cpp
  ${PEPARSE_SOURCE_DIR}/src/parse.cpp
)
if(MSVC)
  list(APPEND PEPARSERLIB_SOURCEFILES ${PEPARSE_SOURCE_DIR}/src/unicode_winapi.cpp)
else()
  list(APPEND PEPARSERLIB_SOURCEFILES ${PEPARSE_SOURCE_DIR}/src/unicode_codecvt.cpp)
endif()
add_library(pe-parser-static STATIC ${PEPARSERLIB_SOURCEFILES})
target_include_directories(pe-parser-static PUBLIC ${PEPARSE_SOURCE_DIR}/include)

set(StringFinderLib_SOURCES "")
file(GLOB_RECURSE StringFinderLib_SOURCES   CONFIGURE_DEPENDS ./lib/**.cpp)
file(GLOB_RECURSE StringFinderLib_SOURCES_C CONFIGURE_DEPENDS ./lib/**.c)
list(APPEND StringFinderLib_SOURCES ${StringFinderLib_SOURCES_C})
add_library(StringFinderLib STATIC ${StringFinderLib_SOURCES})
target_link_libraries(StringFinderLib PUBLIC dlib)
target_link_libraries(StringFinderLib PUBLIC pe-parser-static)
set_property(TARGET StringFinderLib PROPERTY CXX_STANDARD 17)
target_include_directories(StringFinderLib PUBLIC include)
target_include_directories(StringFinderLib PUBLIC thirdparty/aho_corasick/src/aho_corasick)

add_library(cxxopts INTERFACE)
target_include_directories(cxxopts INTERFACE thirdparty/cxxopts/include)

set(StringFinder strfd)
set(StringFinder_SOURCES "")
list(APPEND StringFinder_SOURCES
	"src/main.cpp"
	"src/print_transforms.cpp"
	"src/trainsvm.cpp"
    "src/pefile.cpp"
    "src/search_process.cpp"
    )
add_executable(${StringFinder})
target_sources(${StringFinder} PRIVATE ${StringFinder_SOURCES})
target_link_libraries(${StringFinder} PRIVATE cxxopts)
target_link_libraries(${StringFinder} PRIVATE StringFinderLib)
set_property(TARGET ${StringFinder} PROPERTY CXX_STANDARD 17)
target_include_directories(${StringFinder} PRIVATE include)


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
    target_link_libraries(${execname} StringFinderLib)
    gtest_discover_tests(${execname})
endforeach()

