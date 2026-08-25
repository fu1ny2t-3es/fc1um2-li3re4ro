/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2022
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
static void Mapper456_PRGWrap(uint32_t A, uint8_t V) {
	setprg8(A, V &0x0F | EXPREGS[0] <<4);
}

static void Mapper456_CHRWrap(uint32_t A, uint8_t V) {
	setchr1(A, V &0x7F | EXPREGS[0] <<7);
=======
static uint8 reg;

static void M456PW(uint16 A, uint16 V) {
	setprg8(A, (reg << 4) | (V & 0x0F));
}

static void M456CW(uint16 A, uint16 V) {
	setchr1(A, (reg << 7) | (V & 0x7F));
>>>>>>> 8bf4e730 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
}

static DECLFW(M456Write) {
	if (A & 0x100) {
		reg = V;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

static void M456Reset(void) {
	reg = 0;
	MMC3_Reset();
}

static void M456Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x4100, 0x5FFF, M456Write);
}

void Mapper456_Init(CartInfo *info) {
	MMC3_Init(info, 8, 0);
	MMC3_cwrap = M456CW;
	MMC3_pwrap = M456PW;
	info->Power = M456Power;
	info->Reset = M456Reset;
	AddExState(&reg, 1, 0, "EXPR");
}
