/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2019 Libretro Team
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

/* Mapper 237 - "Teletubbies / Y2K" 420-in-1 pirate multicart.
 * Dipswitch settings:
 * 0: 42-in-1
 * 1: 5,000-in-1
 * 2: 420-in-1
 * 3: 10,000,000-in-1 (lol)
 */

#include "mapinc.h"
#include "latch.h"

<<<<<<< HEAD
static uint8_t reg[2];
static uint8_t dipswitch;
=======
static uint8 dipsw;
>>>>>>> 5926d713 (Update libretro.c)

static SFORMAT StateRegs[] = {
	{ &dipsw, 1, "DPSW" },
	{ 0 }
};

static void Sync(void) {
<<<<<<< HEAD
	uint8_t bank = (reg[1] & 0x07);
	uint8_t base = (reg[1] & 0x18) | ((reg[0] & 0x04) << 3);
	uint8_t mode = (reg[1] & 0xC0) >> 6;
=======
	uint8 bank = (latch.data & 0x07);
	uint8 base = ((latch.addr << 3) & 0x20) | (latch.data & 0x18);
	uint8 A14  = (latch.data >> 6) & 0x01;
>>>>>>> 5926d713 (Update libretro.c)

	setprg16(0x8000, base | (bank & ~A14));
	if (latch.data & 0x80) {
		setprg16(0xC000, base | (bank | A14));
	} else {
		setprg16(0xC000, base | 0x07);
	}
	setchr8(0);
	setmirror(((latch.data & 0x20) >> 5) ^ 0x01);
}

static DECLFW(M237Write) {
	if (latch.addr & 0x02) {
		latch.data = (latch.data & 0xF8) | (V & 0x07);
	} else {
		latch.addr = A;
		latch.data = V;
	}
	Sync();
}

<<<<<<< HEAD
static uint8 M237Read(uint32 A) {
	if (!(reg[0] & 0x02) && (reg[0] & 0x01))
		return dipswitch &3;
=======
static DECLFR(M237Read) {
	if (latch.addr & 0x01) {
		return dipsw;
	}
>>>>>>> 5d22892 (Update libretro.c)
	return CartBR(A);
}

static void M237Reset(void) {
	dipsw++;
	dipsw &= 3;
	Latch_RegReset();
}

static void M237Power(void) {
	Latch_Power();
	SetWriteHandler(0x8000, 0xFFFF, M237Write);
}

<<<<<<< HEAD
static void M237Reset(void) {
	reg[0] = reg[1] = 0;
	dipswitch++;
	Sync();
}

static void StateRestore(int version) {
	Sync();
}

void Mapper237_Init(CartInfo *info) {
	/* The menu system used by this cart seems to be configurable as 4 different types:
	 * 0: 42-in-1
	 * 1: 5,000-in-1
	 * 2: 420-in-1
	 * 3: 10,000,000-in-1 (lol)
	 */
	dipswitch = 0;
=======
void Mapper237_Init(CartInfo *info) {
	Latch_Init(info, Sync, M237Read, FALSE, FALSE);
>>>>>>> 5d22892 (Update libretro.c)
	info->Power = M237Power;
	info->Reset = M237Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
