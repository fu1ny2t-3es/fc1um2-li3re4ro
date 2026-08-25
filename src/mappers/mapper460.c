/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
<<<<<<< HEAD
 *  Copyright (C) 2020
=======
 *  Copyright (C) 2023-2024 negativeExponent
>>>>>>> 7404ed4 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
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

<<<<<<< HEAD
static uint8_t submapper;
static uint8_t reg;
static uint8_t pad;
=======
<<<<<<< HEAD
static uint8 submapper;
static uint8 reg;
static uint8 pad;
>>>>>>> 8bf4e730 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
static DECLFR (readPad) {
	return CartBR(A &~3 | pad &3);
}
=======
static uint8 Mapper460_ReadOB(uint32 A) { return X.DB; }
>>>>>>> 5bd39b4 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
=======
static uint8 Mapper460_ReadOB(uint32 A) { return cpu.DB; }
>>>>>>> 45f08b8 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
=======
static uint8 Mapper460_ReadOB(uint32 A) { return cpu.openbus; }
>>>>>>> 91f7fce (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)

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
>>>>>>> 09aa6a76 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
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
>>>>>>> 5bd39b4 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
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
=======
static uint8 *CHRRAM = NULL;
static uint8 reg;
static uint8 dipsw;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ &dipsw, 1, "DPSW" },
	{ 0 }
};

<<<<<<< HEAD
static void Mapper460_PRGWrap(uint32 A, uint8 V) {
	int prgAND =0x0F;
	int prgOR  =EXPREGS[0] <<4;
	if (EXPREGS[0] &0x38) { /* Menu selection by selectively connecting CPU D7 to reg or not */
		if (~A &0x4000) {
			setprg8(A,         (EXPREGS[0] &0x10? ~2: ~0) &V &prgAND | prgOR &~prgAND);
			setprg8(A |0x4000, (EXPREGS[0] &0x10?  2:  0) |V &prgAND | prgOR &~prgAND);
=======
static void M460PW(uint16 A, uint16 V) {
	uint32 mask = 0x0F;
	uint32 base = reg << 4;

	if (reg & 0x20) {
		/* Menu selection by selectively connecting CPU D7 to reg or not */
		if (reg & 0x10) {
			setprg8(0x8000, (base & ~mask) | ((mmc3.reg[6] & ~0x02) & mask));
			setprg8(0xA000, (base & ~mask) | ((mmc3.reg[7] & ~0x02) & mask));
			setprg8(0xC000, (base & ~mask) | ((mmc3.reg[6] |  0x02) & mask));
			setprg8(0xE000, (base & ~mask) | ((mmc3.reg[7] |  0x02) & mask));
		} else {
			setprg8(0x8000, (base & ~mask) | (mmc3.reg[6] & mask));
			setprg8(0xA000, (base & ~mask) | (mmc3.reg[7] & mask));
			setprg8(0xC000, (base & ~mask) | (mmc3.reg[6] & mask));
			setprg8(0xE000, (base & ~mask) | (mmc3.reg[7] & mask));
>>>>>>> 760535c (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
		}
	} else {
		setprg8(A, (base & ~mask) | (V & mask));
	}
}

static void M460CW(uint16 A, uint16 V) {
	if (reg & 0x04) {
		setchr2(0x0000, mmc3.reg[0] & 0xFE);
		setchr2(0x0800, mmc3.reg[1] | 0x01);
		setchr2(0x1000, mmc3.reg[2]);
		setchr2(0x1800, mmc3.reg[5]);
	} else {
		setchr8r(0x10, 0);
	}
}

static DECLFR(M460Read) {
	/* Menu selection by selectively connecting reg's D7 to PRG /CE or not */
	if ((reg & 0x80) && (dipsw & 0x01)) {
		return cpu.openbus;
	}
	return CartBR(A);
}

static DECLFW(M460Write) {
	if (MMC3_WramIsWritable()) {
		reg = A & 0xFF;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

static void M460Reset(void) {
	reg = 0;
	dipsw++;
	MMC3_Reset();
}

static void M460Power(void) {
	reg = 0;
	dipsw = 0;
	MMC3_Power();
	SetReadHandler(0x8000, 0xFFFF, M460Read);
	SetWriteHandler(0x6000, 0x7FFF, M460Write);
}

static void M460close(void) {
	MMC3_Close();
	if (CHRRAM) {
		FCEU_gfree(CHRRAM);
	}
	CHRRAM = NULL;
}

void Mapper460_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M460CW;
	MMC3_pwrap = M460PW;
	info->Power = M460Power;
	info->Reset = M460Reset;
	info->Close = M460close;
	AddExState(StateRegs, ~0, 0, NULL);

	CHRRAM = (uint8 *)FCEU_gmalloc(8192);
	SetupCartCHRMapping(0x10, CHRRAM, 8192, 1);
	AddExState(CHRRAM, 8192, 0, "CRAM");
>>>>>>> 7404ed4 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
}
