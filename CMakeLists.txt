cmake_minimum_required(VERSION 3.15)
project(kie_toolbox CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(kie_toolbox INTERFACE)
target_include_directories(kie_toolbox INTERFACE include)
add_subdirectory(src)


if(ENABLE_TEST)
    message(STATUS "ENABLE_TEST is set to ON, so test case will be compiled")
    include(CTest)
    add_subdirectory(test)
endif()

install(TARGETS kie_toolbox)
# header files are install by the submodule.
install(FILES LICENSE DESTINATION license)