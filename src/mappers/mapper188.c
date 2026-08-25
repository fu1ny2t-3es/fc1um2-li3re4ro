/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
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

#include "mapinc.h"
#include "latch.h"

<<<<<<< HEAD
extern uint32_t ROM_size;
static uint8_t latche;

static void M188Sync(void) {
	if (latche) {
		if (latche & 0x10)
			setprg16(0x8000, (latche & 7));
		else
			setprg16(0x8000, (latche & 7) | 8);
	} else
		setprg16(0x8000, 7 + (ROM_size >> 4));
}

static void M188Write(uint32 A, uint8 V) {
	latche = V;
	M188Sync();
}

static uint8 M188ExtDev(uint32 A) { return 3; }

static void M188Power(void) {
	latche = 0;
	M188Sync();
=======
static void Sync(void) {
	if (latch.data & 0x10) {
		setprg16(0x8000, (latch.data & 0x07));
	} else {
		setprg16(0x8000, (0x08 | latch.data));
	}
	setprg16(0xC000, 0x07);
>>>>>>> 8bf4e730 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
	setchr8(0);
}

static DECLFR(ExtDev) {
	return(3);
}

static void M118Power(void) {
	Latch_Power();
	SetReadHandler(0x6000, 0x7FFF, ExtDev);
}

void Mapper188_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
	info->Power = M118Power;
}
