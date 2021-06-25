//===== Copyright ?1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//
//===========================================================================//
#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _XBOX
#include "xbox/xbox_platform.h"
#include "xbox/xbox_win32stubs.h"
#endif

#if !defined( DONT_PROTECT_FILEIO_FUNCTIONS )
#define DONT_PROTECT_FILEIO_FUNCTIONS // for protected_things.h
#endif

#if defined( PROTECTED_THINGS_ENABLE )
#undef PROTECTED_THINGS_ENABLE // from protected_things.h
#endif

#include <stdio.h>
#include "interface.h"
#include "tier0/basetypes.h"
#include <string.h>
#include <stdlib.h>
#include "vstdlib/strtools.h"
#include "tier0/icommandline.h"
#include "tier0/dbg.h"
#ifdef _WIN32
#include <direct.h> // getcwd
#elif _LINUX
#define _getcwd getcwd
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"



// ------------------------------------------------------------------------------------ //
// InterfaceReg.
// ------------------------------------------------------------------------------------ //
InterfaceReg *InterfaceReg::s_pInterfaceRegs = NULL;

InterfaceReg::InterfaceReg( InstantiateInterfaceFn fn, const char *pName ) :
	m_pName(pName)
{
	m_CreateFn = fn;
	m_pNext = s_pInterfaceRegs;
	s_pInterfaceRegs = this;
}

// ------------------------------------------------------------------------------------ //
// CreateInterface.
// This is the primary exported function by a dll, referenced by name via dynamic binding
// that exposes an opqaue function pointer to the interface.
// ------------------------------------------------------------------------------------ //
void* CreateInterface( const char *pName, int *pReturnCode )
{
	InterfaceReg *pCur;
	
	for (pCur=InterfaceReg::s_pInterfaceRegs; pCur; pCur=pCur->m_pNext)
	{
		if (strcmp(pCur->m_pName, pName) == 0)
		{
			if (pReturnCode)
			{
				*pReturnCode = IFACE_OK;
			}
			return pCur->m_CreateFn();
		}
	}
	
	if (pReturnCode)
	{
		*pReturnCode = IFACE_FAILED;
	}
	return NULL;	
}


#ifdef _LINUX
// Linux doesn't have this function so this emulates its functionality
void *GetModuleHandle(const char *name)
{
	void *handle;

	if( name == NULL )
	{
		// hmm, how can this be handled under linux....
		// is it even needed?
		return NULL;
	}

    if( (handle=dlopen(name, RTLD_NOW))==NULL)
    {
            printf("DLOPEN Error:%s\n",dlerror());
            // couldn't open this file
            return NULL;
    }

	// read "man dlopen" for details
	// in short dlopen() inc a ref count
	// so dec the ref count by performing the close
	dlclose(handle);
	return handle;
}
#endif

#if defined(_WIN32) && !defined(_XBOX)
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

//-----------------------------------------------------------------------------
// Purpose: returns a pointer to a function, given a module
// Input  : pModuleName - module name
//			*pName - proc name
//-----------------------------------------------------------------------------
static void *Sys_GetProcAddress( const char *pModuleName, const char *pName )
{
	return GetProcAddress( GetModuleHandle(pModuleName), pName );
}

static void *Sys_GetProcAddress( HMODULE hModule, const char *pName )
{
	return GetProcAddress( hModule, pName );
}

static bool Sys_IsDebuggerPresent()
{
#if defined(_WIN32) && !defined(_XBOX)
	static BOOL (*pfnIsDebuggerPresent)(VOID);
	static bool checked = false;
	if ( !checked )
	{
		checked = true;
		// We need to do this this way to work on win98/me without causing a run time .dll error (says Nick)
		// Win98/Me don't export this from the Kernel
		pfnIsDebuggerPresent = (BOOL (*)(VOID))Sys_GetProcAddress( "kernel32", "IsDebuggerPresent" );
	}

	if ( pfnIsDebuggerPresent )
	{
		return (*pfnIsDebuggerPresent)() ? true : false;
	}
#endif
	return false;
}

HMODULE Sys_LoadLibrary( const char *pLibraryName )
{
	char str[1024];
#if defined(_WIN32)
	const char *pModuleExtension = ".dll";
	const char *pModuleAddition = pModuleExtension;
#elif _LINUX
	const char *pModuleExtension = ".so";
	const char *pModuleAddition = "_i486.so"; // if an extension is on the filename assume the i486 binary set
#endif
	Q_strncpy(str, pLibraryName, sizeof(str));
	if ( !Q_stristr( str, pModuleExtension ) )
	{
		Q_strncat( str, pModuleAddition, sizeof(str) );
	}
	Q_FixSlashes( str );
#ifdef _WIN32
	return LoadLibrary( str );
#elif _LINUX
	return dlopen( str, RTLD_NOW );
#endif
}

//-----------------------------------------------------------------------------
// Purpose: Loads a DLL/component from disk and returns a handle to it
// Input  : *pModuleName - filename of the component
// Output : opaque handle to the module (hides system dependency)
CSysModule	*Sys_LoadModule( const char *pModuleName )
{
#if defined ( _WIN32 )
	HMODULE hDLL = LoadLibrary( pModuleName );
#else
	HMODULE hDLL  = NULL;
	char szAbsoluteModuleName[1024];
	szAbsoluteModuleName[0] = 0;
	if ( pModuleName[0] != '/' )
	{
		char szCwd[1024];
		char szAbsoluteModuleName[1024];

		getcwd( szCwd, sizeof( szCwd ) );
		if ( szCwd[ strlen( szCwd ) - 1 ] == '/' )
			szCwd[ strlen( szCwd ) - 1 ] = 0;

		_snprintf( szAbsoluteModuleName, sizeof(szAbsoluteModuleName), "%s/%s", szCwd, pModuleName );

		hDLL = dlopen( szAbsoluteModuleName, RTLD_NOW );
	}
	else
	{
		_snprintf( szAbsoluteModuleName, sizeof(szAbsoluteModuleName), "%s", pModuleName );
		 hDLL = dlopen( pModuleName, RTLD_NOW );
	}
#endif

	if( !hDLL )
	{
		char str[512];
#if defined ( _WIN32 )
		snprintf( str, sizeof(str), "%s.dll", pModuleName );
		hDLL = LoadLibrary( str );
#elif defined(OSX)
		printf("Error:%s\n",dlerror());
		_snprintf( str, sizeof(str), "%s.dylib", szAbsoluteModuleName );
		hDLL = dlopen(str, RTLD_NOW);		
#else
		printf("Error:%s\n",dlerror());
		_snprintf( str, sizeof(str), "%s.so", szAbsoluteModuleName );
		hDLL = dlopen(str, RTLD_NOW);
#endif
	}

	return reinterpret_cast<CSysModule *>(hDLL);
}


//-----------------------------------------------------------------------------
// Purpose: Unloads a DLL/component from
// Input  : *pModuleName - filename of the component
// Output : opaque handle to the module (hides system dependency)
//-----------------------------------------------------------------------------
void Sys_UnloadModule( CSysModule *pModule )
{
	if ( !pModule )
		return;

	HMODULE	hDLL = reinterpret_cast<HMODULE>(pModule);

#ifdef _WIN32
	FreeLibrary( hDLL );
#elif defined(_LINUX)
	dlclose((void *)hDLL);
#endif
}

//-----------------------------------------------------------------------------
// Purpose: returns a pointer to a function, given a module
// Input  : module - windows HMODULE from Sys_LoadModule() 
//			*pName - proc name
// Output : factory for this module
//-----------------------------------------------------------------------------
CreateInterfaceFn Sys_GetFactory( CSysModule *pModule )
{
	if ( !pModule )
		return NULL;

	HMODULE	hDLL = reinterpret_cast<HMODULE>(pModule);
#ifdef _WIN32
	return reinterpret_cast<CreateInterfaceFn>(GetProcAddress( hDLL, CREATEINTERFACE_PROCNAME ));
#elif defined(_LINUX)
	// Linux gives this error:
	//../public/interface.cpp: In function `IBaseInterface *(*Sys_GetFactory
	//(CSysModule *)) (const char *, int *)':
	//../public/interface.cpp:154: ISO C++ forbids casting between
	//pointer-to-function and pointer-to-object
	//
	// so lets get around it :)
	return (CreateInterfaceFn)(GetProcAddress( hDLL, CREATEINTERFACE_PROCNAME ));
#endif
}

//-----------------------------------------------------------------------------
// Purpose: returns the instance of this module
// Output : interface_instance_t
//-----------------------------------------------------------------------------
CreateInterfaceFn Sys_GetFactoryThis( void )
{
	return CreateInterface;
}

//-----------------------------------------------------------------------------
// Purpose: returns the instance of the named module
// Input  : *pModuleName - name of the module
// Output : interface_instance_t - instance of that module
//-----------------------------------------------------------------------------
CreateInterfaceFn Sys_GetFactory( const char *pModuleName )
{
#ifdef _WIN32
	return static_cast<CreateInterfaceFn>( Sys_GetProcAddress( pModuleName, CREATEINTERFACE_PROCNAME ) );
#elif defined(_LINUX)
	// see Sys_GetFactory( CSysModule *pModule ) for an explanation
	return (CreateInterfaceFn)( Sys_GetProcAddress( pModuleName, CREATEINTERFACE_PROCNAME ) );
#endif
}

//-----------------------------------------------------------------------------
// Purpose: get the interface for the specified module and version
// Input  : 
// Output : 
//-----------------------------------------------------------------------------
bool Sys_LoadInterface(
	const char *pModuleName,
	const char *pInterfaceVersionName,
	CSysModule **pOutModule,
	void **pOutInterface )
{
	CSysModule *pMod = Sys_LoadModule( pModuleName );
	if ( !pMod )
		return false;

	CreateInterfaceFn fn = Sys_GetFactory( pMod );
	if ( !fn )
	{
		Sys_UnloadModule( pMod );
		return false;
	}

	*pOutInterface = fn( pInterfaceVersionName, NULL );
	if ( !( *pOutInterface ) )
	{
		Sys_UnloadModule( pMod );
		return false;
	}

	if ( pOutModule )
		*pOutModule = pMod;

	return true;
}



PUBLISH_DLL_SUBSYSTEM()
