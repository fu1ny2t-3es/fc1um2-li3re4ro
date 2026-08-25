/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2005 CaH4e3
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
 * A98402 board, A9711, A9746 similar
 * King of Fighters 96, The (Unl), Street Fighter Zero 2 (Unl)
 *
 */

#include "mapinc.h"
#include "mmc3.h"

<<<<<<< HEAD
static void M187CW(uint32_t A, uint8_t V) {
	if ((A & 0x1000) == ((MMC3_cmd & 0x80) << 5))
=======
static uint8 reg;

static void M187CW(uint16 A, uint16 V) {
	if ((A & 0x1000) == ((mmc3.cmd & 0x80) << 5)) {
>>>>>>> e98261e5 (Update ppu.c)
		setchr1(A, V | 0x100);
	} else {
		setchr1(A, V);
	}
}

<<<<<<< HEAD
static void M187PW(uint32_t A, uint8_t V) {
	if (EXPREGS[0] & 0x80) {
		uint8_t bank = EXPREGS[0] & 0x1F;
		if (EXPREGS[0] & 0x20) {
			if (EXPREGS[0] & 0x40)
				setprg32(0x8000, bank >> 2);
			else
				setprg32(0x8000, bank >> 1);	/* hacky hacky! two mappers in one! need real hw carts to test */
=======
static void M187PW(uint16 A, uint16 V) {
	if (reg & 0x80) {
		uint8 bank = (reg >> 1) & 0x0F;

		if (reg & 0x20) {
			setprg32(0x8000, bank >> 1);
>>>>>>> e98261e5 (Update ppu.c)
		} else {
			setprg16(0x8000, bank);
			setprg16(0xC000, bank);
		}
	} else {
		setprg8(A, V & 0x3F);
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static const uint8_t prot_data[4] = { 0x83, 0x83, 0x42, 0x00 };
static DECLFR(M187Read) {
	return prot_data[EXPREGS[1] & 3];
}
=======
static uint8 prot_data[4] = { 0x83, 0x83, 0x42, 0x00 };
static uint8 M187Read(uint32 A) { return prot_data[EXPREGS[1] & 3]; }
>>>>>>> f9553c43 (Update ppu.c)
=======
static DECLFW(M187Write) {
	if (!(A & 0x01)) {
		reg = V;
		MMC3_FixPRG();
	}
}

static DECLFR(M187Read) {
	return cpu.openbus | 0x80;
}
>>>>>>> e98261e5 (Update ppu.c)

static void M187Power(void) {
	reg = 0;
	MMC3_Power();
	SetReadHandler(0x5000, 0x5FFF, M187Read);
	SetWriteHandler(0x5000, 0x5FFF, M187Write);
}

void Mapper187_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_pwrap = M187PW;
	MMC3_cwrap = M187CW;
	info->Power = M187Power;
	AddExState(&reg, 1, 0, "EXPR");
}
