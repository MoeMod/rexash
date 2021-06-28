/*
enginecallback.h - actual engine callbacks
Copyright (C) 2010 Uncle Mike
Copyright (C) 2017 a1batross

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#pragma once
#ifndef ENGINECALLBACKS_H
#define ENGINECALLBACKS_H

#include "extdll_menu.h"
#include "Primitive.h"
#include "netadr.h"

#ifdef NEW_ENGINE_INTERFACE
#define ref_menu_params_t ref_viewpass_t
#define ref_menu_params_s ref_viewpass_s
#else
#define ref_menu_params_t ref_params_t
#define ref_menu_params_s ref_params_s
#endif

class EngFuncs
{
public:
	// image handlers
	static inline HIMAGE PIC_Load( const char *szPicName, const byte *ucRawImage, long ulRawImageSize, long flags = 0)
	{ return engfuncs.pfnPIC_Load( szPicName, ucRawImage, ulRawImageSize, flags );	}
	static inline HIMAGE PIC_Load( const char *szPicName, long flags = 0)
	{ return engfuncs.pfnPIC_Load( szPicName, 0, 0, flags ); }
	static inline void	PIC_Free( const char *szPicName )
	{ engfuncs.pfnPIC_Free( szPicName ); }
	static inline int	PIC_Width( HIMAGE hPic )
	{ return engfuncs.pfnPIC_Width( hPic ); }
	static inline int	PIC_Height( HIMAGE hPic )
	{ return engfuncs.pfnPIC_Height( hPic ); }
	static inline Size PIC_Size( HIMAGE hPic )
	{
		return Size( PIC_Width( hPic ), PIC_Height( hPic ));
	}
	static void	PIC_Set( HIMAGE hPic, int r, int g, int b, int a = 255 );

	static inline void	PIC_Draw( int x, int y, int width, int height, const wrect_t *prc = NULL )
	{ engfuncs.pfnPIC_Draw( x, y, width, height, prc ); }
	static inline void	PIC_DrawHoles( int x, int y, int width, int height, const wrect_t *prc = NULL )
	{ engfuncs.pfnPIC_DrawHoles( x, y, width, height, prc ); }
	static inline void	PIC_DrawTrans( int x, int y, int width, int height, const wrect_t *prc = NULL )
	{ engfuncs.pfnPIC_DrawTrans( x, y, width, height, prc ); }
	static inline void	PIC_DrawAdditive( int x, int y, int width, int height, const wrect_t *prc = NULL )
	{ engfuncs.pfnPIC_DrawAdditive( x, y, width, height, prc ); }
	static inline void	PIC_Draw( int x, int y, const wrect_t *prc = NULL )
	{ PIC_Draw( x, y, -1, -1, prc ); }
	static inline void	PIC_DrawHoles( int x, int y, const wrect_t *prc = NULL )
	{ PIC_DrawHoles( x, y, -1, -1, prc ); }
	static inline void	PIC_DrawTrans( int x, int y, const wrect_t *prc = NULL )
	{ PIC_DrawTrans( x, y, -1, -1, prc ); }
	static inline void	PIC_DrawAdditive( int x, int y, const wrect_t *prc = NULL )
	{ PIC_DrawAdditive( x, y, -1, -1, prc ); }

	static inline void PIC_Draw( Point p, Size s, const wrect_t *prc = NULL )
	{ PIC_Draw( p.x, p.y, s.w, s.h, prc ); }
	static inline void PIC_DrawHoles( Point p, Size s, const wrect_t *prc = NULL )
	{ PIC_DrawHoles( p.x, p.y, s.w, s.h, prc ); }
	static inline void PIC_DrawTrans( Point p, Size s, const wrect_t *prc = NULL )
	{ PIC_DrawTrans( p.x, p.y, s.w, s.h, prc ); }
	static inline void PIC_DrawAdditive( Point p, Size s, const wrect_t *prc = NULL )
	{ PIC_DrawAdditive( p.x, p.y, s.w, s.h, prc ); }
	static inline void PIC_Draw( Point p, const wrect_t *prc = NULL )
	{ PIC_Draw( p.x, p.y, prc ); }
	static inline void PIC_DrawHoles( Point p, const wrect_t *prc = NULL )
	{ PIC_DrawHoles( p.x, p.y, prc ); }
	static inline void PIC_DrawAdditive( Point p, const wrect_t *prc = NULL )
	{ PIC_DrawAdditive( p.x, p.y, prc ); }
	static inline void PIC_DrawTrans( Point p, const wrect_t *prc = NULL )
	{ PIC_DrawTrans( p.x, p.y, prc ); }

	static inline void	PIC_EnableScissor( int x, int y, int width, int height )
	{ engfuncs.pfnPIC_EnableScissor( x, y, width, height ); }

	static inline void	PIC_DisableScissor( void )
	{ engfuncs.pfnPIC_DisableScissor(); }

	// screen handlers
	static inline void	FillRGBA( int x, int y, int width, int height, int r, int g, int b, int a )
	{ return engfuncs.pfnFillRGBA(x, y, width, height, r, g, b, a); }

	// cvar handlers
	static inline cvar_t*	CvarRegister( const char *szName, const char *szValue, int flags )
	{ return engfuncs.pfnRegisterVariable( szName, szValue, flags ); }

	static inline float	GetCvarFloat( const char *szName )
	{ return engfuncs.pfnGetCvarFloat( szName ); }
	static inline const char*	GetCvarString( const char *szName )
	{ return engfuncs.pfnGetCvarString( szName ); }
	static inline void	CvarSetString( const char *szName, const char *szValue )
	{ engfuncs.pfnCvarSetString( szName, szValue ); }
	static inline void	CvarSetValue( const char *szName, float flValue )
	{ engfuncs.pfnCvarSetValue( szName, flValue ); }

	// command handlers
	static inline int	Cmd_AddCommand( const char *cmd_name, void function(void) )
	{ return engfuncs.pfnAddCommand( cmd_name, function ); }
	static inline void	ClientCmd( int execute_now, const char *szCmdString )
	{ engfuncs.pfnClientCmd( execute_now, szCmdString ); }
	static inline void	Cmd_RemoveCommand( const char *cmd_name )
	{ engfuncs.pfnDelCommand( cmd_name ); }
	static inline int   CmdArgc( void )
	{ return engfuncs.pfnCmdArgc(); }
	static inline const char*	CmdArgv( int argi )
	{ return engfuncs.pfnCmdArgv( argi ); }
	static inline const char*	CmdArgs( void )
	{ return engfuncs.pfnCmd_Args(); }

	// sound handlers
	static inline void	PlayLocalSound( const char *szSound )
	{ engfuncs.pfnPlayLocalSound( szSound ); }

	// cinematic handlers
	static void	DrawLogo( const char *filename, float x, float y, float width, float height );
	static inline void	PrecacheLogo( const char *filename )
	{ engfuncs.pfnDrawLogo( filename, 0, 0, 0, 0 ); }
	static inline int	GetLogoWidth( void )
	{ return engfuncs.pfnGetLogoWidth(); }
	static inline int	GetLogoHeight( void )
	{ return engfuncs.pfnGetLogoHeight(); }
	static inline float	GetLogoLength( void ) // cinematic duration in seconds
	{ return engfuncs.pfnGetLogoLength(); }

	// text message system
	static void	DrawCharacter( int x, int y, int width, int height, int ch, int ulRGBA, HIMAGE hFont );
	static int DrawConsoleString( int x, int y, const char *string );
	static void	DrawSetTextColor( int r, int g, int b, int alpha = 255 );
	static void	ConsoleStringLen(  const char *string, int *length, int *height );
	static int   ConsoleCharacterHeight();

	static inline int   DrawConsoleString( Point coord, const char *string )
	{ return DrawConsoleString( coord.x, coord.y, string ); }

	static inline void	SetConsoleDefaultColor( int r, int g, int b ) // color must came from colors.lst
	{
		engfuncs.pfnSetConsoleDefaultColor( r, g, b );
	}

	// TODO: Move into UI class

	// custom rendering (for playermodel preview)
	static inline struct cl_entity_s* GetPlayerModel( void )	// for drawing playermodel previews
	{ return engfuncs.pfnGetPlayerModel(); }
	static inline void	SetModel( struct cl_entity_s *ed, const char *path )
	{  engfuncs.pfnSetModel( ed, path ); }
	static inline void	ClearScene( void )
	{  engfuncs.pfnClearScene(); }
	static inline void	RenderScene( const struct ref_menu_params_s *fd )
	{  engfuncs.pfnRenderScene( fd ); }
	static inline int	CL_CreateVisibleEntity( int type, struct cl_entity_s *ent )
	{  return engfuncs.CL_CreateVisibleEntity( type, ent ); }

	// misc handlers
	// static inline void	HostError( const char *szFmt, ... );
	static inline int	FileExists( const char *filename, int gamedironly = 0 )
	{ return engfuncs.pfnFileExists( filename, gamedironly ); }
	static inline void	GetGameDir( char *szGetGameDir )
	{ engfuncs.pfnGetGameDir( szGetGameDir ); }

	// gameinfo handlers
	static inline int	CreateMapsList( int iRefresh )
	{ return engfuncs.pfnCreateMapsList( iRefresh ); }
	static inline int	ClientInGame( void )
	{  return engfuncs.pfnClientInGame(); }
	static inline void	ClientJoin( const struct netadr_s adr )
	{  engfuncs.pfnClientJoin( adr ); }

	// parse txt files
	static inline byte*	COM_LoadFile( const char *filename, int *pLength = 0 )
	{ return engfuncs.COM_LoadFile( filename, pLength ); }
	static inline char*	COM_ParseFile( char *data, char *token )
	{ return engfuncs.COM_ParseFile( data, token ); }
	static inline void	COM_FreeFile( void *buffer )
	{  engfuncs.COM_FreeFile( buffer ); }

	// keyfuncs
	static inline void	KEY_ClearStates( void ) // call when menu open or close
	{ engfuncs.pfnKeyClearStates(); }
	static inline void	KEY_SetDest( int dest )
	{ engfuncs.pfnSetKeyDest( dest ); }
	static inline const char *KeynumToString( int keynum )
	{ return  engfuncs.pfnKeynumToString( keynum ); }
	static inline const char *KEY_GetBinding( int keynum )
	{ return engfuncs.pfnKeyGetBinding( keynum ); }
	static inline void	KEY_SetBinding( int keynum, const char *binding )
	{ engfuncs.pfnKeySetBinding( keynum, binding ); }
	static inline int	KEY_IsDown( int keynum )
	{ return  engfuncs.pfnKeyIsDown( keynum ); }
	static inline int	KEY_GetOverstrike( void )
	{ return  engfuncs.pfnKeyGetOverstrikeMode(); }

	static inline void	KEY_SetOverstrike( int fActive )
	{  engfuncs.pfnKeySetOverstrikeMode( fActive ); }
	static inline void	*KEY_GetState( const char *name )			// for mlook, klook etc
	{ return engfuncs.pfnKeyGetState( name ); }

	// engine memory manager
	static inline void*	MemAlloc( size_t cb, const char *filename, const int fileline )
	{ return engfuncs.pfnMemAlloc( cb, filename, fileline ); }
	static inline void	MemFree( void *mem, const char *filename, const int fileline )
	{ engfuncs.pfnMemFree( mem, filename, fileline ); }

	// collect info from engine
	static inline int	GetGameInfo( GAMEINFO *pgameinfo )
	{ return engfuncs.pfnGetGameInfo( pgameinfo ); }
	static inline GAMEINFO	**GetGamesList( int *numGames ) // collect info about all mods
	{ return  engfuncs.pfnGetGamesList( numGames ); }
	static inline char 	**GetFilesList( const char *pattern, int *numFiles, int gamedironly ) // find in files
	{ return engfuncs.pfnGetFilesList( pattern, numFiles, gamedironly ); }
	static inline int 	GetSaveComment( const char *savename, char *comment )
	{ return  engfuncs.pfnGetSaveComment( savename, comment ); }
	static inline int	GetDemoComment( const char *demoname, char *comment )
	{return  engfuncs.pfnGetDemoComment( demoname, comment ); }
	static inline int	CheckGameDll( void )	// returns false if hl.dll is missed or invalid
	{ return engfuncs.pfnCheckGameDll(); }
	static inline char	*GetClipboardData( void )
	{ return  engfuncs.pfnGetClipboardData(); }

	// engine launcher
	static inline void	ShellExecute( const char *name, const char *args, int closeEngine )
	{ engfuncs.pfnShellExecute( name, args, closeEngine ); }
	static inline void	WriteServerConfig( const char *name )
	{ engfuncs.pfnWriteServerConfig( name ); }
	static inline void	ChangeInstance( const char *newInstance, const char *szFinalMessage )
	{ engfuncs.pfnChangeInstance( newInstance, szFinalMessage ); }
	static inline void	PlayBackgroundTrack( const char *introName, const char *loopName )
	{ engfuncs.pfnPlayBackgroundTrack( introName, loopName ); }
	static inline void  StopBackgroundTrack( )
	{ engfuncs.pfnPlayBackgroundTrack( NULL, NULL ); }
	static inline void	HostEndGame( const char *szFinalMessage )
	{ engfuncs.pfnHostEndGame( szFinalMessage ); }

	// menu interface is freezed at version 0.75
	// new functions starts here
	static inline float	RandomFloat( float flLow, float flHigh )
	{ return engfuncs.pfnRandomFloat( flLow, flHigh ); }
	static inline int	RandomLong( int lLow, int lHigh )
	{ return engfuncs.pfnRandomLong( lLow, lHigh ); }

	static inline void	SetCursor( void *hCursor ) // change cursor
	{ engfuncs.pfnSetCursor( hCursor ); }
	static inline int	IsMapValid( const char *filename )
	{ return engfuncs.pfnIsMapValid( (char*)filename ); }
	static inline void	ProcessImage( int texnum, float gamma, int topColor = -1, int bottomColor = -1 )
	{ engfuncs.pfnProcessImage( texnum, gamma, topColor, bottomColor ); }
	static inline int	CompareFileTime( char *filename1, char *filename2, int *iCompare )
	{ return engfuncs.pfnCompareFileTime( filename1, filename2, iCompare ); }
	static inline const char *GetModeString( int mode )
	{ return engfuncs.pfnGetModeString( mode ); }
	static inline int COM_SaveFile( const char *filename, const void *buffer, int len )
	{
#ifdef NEW_ENGINE_INTERFACE
		return engfuncs.COM_SaveFile( filename, buffer, len );
#else
		return false;
#endif
	}
	static inline int DeleteFile( const char *filename )
	{
#ifdef NEW_ENGINE_INTERFACE
		return engfuncs.COM_RemoveFile( filename );
#else
		return false;
#endif
	}
	static ui_enginefuncs_t engfuncs;

	// text funcs
#ifndef XASH_DISABLE_FWGS_EXTENSIONS
	static ui_textfuncs_t textfuncs;
	static inline void EnableTextInput( int enable )
	{ if( textfuncs.pfnEnableTextInput ) textfuncs.pfnEnableTextInput( enable ); }
#else
	static inline void EnableTextInput( int enable ) { }
#endif
	static int UtfProcessChar(int ch) { return ch; }
	static int UtfMoveLeft( const char *str, int pos );
	static int UtfMoveRight( const char *str, int pos, int length );
};


// built-in memory manager
// NOTE: not recommeded to use, because object destruction may be after engine halts
#define MALLOC( x )		EngFuncs::MemAlloc( x, __FILE__, __LINE__ )
#define CALLOC( x, y )	EngFuncs::MemAlloc((x) * (y), __FILE__, __LINE__ )
#define FREE( x )		EngFuncs::MemFree( x, __FILE__, __LINE__ )

#define CL_IsActive()	(EngFuncs::ClientInGame() && !EngFuncs::GetCvarFloat( "cl_background" ))
#define Host_Error (*EngFuncs::engfuncs.pfnHostError)
#define Con_DPrintf (*EngFuncs::engfuncs.Con_DPrintf)
#define Con_NPrintf (*EngFuncs::engfuncs.Con_NPrintf)
#define Con_NXPrintf (*EngFuncs::engfuncs.Con_NXPrintf)
#define Con_Printf (*EngFuncs::engfuncs.Con_Printf)

#endif // ENGINECALLBACKS_H
