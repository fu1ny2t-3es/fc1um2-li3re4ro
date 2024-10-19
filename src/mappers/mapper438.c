/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2012 CaH4e3
 *  Copyright (C) 2002 Xodnizel
 *  Copyright (C) 2023-2024 negativeExponent
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

/* K-3071 */

#include "mapinc.h"
#include "latch.h"

static void Sync(void) {
	uint16 prg = latch.addr >> 1;
	uint16 chr = latch.data >> 1;
	uint16 mirr = (latch.data & 0x01) ^ 0x01;

	if (latch.addr & 1)
		setprg32(0x8000, prg >> 1);
	else {
		setprg16(0x8000, prg);
		setprg16(0xC000, prg);
	}
	setchr8(chr);
	setmirror(mirr);
}

void Mapper438_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
	info->Reset = Latch_RegReset;
}
