cmake_minimum_required(VERSION 3.10.2)

option(USE_VCPKG "Use vcpkg to find 3rdparty libraries." OFF)
set(SDL2_PATH ${CMAKE_CURRENT_SOURCE_DIR}/SDL2 CACHE PATH "SDL2 path")

add_library(sdl2 INTERFACE)
if(USE_VCPKG)
	find_package(SDL2 CONFIG REQUIRED)
	target_link_libraries(sdl2 INTERFACE SDL2::SDL2)
elseif(IOS)
	target_include_directories(sdl2 INTERFACE "${SDL2_PATH}/include")
else()
	if(NOT CMAKE_CROSSCOMPILING)
		find_package(SDL2 QUIET)
	endif()

	if(SDL2_FOUND)
		target_link_libraries(sdl2 INTERFACE ${SDL2_LIBRARY})
		target_include_directories(sdl2 INTERFACE ${SDL2_INCLUDE_DIR})
		if(MSVC)
			string(REGEX REPLACE "lib$" "dll" SDL2_DLL "${SDL2_LIBRARY}")
			install(FILES ${SDL2_LIBRARY} DESTINATION "." OPTIONAL)
		endif()
	else()
		if(MSVC)
			add_definitions(-D_ftol2_sse=SDL_ftol2_sse) # fix x86 compile
		endif()
		set(DIRECTX OFF CACHE BOOL "" FORCE) # win10 arm dont have dx8
		set(LIBC ON CACHE BOOL "" FORCE) # win10 arm dont have dx8
		add_subdirectory(SDL2 EXCLUDE_FROM_ALL)
		target_link_libraries(sdl2 INTERFACE SDL2-static)
	endif()
endif()
