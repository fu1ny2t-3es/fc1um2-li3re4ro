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
static void Mapper376CW(uint32_t A, uint8_t V) {
	uint32_t base = (EXPREGS[0] &0x40? 0x080: 0x000) | (EXPREGS[1] &0x01? 0x100: 0x000);
	setchr1(A, base | (V & 0x7F));
}

static void Mapper376PW(uint32_t A, uint8_t V) {
	uint32_t base = (EXPREGS[0] &0x07) | (EXPREGS[0] &0x40? 0x08: 0x00) | (EXPREGS[1] &0x01? 0x10: 0x00);
	if (EXPREGS[0] & 0x80) {
		if (EXPREGS[0] &0x20) {
			if (A ==0x8000) setprg32(A, base >>1);
=======
static uint8 reg[2];

static SFORMAT StateRegs[] = {
	{ reg, 2, "REGS" },
	{ 0 }
};

static void M376CW(uint16 A, uint16 V) {
	uint16 base = ((reg[1] << 8) & 0x100) | ((reg[0] << 1) & 0x80);

	setchr1(A, base | (V & 0x7F));
}

static void M376PW(uint16 A, uint16 V) {
	uint16 base = ((reg[1] << 4) & 0x10) | ((reg[0] >> 3) & 0x08) | (reg[0] & 0x07);

	if (reg[0] & 0x80) {
		if (reg[0] & 0x20) {
			setprg32(0x8000, base >> 1);
>>>>>>> 7b06ddc4 (Update libretro.c)
		} else {
			setprg16(0x8000, base);
			setprg16(0xC000, base);
		}
	} else {
		setprg8(A, ((base << 1) & ~0x0F) | (V & 0x0F));
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static DECLFR(Mapper376Read) {
	return EXPREGS[2];
}

static DECLFW(Mapper376Write) {
=======
static void Mapper376Write(uint32 A, uint8 V) {
>>>>>>> dcc6f072 (Update libretro.c)
	EXPREGS[A & 1] = V;
	FixMMC3PRG(MMC3_cmd);
	FixMMC3CHR(MMC3_cmd);
}

static void Mapper376Reset(void) {
	EXPREGS[0] = 0;
	EXPREGS[1] = 0;
	EXPREGS[2]++;
	MMC3RegReset();
}

static void Mapper376Power(void) {
	EXPREGS[0] = 0;
	EXPREGS[1] = 0;
	EXPREGS[2] = 0;
	GenMMC3Power();
	SetReadHandler(0x6000, 0x7FFF, Mapper376Read);
	SetWriteHandler(0x6000, 0x7FFF, Mapper376Write);
}

void Mapper376_Init(CartInfo *info) {
	GenMMC3_Init(info, 128, 256, 1, 0);
	pwrap = Mapper376PW;
	cwrap = Mapper376CW;
	info->Power = Mapper376Power;
	info->Reset = Mapper376Reset;
	AddExState(EXPREGS, 3, 0, "EXPR");
=======
static DECLFW(M376Write) {
	reg[A & 0x01] = V;
	MMC3_FixPRG();
	MMC3_FixCHR();
}

static void M376Power(void) {
	reg[0] = 0;
	reg[1] = 0;
	MMC3_Power();
	SetWriteHandler(0x7000, 0x7FFF, M376Write);
}

void Mapper376_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_pwrap = M376PW;
	MMC3_cwrap = M376CW;
	info->Power = M376Power;
	AddExState(StateRegs, ~0, 0, NULL);
>>>>>>> e257e4ba (Update libretro.c)
}
