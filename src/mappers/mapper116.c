/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
<<<<<<< HEAD
 *  Copyright (C) 2023
=======
 *  Copyright (C) 2011 CaH4e3
 *  Copyright (C) 2023-2024 negativeExponent
>>>>>>> 78bbfe6 (Update libretro.c)
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
#include "asic_mmc1.h"
#include "asic_mmc3.h"
#include "asic_vrc2and4.h"

static uint8_t submapper;
static uint8_t reg;
static uint8_t init; /* Games switch between ASICs expecting registers to keep their value, so initialize each ASIC only on the first switch and use this bitfield to track it */
static uint8_t game;

static SFORMAT stateRegs[] = {
	{ &reg, 1, "MODE" },
	{ &init, 1, "INIT" },
	{ 0 },
=======
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
>>>>>>> 78bbfe6 (Update libretro.c)
};

static SFORMAT stateRegsMulti[] = {
	{ &game, 1, "GAME" },
	{ 0 },
};

static void sync (void) {
	int prgAND, chrAND, prgOR, chrOR;
	if (submapper == 3) { /* First game is 256K+256K, the others 128K+128K */
		prgAND = game? 0x0F: 0x1F;
		chrAND = game? 0x7F: 0xFF;
		prgOR  = game <<4 &~prgAND;
		chrOR  = game <<7 &~chrAND;
	} else {
		prgAND = 0x3F;
		chrAND = 0xFF;
		prgOR  = 0x00 &~prgAND;
		chrOR  = reg <<6 &0x100 &~chrAND;
	}
	if (reg &0x02) {
		prgAND >>= 1;
		prgOR >>= 1;
		chrAND >>= 2;
		chrOR >>= 2;
		MMC1_syncPRG(prgAND, prgOR);
		MMC1_syncCHR(chrAND, chrOR);
		MMC1_syncMirror();
	} else
	if (reg &0x01) {
		MMC3_syncPRG(prgAND, prgOR);
		MMC3_syncCHR(chrAND, chrOR);
		MMC3_syncMirror();
	} else {
		VRC24_syncPRG(prgAND, prgOR);
		VRC24_syncCHR(chrAND, chrOR);
		VRC24_syncMirror();
	}
}

static int Huang2_getPRGBank (uint8_t bank) {
	return MMC1_getPRGBank(bank) >>1;
}

<<<<<<< HEAD
static void applyMode (uint8_t clear) {
=======
<<<<<<< HEAD
static void applyMode (uint8 clear) {
>>>>>>> 74114ad0 (Update libretro.c)
	PPU_hook = NULL;
	MapIRQHook = NULL;
	GameHBIRQHook = NULL;
	if (reg &0x02) {
		MMC1_activate(clear && init &1, sync, MMC1_TYPE_MMC1B, submapper == 2? Huang2_getPRGBank: NULL, NULL, NULL, NULL);
		if (submapper != 1) MMC1_writeReg(0x8000, 0x80);
		init &= ~1;
	} else
	if (reg &0x01) {
		MMC3_activate(clear && init &2, sync, MMC3_TYPE_AX5202P, NULL, NULL, NULL, NULL);
		init &= ~2;
	} else {
		VRC2_activate(clear && init &4, sync, 0x01, 0x02, NULL, NULL, NULL, NULL);
		if (init &4) { /* The earlier version of Somari needs specific VRC2 CHR register initialization */
			VRC24_writeReg(0xB000, 0xFF); VRC24_writeReg(0xB001, 0xFF); VRC24_writeReg(0xB002, 0xFF); VRC24_writeReg(0xB003, 0xFF);
			VRC24_writeReg(0xC000, 0xFF); VRC24_writeReg(0xC001, 0xFF); VRC24_writeReg(0xC002, 0xFF); VRC24_writeReg(0xC003, 0xFF);
			VRC24_writeReg(0xD000, 0xFF); VRC24_writeReg(0xD001, 0xFF); VRC24_writeReg(0xD002, 0xFF); VRC24_writeReg(0xD003, 0xFF);
			VRC24_writeReg(0xE000, 0xFF); VRC24_writeReg(0xE001, 0xFF); VRC24_writeReg(0xE002, 0xFF); VRC24_writeReg(0xE003, 0xFF);
			init &= ~4;
=======
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
>>>>>>> 5571a0c (Update libretro.c)
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
>>>>>>> 6caec92 (#1 batch of backports/cleanups (#575))
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
>>>>>>> 5571a0c (Update libretro.c)
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

<<<<<<< HEAD
static void UNLSL12Write(uint32 A, uint8 V) {
	switch (mode & 3) {
	case 0: {
		if ((A >= 0xB000) && (A <= 0xE003)) {
			int32 ind = ((((A & 2) | (A >> 10)) >> 1) + 2) & 7;
			int32 sar = ((A & 1) << 2);
			vrc2_chr[ind] = (vrc2_chr[ind] & (0xF0 >> sar)) | ((V & 0x0F) << sar);
			SyncCHR();
		} else
			switch (A & 0xF000) {
			case 0x8000: vrc2_prg[0] = V; SyncPRG(); break;
			case 0xA000: vrc2_prg[1] = V; SyncPRG(); break;
			case 0x9000: vrc2_mirr = V; SyncMIR(); break;
			}
		break;
	}
	case 1: {
		switch (A & 0xE001) {
		case 0x8000: {
			uint8 old_ctrl = mmc3_ctrl;
			mmc3_ctrl = V;
			if ((old_ctrl & 0x40) != (mmc3_ctrl & 0x40))
				SyncPRG();
			if ((old_ctrl & 0x80) != (mmc3_ctrl & 0x80))
				SyncCHR();
			break;
		}
		case 0x8001:
			mmc3_regs[mmc3_ctrl & 7] = V;
			if ((mmc3_ctrl & 7) < 6)
				SyncCHR();
			else
				SyncPRG();
			break;
		case 0xA000:
			mmc3_mirr = V;
			SyncMIR();
			break;
		case 0xC000:
			IRQLatch = V;
			break;
		case 0xC001:
			IRQReload = 1;
			break;
		case 0xE000:
			X6502_IRQEnd(FCEU_IQEXT);
			IRQa = 0;
			break;
		case 0xE001:
			IRQa = 1;
			break;
		}
		break;
	}
	case 2:
	case 3: {
		if (V & 0x80) {
			mmc1_regs[0] |= 0xc;
			mmc1_buffer = mmc1_shift = 0;
			SyncPRG();
		} else {
			uint8 n = (A >> 13) - 4;
			mmc1_buffer |= (V & 1) << (mmc1_shift++);
			if (mmc1_shift == 5) {
				mmc1_regs[n] = mmc1_buffer;
				mmc1_buffer = mmc1_shift = 0;
				switch (n) {
				case 0: SyncMIR(); break;
				case 2: SyncCHR(); break;
				case 3:
				case 1: SyncPRG(); break;
				}
			}
		}
		break;
	}
	}
}

static void UNLSL12HBIRQ(void) {
	if ((mode & 3) == 1) {
		int32 count = IRQCount;
		if (!count || IRQReload) {
			IRQCount = IRQLatch;
			IRQReload = 0;
		} else
			IRQCount--;
		if (!IRQCount) {
			if (IRQa)
				X6502_IRQBegin(FCEU_IQEXT);
>>>>>>> fd87b87 (Update libretro.c)
		}
	}
}

static DECLFW (writeReg) {
	if (A &0x100) {
		uint8_t previousReg = reg;
		reg = V;
		if ((previousReg ^V) &3)
			applyMode(1);
		else
			sync();
=======
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
>>>>>>> 78bbfe6 (Update libretro.c)
	}
}

static void reset (void) {
	reg = 1;
	init = 7;
	if (++game == 1) ++game; /* First game is twice as large */
	if (game >= ROM_size /8) game = 0;
	applyMode(1);
}

static void power (void) {
	reg = 1;
	init = 7;
	game = 0;
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x4020, 0x5FFF, writeReg);
	applyMode(1);
}

<<<<<<< HEAD
static void restore (int version) {
	applyMode(0);
}

void UNLSL12_Init (CartInfo *info) {
	submapper = info->submapper;
	MMC1_addExState();
	MMC3_addExState();
	VRC24_addExState();
	info->Reset = reset;
	info->Power = power;
	GameStateRestore = restore;
	AddExState(stateRegs, ~0, 0, 0);
	if (submapper == 3) AddExState(stateRegsMulti, ~0, 0, 0);
=======
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
>>>>>>> 5571a0c (Update libretro.c)
	}
>>>>>>> 78bbfe6 (Update libretro.c)
}
