/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2011 CaH4e3
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
 *
 * SL12 Protected 3-in-1 mapper hardware (VRC2, MMC3, MMC1)
 * the same as 603-5052 board (TODO: add reading registers, merge)
 * SL1632 2-in-1 protected board, similar to SL12 (TODO: find difference)
 *
 * Known PCB:
 *
 * Garou Densetsu Special (G0904.PCB, Huang-1, GAL dip: W conf.)
 * Kart Fighter (008, Huang-1, GAL dip: W conf.)
 * Somari (008, C5052-13, GAL dip: P conf., GK2-P/GK2-V maskroms)
 * Somari (008, Huang-1, GAL dip: W conf., GK1-P/GK1-V maskroms)
 * AV Mei Shao Nv Zhan Shi (aka AV Pretty Girl Fighting) (SL-12 PCB, Hunag-1, GAL dip: unk conf. SL-11A/SL-11B maskroms)
 * Samurai Spirits (Full version) (Huang-1, GAL dip: unk conf. GS-2A/GS-4A maskroms)
 * Contra Fighter (603-5052 PCB, C5052-3, GAL dip: unk conf. SC603-A/SCB603-B maskroms)
 *
 */

#include "mapinc.h"
#include "vrc2and4.h"
#include "mmc3.h"
#include "mmc1.h"

#define MODE_MMC1 mode & 0x02
#define MODE_MMC3 mode & 0x01

static uint8 mode = 0;
static uint8 game = 0;

<<<<<<< HEAD
static uint8 mmc1_regs[4]  = { 0 };
static uint8 mmc1_buffer   = 0;
static uint8 mmc1_shift    = 0;

static uint8 IRQCount      = 0;
static uint8 IRQLatch      = 0;
static uint8 IRQa          = 0;
static uint8 IRQReload     = 0;
static uint8 mode          = 0;

static uint8 submapper     = 0;
static uint8 game          = 0;

extern uint32 ROM_size;
extern uint32 VROM_size;

static SFORMAT StateRegs[] =
{
	{ &mode,        1, "MODE" },
	{ vrc2_chr,     8, "VRCC" },
	{ vrc2_prg,     2, "VRCP" },
	{ &vrc2_mirr,   1, "VRCM" },
	{ mmc3_regs,   10, "M3RG" },
	{ &mmc3_ctrl,   1, "M3CT" },
	{ &mmc3_mirr,   1, "M3MR" },
	{ &IRQReload,   1, "IRQR" },
	{ &IRQCount,    1, "IRQC" },
	{ &IRQLatch,    1, "IRQL" },
	{ &IRQa,        1, "IRQA" },
	{ mmc1_regs,    4, "M1RG" },
	{ &mmc1_buffer, 1, "M1BF" },
	{ &mmc1_shift,  1, "M1MR" },
	{ &submapper,   1, "SUBM" },
	{ &game,        1, "GAME" },
	{ 0 }
};

static void SyncPRG(void) {
	uint8 mask  = (submapper != 3) ? 0x3F : (game ? 0x0F : 0x1F);
	uint8 outer = game ? (game + 1) * 0x10 : 0;
	switch (mode & 3) {
	case 0:
		setprg8(0x8000, (outer & ~mask) | (vrc2_prg[0] & mask));
		setprg8(0xA000, (outer & ~mask) | (vrc2_prg[1] & mask));
		setprg8(0xC000, (outer & ~mask) | (~1 & mask));
		setprg8(0xE000, (outer & ~mask) | (~0 & mask));
		break;
	case 1:
	{
		uint32 swap = (mmc3_ctrl >> 5) & 2;
		setprg8(0x8000, (outer & ~mask) | (mmc3_regs[6 + swap] & mask));
		setprg8(0xA000, (outer & ~mask) | (mmc3_regs[7] & mask));
		setprg8(0xC000, (outer & ~mask) | (mmc3_regs[6 + (swap ^ 2)] & mask));
		setprg8(0xE000, (outer & ~mask) | (mmc3_regs[9] & mask));
		break;
	}
	case 2:
	case 3:
	{
		uint8 bank = mmc1_regs[3] & mask;
		if (mmc1_regs[0] & 8) {
			if (submapper == 2)
				bank >>= 1;
			if (mmc1_regs[0] & 4) {
				setprg16(0x8000, bank);
				setprg16(0xC000, 0x0F);
			} else {
				setprg16(0x8000, 0);
				setprg16(0xC000, bank);
			}
		} else
			setprg32(0x8000, ((outer & ~mask) >> 1) | (bank >> 1));
		}
		break;
	}
}

static void SyncCHR(void) {
	uint32 mask  = game ? 0x7F : 0xFF;
	uint32 outer = game ? (game + 1) * 0x80 : 0;
	uint32 base  = (mode & 4) << 6;
	switch (mode & 3) {
	case 0:
		setchr1(0x0000, ((outer | base) & ~mask) | (vrc2_chr[0] & mask));
		setchr1(0x0400, ((outer | base) & ~mask) | (vrc2_chr[1] & mask));
		setchr1(0x0800, ((outer | base) & ~mask) | (vrc2_chr[2] & mask));
		setchr1(0x0c00, ((outer | base) & ~mask) | (vrc2_chr[3] & mask));
		setchr1(0x1000, ((outer | base) & ~mask) | (vrc2_chr[4] & mask));
		setchr1(0x1400, ((outer | base) & ~mask) | (vrc2_chr[5] & mask));
		setchr1(0x1800, ((outer | base) & ~mask) | (vrc2_chr[6] & mask));
		setchr1(0x1c00, ((outer | base) & ~mask) | (vrc2_chr[7] & mask));
		break;
	case 1: {
		uint32 swap = (mmc3_ctrl & 0x80) << 5;
		setchr1(0x0000 ^ swap, ((outer | base) & ~mask) | ((mmc3_regs[0] & 0xFE) & mask));
		setchr1(0x0400 ^ swap, ((outer | base) & ~mask) | ((mmc3_regs[0] | 1) & mask));
		setchr1(0x0800 ^ swap, ((outer | base) & ~mask) | ((mmc3_regs[1] & 0xFE) & mask));
		setchr1(0x0c00 ^ swap, ((outer | base) & ~mask) | ((mmc3_regs[1] | 1) & mask));
		setchr1(0x1000 ^ swap, ((outer | base) & ~mask) | (mmc3_regs[2] & mask));
		setchr1(0x1400 ^ swap, ((outer | base) & ~mask) | (mmc3_regs[3] & mask));
		setchr1(0x1800 ^ swap, ((outer | base) & ~mask) | (mmc3_regs[4] & mask));
		setchr1(0x1c00 ^ swap, ((outer | base) & ~mask) | (mmc3_regs[5] & mask));
		break;
	}
	case 2:
	case 3:
		if (mmc1_regs[0] & 0x10) {
			setchr4(0x0000, (outer & ~mask) | (mmc1_regs[1] & mask));
			setchr4(0x1000, (outer & ~mask) | (mmc1_regs[2] & mask));
		} else
			setchr8(((outer & ~mask) >> 1) | (mmc1_regs[1] & mask) >> 1);
		break;
	}
}

static void SyncMIR(void) {
	switch (mode & 3) {
	case 0: {
		setmirror((vrc2_mirr & 1) ^ 1);
		break;
	}
	case 1: {
		setmirror((mmc3_mirr & 1) ^ 1);
		break;
	}
	case 2:
	case 3: {
		switch (mmc1_regs[0] & 3) {
		case 0: setmirror(MI_0); break;
		case 1: setmirror(MI_1); break;
		case 2: setmirror(MI_V); break;
		case 3: setmirror(MI_H); break;
		}
		break;
	}
	}
=======
static SFORMAT StateRegs[] = {
	{ &mode, 1, "MODE"},
	{ &game, 1, "GAME"},
	{ 0 }
};

static uint32 GetPRGMask(void) {
	if (iNESCart.submapper != 3) {
		return 0x3F;
	}
	return (game ? 0x0F : 0x1F);
}

static uint32 GetPRGBase(void) {
	if (game) {
		return (game + 1) * 0x10;
	}
	return 0;
}

static uint32 GetCHRMask(void) {
	return (game ? 0x7F : 0xFF);
}

static uint32 GetCHRBase(void) {
	return (game ? (game + 1) * 0x80 : 0);
}

static void M116VRC24PW(uint16 A, uint16 V) {
	setprg8(A, GetPRGBase() | (V & GetPRGMask()));
}

static void M116VRC24CW(uint16 A, uint16 V) {
	setchr1(A, ((mode << 6) & 0x100) | GetCHRBase() | (V & GetCHRMask()));
}

static void M116MMC3PW(uint16 A, uint16 V) {
	setprg8(A, GetPRGBase() | (V & GetPRGMask()));
}

static void M116MMC3CW(uint16 A, uint16 V) {
	setchr1(A, ((mode << 6) & 0x100) | GetCHRBase() | (V & GetCHRMask()));
}

static void M116MMC1PW(uint16 A, uint16 V) {
	if (iNESCart.submapper == 2) {
		setprg16(A, V >> 1);
	} else {
		setprg16(A, (GetPRGBase() >> 1) | (V & (GetPRGMask() >> 1)));
	}
}

static void M116MMC1CW(uint16 A, uint16 V) {
	setchr4(A, (GetCHRBase() >> 2) | (V & (GetCHRMask() >> 2)));
>>>>>>> d8abdd7 (Update Makefile.libretro)
}

static void Sync(void) {
	if (MODE_MMC1) {
		MMC1_FixPRG();
		MMC1_FixCHR();
		MMC1_FixMIR();
	} else if (MODE_MMC3) {
		MMC3_FixPRG();
		MMC3_FixCHR();
		MMC3_FixMIR();
	} else {
		VRC24_FixPRG();
		VRC24_FixCHR();
		VRC24_FixMIR();
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static DECLFW(UNLSL12ModeWrite) {
	if (A & 0x100) {
=======
static void UNLSL12ModeWrite(uint32 A, uint8 V) {
	if ((A & 0x4100) == 0x4100) {
>>>>>>> 4322fc1 (#1 batch of backports/cleanups (#575))
		mode = V;
		if (A & 1) {	/* hacky hacky, there are two configuration modes on SOMARI HUANG-1 PCBs
						 * Solder pads with P1/P2 shorted called SOMARI P,
						 * Solder pads with W1/W2 shorted called SOMARI W
						 * Both identical 3-in-1 but W wanted MMC1 registers
						 * to be reset when switch to MMC1 mode P one - doesn't
						 * There is issue with W version of Somari at starting copyrights
						 */
			mmc1_regs[0] = 0xc;
			mmc1_regs[3] = 0;
			mmc1_buffer = 0;
			mmc1_shift = 0;
=======
static void applyMode(void) {
	if (MODE_MMC1) {
		SetWriteHandler(0x8000, 0xFFFF, MMC1_Write);
		if (iNESCart.submapper != 1) {
			MMC1_Write(0x8000, 0x80);
>>>>>>> d8abdd7 (Update Makefile.libretro)
		}
	} else if (MODE_MMC3) {
		SetWriteHandler(0x8000, 0xFFFF, MMC3_Write);
	} else {
		SetWriteHandler(0x8000, 0xFFFF, VRC24_Write);
	}
}

static DECLFW(M116ModeWrite) {
	if (A & 0x100) {
		mode = V;
		applyMode();
		Sync();
	}
}

static void M116HBIRQ(void) {
	if ((mode & 0x03) == 0x01) {
		MMC3_IRQHBHook();
	}
}

static void StateRestore(int version) {
	Sync();
}

<<<<<<< HEAD
static void UNLSL12Reset(void) {
	/* this is suppose to increment during power cycle */
	/* but we dont have a way to do that, so increment on reset instead. */
	if (submapper == 3) {
		game = game + 1;
		if (game > 4)
			game = 0;
	}
	Sync();
}

static void UNLSL12Power(void) {
	game = (submapper == 3) ? 4 : 0;
	mode = 1;
	vrc2_chr[0] = ~0;
	vrc2_chr[1] = ~0;
	vrc2_chr[2] = ~0;
	vrc2_chr[3] = ~0;	/* W conf. of Somari wanted CHR3 has to be set to BB bank (or similar), but doesn't do that directly */
	vrc2_chr[4] = 4;
	vrc2_chr[5] = 5;
	vrc2_chr[6] = 6;
	vrc2_chr[7] = 7;
	vrc2_prg[0] = 0;
	vrc2_prg[1] = 1;
	vrc2_mirr = 0;
	mmc3_regs[0] = 0;
	mmc3_regs[1] = 2;
	mmc3_regs[2] = 4;
	mmc3_regs[3] = 5;
	mmc3_regs[4] = 6;
	mmc3_regs[5] = 7;
	mmc3_regs[6] = ~3;
	mmc3_regs[7] = ~2;
	mmc3_regs[8] = ~1;
	mmc3_regs[9] = ~0;
	mmc3_ctrl = mmc3_mirr = IRQCount = IRQLatch = IRQa = 0;
	mmc1_regs[0] = 0xc;
	mmc1_regs[1] = 0;
	mmc1_regs[2] = 0;
	mmc1_regs[3] = 0;
	mmc1_buffer = 0;
	mmc1_shift = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x4100, 0x5FFF, UNLSL12ModeWrite);
	SetWriteHandler(0x8000, 0xFFFF, UNLSL12Write);
}

void UNLSL12_Init(CartInfo *info) {
	info->Power      = UNLSL12Power;
	info->Reset      = UNLSL12Reset;
	GameHBIRQHook    = UNLSL12HBIRQ;
	GameStateRestore = StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
	submapper        = info->submapper;
	if (submapper == 0) {
		/* PRG 128K and CHR 128K is Huang-2 (submapper 2) */
		if (ROM_size == 8 && VROM_size == 16)
			submapper = 2;
=======
static void M116Reset(void) {
	if (iNESCart.submapper == 3) {
		game = game + 1;
		if (game > 4) {
			game = 0;
		}
	}
	applyMode();
	Sync();
}

static void M116Power(void) {
	game = (iNESCart.submapper == 3) ? 4 : 0;
	mode = 1;

	MMC3_Power();
	MMC1_Reset();
	VRC24_Power();

	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x4100, 0x5FFF, M116ModeWrite);

	vrc24.chr[0] = ~0;
	vrc24.chr[1] = ~0;
	vrc24.chr[2] = ~0;
	vrc24.chr[3] = ~0;

	applyMode();
	Sync();
}

void Mapper116_Init(CartInfo *info) {
	VRC24_Init(info, VRC2, 0x01, 0x02, FALSE, TRUE);
	VRC24_pwrap = M116VRC24PW;
	VRC24_cwrap = M116VRC24CW;

	MMC3_Init(info, FALSE, FALSE);
	MMC3_pwrap = M116MMC3PW;
	MMC3_cwrap = M116MMC3CW;

	MMC1_Init(info, FALSE, FALSE);
	MMC1_pwrap = M116MMC1PW;
	MMC1_cwrap = M116MMC1CW;
	mmc1_type = MMC1A;

	info->Power = M116Power;
	info->Reset = M116Reset;

	GameHBIRQHook = M116HBIRQ;

	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	/* PRG 128K and CHR 128K is Huang-2 (iNESCart.submapper 2) */
	if (((ROM.prg.size * 16) == 128) && ((ROM.chr.size * 8) == 128)) {
		info->submapper = 2;
>>>>>>> d8abdd7 (Update Makefile.libretro)
	}
}
