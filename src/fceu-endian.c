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

/*  Contains file I/O functions that write/read data    */
/*  LSB first.              */

#include <string.h>

#include <streams/memory_stream.h>

#include "fceu-memory.h"
#include "fceu-types.h"
#include "fceu-endian.h"

<<<<<<< HEAD
uint32_t FCEU_de32lsb(const uint8_t *morp)
{
<<<<<<< HEAD
#ifndef MSB_FIRST
   uint32_t v;
   memcpy(&v, morp, 4);
   return v;
=======
   uint8 *start = src;
   uint8 *end = src + count - 1;

   if ((count & 1) || !count)
      return;     /* This shouldn't happen. */

   while (count--)
   {
      uint8 tmp = *end;
      *end = *start;
      *start = tmp;
      end--;
      start++;
   }
}

=======
>>>>>>> 3ef6de2c (Refactors)
int write32le_mem(uint32 b, memstream_t *mem)
{
   uint8 s[4];
   s[0]=b;
   s[1]=b>>8;
   s[2]=b>>16;
   s[3]=b>>24;
   return((memstream_write(mem, s, 4)<4)?0:4);
}

int read32le_mem(uint32 *Bufo, memstream_t *mem)
{
   uint32 buf;
   if(memstream_read(mem, &buf, 4)<4)
      return 0;
#ifdef MSB_FIRST
   *(uint32*)Bufo=((buf&0xFF)<<24)|((buf&0xFF00)<<8)|((buf&0xFF0000)>>8)|((buf&0xFF000000)>>24);
>>>>>>> 3cd7a223 (Update libretro_core_options.h)
#else
   return(morp[0] | (morp[1] << 8) | (morp[2] << 16) | (morp[3] << 24));
#endif
}
