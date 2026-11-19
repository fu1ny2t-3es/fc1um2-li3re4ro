/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
<<<<<<< HEAD
 *  Copyright (C) 2025 NewRisingSun
=======
 *  Copyright (C) 2023-2024 negativeExponent
>>>>>>> 46df7ab (Update libretro_core_options.h)
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
<<<<<<< HEAD
=======
 *
 * NES 2.0 Mapper 396 - BMC-830752C
 * 1995 Super 8-in-1 (JY-050 rev0)
 * Super 8-in-1 Gold Card Series (JY-085)
 * Super 8-in-1 Gold Card Series (JY-086)
 * 2-in-1 (GN-51)
>>>>>>> 46df7ab (Update libretro_core_options.h)
 */

#include "mapinc.h"
#include "latch.h"

<<<<<<< HEAD
static uint8_t submapper;
static uint8_t reg[2];
=======
<<<<<<< HEAD
static uint8 submapper;
static uint8 reg[2];
>>>>>>> f6dccad9 (Update libretro_core_options.h)

static void sync () {
	setprg16(0x8000, reg[0] <<3 | reg[1] &0x07);
	setprg16(0xC000, reg[0] <<3 |         0x07);
	setchr8(0);
	switch(submapper) {
		case 1:	/* J.Y. YY850437C */
			setmirror(reg[0] &0x40? MI_H: MI_V);
			break;
		case 2:	/* Realtec GN-51 */
		case 3:	/* Realtec 8030 */
			setmirror(reg[0] &0x20? MI_H: MI_V);
			break;
		default:
			setmirror(reg[0] &0x60? MI_H: MI_V);
			break;
	}
}

<<<<<<< HEAD
static DECLFW (writeReg) {
	reg[(A &0xE000) == 0xA000? 0: 1] = V;
	sync();
}

static DECLFW (writeReg_submapper3) {
	reg[A >>14 &1] = V;
	sync();
=======
static void M396WriteInnerBank(uint32 A, uint8 V) {
	reg[0] = V;
	Sync();
}

static void M396WriteOuterBank(uint32 A, uint8 V) {
	reg[1] = V;
	Sync();
>>>>>>> e94122c (Update libretro_core_options.h)
}

static void power () {
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, submapper == 3? writeReg_submapper3: writeReg);
	reg[0] = reg[1] = 0;
	sync();
}

static void stateRestore (int version) {
	sync();
}

void Mapper396_Init (CartInfo *info) {
	submapper = info->submapper;
	info->Power = power;
	info->Reset = submapper == 3? sync: power;
	GameStateRestore = stateRestore;
	AddExState(reg, 2, 0, "REGS");
=======
static uint8 reg;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ 0 }
};

static void Sync(void) {
	if ((latch.addr & 0x6000) == 0x2000) {
		reg = latch.data;
	}
	setprg16(0x8000, (reg << 3) | (latch.data & 0x07));
	setprg16(0xC000, (reg << 3) | 0x07);
	setchr8(0);
	setmirror((reg & 0x60) ? MI_H : MI_V);
}

static void M396Reset(void) {
	reg = 0;
	Latch_RegReset();
}

void Mapper396_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
	info->Reset = M396Reset;
	AddExState(StateRegs, ~0, 0, NULL);
>>>>>>> 46df7ab (Update libretro_core_options.h)
}
