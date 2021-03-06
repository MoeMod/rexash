//========= Copyright (C) 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#ifndef VSTDLIB_IKEYVALUESSYSTEM_H
#define VSTDLIB_IKEYVALUESSYSTEM_H
#ifdef _WIN32
#pragma once
#endif

#include "tier1/interface.h"
#include "vstdlib/vstdlib.h"

// handle to a KeyValues key name symbol
typedef int HKeySymbol;
#define INVALID_KEY_SYMBOL (-1)

typedef int HLocalized;
#define INVALID_LOCALIZED (-1)

//-----------------------------------------------------------------------------
// Purpose: Interface to shared data repository for KeyValues (included in vgui_controls.lib)
//			allows for central data storage point of KeyValues symbol table
//-----------------------------------------------------------------------------
class IKeyValuesSystem : public IBaseInterface
{
public:
	virtual void RegisterSizeofKeyValues(int size) = 0;
	virtual void *AllocKeyValuesMemory(int size) = 0;
	virtual void FreeKeyValuesMemory(void *pMem) = 0;
	virtual HKeySymbol GetSymbolForString(const char *name) = 0;
	virtual const char *GetStringForSymbol(HKeySymbol symbol) = 0;
	virtual HLocalized GetLocalizedFromANSI(const char *string) = 0;
	virtual const char *GetANSIFromLocalized(HLocalized l) = 0;
	
	virtual void AddKeyValuesToMemoryLeakList(void *pMem, HKeySymbol name) = 0;
	virtual void RemoveKeyValuesFromMemoryLeakList(void *pMem) = 0;
};

VSTDLIB_INTERFACE IKeyValuesSystem* KeyValuesSystem();

#define KEYVALUESSYSTEM_INTERFACE_VERSION "KeyValues003"

#endif // VSTDLIB_IKEYVALUESSYSTEM_H
