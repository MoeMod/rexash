
add_library(GameUI SHARED
	BackgroundMenuButton.cpp
	BasePanel.cpp
	BitmapImagePanel.cpp
	CreateMultiplayerGameBotPage.cpp
	CreateMultiplayerGameDialog.cpp
	CreateMultiplayerGameGameplayPage.cpp
	CreateMultiplayerGameServerPage.cpp
	CvarNegateCheckButton.cpp
	CvarSlider.cpp
	CvarTextEntry.cpp
	CvarToggleCheckButton.cpp
	GameConsole.cpp
	GameConsoleDialog.cpp
	GameLoading.cpp
	GameUI_Interface.cpp
	KeyToggleCheckButton.cpp
	LabeledCommandComboBox.cpp
	LoadingDialog.cpp
	LogoFile.cpp
	ModInfo.cpp
	MultiplayerAdvancedDialog.cpp
	OptionsDialog.cpp
	OptionsSubAdvanced.cpp
	OptionsSubAudio.cpp
	OptionsSubKeyboard.cpp
	OptionsSubMouse.cpp
	OptionsSubMultiplayer.cpp
	OptionsSubVideo.cpp
	OptionsSubVoice.cpp
	PanelListPanel.cpp
	ScriptObject.cpp
	ToolBar.cpp
	URLButton.cpp
	VControlsListPanel.cpp
)

target_link_libraries(GameUI PUBLIC vgui_controls)