/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
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

/* added 2020-1-28 - */
/* NES 2.0 Mapper 372 is used for a revision of the Rockman I-VI multicart (PCB ID SFC-12).
 * It is INES Mapper 045 but with one bit of outer bank register #2 working as a CHR-ROM/RAM switch.
 */

#include "mapinc.h"
#include "mmc3.h"

<<<<<<< HEAD
static uint8_t WRAM[0x2000];
static uint32_t CHRRAMSIZE;
static uint8_t *CHRRAM;

static void M372CW(uint32_t A, uint8_t V) {
	if (!UNIFchrrama) {
		uint32_t NV = V;
		if (EXPREGS[2] & 8)
			NV &= (1 << ((EXPREGS[2] & 7) + 1)) - 1;
		else if (EXPREGS[2])
			NV &= 0;	/* hack ;( don't know exactly how it should be */
		NV |= EXPREGS[0] | ((EXPREGS[2] & 0xF0) << 4);
		if (EXPREGS[2] & 0x20)
			setchr1r(0x10, A, V);
		else
			setchr1(A, NV);
	} else
		/* setchr8(0); */		/* i don't know what cart need this, but a new one need other lol */
		setchr1(A, V);
}

static void M372PW(uint32_t A, uint8_t V) {
	uint32_t MV = V & ((EXPREGS[3] & 0x3F) ^ 0x3F);
	MV |= EXPREGS[1];
	if(UNIFchrrama)
		MV |= ((EXPREGS[2] & 0x40) << 2);
	setprg8(A, MV);
}
=======
static uint8 reg[4];
static uint8 cmd;

static uint32 CHRRAMSIZE;
static uint8 *CHRRAM;

static SFORMAT StateRegs[] = {
	{ reg, 4, "REGS" },
	{ &cmd, 1, "CMD0" },
	{ 0 }
};

static void M372CW(uint16 A, uint16 V) {
	if (reg[2] & 0x20) {
		setchr8r(0x10, 0);
	} else {
		uint32 mask = 0xFF >> (~reg[2] & 0x0F);
		uint32 base = ((reg[2] << 4) & 0xF00) | reg[0];
>>>>>>> a1c8c17c (Update libretro_core_options.h)

		setchr1(A, (base & ~mask) | (V & mask));
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static DECLFR(M372Read) {
	uint32_t addr = 1 << (EXPREGS[5] + 4);
=======
static uint8 M372Read(uint32 A) {
	uint32 addr = 1 << (EXPREGS[5] + 4);
>>>>>>> d5085b8d (Update libretro_core_options.h)
	if (A & (addr | (addr - 1)))
		return cpu.openbus | 1;
	return cpu.openbus;
=======
static void M372PW(uint16 A, uint16 V) {
	uint32 mask = ~reg[3] & 0x3F;
	uint32 base = ((reg[2] << 2) & 0x300) | reg[1];

	setprg8(A, (base & ~mask) | (V & mask));
}

static DECLFW(M372Write) {
	if (!(reg[3] & 0x40)) {
		reg[cmd] = V;
		cmd = (cmd + 1) & 0x03;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
>>>>>>> a1c8c17c (Update libretro_core_options.h)
}

static void M372Reset(void) {
	reg[0] = reg[1] = reg[3] = cmd = 0;
	reg[2] = 0x0F;
	MMC3_Reset();
}

static void M372Power(void) {
	reg[0] = reg[1] = reg[3] = cmd = 0;
	reg[2] = 0x0F;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M372Write);
}

static void M372Close(void) {
	MMC3_Close();
	if (CHRRAM) {
		FCEU_gfree(CHRRAM);
	}
	CHRRAM = NULL;
}

void Mapper372_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M372CW;
	MMC3_pwrap = M372PW;
	info->Reset = M372Reset;
	info->Power = M372Power;
	info->Close = M372Close;
	AddExState(StateRegs, ~0, 0, NULL);

	CHRRAMSIZE = 8192;
<<<<<<< HEAD
	CHRRAM = (uint8_t*)FCEU_gmalloc(CHRRAMSIZE);
=======
	CHRRAM = (uint8 *)FCEU_gmalloc(CHRRAMSIZE);
>>>>>>> a1c8c17c (Update libretro_core_options.h)
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
	AddExState(CHRRAM, CHRRAMSIZE, 0, "CHRR");
}
