/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
<<<<<<< HEAD
 *  Copyright (C) 2020
=======
 *  Copyright (C) 2022
 *  Copyright (C) 2023-2024 negativeExponent
>>>>>>> 46df7ab (Update libretro_core_options.h)
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

static uint8_t reg[2];

<<<<<<< HEAD
static void sync () {
	if (reg[0] &0x01) {
		if (reg[0] &0x02)
			setprg32(0x8000, reg[0] >>3);
		else {
			setprg16(0x8000, reg[0] >>2);
			setprg16(0xC000, reg[0] >>2);
=======
static void M455PW(uint16 A, uint16 V) {
	uint16 mask = (reg[1] & 0x01) ? 0x1F : 0x0F;
	uint16 base = ((reg[0] >> 2) & 0x10) | ((reg[1] << 1) & 0x08) | ((reg[0] >> 2) & 0x07);

	if (reg[0] & 0x01) {
		if (reg[0] & 0x02) {
			setprg32(0x8000, base >> 1);
		} else {
			setprg16(0x8000, base);
			setprg16(0xC000, base);
>>>>>>> 46df7ab (Update libretro_core_options.h)
		}
		MMC3_syncCHR(0xFF, 0x00);
	} else {
<<<<<<< HEAD
		int prgAND = 0x1F;
		int chrAND = reg[1] &0x02? 0x7F: 0xFF;
		int prgOR  = reg[0] >>1;
		int chrOR  = reg[1] <<6;
		MMC3_syncPRG(prgAND, prgOR &~prgAND);
		MMC3_syncCHR(chrAND, chrOR &~chrAND);
=======
		setprg8(A, ((base << 1) & ~mask) | (V & mask));
>>>>>>> 46df7ab (Update libretro_core_options.h)
	}
	MMC3_syncMirror();
}

<<<<<<< HEAD
<<<<<<< HEAD
static DECLFW (writeReg) {
=======
static void Mapper455_CHRWrap(uint32 A, uint8 V) {
	int chrAND =EXPREGS[1] &0x02? 0xFF: 0x7F;
	int chrOR  =(EXPREGS[0] >>2 &0x07 | EXPREGS[1] <<1 &0x08 | EXPREGS[0] >>2 &0x10) <<4;
	setchr1(A, V &chrAND | chrOR &~chrAND);
}

static void Mapper455_Write(uint32 A, uint8 V) {
>>>>>>> e94122c (Update libretro_core_options.h)
	if (A &0x100) {
		reg[0] = V;
		reg[1] = A &0xFF;
		sync();
	}
}

static void reset () {
	reg[0] = reg[1] = 0;
	MMC3_clear();
}

static void power () {
	reg[0] = reg[1] = 0;
	MMC3_power();
	SetWriteHandler(0x4020, 0x5FFF, writeReg);
}

void Mapper455_Init (CartInfo *info) {
	MMC3_init(info, sync, MMC3_TYPE_AX5202P, NULL, NULL, NULL, NULL);
	info->Power = power;
	info->Reset = reset;
=======
static void M455CW(uint16 A, uint16 V) {
	uint16 mask = (reg[1] & 0x02) ? 0xFF : 0x7F;
	uint16 base = ((reg[0] >> 2) & 0x10) | ((reg[1] << 1) & 0x08) | ((reg[0] >> 2) & 0x07);

	setchr1(A, ((base << 4) & ~mask) | (V & mask));
}

static DECLFW(M455Write) {
	if (A & 0x100) {
		reg[0] = V;
		reg[1] = A & 0xFF;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

static void M455Reset(void) {
	reg[0] = 1;
	reg[1] = 0;
	MMC3_Reset();
}

static void M455Power(void) {
	reg[0] = 1;
	reg[1] = 0;
	MMC3_Power();
	SetWriteHandler(0x4100, 0x5FFF, M455Write);
}

void Mapper455_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M455CW;
	MMC3_pwrap = M455PW;
	info->Power = M455Power;
	info->Reset = M455Reset;
>>>>>>> 46df7ab (Update libretro_core_options.h)
	AddExState(reg, 2, 0, "EXPR");
}
