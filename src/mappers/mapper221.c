/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2006 CaH4e3
 *  Copyright (C) 2023-2024 negativeExponent
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR reg[0] PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Mapper 221 - UNL-N625092
 * 700in1 and 400in1 carts
 * 1000-in-1
 */

#include "mapinc.h"

static uint16 reg[2];
<<<<<<< HEAD
static uint8 submapper;

static SFORMAT StateRegs[] ={
	{ reg, 4 | FCEUSTATE_RLSB, "REGS" },
	{ 0 }
};

<<<<<<< HEAD
static DECLFR(Mapper221_ReadOB)
{
   return X.DB;
}

static void sync(void) {
	uint8 prg =reg[0] >>(submapper ==1? 2: 3) &0x40 | reg[0] >>2 &0x38 | reg[1] &0x07;
	SetReadHandler(0x8000, 0xFFFF, prg <<14 >=PRGsize[0]? Mapper221_ReadOB: CartBR); /* Selecting unpopulated banks results in open bus */
	if (reg[0] &(submapper ==1? 0x200: 0x100)) { /* UNROM */
		setprg16(0x8000, prg);
		setprg16(0xC000, prg |7);
	} else
	if (reg[0] &0x0002) /* NROM-256 */
		setprg32(0x8000, prg >>1);
	else { /* NROM-128 */
		setprg16(0x8000, prg);
		setprg16(0xC000, prg);
	}
=======
static void UNLN625092Sync(void) {
	setmirror((cmd & 1) ^ 1);
>>>>>>> 11b12c0 (Update Makefile.libretro)
	setchr8(0);
	SetupCartCHRMapping(0, CHRptr[0], 0x2000, submapper ==1? !(reg[0] &0x0400): !(reg[1] &0x0008));
	setmirror(reg[0] &0x0001? MI_H: MI_V);
}

<<<<<<< HEAD
static DECLFW(Mapper221Write) {
	reg[A >>14 &1] =A;
	sync();
}

static void UNLN625092Power(void) {
	reg[0] =reg[1] =0;
	sync();	
	SetWriteHandler(0x8000, 0xFFFF, Mapper221Write);
}

static void UNLN625092Reset(void) {
	reg[0] =reg[1] =0;
	sync();
}

static void StateRestore(int version) {
	sync();
=======
static uint16 ass = 0;

static void UNLN625092WriteCommand(uint32 A, uint8 V) {
	cmd = A;
	if (A == 0x80F8) {
		setprg16(0x8000, ass);
		setprg16(0xC000, ass);
	} else {
		UNLN625092Sync();
	}
=======

static SFORMAT StateRegs[] = {
	{ reg, 4, "REGS" },
	{ 0 }
};

static void Sync(void) {
	uint16 prg = ((reg[0] >> 3) & 0x40) | ((reg[0] >> 2) & 0x38) | (reg[1] & 0x07);

	if (!(reg[0] & 0x02)) {
		setprg16(0x8000, prg);
		setprg16(0xC000, prg);
	} else {
		if (reg[0] & 0x100) {
			setprg16(0x8000, prg);
			setprg16(0xC000, prg | 0x07);
		} else {
			setprg16(0x8000, prg & ~1);
			setprg16(0xC000, prg |  1);
		}
	}
	SetupCartCHRMapping(0, CHRptr[0], CHRsize[0], !(reg[1] & 0x08));
	setchr8(0);
	setmirror((reg[0] & 0x01) ^ 0x01);
}

static DECLFW(M221Write) {
	reg[(A >> 14) & 0x01] = A;
	Sync();
>>>>>>> d528b93 (Update Makefile.libretro)
}

static void M221Power(void) {
	reg[0] = reg[1] = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M221Write);
}

static void StateRestore(int version) {
	Sync();
}

<<<<<<< HEAD
static void UNLN625092StateRestore(int version) {
	UNLN625092Sync();
>>>>>>> 11b12c0 (Update Makefile.libretro)
}

void UNLN625092_Init(CartInfo *info) {
	submapper =info->submapper;
	info->Reset = UNLN625092Reset;
	info->Power = UNLN625092Power;
	GameStateRestore = UNLN625092StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
=======
void Mapper221_Init(CartInfo *info) {
	info->Power = M221Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
>>>>>>> d528b93 (Update Makefile.libretro)
}
