/*
dll_int.cpp - dll entry point
Copyright (C) 2010 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/


#include "vgui_api.h"
#include "cdll_int.h"
#include "menu_int.h"
#include "gameinfo.h"
#include "keydefs.h"
#include "netadr.h"

#include <FileSystem.h>
#include "IBaseUI.h"
#include "vgui/IInput.h"
#include "vgui/IInputInternal.h"
#include "vgui/KeyCode.h"
#include "vgui/MouseCode.h"
#include "BaseUISurface.h"

#include "GameUI/IGameUI.h"

#include "keydest.h"

static IBaseUI* staticUIFuncs;
extern IGameUI* staticGameUIFuncs;
extern cl_enginefunc_t gEngfuncs;
extern BaseUISurface* staticSurface;
extern qboolean g_bScissor;
extern vgui2::IInputInternal* g_pInputInternal;

namespace ui
{
	ui_enginefuncs_t engfuncs;
#ifndef XASH_DISABLE_FWGS_EXTENSIONS
	ui_textfuncs_t	textfuncs;
#endif
}

namespace ui
{
	ui_globalvars_t	*gpGlobals;
}

static bool pending_activate = false;

void UI_Init(void)
{
	CreateInterfaceFn pEngineFactory = Sys_GetFactoryThis();
	staticUIFuncs = (IBaseUI*)pEngineFactory(BASEUI_INTERFACE_VERSION, NULL);
}
int UI_VidInit(void)
{
	return 0;
}
void UI_Shutdown(void)
{

}
void UI_KeyEvent(int key, int down)
{
	if (!staticUIFuncs || !staticGameUIFuncs)
	{
		return;
	}
	
	bool want_text_input = staticUIFuncs->Key_Event(down, key, "");
	ui::textfuncs.pfnEnableTextInput(want_text_input);
}
void UI_MouseMove(int x, int y)
{

}
void UI_SetActiveMenu(int fActive)
{
	if (!staticUIFuncs || !staticGameUIFuncs)
	{
		pending_activate = fActive;
		return;
	}
	pending_activate = false;

	ui::engfuncs.pfnKeyClearStates();

	if (fActive)
		staticUIFuncs->ActivateGameUI();
	else
		staticUIFuncs->HideGameUI();
}
void UI_UpdateMenu(float flTime)
{
	if (pending_activate)
	{
		// real VidInit goes here
		UI_SetActiveMenu(true);
		staticUIFuncs->ShowConsole();
	}
	
	if (!staticUIFuncs) {
		return;
	}
	
	int wide, tall;
	staticSurface->GetScreenSize(wide, tall);
	g_bScissor = true;
	staticUIFuncs->Paint(0, 0, wide, tall);
	g_bScissor = false;
}
void UI_AddServerToList(netadr_t adr, const char* info)
{

}
void UI_GetCursorPos(int* pos_x, int* pos_y)
{

}
void UI_SetCursorPos(int pos_x, int pos_y)
{

}
void UI_ShowCursor(int show)
{

}
static int utf8charlen(int in)
{
	if (in >= 0xF8)
		return 0;
	else if (in >= 0xF0)
		return 4;
	else if (in >= 0xE0)
		return 3;
	else if (in >= 0xC0)
		return 2;
	else if (in <= 0x7F)
		return 1; //ascii
}
void UI_CharEvent(int utf8)
{
	static int iCharCounter = 0;
	static int iInputIndex = 0;
	static char szInput[5] = {};

	if(!iCharCounter)
	{
		iCharCounter = utf8charlen(utf8);
		iInputIndex = 0;
		szInput[0] = 0;
	}

	if (!iCharCounter)
		return;
	
	szInput[iInputIndex] = (char)utf8;
	++iInputIndex;
	--iCharCounter;
	if(!iCharCounter)
	{
		wchar_t unicode[16] = {};
		V_UTF8ToUnicode(szInput, unicode, 16);
		for(int i = 0; unicode[i]; ++i)
			g_pInputInternal->InternalKeyTyped(unicode[i]);
	}
}
int UI_MouseInRect(void)
{
	return 0;
}
int UI_IsVisible(void)
{
	return staticUIFuncs->IsGameUIVisible();
}
int UI_CreditsActive(void)
{
	return 0;
}
void UI_FinalCredits(void)
{

}

static UI_FUNCTIONS gFunctionTable = 
{
	UI_VidInit,
	UI_Init,
	UI_Shutdown,
	UI_UpdateMenu,
	UI_KeyEvent,
	UI_MouseMove,
	UI_SetActiveMenu,
	UI_AddServerToList,
	UI_GetCursorPos,
	UI_SetCursorPos,
	UI_ShowCursor,
	UI_CharEvent,
	UI_MouseInRect,
	UI_IsVisible,
	UI_CreditsActive,
	UI_FinalCredits
};

//=======================================================================
//			GetApi
//=======================================================================
extern "C" EXPORT int GetMenuAPI(UI_FUNCTIONS *pFunctionTable, ui_enginefuncs_t* pEngfuncsFromEngine, ui_globalvars_t *pGlobals)
{
	if( !pFunctionTable || !pEngfuncsFromEngine )
	{
		return FALSE;
	}

	// copy HUD_FUNCTIONS table to engine, copy engfuncs table from engine
	memcpy( pFunctionTable, &gFunctionTable, sizeof( UI_FUNCTIONS ));
	memcpy( &ui::engfuncs, pEngfuncsFromEngine, sizeof( ui_enginefuncs_t ));
#ifndef XASH_DISABLE_FWGS_EXTENSIONS
	memset( &ui::textfuncs, 0, sizeof( ui_textfuncs_t ));
#endif
	ui::gpGlobals = pGlobals;

	return TRUE;
}

#ifndef XASH_DISABLE_FWGS_EXTENSIONS
extern "C" EXPORT int GiveTextAPI( ui_textfuncs_t* pTextfuncsFromEngine )
{
	if( !pTextfuncsFromEngine )
	{
		return FALSE;
	}

	// copy HUD_FUNCTIONS table to engine, copy engfuncs table from engine
	memcpy( &ui::textfuncs, pTextfuncsFromEngine, sizeof( ui_textfuncs_t ));

	return TRUE;
}
extern "C" EXPORT void AddTouchButtonToList( const char *name, const char *texture, const char *command, unsigned char *color, int flags ); // TouchButtons.cpp
#endif

#ifdef XASH_STATIC_GAMELIB
typedef struct dllexport_s
{
const char *name;
void *func;
} dllexport_t;

static dllexport_t switch_mainui_exports[] = {
	{ "GetMenuAPI", (void*)GetMenuAPI },
	{ "GiveTextAPI", (void*)GiveTextAPI },
	{ "AddTouchButtonToList", (void*)AddTouchButtonToList },
	{ NULL, NULL }
};

extern "C" int dll_register( const char *name, dllexport_t *exports );

extern "C" int switch_installdll_mainui( void )
{
	return dll_register( "menu", switch_mainui_exports );
}
#endif
