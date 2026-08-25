/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2015 CaH4e3
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
 * NES 2.0 Mapper 292 - PCB BMW8544
 * UNIF UNL-DRAGONFIGHTER
 * "Dragon Fighter" protected MMC3 based custom mapper board
 * mostly hacky implementation, I can't verify if this mapper can read a RAM of the
 * console or watches the bus writes somehow.
 *
 * TODO: needs updating
 *
 */

#include "mapinc.h"
#include "mmc3.h"

<<<<<<< HEAD
static void UNLBMW8544PW(uint32_t A, uint8_t V) {
	if(A == 0x8000)
		setprg8(A,EXPREGS[0] & 0x1F);	/* the real hardware has this bank overrided with it's own register,
										 * but MMC3 prg swap still works and you can actually change bank C000 at the same time if use 0x46 cmd
										 */
	else
		setprg8(A,V);
}

static void UNLBMW8544CW(uint32_t A, uint8_t V) {
	if(A == 0x0000)
		setchr2(0x0000,(V >> 1) ^ EXPREGS[1]);
	else if (A == 0x0800)
		setchr2(0x0800,(V >> 1) | ((EXPREGS[2] & 0x40) << 1));
	else if (A == 0x1000)
		setchr4(0x1000, EXPREGS[2] & 0x3F);

=======
static uint8 reg[3];

static SFORMAT StateRegs[] = {
	{ reg, 3, "REGS" },
	{ 0 }
};

static void M292PW(uint16 A, uint16 V) {
	setprg8(A, V);
	setprg8(0x8000, reg[0] & 0x1F);
}

static void M292CW(uint16 A, uint16 V) {
	setchr2(0x0000, (mmc3.reg[0] >> 1) ^ reg[1]);
	setchr2(0x0800, (mmc3.reg[1] >> 1) ^ ((reg[2] & 0x40) << 1));
	setchr4(0x1000, reg[2] & 0x3F);
>>>>>>> a1c8c17c (Update libretro_core_options.h)
}

static DECLFW(M292ProtWrite) {
	reg[0] = V;
	MMC3_FixPRG();
}

static DECLFR(M292ProtRead) {
	if ((reg[0] & 0xE0) == 0xC0) {
		reg[1] = ARead[0x6A](0x6A);
	} else {
		reg[2] = ARead[0xFF](0xFF);
	}
	MMC3_FixCHR();
	return cpu.openbus;
}

<<<<<<< HEAD
<<<<<<< HEAD
static DECLFR(UNLBMW8544ProtRead) {
	if(!(A & 1)) {
		if((EXPREGS[0] & 0xE0) == 0xC0) {
			EXPREGS[1] = ARead[0x6a](0x6a);	/* program can latch some data from the BUS, but I can't say how exactly, */
		} else {							/* without more equipment and skills ;) probably here we can try to get any write */
			EXPREGS[2] = ARead[0xff](0xff);	/* before the read operation */
		}
		FixMMC3CHR(MMC3_cmd & 0x7F);		/* there are more different behaviour of the board that's not used by game itself, so unimplemented here and */
	}										/* actually will break the current logic ;) */
=======
static uint8 UNLBMW8544ProtRead(uint32 A) {
	{
		if(!(A & 1)) {
			if((EXPREGS[0] & 0xE0) == 0xC0) {
				EXPREGS[1] = ARead[0x6a](0x6a);	/* program can latch some data from the BUS, but I can't say how exactly, */
			} else {							/* without more equipment and skills ;) probably here we can try to get any write */
				EXPREGS[2] = ARead[0xff](0xff);	/* before the read operation */
			}
			FixMMC3CHR(MMC3_cmd & 0x7F);		/* there are more different behaviour of the board that's not used by game itself, so unimplemented here and */
		}										/* actually will break the current logic ;) */
	}
>>>>>>> d5085b8d (Update libretro_core_options.h)
	return 0;
=======
static void M292Power(void) {
	reg[0] = reg[1] = reg[2] = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M292ProtWrite);
	SetReadHandler(0x6000, 0x7FFF, M292ProtRead);
>>>>>>> a1c8c17c (Update libretro_core_options.h)
}

void Mapper292_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_pwrap  = M292PW;
	MMC3_cwrap  = M292CW;
	info->Power = M292Power;
	AddExState(StateRegs, ~0, 0, NULL);
}
