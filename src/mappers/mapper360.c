/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2020
 *  Copyright (C) 2023-2024 negativeExponent
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/* NES 2.0 Mapper 360 - Bit Corp's 31-in-1 multicart (3150) */

#include "mapinc.h"

<<<<<<< HEAD
static uint8 reg;
static uint8 submapper;

static SFORMAT StateRegs[] =
{
	{ &reg, 1, "DPSW" },
=======
static uint8 dipsw;

static SFORMAT StateRegs[] = {
	{ &dipsw, 1, "DPSW" },
>>>>>>> d528b93 (Update Makefile.libretro)
	{ 0 }
};

static void Sync(void) {
	if (~reg &0x20 && submapper ==1) {
		setprg8(0x8000, 0x40);
		setprg8(0xA000, 0x40);
		setprg8(0xC000, 0x40);
		setprg8(0xE000, 0x40);
	} else
	/* dip 0 and 1 is the same game SMB) */
<<<<<<< HEAD
	if ((reg &0x1F) < 2)
		setprg32(0x8000, reg >> 1 &0x0F);
	else {
		setprg16(0x8000, reg &0x1F);
		setprg16(0xC000, reg &0x1F);
	}
	setchr8(reg);
	setmirror(((reg & 0x10) >> 4) ^ 1);
}

static DECLFW(M360WriteReg) {
	reg =V;
	Sync();
}

static void M360Power(void) {
	reg = 0;
=======
	if (dipsw < 2) {
		setprg32(0x8000, dipsw >> 1);
	} else {
		setprg16(0x8000, dipsw);
		setprg16(0xC000, dipsw);
	}
	setchr8(dipsw);
	setmirror(((dipsw & 0x10) >> 4) ^ 1);
}

static void M360Power(void) {
	dipsw = 0;
>>>>>>> d528b93 (Update Makefile.libretro)
	Sync();
	if (submapper ==1) SetWriteHandler(0x4100, 0x4FFF, M360WriteReg);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0XFFFF, CartBW);
}

static void M360Reset(void) {
<<<<<<< HEAD
	if (submapper ==0)
		reg = (reg + 1) & 31;
	else
		reg = 0;
=======
	dipsw = (dipsw + 1) & 31;
>>>>>>> d528b93 (Update Makefile.libretro)
	Sync();
	FCEU_printf("dipsw = %d\n", dipsw);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper360_Init(CartInfo *info) {
	submapper = info->submapper;
	info->Reset = M360Reset;
	info->Power = M360Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
