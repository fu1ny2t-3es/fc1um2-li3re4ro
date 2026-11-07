/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2005 CaH4e3
 *  Copyright (C) 2020
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
static uint8_t submapper;
=======
<<<<<<< HEAD
>>>>>>> 4096c502 (Update libretro.c)
static uint16_t cmdreg;
static uint8_t unrom, reg, openbus;

static uint32_t PRGROMSize;

static SFORMAT StateRegs[] =
{
	{ &cmdreg,  2 | FCEUSTATE_RLSB, "CREG" },
	{ &unrom,   1, "UNRM" },
	{ &reg,     1, "UNRG" },
	{ &openbus, 1, "OPNB" },
=======
static uint8 mode;

static SFORMAT StateRegs[] = {
	{ &mode,   1, "UROM" },
>>>>>>> 7b06ddc4 (Update libretro.c)
	{ 0 }
};

static void Sync(void) {
<<<<<<< HEAD
	if (unrom) {
		uint8_t PRGPageSize = PRGROMSize / 16384;
		setprg16(0x8000, (PRGPageSize & 0xC0) | (reg & 7));
		setprg16(0xC000, (PRGPageSize & 0xC0) | 7);
		setchr8(0);
		setmirror(MI_V);
	} else {
		uint8_t bank = ((cmdreg & 0x300) >> 3) | (cmdreg & 0x1F);
		if (cmdreg & 0x400)
			setmirror(MI_0);
		else
			setmirror(((cmdreg >> 13) & 1) ^ 1);
		openbus = bank >= PRGROMSize / 32768;
		if (cmdreg & 0x800) {
			setprg16(0x8000, (bank << 1) | ((cmdreg >> 12) & 1));
			setprg16(0xC000, (bank << 1) | ((cmdreg >> 12) & 1));
		} else
<<<<<<< HEAD
			setprg32(0x8000, bank);
		if (submapper == 1 && (bank &0x18) == 0x18) setprg16(0xC000,(bank <<1 | cmdreg >>12 &1) &~7);
=======
=======
	if (mode) { /* Contra mode */
		setprg16(0x8000, (ROM.prg.size & 0xC0) | (latch.data & 0x07));
		setprg16(0xC000, (ROM.prg.size & 0xC0) | 0x07);
>>>>>>> 4096c502 (Update libretro.c)
		setchr8(0);
		setmirror(MI_V);
	} else {
		uint8 bank = ((latch.addr >> 3) & 0x60) | (latch.addr & 0x1F);

		if (latch.addr & 0x800) {
			setprg16(0x8000, (bank << 1) | ((latch.addr >> 12) & 0x01));
			setprg16(0xC000, (bank << 1) | ((latch.addr >> 12) & 0x01));
		} else {
>>>>>>> 7b06ddc4 (Update libretro.c)
			setprg32(0x8000, bank);
		}
		setchr8(0);
		if (latch.addr & 0x400) {
			setmirror(MI_0);
		} else {
			setmirror(((latch.addr >> 13) & 0x01) ^ 0x01);
		}
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static DECLFR(M235Read) {
	if (openbus)
=======
static uint8 M235Read(uint32 A) {
	if (openbus) {
		openbus = 0;
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> 563724c2 (Update libretro.c)
		return X.DB;
	else
		return CartBR(A);
=======
		return cpu.DB;
=======
=======
static DECLFR(M235Read) {
	uint8 bank = ((latch.addr >> 3) & 0x60) | (latch.addr & 0x1F);

	if (!mode && (bank >= (PRGsize[0] / 32768))) {
>>>>>>> 4096c502 (Update libretro.c)
		return cpu.openbus;
>>>>>>> ae1c2483 (Update libretro.c)
	}
	return CartBR(A);
>>>>>>> 4b9e42b4 (Update libretro.c)
}

static void M235Reset(void) {
	if ((ROM.prg.size * 16384) & 0x20000) {
		mode = (mode + 1) & 1;
	}
	Latch_RegReset();
}

void Mapper235_Init(CartInfo *info) {
<<<<<<< HEAD
	submapper = info->submapper;
=======
	Latch_Init(info, Sync, M235Read, FALSE, FALSE);
>>>>>>> 4096c502 (Update libretro.c)
	info->Reset = M235Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
