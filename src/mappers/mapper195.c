/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
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
#include "mmc3.h"

<<<<<<< HEAD
static writefunc writePPU;
<<<<<<< HEAD
static uint8_t *CHRRAM;
static uint32_t CHRRAMSIZE;
static uint8_t mask;
static uint8_t compare;
extern uint32_t RefreshAddr;

static void Mapper195_CHRWrap(uint32_t A, uint8_t V) {
	if ((V &mask) ==compare)
=======
static uint8 *CHRRAM;
=======
static uint8 chrRamMask;
static uint8 chrRamBankSelect;

static uint8 *CHRRAM = NULL;
>>>>>>> 33d40fa (Update libretro_core_options.h)
static uint32 CHRRAMSIZE;

static writefunc writePPU;
extern uint32 RefreshAddr;

static void M195CW(uint16 A, uint16 V) {
	if ((V & chrRamMask) == chrRamBankSelect) {
>>>>>>> f6dccad9 (Update libretro_core_options.h)
		setchr1r(0x10, A, V);
    } else {
		setchr1(A, V);
    }
}

<<<<<<< HEAD
<<<<<<< HEAD
static const uint8_t compares[8] = { 0x28, 0x00, 0x4C, 0x64, 0x46, 0x7C, 0x04, 0xFF };
static DECLFW(Mapper195_InterceptPPUWrite) {
=======
static const uint8 compares[8] = { 0x28, 0x00, 0x4C, 0x64, 0x46, 0x7C, 0x04, 0xFF };
static void Mapper195_InterceptPPUWrite(uint32 A, uint8 V) {
>>>>>>> 3cd7a223 (Update libretro_core_options.h)
	if (RefreshAddr <0x2000) {
		int addr =RefreshAddr;
		int reg, bank;
=======
static const uint8 chrRamLut[8] = {
    0x28, 0x00, 0x4C, 0x64, 0x46, 0x7C, 0x04, 0xFF,
};
>>>>>>> f6dccad9 (Update libretro_core_options.h)

static DECLFW(M195PPUWrite) {
	if (RefreshAddr < 0x2000) {
		uint8 reg, chrBank;
        uint32 addr = RefreshAddr;

		if (mmc3.cmd & 0x80) {
			addr ^= 0x1000;
        }
		if (addr & 0x1000) {
			reg = (addr >> 10) - 2;
        } else {
			reg = addr >> 11;
        }

		chrBank = mmc3.reg[reg];
		if (chrBank & 0x80) {
			if (chrBank & 0x10) {
                /* CHR-RAM disable */
				chrRamMask = 0x00;
				chrRamBankSelect = 0xFF;
			} else {
                uint8 index = ((chrBank >> 4) & 0x04) | ((chrBank >> 2) & 0x02) | ((chrBank >> 1) & 0x01);

				chrRamMask = (chrBank & 0x40) ? 0xFE : 0xFC;
				chrRamBankSelect = chrRamLut[index];
			}
            MMC3_FixCHR();
		}
	}
	writePPU(A, V);
}

static void M195Power(void) {
	chrRamMask = 0xFC;
	chrRamBankSelect = 0x00;
	MMC3_Power();
	setprg4r(0x10, 0x5000, 2);
	SetWriteHandler(0x5000, 0x5FFF, CartBW);
	SetReadHandler(0x5000, 0x5FFF, CartBR);

	writePPU = GetWriteHandler(0x2007);
	SetWriteHandler(0x2007, 0x2007, M195PPUWrite);
}

<<<<<<< HEAD
static void Mapper195_Close(void) {
	if (CHRRAM)
=======
static void M195Close(void) {
	MMC3_Close();
	if (CHRRAM) {
>>>>>>> f6dccad9 (Update libretro_core_options.h)
		FCEU_gfree(CHRRAM);
    }
	CHRRAM = NULL;
}

void Mapper195_Init(CartInfo *info) {
<<<<<<< HEAD
	GenMMC3_Init(info, 512, 256, 16, info->battery);
	cwrap = Mapper195_CHRWrap;
	info->Power = Mapper195_Power;
	info->Reset = MMC3RegReset;
	info->Close = Mapper195_Close;
	CHRRAMSIZE =4096;
	CHRRAM =(uint8_t*)FCEU_gmalloc(CHRRAMSIZE);
=======
	MMC3_Init(info, 16, info->battery);
	info->Power = M195Power;
	info->Close = M195Close;
	MMC3_cwrap = M195CW;

	CHRRAMSIZE = 4096;
	CHRRAM = (uint8 *)FCEU_gmalloc(CHRRAMSIZE);
>>>>>>> f6dccad9 (Update libretro_core_options.h)
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
	AddExState(CHRRAM, CHRRAMSIZE, 0, "CHRR");

	AddExState(&chrRamMask, 1, 0, "CHRM");
	AddExState(&chrRamBankSelect, 1, 0, "CHRB");
}
