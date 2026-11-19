/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2012 CaH4e3
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

<<<<<<< HEAD
static uint8_t bank, mode;
static SFORMAT StateRegs[] =
{
	{ &bank, 1, "BANK" },
=======
static uint8 prg, mode;

static SFORMAT StateRegs[] = {
	{ &prg, 1, "PREG" },
>>>>>>> ae14339e (Update Makefile.libretro)
	{ &mode, 1, "MODE" },
	{ 0 }
};
static uint8_t inesMirroring;

<<<<<<< HEAD
static void M51Sync(void) {
	if (mode & 2) {
		setprg8(0x6000, ((bank & 7) << 2) | 0x23);
		setprg16(0x8000, (bank << 1) | 0);
		setprg16(0xC000, (bank << 1) | 1);
		setmirror(mode &0x10? MI_H: MI_V);
	} else {
		setprg8(0x6000, ((bank & 4) << 2) | 0x2F);
		setprg16(0x8000, (bank << 1) | (mode >> 4));
		setprg16(0xC000, ((bank & 0xC) << 1) | 7);
		setmirror(inesMirroring); /* The UNROM game's mirroring is set via solder pad and not via hardware register, so use the mirroring setting from the iNES header. */
=======
static void Sync(void) {
	if (iNESCart.submapper == 1) {
		setprg8(0x6000, (prg << 1) | 0x23);
		if (mode & 0x022)
			setprg32(0x8000, prg >> 1);
		else {
			setprg16(0x8000, ((prg >> 2) & 0x10) | ((prg >> 1) & 0x08) | (prg & 0x07));
			setprg16(0xC000, ((prg >> 2) & 0x10) | ((prg >> 1) & 0x08) | 0x07);
		}
	} else {
		if (mode & 0x02) {
			setprg8(0x6000, ((prg << 2) & 0x1C) | 0x23);
			setprg32(0x8000, prg);
		} else {
			setprg8(0x6000, ((prg << 2) & 0x10) | 0x2F);
			setprg16(0x8000, (prg << 1) | (prg >> 4));
			setprg16(0xC000, (prg << 1) | 0x07);
		}
>>>>>>> ccaecfc3 (Update Makefile.libretro)
	}
	setchr8(0);
	setmirror(((mode >> 4) & 1) ^ 1);
}

static DECLFW(M051WriteMode) {
	mode = V;
	Sync();
}

static DECLFW(M051WriteBank) {
	prg = V;
	Sync();
}

static void M051Power(void) {
	prg = 0;
	mode = 2;
	Sync();
	SetWriteHandler(0x6000, 0x7FFF, M051WriteMode);
	SetWriteHandler(0x8000, 0xFFFF, M051WriteBank);
	SetReadHandler(0x6000, 0xFFFF, CartBR);
}

static void M051Reset(void) {
	prg = 0;
	mode = 2;
	Sync();
}

static void StateRestore(int version) {
	Sync();
}

<<<<<<< HEAD
void Mapper51_Init(CartInfo *info) {
	inesMirroring = info->mirror;
	info->Power = M51Power;
	info->Reset = M51Reset;
	AddExState(&StateRegs, ~0, 0, 0);
	GameStateRestore = M51StateRestore;
=======
void Mapper051_Init(CartInfo *info) {
	info->Power = M051Power;
	info->Reset = M051Reset;
	AddExState(StateRegs, ~0, 0, NULL);
	GameStateRestore = StateRestore;

	if (!UNIFchrrama && (ROM.chr.size == 1)) {
		/* at least 1 variant has 8K CHR-ROM which should be treated as CHR-RAM */
		SetupCartCHRMapping(0, CHRptr[0], CHRsize[0], 1);
		AddExState(CHRptr[0], CHRsize[0], 0, "CHRR");
	}
>>>>>>> ccaecfc3 (Update Makefile.libretro)
}
