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
static uint8_t preg[2], creg[8], mirr;

<<<<<<< HEAD
static uint8_t *WRAM = NULL;
static uint32_t WRAMSIZE;
=======
static uint8 *WRAM = NULL;

<<<<<<< HEAD
#define M32_WRAMSIZE 8192
>>>>>>> 09aa6a76 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
=======
#ifndef WRAM_SIZE
#define WRAM_SIZE 8192
#endif
>>>>>>> 4cb56bf0 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)

static SFORMAT StateRegs[] =
{
	{ preg, 4, "PREG" },
	{ creg, 8, "CREG" },
	{ &mirr, 1, "MIRR" },
	{ 0 }
};

<<<<<<< HEAD
static void Sync(void) {
	uint8_t i;
	uint16_t swap = ((mirr & 2) << 13);
=======
static void M32Sync(void) {
	uint8 i;
	uint16 swap = ((mirr & 2) << 13);
>>>>>>> 09aa6a76 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
	setmirror((mirr & 1) ^ 1);
	setprg8r(0x10, 0x6000, 0);
	setprg8(0x8000 ^ swap, preg[0]);
	setprg8(0xA000, preg[1]);
	setprg8(0xC000 ^ swap, ~1);
	setprg8(0xE000, ~0);
	for (i = 0; i < 8; i++)
		setchr1(i << 10, creg[i]);
=======
static uint8 prg[2], chr[8], prgMode, mirr;

static SFORMAT StateRegs[] = {
	{ prg, 4, "PREG" },
	{ chr, 8, "CREG" },
	{ &prgMode, 1, "MODE" },
	{ &mirr, 1, "mirr" },
	{ 0 }
};

static void Sync(void) {
	if (prgMode == 0) {
		setprg8(0x8000, prg[0]);
		setprg8(0xA000, prg[1]);
		setprg8(0xC000, ~1);
		setprg8(0xE000, ~0);
	} else {
		setprg8(0x8000, ~1);
		setprg8(0xA000, prg[1]);
		setprg8(0xC000, prg[0]);
		setprg8(0xE000, ~0);
	}

	setchr1(0x0000, chr[0]);
	setchr1(0x0400, chr[1]);
	setchr1(0x0800, chr[2]);
	setchr1(0x0C00, chr[3]);
	setchr1(0x1000, chr[4]);
	setchr1(0x1400, chr[5]);
	setchr1(0x1800, chr[6]);
	setchr1(0x1C00, chr[7]);

	if (iNESCart.submapper == 1) {
		setmirror(MI_1);
	} else {
		setmirror((mirr & 0x01) ^ 0x01);
	}
>>>>>>> 8bf4e730 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
}

static DECLFW(M032Write) {
	switch (A & 0xF000) {
	case 0x8000:
	case 0xA000:
		prg[(A >> 13) & 0x01] = V;
		Sync();
		break;
	case 0x9000:
		mirr = (V & 0x01) != 0;
		prgMode = (V & 0x02) != 0;
		if (iNESCart.submapper == 1) {
			prgMode = 0;
		}
		Sync();
		break;
	case 0xB000:
		chr[A & 0x07] = V;
		Sync();
		break;
	}
}

static void M032Power(void) {
	prg[0] = 0;
	prg[1] = 1;
	chr[0] = 0;
	chr[1] = 1;
	chr[2] = 2;
	chr[3] = 3;
	chr[4] = 4;
	chr[5] = 5;
	chr[6] = 6;
	chr[7] = 7;
	prgMode = 0;
	Sync();

	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M032Write);

	if (WRAM) {
		setprg8r(0x10, 0x6000, 0);
		SetReadHandler(0x6000, 0x7FFF, CartBR);
		SetWriteHandler(0x6000, 0x7FFF, CartBW);
		FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
	}
}

static void M032Close(void) {
}

static void StateRestore(int version) {
	Sync();
}

void Mapper032_Init(CartInfo *info) {
	info->Power = M032Power;
	info->Close = M032Close;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	WRAMSIZE = 8192;
	WRAM = (uint8_t*)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
=======
	WRAM = (uint8*)FCEU_gmalloc(M32_WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, M32_WRAMSIZE, 1);
	AddExState(WRAM, M32_WRAMSIZE, 0, "WRAM");
>>>>>>> 09aa6a76 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
=======
	WRAM = (uint8*)FCEU_gmalloc(WRAM_SIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAM_SIZE, 1);
	AddExState(WRAM, WRAM_SIZE, 0, "WRAM");
>>>>>>> 4cb56bf0 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)

	AddExState(&StateRegs, ~0, 0, 0);
=======
	WRAMSIZE = 8192;
	if (info->iNES2) {
		WRAMSIZE = info->PRGRamSize + info->PRGRamSaveSize;
	}
	if (WRAMSIZE) {
		WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
		SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
		AddExState(WRAM, WRAMSIZE, 0, "WRAM");
	}
>>>>>>> 8bf4e730 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
}
