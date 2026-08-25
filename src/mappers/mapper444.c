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

/* NC7000M PCB, with incorrect UNIF MAPR BS-110 due to a mix-up. Submapper bits 0 and 1. denote the setting of two
 * solder info->submapper that configure CHR banking. */
/* NC8000M PCB, indicated by submapper bit 2. */

#include "mapinc.h"
#include "mmc3.h"

<<<<<<< HEAD
static uint8_t pads;
static uint8_t dip;

static void Mapper444_PRGWrap(uint32_t A, uint8_t V) {
	int prgAND =pads &4 && EXPREGS[0] &0x02? 0x1F: 0x0F;
	int prgOR  =EXPREGS[0] <<4;
	if (EXPREGS[0] &0x04) {
		if (~A &0x4000) {
			setprg8(A,         (~EXPREGS[0] &0x08? ~2: ~0) &V &prgAND | prgOR &~prgAND);
			setprg8(A |0x4000, (~EXPREGS[0] &0x08?  2:  0) |V &prgAND | prgOR &~prgAND);
=======
static uint8 reg;
static uint8 dipsw;

static void M444PW(uint16 A, uint16 V) {
	uint16 mask = ((iNESCart.submapper & 0x04) && (reg & 0x02)) ? 0x1F : 0x0F;
	uint16 base = reg << 4;

	if (reg & 0x04) { /* NROM */
		uint16 bank = (base & ~mask) | (mmc3.reg[6] & mask);
		if (reg & 0x08) { /* NROM-128 */
			setprg16(0x8000, bank >> 1);
			setprg16(0xC000, bank >> 1);
		} else { /* NROM-256 */
			setprg32(0x8000, bank >> 2);
>>>>>>> 7b06ddc4 (Update libretro.c)
		}
	} else { /*  MMC3 */
		setprg8(A, (base & ~mask) | (V & mask));
	}
}

<<<<<<< HEAD
static void Mapper444_CHRWrap(uint32_t A, uint8_t V) {
	int chrAND =pads &1? 0xFF: 0x7F;
	int chrOR  =EXPREGS[0] <<7 &(pads &1? 0x00: 0x80) | EXPREGS[0] <<(pads &2? 4: 7) &0x100;
	setchr1(A, V &chrAND | chrOR &~chrAND);
=======
static void M444CW(uint16 A, uint16 V) {
	uint16 mask = (iNESCart.submapper & 0x01) ? 0xFF : 0x7F;
	uint16 base = ((reg << 7) & ((iNESCart.submapper & 0x01) ? 0x00 : 0x80)) | ((reg << ((iNESCart.submapper & 0x02) ? 4 : 7)) & 0x100);

	setchr1(A, (base & ~mask) | (V & mask));
>>>>>>> 7b06ddc4 (Update libretro.c)
}

static DECLFR(M444Read) {
	if ((reg & 0x0C) == 0x08) {
		return dipsw;
	}
	return CartBR(A);
}

static DECLFW(M444Write) {
	reg = A & 0xFF;
	MMC3_FixPRG();
	MMC3_FixCHR();
}

static void M444Reset(void) {
	dipsw++;
	dipsw &= 3;
	reg = 0;
	MMC3_Reset();
}

static void M444Power(void) {
	dipsw = 0;
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M444Write);
	SetReadHandler(0x8000, 0xFFFF, M444Read);
}

void Mapper444_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M444CW;
	MMC3_pwrap = M444PW;
	info->Power = M444Power;
	info->Reset = M444Reset;
	AddExState(&reg, 1, 0, "EXPR");
	AddExState(&dipsw, 1, 0, "DIPS");
}
