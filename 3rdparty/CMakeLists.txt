cmake_minimum_required(VERSION 3.10.2)

add_library(sdl2 INTERFACE)
if(MSVC)
	add_definitions(-D_ftol2_sse=SDL_ftol2_sse) # fix x86 compile
endif()
set(DIRECTX OFF CACHE BOOL "" FORCE) # win10 arm dont have dx8
set(LIBC ON CACHE BOOL "" FORCE) # win10 arm dont have dx8
add_subdirectory(SDL2 EXCLUDE_FROM_ALL)
target_link_libraries(sdl2 INTERFACE SDL2-static)
