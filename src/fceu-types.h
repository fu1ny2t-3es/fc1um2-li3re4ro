/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2001 Aaron Oneal
 *  Copyright (C) 2002 Xodnizel
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __FCEU_TYPES_H
#define __FCEU_TYPES_H

#include <stdint.h>
<<<<<<< HEAD
<<<<<<< HEAD

/* The custom int8/int16/int32/int64/uint8/uint16/uint32/uint64 typedefs
 * that lived here have been replaced throughout the codebase with the
 * C99 stdint.h equivalents (int8_t/uint8_t/etc). This header is kept
 * for the GINLINE / INLINE / FASTAPASS / FP_FASTAPASS macros and the
 * readfunc/writefunc function-pointer typedefs that depend on them. */

#ifdef __GNUC__
	#define GINLINE inline
#elif MSVC | _MSC_VER
	#define GINLINE		/* Can't declare a function INLINE
						 * and global in MSVC.  Bummer.
						 */
#endif
=======
=======
#include <retro_inline.h>

>>>>>>> 7b06ddc4 (Update libretro.c)
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
<<<<<<< HEAD
typedef uint64_t uint64;
>>>>>>> b1a7b656 (Add new FCEU_gmalloc and cleanup fceu-types.h)
=======
>>>>>>> 7b06ddc4 (Update libretro.c)

#ifdef __GNUC__
typedef unsigned long long uint64;
typedef long long int64;
#elif MSVC | _MSC_VER
typedef __int64 int64;
typedef unsigned __int64 uint64;
#else
typedef unsigned long long uint64;
typedef long long int64;
#endif

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef __GNUC__
	#ifdef C80x86
		#define FASTAPASS(x) __attribute__((regparm(x)))
		#define FP_FASTAPASS FASTAPASS
	#else
		#define FASTAPASS(x)
		#define FP_FASTAPASS(x)
	#endif
#elif MSVC
	#define FP_FASTAPASS(x)
	#define FASTAPASS(x) __fastcall
#else
	#define FP_FASTAPASS(x)
	#define FASTAPASS(x)
#endif

typedef void (FP_FASTAPASS(2) *writefunc)(uint32_t A, uint8_t V);
typedef uint8_t (FP_FASTAPASS(1) *readfunc)(uint32_t A);
=======
typedef void (*writefunc)(uint32 A, uint8 V);
typedef uint8 (*readfunc)(uint32 A);
>>>>>>> 74114ad0 (Update libretro.c)
=======
#define FCEU_UNUSED(x)    (void)(x)
#define FCEU_MAYBE_UNUSED __attribute__((unused))

#if !defined(FALSE)
#define FALSE 0
#endif

#if !defined(TRUE)
#define TRUE 1
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

typedef void (*writefunc)(uint16 A, uint8 V);
typedef uint8 (*readfunc)(uint16 A);
>>>>>>> 7b06ddc4 (Update libretro.c)

#endif
