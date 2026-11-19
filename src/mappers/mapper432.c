/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
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
#include "asic_mmc3.h"

<<<<<<< HEAD
static uint8_t submapper;
static uint8_t reg[2];
static uint8_t pad;
=======
<<<<<<< HEAD
static uint8 submapper;
static uint8 reg[2];
static uint8 pad;
>>>>>>> 8bf4e730 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)

static void sync () {
	int prgAND = reg[1] &0x02? 0x0F: 0x1F;
	int chrAND = reg[1] &0x20 && submapper == 3? 0x1FF: reg[1] &0x04? 0x7F: 0xFF;
	int prgOR  = reg[1] <<4 &0x10 | reg[1] <<1 &0x60;
	int chrOR  = reg[1] <<7 &0x80 | reg[1] <<5 &0x100 | reg[1] <<4 &0x200;
	MMC3_syncPRG(prgAND, prgOR &~prgAND);
	MMC3_syncCHR(chrAND, chrOR &~chrAND);
	MMC3_syncMirror();
}

<<<<<<< HEAD
static int getPRGBank (uint8_t bank) {
=======
<<<<<<< HEAD
static int getPRGBank (uint8 bank) {
>>>>>>> 09aa6a76 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
	if (reg[1] &0x40) {
		int mask = reg[1] &(submapper == 2? 0x20: 0x80)? 3: 1;
		return MMC3_getPRGBank(bank &1) &~mask | bank &mask;
	} else
		return MMC3_getPRGBank(bank);
}

static int getCHRBank (uint8_t bank) {
	if (reg[1] &0x20 && submapper == 3)
		return MMC3_getCHRBank(bank &6 | bank >>1 &1) <<1 | bank &1;
	else
		return MMC3_getCHRBank(bank);
=======
<<<<<<< HEAD
static DECLFR(M432Read) {
   if (submapper ==1? !!(EXPREGS[1] &0x20): !!(EXPREGS[0] &0x01)) return EXPREGS[2];
=======
static uint8 M432Read(uint32 A) {
   if (EXPREGS[0] & 1 || EXPREGS[1] & 0x20 && ROM_size <64)
	  return EXPREGS[2];
>>>>>>> 66df110 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
   return CartBR(A);
}

static void M432Write(uint32 A, uint8 V) {
	EXPREGS[A & 1] = V;
	FixMMC3PRG(MMC3_cmd);
	FixMMC3CHR(MMC3_cmd);
<<<<<<< HEAD
>>>>>>> 5bd39b4 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
}

static DECLFR (interceptPRGRead_submapper023) {
	return reg[0] &0x01? (pad &0x0F): CartBR(A);
}

static DECLFR (interceptPRGRead_submapper1) {
	return reg[1] &0x20? (pad &0x0F): CartBR(A);
}

static DECLFR (interceptPRGRead_submapper4) {
	return reg[0] &0x01 && pad &0x01? X.DB: CartBR(A);
}

static DECLFW (writeReg) {
	if (submapper == 3 && reg[1] &0x80)
		;
	else {
		reg[A &1] = V;
		sync();
	}
}

static void reset () {
	reg[0] = reg[1] = 0;
	pad++;
	MMC3_clear();
}

static void power () {
	reg[0] = reg[1] = 0;
	pad = 0;
	MMC3_power();
	SetReadHandler(0x8000, 0xFFFF, submapper == 4? interceptPRGRead_submapper4: submapper == 1? interceptPRGRead_submapper1: interceptPRGRead_submapper023);
}

void Mapper432_Init (CartInfo *info) {
	submapper =info->submapper;
	MMC3_init(info, sync, MMC3_TYPE_AX5202P, getPRGBank, getCHRBank, NULL, writeReg);
	info->Power = power;
	info->Reset = reset;
	AddExState(reg, 2, 0, "EXPR");
=======
=======
static uint8 reg[2];
static uint8 dipsw;

static void M432CW(uint16 A, uint16 V) {
	uint16 mask = (reg[1] & 0x04) ? 0x7F : 0xFF;
	uint16 base  = ((reg[1] << 7) & 0x080) | ((reg[1] << 5) & 0x100) | ((reg[1] << 4) & 0x200);

	setchr1(A, (base & ~mask) | (V & mask));
}

static void M432PW(uint16 A, uint16 V) {
	uint16 mask = (reg[1] & 0x02) ? 0x0F : 0x1F;
	uint16 base = ((reg[1] << 4) & 0x10) | ((reg[1] << 1) & 0x60);

	if (reg[1] & 0x40) { /* NROM */
		if (reg[1] & 0x80) { /* NROM-256 */
			setprg8(0x8000, (base & ~mask) | ((mmc3.reg[6] & ~0x02) & mask));
			setprg8(0xA000, (base & ~mask) | ((mmc3.reg[7] & ~0x02) & mask));
			setprg8(0xC000, (base & ~mask) | ((mmc3.reg[6] |  0x02) & mask));
			setprg8(0xE000, (base & ~mask) | ((mmc3.reg[7] |  0x02) & mask));
		} else { /* NROM-128 */
			setprg8(0x8000, (base & ~mask) | (mmc3.reg[6] & mask));
			setprg8(0xA000, (base & ~mask) | (mmc3.reg[7] & mask));
			setprg8(0xC000, (base & ~mask) | (mmc3.reg[6] & mask));
			setprg8(0xE000, (base & ~mask) | (mmc3.reg[7] & mask));
		}
	} else { /* MMC3 */
		setprg8(A, (base & ~mask) | (V & mask));
	}
}

static DECLFR(M432Read) {
	if ((reg[0] & 0x01) || ((reg[1] & 0x20) && ((ROM.prg.size * 16) < 1024))) {
		return dipsw;
	}
	return CartBR(A);
}

static DECLFW(M432Write) {
	if (MMC3_WramIsWritable()) {
		reg[A & 0x01] = V;
		if (!(A & 0x01) && !(V & 0x01) && ((ROM.prg.size * 16) < 1024)) {
			reg[1] &= ~0x20; /* Writing 0 to register 0 clears register 1's DIP bit */
		}
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
>>>>>>> f4cdc73 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
}

static void M432Reset(void) {
	reg[0] = 0;
	reg[1] = 0;
	dipsw++;
	MMC3_Reset();
}

static void M432Power(void) {
	reg[0] = 0;
	reg[1] = 0;
	dipsw = 0;
	MMC3_Power();
	SetReadHandler(0x8000, 0xFFFF, M432Read);
	SetWriteHandler(0x6000, 0x7FFF, M432Write);
}

void Mapper432_Init(CartInfo *info) {
<<<<<<< HEAD
	submapper =info->submapper;
	GenMMC3_Init(info, 256, 256, 0, 0);
	cwrap = M432CW;
	pwrap = M432PW;
=======
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M432CW;
	MMC3_pwrap = M432PW;
>>>>>>> f4cdc73 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
	info->Power = M432Power;
	info->Reset = M432Reset;
	AddExState(reg, 2, 0, "EXPR");
	AddExState(&dipsw, 1, 0, "CMD0");
>>>>>>> 7404ed4 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
}
