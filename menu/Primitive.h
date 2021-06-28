/*
Primitive.h - menu primitives
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
#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#define BIT( x ) ( 1U << x )

#ifdef _MSC_VER
#pragma warning(disable:4244) // float->int
#endif

template<class T>
inline bool isrange( T min, T value, T max )
{ return (((value) >= (min))) && (((value) <= (max))); };

#define bound( min, num, max )	((num) >= (min) ? ((num) < (max) ? (num) : (max)) : (min))
#define Q_min( a, b ) (((a) < (b)) ? (a) : (b))
#define Q_max( a, b ) (((a) < (b)) ? (b) : (a))

// engine constants
enum
{
	GAME_NORMAL = 0,
	GAME_SINGLEPLAYER_ONLY,
	GAME_MULTIPLAYER_ONLY
};

enum
{
	KEY_CONSOLE = 0,
	KEY_GAME,
	KEY_MENU
};

enum
{
	QMF_GRAYED             = BIT( 1 ), // Grays and disables
	QMF_INACTIVE           = BIT( 2 ), // Disables any input
	QMF_DROPSHADOW         = BIT( 4 ),
	QMF_SILENT             = BIT( 5 ), // Don't play sounds
	QMF_HASMOUSEFOCUS      = BIT( 6 ),
	QMF_MOUSEONLY          = BIT( 7 ), // Only mouse input allowed
	QMF_NOTIFY             = BIT( 9 ), // draw notify at right screen side
	QMF_ACT_ONRELEASE      = BIT( 10 ), // call Key_Event when button is released
	QMF_HASKEYBOARDFOCUS   = BIT( 11 ),
	QMF_DIALOG             = BIT( 12 ), // modal windows. Will grab key, char and mousemove events
	QMF_DISABLESCAILING    = BIT( 13 ), // disables CalcPosition and CalcSizes
	QMF_EVENTSIGNOREFOCUS  = BIT( 14 ), // don't care if item have focus, it must get events anyway. NOTE: they cannot issue sounds

	QMF_HIDDENBYPARENT     = BIT( 30 ), // INTERNAL USE ONLY: parent set this flag and don't want to draw this control
	QMF_HIDDEN             = BIT( 31 ), // INTERNAL USE ONLY: Use Show/Hide/SetVisibility/ToggleVisibility
};


enum ETextAlignment
{
	QM_CENTER		 = 0,
	QM_TOP           = BIT( 0 ),
	QM_BOTTOM        = BIT( 1 ),
	QM_LEFT          = BIT( 2 ),
	QM_RIGHT         = BIT( 3 ),

	QM_TOPLEFT       = QM_TOP     | QM_LEFT,
	QM_TOPRIGHT      = QM_TOP     | QM_RIGHT,
	QM_BOTTOMLEFT    = QM_BOTTOM  | QM_LEFT,
	QM_BOTTOMRIGHT   = QM_BOTTOM  | QM_RIGHT,
};

enum EFocusAnimation
{
	QM_NOFOCUSANIMATION = 0,
	QM_HIGHLIGHTIFFOCUS,      // just simple hightlight
	QM_PULSEIFFOCUS           // pulse animation
};

enum ELetterCase
{
	QM_NOLETTERCASE = 0,
	QM_LOWERCASE,
	QM_UPPERCASE
};

enum ERenderMode
{
	QM_DRAWNORMAL = 0, // normal RGB picture, ignore alpha
	QM_DRAWHOLES,      // holes
	QM_DRAWTRANS,      // RGBA
	QM_DRAWADDITIVE    // additive
};

struct Size;

struct Point
{
	Point() : x(0), y(0) {}
	Point( int x, int y ) : x(x), y(y) {}

	int x, y;
	Point Scale();
	friend Point operator +( const Point &a, const Point &b )
	{
		return Point( a.x + b.x, a.y + b.y );
	}

	friend Point operator -( const Point &a, const Point &b )
	{
		return Point( a.x - b.x, a.y - b.y );
	}

	Point& operator+=( const Point &a )
	{
		x += a.x;
		y += a.y;
		return *this;
	}

	Point& operator-=( const Point &a )
	{
		x -= a.x;
		y -= a.y;
		return *this;
	}

	Point& operator +=( const Size &a );
	Point& operator -=( const Size &b );

	Point operator *( const float scale ) { return Point( (int)(x * scale), (int)(y * scale) ); }
	Point operator /( const float scale ) { return Point( (int)(x / scale), (int)(y / scale) ); }
};

struct Size
{
	Size() : w(0), h(0) {}
	Size( int w, int h ) : w(w), h(h) {}

	friend Size operator +( const Size &a, const Size &b )
	{
		return Size( a.w + b.w, a.h + b.h );
	}

	friend Size operator -( const Size &a, const Size &b )
	{
		return Size( a.w - b.w, a.h - b.h );
	}

	Size AddVertical( const Size &a )
	{
		return Size( w, a.h + h );
	}

	Size AddHorizontal( const Size &a )
	{
		return Size( w + a.w, h );
	}

	int w, h;
	Size Scale();

	Size operator *( const float scale ) { return Size( (int)(w * scale), (int)(h * scale) ); }
	Size operator /( const float scale ) { return Size( (int)(w / scale), (int)(h / scale) ); }
};

inline Point operator +(const Point &a, const Size &b)
{
	return Point( a.x + b.w, a.y + b.h );
}

inline Point operator -(const Point &a, const Size &b)
{
	return Point( a.x + b.w, a.y + b.h );
}

inline Point& Point::operator +=(const Size &a)
{
	x += a.w;
	y += a.h;
	return *this;
}

inline Point& Point::operator -=(const Size &a)
{
	x -= a.w;
	y -= a.h;
	return *this;
}

#endif // PRIMITIVE_H
