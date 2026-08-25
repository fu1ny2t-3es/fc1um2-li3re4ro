/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2017 CaH4e3
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

/* added on 2019-5-23 - NES 2.0 Mapper 260
 * HP10xx/HP20xx - a simplified version of FK23C mapper with pretty strict and better
 * organized banking behaviour. It seems that some 176 mapper assigned game may be
 * actually this kind of board instead but in common they aren't compatible at all,
 * the games on the regular FK23C boards couldn't run on this mapper and vice versa...
 */

<<<<<<< HEAD
static uint8_t unromchr, lock;
static uint32_t dipswitch;

static void BMCHPxxCW(uint32_t A, uint8_t V) {
	if (EXPREGS[0] & 4) {		/* custom banking */
		switch(EXPREGS[0] & 3) {
		case 0:
		case 1:
			setchr8(EXPREGS[2] & 0x3F);
			break;
		case 2:
			setchr8((EXPREGS[2] & 0x3E) | (unromchr & 1));
			break;
		case 3:
			setchr8((EXPREGS[2] & 0x3C) | (unromchr & 3));
			break;
		}
	} else {				/* mmc3 banking */
		int base, mask;
		if(EXPREGS[0] & 1) {	/* 128K mode */
			base = EXPREGS[2] & 0x30;
			mask = 0x7F;
		} else {			/* 256K mode */
			base = EXPREGS[2] & 0x20;
			mask = 0xFF;
		}
		setchr1(A, (V & mask) | (base << 3));
	}
}

/* PRG wrapper */
static void BMCHPxxPW(uint32_t A, uint8_t V) {
	if(EXPREGS[0] & 4) {		/* custom banking */
		if((EXPREGS[0] & 0xF) == 4) {	/* 16K mode */
			setprg16(0x8000, EXPREGS[1] & 0x1F);
			setprg16(0xC000, EXPREGS[1] & 0x1F);
		} else {			/* 32K modes */
			setprg32(0x8000, (EXPREGS[1] & 0x1F) >> 1);
		}
	} else {				/* mmc3 banking */
		uint8_t base, mask;
		if(EXPREGS[0] & 2) {	/* 128K mode */
			base = EXPREGS[1] & 0x18;
			mask = 0x0F;
		} else {			/* 256K mode */
			base = EXPREGS[1] & 0x10;
			mask = 0x1F;
		}
		setprg8(A, (V & mask) | (base << 1));
		setprg8r(0x10, 0x6000, A001B & 3);
	}
}

/* MIRROR wrapper */
static void BMCHPxxMW(uint8_t V) {
	if(EXPREGS[0] & 4) {		/* custom banking */
		setmirror(((unromchr >> 2) & 1) ^ 1);
	} else {				/* mmc3 banking */
		A000B = V;
		setmirror((A000B & 1) ^ 1);
=======
static uint8 reg[4];
static uint8 dipsw;

static SFORMAT StateRegs[] = {
	{ reg, 4, "REGS" },
	{ &dipsw, 1, "DPSW" },
	{ 0 }
};

static void M260CW(uint16 A, uint16 V) {
	uint16 base = reg[2] & 0x7F;

	switch (reg[0] & 0x07) {
	case 0: setchr1(A, ((base << 3) & ~0xFF) | (V & 0xFF)); break;
	case 1: setchr1(A, ((base << 3) & ~0x7F) | (V & 0x7F)); break;
	case 2: setchr1(A, ((base << 3) & ~0xFF) | (V & 0xFF)); break;	
	case 3: setchr1(A, ((base << 3) & ~0x7F) | (V & 0x7F)); break;
	case 4: setchr8(base); break;
	case 5: setchr8(base); break;
	case 6: setchr8((base & ~0x01) | (reg[3] & 0x01)); break;
	case 7: setchr8((base & ~0x03) | (reg[3] & 0x03)); break;
	}
}

static void M260PW(uint16 A, uint16 V) {
	uint8 base = reg[1] & 0x3F;

	switch (reg[0] & 0x07) {
	case 0: setprg8(A, ((base << 1) & ~0x1F) | (V & 0x1F)); break;
	case 1: setprg8(A, ((base << 1) & ~0x1F) | (V & 0x1F)); break;
	case 2: setprg8(A, ((base << 1) & ~0x0F) | (V & 0x0F)); break;
	case 3: setprg8(A, ((base << 1) & ~0x0F) | (V & 0x0F)); break;
	case 4:
		setprg16(0x8000, base);
		setprg16(0xC000, base);
		break;
	case 5: setprg32(0x8000, base >> 1); break;
	case 6: setprg32(0x8000, base >> 1); break;
	case 7: setprg32(0x8000, base >> 1); break;
	}
}

static DECLFR(M260Read) {
	return ((cpu.openbus & ~0x03) | (dipsw & 0x03));
}

static DECLFW(M260WriteReg) {
	if (!(reg[0] & 0x80)) {
		reg[A & 0x03] = V;
		MMC3_FixPRG();
		MMC3_FixCHR();
>>>>>>> f6dccad9 (Update libretro_core_options.h)
	}
}

static DECLFW(M260WriteLatch) {
	if((reg[0] & 0xE6) == 0x06 ) {
		reg[3] = V;
		MMC3_FixCHR();
	} else {
		MMC3_Write(A, V);
	}
}

static void M260Reset(void) {
	dipsw++;
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	MMC3_Reset();
	MMC3_FixPRG();
	MMC3_FixCHR();
}

static void M260Power(void) {
	dipsw = 0;
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	MMC3_Power();
	SetReadHandler(0x5000, 0x5FFF, M260Read);
	SetWriteHandler(0x5000, 0x5FFF, M260WriteReg);
	SetWriteHandler(0x8000, 0xFFFF, M260WriteLatch);
}

void Mapper260_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M260CW;
	MMC3_pwrap = M260PW;
	info->Power = M260Power;
	info->Reset = M260Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
