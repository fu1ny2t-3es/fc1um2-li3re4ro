/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2020
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
 *
 *
 */

#include "mapinc.h"
#include "asic_mmc3.h"
#include "cartram.h"

static uint8_t submapper;
static uint8_t reg;
static uint8_t pad;

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
static DECLFR (readPad) {
	return CartBR(A &~3 | pad &3);
}
=======
static uint8 Mapper460_ReadOB(uint32 A) { return X.DB; }
>>>>>>> 4d55ef8 (Update libretro_core_options.h)
=======
static uint8 Mapper460_ReadOB(uint32 A) { return cpu.DB; }
>>>>>>> 3a833ec (Update libretro_core_options.h)
=======
static uint8 Mapper460_ReadOB(uint32 A) { return cpu.openbus; }
>>>>>>> cbccb70 (Update libretro_core_options.h)

static void sync () {
	MMC3_syncPRG(0x0F, reg <<4 &~0x0F);
	if (reg &0x04)
		MMC3_syncCHR(0x1FF, 0x000);
	else
		setchr8r(0x10, 0);
	MMC3_syncMirror();
}

<<<<<<< HEAD
static int getPRGBank (uint8_t bank) {
=======
<<<<<<< HEAD
static int getPRGBank (uint8 bank) {
>>>>>>> d5085b8d (Update libretro_core_options.h)
	if (reg &0x08) {
		int mask = reg &0x10? 3: 1;
		return MMC3_getPRGBank(bank &1) &~mask | bank &mask;
	} else
		return MMC3_getPRGBank(bank);
=======
static void Mapper460_WriteExtra(uint32 A, uint8 V) {
	if (A001B &0x80 && ~A001B &0x40) EXPREGS[0] =A &0xFF;
	FixMMC3PRG(MMC3_cmd);
	FixMMC3CHR(MMC3_cmd);
>>>>>>> 4d55ef8 (Update libretro_core_options.h)
}

static int getCHRBank (uint8_t bank) {
	if (reg &0x04)
		return MMC3_getCHRBank(bank &6 | bank >>1 &1) <<1 | bank &1;
	else
		return MMC3_getCHRBank(bank);
}

static DECLFR (interceptPRGRead_submapper0) {
	return reg &0x80? CartBR(A &~1 | pad &1): CartBR(A);
}

static DECLFR (interceptPRGRead_submapper1) {
	return reg &0x20? CartBR(A &~1 | pad &1): CartBR(A);
}

static DECLFW (writeReg) {
	reg = A &0xFF;
	sync();
}

static void reset () {
	reg = 0;
	pad++;
	MMC3_clear();
}

static void power () {
	reg = 0;
	pad = 0;
	MMC3_power();
	SetReadHandler(0x8000, 0xFFFF, submapper == 1? interceptPRGRead_submapper1: interceptPRGRead_submapper0);
}

void Mapper460_Init (CartInfo *info) {
	submapper = info->submapper;
	MMC3_init(info, sync, MMC3_TYPE_SHARP, getPRGBank, getCHRBank, NULL, writeReg);
	CHRRAM_init(info, 8);
	info->Power = power;
	info->Reset = reset;
	AddExState(&reg, 1, 0, "EXPR");
	AddExState(&reg, 1, 0, "DIPS");
}
