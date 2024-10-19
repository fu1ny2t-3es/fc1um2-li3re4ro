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
 *
 */

/* Mapper 411 - A88S-1
 * 1997 Super 7-in-1 (JY-201)
 * 1997 Super 6-in-1 (JY-202)
 * 1997 Super 7-in-1 (JY-203)
 * 1997 龍珠武鬥會 7-in-1 (JY-204)
 * 1997 Super 7-in-1 (JY-205)
 * 1997 Super 7-in-1 (JY-206)
 */

#include "mapinc.h"
#include "mmc3.h"

<<<<<<< HEAD
static uint8 submapper;

static void M411CW(uint32 A, uint8 V) {
	int chrAND, chrOR;
	switch(submapper) {
		default:chrOR =EXPREGS[1] <<5 &0x080 | EXPREGS[0] <<4 &0x100 | EXPREGS[1] <<2 &0x200;
			chrAND =EXPREGS[1] &0x02? 0xFF: 0x7F;
			break;
		case 1: chrOR =EXPREGS[1] <<5 &0x080 | EXPREGS[1] <<2 &0x100;
			chrAND =EXPREGS[1] &0x02? 0xFF: 0x7F;
			break;
		case 2: chrOR =EXPREGS[1] <<5 &0x080 | EXPREGS[0] <<4 &0x100 | EXPREGS[1] <<2 &0x200;
			chrAND =EXPREGS[1] &0x02? 0xFF: 0x7F;
			break;
	}
	setchr1(A, V &chrAND | chrOR &~chrAND);
}

static void M411PW(uint32 A, uint8 V) {
	int prgAND, prgOR;
	switch(submapper) {
		default:prgOR =EXPREGS[1] <<1 &0x10 | EXPREGS[1] >>1 &0x60;
			prgAND =EXPREGS[1] &0x02? 0x1F: 0x0F;
			break;
		case 1: prgOR =EXPREGS[1] <<1 &0x10 | EXPREGS[1] >>1 &0x60;
			prgAND =EXPREGS[1] &0x02? 0x1F: 0x0F;
			break;
		case 2: prgOR =EXPREGS[1] <<1 &0x10 | EXPREGS[1] >>1 &0x60;
			prgAND =EXPREGS[1] &0x01? 0x1F: 0x0F;
			break;
	}	
	if (EXPREGS[0] & 0x40) { /* NROM Mode */
		uint32 bank = EXPREGS[0] &5 | EXPREGS[0] >>2 &2 | prgOR >>1;		
		if (EXPREGS[0] & 0x02) /* NROM-256 */
			setprg32(0x8000, bank >> 1);
		else { 	/* NROM-128 */
			setprg16(0x8000, bank);
			setprg16(0xC000, bank);
		}
	} else
		setprg8(A, V &prgAND | prgOR &~prgAND);
}

static DECLFR(M411Read5000) {
	return EXPREGS[2];
}

<<<<<<< HEAD
static DECLFW(M411Write5000) {
	if (submapper ==2 || A &0x800) {
		EXPREGS[A & 1] = V;
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
	}
}

static void M411Reset(void) {
	EXPREGS[2]++;
=======
static void M411Write5000(uint32 A, uint8 V) {
	EXPREGS[A & 1] = V;
	FixMMC3PRG(MMC3_cmd);
	FixMMC3CHR(MMC3_cmd);
>>>>>>> a2c232f (Update libretro_core_options.h)
}

static void M411Power(void) {
	EXPREGS[0] = 0x0;
	EXPREGS[1] = 0x3;
	EXPREGS[2] = 0x0; /* Serves as DIP value */
	GenMMC3Power();
	SetReadHandler(0x5000, 0x5FFF, M411Read5000);
	SetWriteHandler(0x5000, 0x5FFF, M411Write5000);
}

void Mapper411_Init(CartInfo *info) {
	submapper =info->submapper;
	GenMMC3_Init(info, 256, 256, 0, 0);
	pwrap = M411PW;
	cwrap = M411CW;
	info->Power = M411Power;
	info->Reset = M411Reset;
	AddExState(EXPREGS, 3, 0, "EXPR");
=======
static uint8 reg[2];

static void M411CW(uint16 A, uint16 V) {
	uint16 mask = (reg[1] & 0x02) ? 0xFF : 0x7F;
	uint16 base = ((reg[0] << 4) & 0x100) | ((reg[1] << 5) & 0x80);

	setchr1(A, (base & ~mask) | (V & mask));
}

static void M411PW(uint16 A, uint16 V) {
	uint16 mask = (reg[1] & 0x02) ? 0x1F : 0x0F;
	uint16 base = ((reg[1] >> 2) & 0x10) | (reg[1] & 0x08) | (reg[0] & 0x04) | ((reg[0] >> 2) & 0x02) | (reg[0] & 0x01);

	/* NROM Mode */
	if ((reg[0] & 0x40) && !(reg[0] & 0x20)) { /* $5xx0 bit 5 check required for JY-212 */
		if (reg[0] & 0x02) { /* NROM-256 */
			setprg32(0x8000, base >> 1);
		} else { /* NROM-128 */
			setprg16(0x8000, base);
			setprg16(0xC000, base);
		}
	} else { /* MMC3 */
		setprg8(A, ((base << 1) & ~mask) | (V & mask));
	}
}

static DECLFW(M411Write) {
	if (A & 0x800) {
		reg[A & 0x01] = V;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

static void M411Reset(void) {
	reg[0] = 0x80;
	reg[1] = 0x82;
	MMC3_Reset();
}

static void M411Power(void) {
	reg[0] = 0x80;
	reg[1] = 0x82;
	MMC3_Power();
	SetWriteHandler(0x5000, 0x5FFF, M411Write);
}

void Mapper411_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_pwrap = M411PW;
	MMC3_cwrap = M411CW;
	info->Power = M411Power;
	info->Reset = M411Reset;
	AddExState(reg, 2, 0, "EXPR");
>>>>>>> 61ed4a1 (Update libretro_core_options.h)
}
