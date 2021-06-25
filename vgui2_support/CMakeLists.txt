
add_library(vgui2_interfaces INTERFACE)
target_include_directories(vgui2_interfaces INTERFACE 
    ./ 
    interfaces/
    ../common
    ../engine
    ../pm_shared
)

add_subdirectory(vgui_controls)

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
    vguimatsurface/Clip2D.cpp
    vgui_controls/controls.cpp
)
target_include_directories(vgui2 PRIVATE 
    vgui2/
)

add_library(vgui2_support SHARED
	BaseUISurface.cpp
    CBaseUI.cpp
    cdll_int.cpp
    CGameUIFuncs.cpp
    LoadBMP.cpp
    LoadTGA.cpp
    vgui2/vgui_surfacelib/FontAmalgam.cpp
    vgui2/vgui_surfacelib/FontManager.cpp
    vgui2/vgui_surfacelib/Win32Font.cpp
    vguimatsurface/FontTextureCache.cpp
    vgui_int.cpp
)

target_link_libraries(vgui2 PUBLIC vgui2_interfaces)
target_link_libraries(vgui2 PRIVATE SourceSDK)
target_link_libraries(vgui2_support PUBLIC vgui2_interfaces)
target_link_libraries(vgui2_support PRIVATE SourceSDK)
target_link_libraries(vgui2_support PRIVATE vgui2)