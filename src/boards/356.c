/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2020
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
 *
 */

#include "mapinc.h"
#include "asic_tc3294.h"

static uint8_t *CHRRAM = NULL;
static uint32_t CHRRAMSize;

static void sync () {
	TC3294_syncPRG(0xFF, 0x00);
	if (TC3294_reg[2] &0x20)
		TC3294_syncCHR(0x1FF, 0x00);
	else
		setchr8r(0x10, 0);
	if (TC3294_reg[2] &0x40) {
		setntamem(CHRptr[0x10] +0x0000, 1, 0);
		setntamem(CHRptr[0x10] +0x0400, 1, 1);
		setntamem(CHRptr[0x10] +0x0800, 1, 2);
		setntamem(CHRptr[0x10] +0x0C00, 1, 3);
	} else
		TC3294_syncMirror();
}

<<<<<<< HEAD
static void close (void) {
	if (CHRRAM) {
		FCEU_gfree(CHRRAM);
		CHRRAM =NULL;
=======
static void M356Write(uint32 A, uint8 V) {
	if (!(EXPREGS[3] & 0x40)) {
		EXPREGS[EXPREGS[4]] = V;
		EXPREGS[4] = (EXPREGS[4] + 1) & 3;
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
>>>>>>> 5bd39b4 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
	}
}

void Mapper356_Init (CartInfo *info) {
	TC3294_init(info, sync);
	info->Close = close;
	CHRRAMSize = 8192;
	CHRRAM = (uint8_t*)FCEU_gmalloc(CHRRAMSize);
	AddExState(CHRRAM, CHRRAMSize, 0, "CRAM");
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSize, 1);
}
