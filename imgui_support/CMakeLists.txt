

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

add_library(imgui STATIC 
	imgui/imgui_demo.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_widgets.cpp
	imgui/imgui.cpp
)
target_include_directories(imgui PUBLIC 
	imgui/
)
add_library(imgui_ext STATIC 
	imgui_ext/ImGuiFileDialog/ImGuiFileDialog.cpp
	)
target_include_directories(imgui_ext PUBLIC 
	imgui/
	imgui_ext/
)
if(WIN32)
	target_include_directories(imgui_ext PUBLIC imgui_ext/dirent)
endif()
target_link_libraries(imgui_ext PUBLIC imgui)