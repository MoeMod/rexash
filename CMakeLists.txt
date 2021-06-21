cmake_minimum_required(VERSION 3.16)
project(rexash)

# Install custom module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

add_subdirectory(3rdparty)
add_subdirectory(SourceSDK)
add_subdirectory(engine)