/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
<<<<<<< HEAD
 *  Copyright (C) 2025 NewRisingSun
=======
 *  Copyright (C) 2005 CaH4e3
 *  Copyright (C) 2023-2024 negativeExponent
>>>>>>> 4fb69f6 (Update Makefile.libretro)
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
<<<<<<< HEAD
=======
 *
 * iNES mapper 183
 * Gimmick Bootleg (VRC4 mapper)
>>>>>>> 4fb69f6 (Update Makefile.libretro)
 */

#include "mapinc.h"
#include "asic_vrc2and4.h"

<<<<<<< HEAD
static uint8_t  prg;
=======
<<<<<<< HEAD
static uint8  prg;
>>>>>>> ae14339e (Update Makefile.libretro)

static SFORMAT stateRegs[] ={
	{ &prg, 1, "PRG6" },
	{ 0 }
};

<<<<<<< HEAD
static void sync () {
	setprg8(0x6000, prg);
	VRC24_syncPRG(0x01F, 0x000);
	VRC24_syncCHR(0x1FF, 0x000);
	VRC24_syncMirror();
}

static DECLFW (writePRG) {
	prg =A &0xFF;
	sync();
}

static void power(void) {
	prg =0;
	VRC24_power();
}

void Mapper183_Init (CartInfo *info) {
<<<<<<< HEAD
	VRC4_init(info, sync, 0x04, 0x08, 1, NULL, NULL, CartBR, writePRG, NULL);
	AddExState(stateRegs, ~0, 0, 0);
	info->Power =power;
=======
	VRC24_init(info, sync, 0x04, 0x08, 1, 1, 0);
	VRC24_WRAMRead =CartBR;
	VRC24_WRAMWrite =Mapper183_writePRG;
	AddExState(Mapper183_stateRegs, ~0, 0, 0);
	info->Power =Mapper183_power;
=======
static void M183SyncPrg(void) {
	setprg8(0x6000, prg[3]);
	setprg8(0x8000, prg[0]);
	setprg8(0xA000, prg[1]);
	setprg8(0xC000, prg[2]);
	setprg8(0xE000, ~0);
=======
static uint8 prg[4];

static SFORMAT StateRegs[] = {
	{ prg, 4, "PRG" },
	{ 0 }
};

static void M183PRG(void) {
	setprg8(0x6000, prg[0] & 0x3F);
	setprg8(0x8000, prg[1] & 0x3F);
	setprg8(0xA000, prg[2] & 0x3F);
	setprg8(0xC000, prg[3] & 0x3F);
	setprg8(0xE000,   (~0) & 0x3F);
>>>>>>> 4fb69f6 (Update Makefile.libretro)
}

static DECLFW(M183Write) {
	switch (A & 0xF800) {
	case 0x6800:
		prg[0] = A;
		VRC24_FixPRG();
		break;
	case 0x8800:
		prg[1] = V;
		VRC24_FixPRG();
		break;
	case 0xA800:
		prg[2] = V;
		VRC24_FixPRG();
		break;
	case 0xA000:
		prg[3] = V;
		VRC24_FixPRG();
		break;
	case 0x9800:
		VRC24_Write(0x9000, V);
		break;
	case 0x6000:
	case 0x7000:
	case 0x7800:
	case 0x8000:
	case 0x9000:
		return;
	default:
		VRC24_Write(A, V);
		return;
	}
}

static void M183Power(void) {
	VRC24_Power();
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0xFFFF, M183Write);
}

void Mapper183_Init(CartInfo *info) {
	VRC24_Init(info, VRC4, 0x04, 0x08, 0, 1);
	info->Power = M183Power;
<<<<<<< HEAD
	GameHBIRQHook = M183IRQCounter;
	GameStateRestore = StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
>>>>>>> 9037fa8 (Update Makefile.libretro)
<<<<<<< HEAD
>>>>>>> d627cc9 (Update Makefile.libretro)
=======
=======
	VRC24_FixPRG = M183PRG;
	AddExState(StateRegs, ~0, 0, NULL);
>>>>>>> 4fb69f6 (Update Makefile.libretro)
>>>>>>> 7733d52 (Update Makefile.libretro)
}
