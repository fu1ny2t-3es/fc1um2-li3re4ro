/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright (C) 2019 Libretro Team
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

/* NES 2.0 mapper 339 is used for a 21-in-1 multicart.
 * Its UNIF board name is BMC-K-3006. 
 * http://wiki.nesdev.com/w/index.php/NES_2.0_Mapper_339
 */

#include "mapinc.h"
#include "mmc3.h"

<<<<<<< HEAD
static uint8_t submapper; /* 0: K-3006, 1: unmarked, 2: TL 8058, 3: K-3091/GN-16 */
=======
<<<<<<< HEAD
static uint8 submapper; /* 0: K-3006, 1: unmarked, 2: TL 8058, 3: K-3091/GN-16 */
>>>>>>> ae14339e (Update Makefile.libretro)

static DECLFR (readPad) {
	return CartBR(A &~3 | EXPREGS[1] &3);
}
=======
<<<<<<< HEAD
static uint8 submapper;
>>>>>>> 7733d52 (Update Makefile.libretro)

static void BMCK3006CW(uint32_t A, uint8_t V) {
	setchr1(A, (V & 0x7F) | (EXPREGS[0] & 0x18) << 4);
=======
static uint8 reg;

static void M339CW(uint16 A, uint16 V) {
	setchr1(A, ((reg << 4) & ~0x7F) | (V & 0x7F));
>>>>>>> ff5b809 (Update Makefile.libretro)
}

<<<<<<< HEAD
static void BMCK3006PW(uint32_t A, uint8_t V) {
	if (EXPREGS[0] & 0x20) {				/* MMC3 mode */
		setprg8(A, (V & 0x0F) | (EXPREGS[0] & 0x18) << 1);
=======
static void M339PW(uint16 A, uint16 V) {
	uint16 base = reg & 0x1F;

	if (reg & 0x20) { /* MMC3 mode */
		setprg8(A, ((base << 1) & ~0x0F) | (V & 0x0F));
>>>>>>> ae14339e (Update Makefile.libretro)
	} else {
<<<<<<< HEAD
		if (submapper ==0 && (EXPREGS[0] & 0x06) == 0x06 ||
<<<<<<< HEAD
		    submapper ==1 && EXPREGS[0] &0x04 ||
		    submapper ==2 && EXPREGS[0] &0x11 ||
		    submapper ==3 && EXPREGS[0] &0x18 ||
		    submapper ==4 && EXPREGS[0] &0x14) { /* NROM-256 */
=======
		    submapper ==1 && EXPREGS[0] &4 ||
		    submapper ==2 && (EXPREGS[0] &1 || EXPREGS[1] &1) ||
		    submapper ==3 && EXPREGS[0] &0x18) { /* NROM-256 */
=======
<<<<<<< HEAD
		if ((EXPREGS[0] & 0x07) == 0x06 && submapper ==0 || EXPREGS[0] &4 && submapper ==1) {	/* NROM-256 */
>>>>>>> 7733d52 (Update Makefile.libretro)
>>>>>>> 30faeea0 (Update Makefile.libretro)
			setprg32(0x8000, (EXPREGS[0] >> 1) & 0x0F);
		} else { /* NROM-128 */
			setprg16(0x8000, EXPREGS[0] & 0x1F);
			setprg16(0xC000, EXPREGS[0] & 0x1F);
=======
		if ((reg & 0x07) == 0x06) { /* NROM-256 */
			setprg32(0x8000, base >> 1);
		} else { /* NROM-128 */
			setprg16(0x8000, base);
			setprg16(0xC000, base);
>>>>>>> ff5b809 (Update Makefile.libretro)
		}
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static DECLFW(BMCK3006Write) {
	EXPREGS[0] = A &0xFF;
<<<<<<< HEAD
=======
	EXPREGS[1] = A >>8 &0xFF;
=======
static void BMCK3006Write(uint32 A, uint8 V) {
	EXPREGS[0] = A & 0x3F;
>>>>>>> d627cc9 (Update Makefile.libretro)
>>>>>>> f8068a80 (Update Makefile.libretro)
	FixMMC3PRG(MMC3_cmd);
	FixMMC3CHR(MMC3_cmd);
	if (submapper == 2 || submapper == 3) SetReadHandler(0x8000, 0xFFFF, EXPREGS[0] &0x80? readPad: CartBR);
}

static void BMCK3006Reset(void) {
	EXPREGS[0] = 0;
	EXPREGS[1]++;
	MMC3RegReset();
}

static void BMCK3006Power(void) {
	EXPREGS[0] = EXPREGS[1] = 0;
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x7FFF, BMCK3006Write);
=======
static DECLFW(M339Write) {
	if (MMC3_WramIsWritable()) {
		reg = A & 0x3F;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

static void M339Reset(void) {
	reg = 0;
	MMC3_Reset();
}

static void M339Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M339Write);
>>>>>>> 7733d52 (Update Makefile.libretro)
}

<<<<<<< HEAD
void BMCK3006_Init(CartInfo *info) {
	submapper =info->submapper;
	GenMMC3_Init(info, 512, 512, 8, 0);
	pwrap = BMCK3006PW;
	cwrap = BMCK3006CW;
	info->Power = BMCK3006Power;
	info->Reset = BMCK3006Reset;
<<<<<<< HEAD
	AddExState(EXPREGS, 2, 0, "EXPR");
=======
<<<<<<< HEAD
	AddExState(EXPREGS, 3, 0, "EXPR");
=======
	AddExState(EXPREGS, 1, 0, "EXPR");
=======
void Mapper339_Init(CartInfo *info) {
	MMC3_Init(info, 8, 0);
	MMC3_pwrap = M339PW;
	MMC3_cwrap = M339CW;
	info->Power = M339Power;
	info->Reset = M339Reset;
	AddExState(&reg, 1, 0, "EXPR");
>>>>>>> ff5b809 (Update Makefile.libretro)
>>>>>>> 7733d52 (Update Makefile.libretro)
>>>>>>> 30faeea0 (Update Makefile.libretro)
}
