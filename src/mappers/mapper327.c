/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *	Copyright (C) 2015 Cluster
 *	http://clusterrr.com
 *	clusterrr@clusterrr.com
 *
 *  Copyright (C) 2023-2024 negativeExponent
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA	02110-1301	USA
 */
 
/*
    NES 2.0 mapper 327 is used for a 6-in-1 multicart. Its UNIF board name is BMC-10-24-C-A1.

	MMC3-based multicart mapper with CHR RAM, CHR ROM and PRG RAM
	
	$6000-7FFF:	A~[011xxxxx xxMRSBBB]	Multicart reg
		This register can only be written to if PRG-RAM is enabled and writable (see $A001)
		and BBB = 000 (power on state)
	
	BBB = CHR+PRG block select bits (A19, A18, A17 for both PRG and CHR)
	S = PRG block size (0=256k	 1=128k)
	R = CHR mode (0=CHR ROM	 1=CHR RAM)
	M = CHR block size (0=256k	 1=128k)
		ignored when S is 0 for some reason
		
 Example Game:
 --------------------------
 6 in 1 multicart (SMB3, TMNT2, Contra, Ninja Cat, Ninja Crusaders, Rainbow Islands 2)
*/

#include "mapinc.h"
#include "mmc3.h"

<<<<<<< HEAD
static uint8_t submapper;
static uint8_t *CHRRAM = NULL;
static uint32_t CHRRAMSize;

static void BMC1024CA1PW(uint32_t A, uint8_t V) {
	if ((EXPREGS[0]>>3)&1)
		setprg8(A, (V&0x1F) | ((EXPREGS[0] & 7) << 4));
	else 
		setprg8(A, (V&0x0F) | ((EXPREGS[0] & 7) << 4));
}

static void BMC1024CA1CW(uint32_t A, uint8_t V) {
=======
<<<<<<< HEAD
static uint8 submapper;
=======
static uint8 reg;

>>>>>>> 66cc868 (Update ppu.c)
static uint8 *CHRRAM = NULL;
static uint32 CHRRAMSize;

static void M327PW(uint16 A, uint16 V) {
	uint8 base = (reg << 4) & 0x70;
	uint8 mask = (reg & 0x08) ? 0x1F : 0x0F;

	setprg8(A, (base & ~mask) | (V & mask));
}

<<<<<<< HEAD
static void BMC1024CA1CW(uint32 A, uint8 V) {
>>>>>>> e98261e5 (Update ppu.c)
	if ((EXPREGS[0]>>4)&1)
	 	setchr1r(0x10, A, V);
	else if (((EXPREGS[0]>>5)&1) && ((EXPREGS[0]>>3)&1))
		setchr1(A, V | ((EXPREGS[0] & 7) << 7));
	else
	 	setchr1(A, (V&0x7F) | ((EXPREGS[0] & 7) << 7));
	
	if (submapper == 1 && EXPREGS[0] &0x08) setmirror(DRegBuf[MMC3_cmd &0x80? 2: 0] &0x80? MI_1: MI_0);
}

static void BMC1024CA1MW(uint8_t V) {
	A000B = V;
	if (EXPREGS[0] &0x08)
		setmirror(DRegBuf[MMC3_cmd &0x80? 2: 0] &0x80? MI_1: MI_0);
	else
		setmirror(V &1? MI_H: MI_V);
}

static void BMC1024CA1Write(uint32 A, uint8 V) {
	if (((A001B & 0xC0) == 0x80) && !(EXPREGS[0] & 7))
	{
		EXPREGS[0] = A & 0x3F;
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
=======
static void M327CW(uint16 A, uint16 V) {
	if (reg & 0x10) {
		setchr8r(0x10, 0);
>>>>>>> 66cc868 (Update ppu.c)
	} else {
		uint16 base = (reg << 7) & 0x380;
		uint16 mask = (reg & 0x20) ? 0xFF : 0x7F;
		
		setchr1(A, base | (V & mask));
	}
}

static DECLFW(M327Write) {
	if (MMC3_WramIsWritable()) {
		CartBW(A, V);
		if ((reg & 7) == 0) {
			reg = A & 0x3F;
			MMC3_FixPRG();
			MMC3_FixCHR();
		}
	}
}

static void M327Reset(void) {
	reg = 0;
	MMC3_Reset();
}

static void M327Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M327Write);
}

static void M327Close(void) {
	MMC3_Close();
	if (CHRRAM) {
		FCEU_gfree(CHRRAM);
	}
	CHRRAM = NULL;
}

<<<<<<< HEAD
void BMC1024CA1_Init(CartInfo *info) {
	submapper = info->submapper;
	GenMMC3_Init(info, 256, 256, 8, 0);
=======
void Mapper327_Init(CartInfo *info) {
	MMC3_Init(info, 8, 0);
	MMC3_pwrap = M327PW;
	MMC3_cwrap = M327CW;

	info->Power = M327Power;
	info->Reset = M327Reset;
	info->Close = M327Close;
	AddExState(&reg, 1, 0, "EXPR");

>>>>>>> 66cc868 (Update ppu.c)
	CHRRAMSize = 8192;
	CHRRAM = (uint8_t*)FCEU_gmalloc(CHRRAMSize);
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSize, 1);
	AddExState(CHRRAM, CHRRAMSize, 0, "CHRR");
<<<<<<< HEAD
	pwrap = BMC1024CA1PW;
	cwrap = BMC1024CA1CW;
	if (submapper == 1) mwrap = BMC1024CA1MW;
	info->Power = BMC1024CA1Power;
	info->Reset = BMC1024CA1Reset;
	info->Close = BMC1024CA1Close;
	AddExState(EXPREGS, 1, 0, "EXPR");
=======
>>>>>>> 66cc868 (Update ppu.c)
}
