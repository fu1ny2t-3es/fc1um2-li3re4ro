/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
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

/* Various macros for faster memory stuff
	(at least that's the idea)
*/

#ifndef _FCEU_MEMORY_H_
#define _FCEU_MEMORY_H_

<<<<<<< HEAD
<<<<<<< HEAD
#include <string.h>
#include "fceu-types.h"

/* Fill 'n' bytes of 'd' with the 32-bit pattern 'c'. Uses memcpy to
 * avoid the strict-aliasing UB that the previous *(uint32_t*)& cast
 * implied; GCC and Clang both lower the do { memcpy } while loop to
 * the same single-instruction-per-iteration code as the cast did, so
 * there's no performance change. 'n' must be a multiple of 4. */
#define FCEU_dwmemset(d, c, n) do { \
	uint32_t _fcdms_v = (uint32_t)(c); \
	int _fcdms_x; \
	for (_fcdms_x = (int)(n) - 4; _fcdms_x >= 0; _fcdms_x -= 4) \
		memcpy((uint8_t *)(d) + _fcdms_x, &_fcdms_v, 4); \
} while (0)

void *FCEU_malloc(uint32_t size);
void *FCEU_gmalloc(uint32_t size);
void FCEU_gfree(void *ptr);
=======
#include <stdlib.h>
=======
>>>>>>> 8bf4e730 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
#include "fceu-types.h"

#define FCEU_dwmemset(d, c, n) { int _x; for (_x = n - 4; _x >= 0; _x -= 4) *(uint32*)& (d)[_x] = c; }

/* returns an aligned buffer  */
void *FCEU_amalloc(uint32 size);

/* returns a buffer initialized to 0 */
void *FCEU_malloc(uint32 size);
<<<<<<< HEAD
>>>>>>> 69f50aa3 (Refactors)
=======

/* returns a buffer with initialization based on FCEU_MemoryRand() */
/* Used by mappers for wram, chr ram, etc */
void *FCEU_gmalloc(uint32 size);

<<<<<<< HEAD
>>>>>>> a598ec70 (Add new FCEU_gmalloc and cleanup fceu-types.h)
=======
/* free memory allocated by FCEU_amalloc */
void FCEU_afree(void *ptr);

/* free memory allocated by FCEU_gmalloc */
void FCEU_gfree(void *ptr);

/* free memory allocated by FCEU_malloc */
>>>>>>> 8bf4e730 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
void FCEU_free(void *ptr);

#endif
