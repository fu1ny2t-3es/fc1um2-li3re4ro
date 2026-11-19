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

/*
 Mapper 228:
 Action 52 is highly uncommon in that its PRG ROM has a non-power-of-two ROM size: three 512 KiB PRG ROMs alongside one
 512 KiB CHR ROM.

 It is claimed that there are four 4-bit RAM locations at $4020-$4023, mirrored throughout $4020-$5FFF. This 16-bit RAM
 is definitely not present on either cartridge, Nestopia does not implement it at all, and neither cartridge ever writes
 to these addresses.
 */

#include "mapinc.h"
#include "latch.h"

<<<<<<< HEAD
static uint8_t mram[4], vreg;
static uint16_t areg;

static SFORMAT StateRegs[] =
{
	{ mram, 4, "MRAM" },
	{ &areg, 2 | FCEUSTATE_RLSB, "AREG" },
	{ &vreg, 1, "VREG" },
	{ 0 }
};

<<<<<<< HEAD
static void Sync(void) {
	uint32_t prgl, prgh, page = (areg >> 7) & 0x3F;
=======
static void M228Sync(void) {
	uint32 prgl, prgh, page = (areg >> 7) & 0x3F;
>>>>>>> 3cd7a223 (Update libretro_core_options.h)
	if ((page & 0x30) == 0x30)
		page -= 0x10;
	prgl = prgh = (page << 1) + (((areg >> 6) & 1) & ((areg >> 5) & 1));
	prgh += ((areg >> 5) & 1) ^ 1;

	setmirror(((areg >> 13) & 1) ^ 1);
	setprg16(0x8000, prgl);
	setprg16(0xc000, prgh);
	setchr8(((vreg & 0x3) | ((areg & 0xF) << 2)));
}

static void M228RamWrite(uint32 A, uint8 V) {
	mram[A & 3] = V & 0x0F;
}

static uint8 M228RamRead(uint32 A) {
	return mram[A & 3];
}

static void M228Write(uint32 A, uint8 V) {
	areg = A;
	vreg = V;
	M228Sync();
}

static void M228Reset(void) {
	areg = 0x8000;
	vreg = 0;
	memset(mram, 0, sizeof(mram));
	M228Sync();
}

static void M228Power(void) {
	M228Reset();
	SetReadHandler(0x5000, 0x5FFF, M228RamRead);
	SetWriteHandler(0x5000, 0x5FFF, M228RamWrite);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M228Write);
}

static void M228StateRestore(int version) {
	M228Sync();
=======
static void Sync(void) {
	if (latch.addr & 0x20) {
		setprg16(0x8000, (latch.addr >> 6) & 0x7F);
		setprg16(0xC000, (latch.addr >> 6) & 0x7F);
	} else {
		setprg32(0x8000, (latch.addr >> 7) & 0x3F);
	}
	setchr8(((latch.addr << 2) & 0x3C) | (latch.data & 0x3));
	setmirror(((latch.addr >> 13) & 0x01) ^ 0x01);
>>>>>>> f6dccad9 (Update libretro_core_options.h)
}

void Mapper228_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);

	if (iNESCart.PRGRomSize == 0x180000) {
		int i;
		size_t ssize = 0x200000;
		uint8 *tmp = (uint8 *)FCEU_malloc(ssize);

		for (i = 0; i < (int)iNESCart.PRGRomSize; i++) {
			tmp[i] = ROM.prg.data[i];
		}
		for (i = 0x000000; i < 0x080000; i++) {
			tmp[0x180000 + i] = tmp[0x100000 + i];
			tmp[0x100000 + i] = (i >> 8) & 0xFF;
		}

		iNESCart.PRGRomSize = ssize;
		if (ROM.prg.data) FCEU_free(ROM.prg.data);
		ROM.prg.data = (uint8 *)FCEU_malloc(ssize);
		for (i = 0; i < (int)iNESCart.PRGRomSize; i++) {
			ROM.prg.data[i] = tmp[i];
		}
		FCEU_free(tmp);

		SetupCartPRGMapping(0, ROM.prg.data, iNESCart.PRGRomSize, 0);
	}
}
