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

#include <stdlib.h>
#include <string.h>

#include "fceu-types.h"
#include "fceu.h"
#include "fceu-memory.h"
#include "general.h"

<<<<<<< HEAD
<<<<<<< HEAD
void *FCEU_gmalloc(uint32_t size)
{
   void *ret = malloc(size);
   if (!ret)
   {
      /* Returning NULL is a behaviour change from the old exit(1)
       * semantics, but a libretro core must not exit() because that
       * tears down the entire frontend. All FCEU_gmalloc call sites
       * are now NULL-aware - they propagate the failure up to the
       * loader which logs an error and refuses to load the cart. */
      FCEU_PrintError("Error allocating memory!");
      return NULL;
   }
   memset(ret, 0, size);
   return ret;
}

void *FCEU_malloc(uint32_t size)
=======
void *FCEU_malloc(uint32 size)
>>>>>>> 9c0002ac (Refactors)
{
   void *ret = (void*)malloc(size);
   if (!ret)
<<<<<<< HEAD
   {
      FCEU_PrintError("Error allocating memory!");
      return NULL;
   }
=======
      ret = 0;
>>>>>>> 9c0002ac (Refactors)
   memset(ret, 0, size);
   return ret;
=======
#include "memalign.h"

void *FCEU_amalloc(uint32 size) {
	void *ret = memalign_alloc(256, size);
	if (!ret) {
		FCEU_PrintError("Error allocating memory!  Doing a hard exit.");
		exit(1);
	}
	return ret;
>>>>>>> a1c8c17c (Update libretro_core_options.h)
}

void *FCEU_gmalloc(uint32 size) {
	void *ret = malloc(size);
	if (!ret) {
		FCEU_PrintError("Error allocating memory!  Doing a hard exit.");
		exit(1);
	}
	FCEU_MemoryRand((uint8 *)ret, size);
	return ret;
}

void *FCEU_malloc(uint32 size) {
	void *ret = (void *)malloc(size);

	if (!ret) {
		FCEU_PrintError("Error allocating memory!");
		ret = 0;
	}
	memset(ret, 0, size);
	return ret;
}

void FCEU_afree(void *ptr) {
	memalign_free(ptr);
}

void FCEU_free(void *ptr) {
	free(ptr);
}

void FCEU_gfree(void *ptr) {
	free(ptr);
}
