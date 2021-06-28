cmake_minimum_required(VERSION 3.12.0)
project (menu)

include_directories (../common ../engine ../pm_shared ../hymenu)
add_library (menu SHARED 
	udll_int.cpp
	Utils.cpp
	UIFuncs.cpp
)

install( TARGETS menu RUNTIME DESTINATION ".")