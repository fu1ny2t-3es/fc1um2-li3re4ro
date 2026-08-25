/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2008 CaH4e3
 *  Copyright (C) 2019 Libretro Team
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

/* NES 2.0 Mapper 287
 * BMC-411120-C, actually cart ID is 811120-C, sorry ;) K-3094 - another ID
 * - 4-in-1 (411120-C)
 * - 4-in-1 (811120-C,411120-C) [p4][U]
 *
 * BMC-K-3088, similar to BMC-411120-C but without jumper or dipswitch
 * - 19-in-1(K-3088)(810849-C)(Unl)
 */

/* 2023-03-02
 - use PRG size to determine variant
 - remove forced mask for outer-bank (rely on internal mask set during PRG/CHR mapping)
 */

#include "mapinc.h"
#include "mmc3.h"

<<<<<<< HEAD
<<<<<<< HEAD
static uint8_t reset_flag = 0;
static uint8_t isK3088;

static void BMC411120CCW(uint32_t A, uint8_t V) {
	uint32_t mask = isK3088 ? 0x07 : 0x03;
	setchr1(A, V | ((EXPREGS[0] & mask) << 7));
}

static void BMC411120CPW(uint32_t A, uint8_t V) {
	uint32_t mask = isK3088 ? 0x07 : 0x03;
	if (EXPREGS[0] & (isK3088 ? 8 : (8 | reset_flag))) { 	/* 32K Mode */
=======
static uint8 reset_flag = 0;
=======
static uint8 reg;
static uint8 dipsw;
>>>>>>> 7b06ddc4 (Update libretro.c)

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ &dipsw, 1, "DPSW" },
	{ 0 }
};

static void M287CW(uint16 A, uint16 V) {
	uint16 base = reg & 0x07;

	setchr1(A, (base << 7) | (V & 0x7F));
}

static void M287PW(uint16 A, uint16 V) {
	uint16 mode = reg & ((dipsw && ((ROM.prg.size * 16) <= 512)) ? 0x0C : 0x08);
	uint16 base = reg & 0x07;

	if (mode) {
		/* 32K Mode */
		setprg32(0x8000, (base << 2) | ((reg >> 4) & 0x03));
		/* FCEU_printf("32K mode: bank:%02x\n", ((reg >> 4) & 3) | ((reg & 7) << 2)); */
	} else {
		/* MMC3 Mode */
		setprg8(A, (base << 4) | (V & 0x0F));
		/* FCEU_printf("MMC3: %04x:%02x\n", A, (V & 0x0F) | ((reg & 7) << 4)); */
	}
}

static DECLFW(M287WriteReg) {
	/*	printf("Wr: A:%04x V:%02x\n", A, V); */
	if (MMC3_WramIsWritable()) {
		reg = A;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

<<<<<<< HEAD
static void BMC411120CPW(uint32 A, uint8 V) {
	if (EXPREGS[0] & (8 | reset_flag)) { 	/* 32K Mode */
>>>>>>> bde79411 (Get rid of conditional in mapper287.c)
		if (A == 0x8000)
			/* bit 0-1 of register should be used as outer bank regardless of banking modes */
			setprg32(A, ((EXPREGS[0] >> 4) & 3) | ((EXPREGS[0] & 0x03) << 2));
	} else													/* MMC3 Mode */
		setprg8(A, (V & 0x0F) | ((EXPREGS[0] & 0x03) << 4));
=======
static void M287Reset(void) {
	reg = 0;
	dipsw ^= 4;
	MMC3_Reset();
>>>>>>> 7b06ddc4 (Update libretro.c)
}

static void M287Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M287WriteReg);
}

void Mapper287_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_pwrap = M287PW;
	MMC3_cwrap = M287CW;
	info->Power = M287Power;
	info->Reset = M287Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
