
add_library(vgui2_interfaces INTERFACE)
target_include_directories(vgui2_interfaces INTERFACE 
    ./ 
    interfaces/
    ../common
    ../engine
    ../pm_shared
)
target_link_libraries(vgui2_interfaces INTERFACE vinterface mathlib)

add_subdirectory(vgui_controls)

add_library(vgui_surfacelib STATIC
    vgui2/vgui_surfacelib/FontAmalgam.cpp
    vgui2/vgui_surfacelib/FontManager.cpp
    vgui2/vgui_surfacelib/Win32Font.cpp
)
target_include_directories(vgui_surfacelib PUBLIC 
    vgui2/vgui_surfacelib/
)
target_link_libraries(vgui_surfacelib PUBLIC vgui2_interfaces)

add_library(vguimatsurface STATIC
    vguimatsurface/Clip2D.cpp
    vguimatsurface/FontTextureCache.cpp
)
target_include_directories(vgui_surfacelib PRIVATE 
    vguimatsurface/
)
target_link_libraries(vguimatsurface PUBLIC vgui2_interfaces)
target_link_libraries(vguimatsurface PRIVATE vgui_surfacelib)

add_library(dead_wrapper STATIC
    dead_wrapper/deadfilesystem.cpp
    dead_wrapper/deadinput.cpp
    dead_wrapper/deadipanel.cpp
    dead_wrapper/deadscheme.cpp
    dead_wrapper/deadsurface.cpp
)
target_link_libraries(dead_wrapper PUBLIC vgui2_interfaces)
target_link_libraries(dead_wrapper PRIVATE vguimatsurface)
target_link_libraries(dead_wrapper PRIVATE tier0)

add_library(vgui2_support SHARED
	BaseUISurface.cpp
    CBaseUI.cpp
    cdll_int.cpp
    CGameUIFuncs.cpp
    LoadBMP.cpp
    LoadTGA.cpp
    vgui_int.cpp
)

target_link_libraries(vgui2_support PUBLIC vgui2_interfaces)
target_link_libraries(vgui2_support PRIVATE vgui_controls)
target_link_libraries(vgui2_support PRIVATE vgui_surfacelib)
target_link_libraries(vgui2_support PRIVATE vguimatsurface)
target_link_libraries(vgui2_support PRIVATE dead_wrapper)
target_link_libraries(vgui2_support PRIVATE tier0)

add_library(vgui2 SHARED
    vgui2/Bitmap.cpp
    vgui2/Border.cpp
    vgui2/HtmlWindow.cpp
    vgui2/input.cpp
    vgui2/LocalizedStringTable.cpp
    vgui2/MemoryBitmap.cpp
    vgui2/Scheme.cpp
    vgui2/system_win32.cpp
    vgui2/UnicodeFileHelpers.cpp
    vgui2/vgui.cpp
    vgui2/VGuiKeyValues.cpp
    vgui2/vgui_key_translation.cpp
    vgui2/VPanel.cpp
    vgui2/VPanelWrapper.cpp
    vgui_controls/controls.cpp
)
target_include_directories(vgui2 PRIVATE 
    vgui2/
)
target_link_libraries(vgui2 PUBLIC vgui2_interfaces)
target_link_libraries(vgui2 PRIVATE tier0)
target_link_libraries(vgui2 PRIVATE tier1)
target_link_libraries(vgui2 PRIVATE vstdlib)
target_link_libraries(vgui2 PRIVATE SourceSDK)

set_target_properties(vgui2_support PROPERTIES PREFIX "" OUTPUT_NAME "vgui_support")
install(TARGETS vgui2_support RUNTIME DESTINATION ".")