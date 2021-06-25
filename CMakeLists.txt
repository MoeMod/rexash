cmake_minimum_required(VERSION 3.16)
project(rexash)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

# Install custom module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

add_subdirectory(3rdparty)
add_subdirectory(SourceSDK)
add_subdirectory(vgui2_support)
add_subdirectory(engine)