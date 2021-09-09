cmake_minimum_required(VERSION 3.4)
project(searchString VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(searchStringLib_SOURCES "")
file(GLOB_RECURSE searchStringLib_SOURCES   CONFIGURE_DEPENDS ./lib/**.cpp)
file(GLOB_RECURSE searchStringLib_SOURCES_C CONFIGURE_DEPENDS ./lib/**.c)
list(APPEND searchStringLib_SOURCES ${searchStringLib_SOURCES_C})
add_library(searchStringLib STATIC ${searchStringLib_SOURCES})
set_property(TARGET searchStringLib PROPERTY CXX_STANDARD 14)
target_include_directories(searchStringLib PUBLIC include)

set(searchString_SOURCES "")
list(APPEND searchString_SOURCES
	"src/main.cpp"
)
add_executable(searchString)
target_sources(searchString PRIVATE ${searchString_SOURCES})
target_link_libraries(searchString PRIVATE searchStringLib)
set_property(TARGET searchString PROPERTY CXX_STANDARD 14)
target_include_directories(searchString PRIVATE include)

