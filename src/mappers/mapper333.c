/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2016 CaH4e3
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
 * 8-in-1  Rockin' Kats, Snake, (PCB marked as "8 in 1"), similar to 12IN1,
 * but with MMC3 on board, all games are hacked the same, Snake is buggy too!
 *
 * no reset-citcuit, so selected game can be reset, but to change it you must use power
 *
 */

#include "mapinc.h"
#include "mmc3.h"

<<<<<<< HEAD
static uint8_t submapper;
=======
<<<<<<< HEAD
static uint8 submapper;
>>>>>>> 8bf4e730 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)

static void BMC8IN1CW(uint32_t A, uint8_t V) {
	setchr1(A, ((EXPREGS[0] & 0xC) << 5) | (V & 0x7F));
}

static void BMC8IN1PW(uint32_t A, uint8_t V) {
	if(EXPREGS[0] & 0x10 || submapper == 1) {		/* MMC3 mode */
		setprg8(A, ((EXPREGS[0] & 0xC) << 2) | (V & 0xF));
=======
static uint8 reg;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ 0 }
};

static void M333CW(uint16 A, uint16 V) {
	setchr1(A, ((reg & 0x0C) << 5) | (V & 0x7F));
}

static void M333PW(uint16 A, uint16 V) {
	if (reg & 0x10) { /* MMC3 mode */
		setprg8(A, ((reg & 0x0C) << 2) | (V & 0x0F));
>>>>>>> 15c1ee6c (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
	} else {
		setprg32(0x8000, reg & 0x0F);
	}
}

static DECLFW(M333Write) {
	if (A & 0x1000) {
		reg = V;
		MMC3_FixPRG();
		MMC3_FixCHR();
	} else {
		MMC3_Write(A, V);
	}
}

static void M333Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x8000, 0xFFFF, M333Write);
}

<<<<<<< HEAD
static void BMC8IN1Reset(void) {
	EXPREGS[0] = 0;
	FixMMC3PRG(0);
	FixMMC3CHR(0);
}

void BMC8IN1_Init(CartInfo *info) {
	submapper = info->submapper;
	GenMMC3_Init(info, 128, 128, 0, 0);
	cwrap = BMC8IN1CW;
	pwrap = BMC8IN1PW;
	info->Power = BMC8IN1Power;
	info->Reset = BMC8IN1Reset;
	AddExState(EXPREGS, 1, 0, "EXPR");
=======
void Mapper333_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M333CW;
	MMC3_pwrap = M333PW;
	info->Power = M333Power;
	AddExState(&StateRegs, ~0, 0, NULL);
>>>>>>> 760535c (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
}
