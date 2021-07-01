/*
vgui_draw.h - vgui draw methods
Copyright (C) 2011 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#ifndef VGUI_DRAW_H
#define VGUI_DRAW_H

#include "port.h"

#define VGUI_MAX_TEXTURES	2048	// a half of total textures count

//
// vgui_draw.c
//
void VGui_Startup( int width, int height );
void VGui_Shutdown( void );
void VGui_Paint();
void VGui_RunFrame();
void VGui_KeyEvent( int key, int down );
void VGui_MouseMove( int x, int y );
qboolean VGui_IsActive( void );
void *pfnVGui_GetPanel();

#endif//VGUI_DRAW_H
