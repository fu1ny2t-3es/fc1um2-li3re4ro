/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2022
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
#include "latch.h"

<<<<<<< HEAD
static uint8_t latch_data;
static uint32_t latch_addr;
static uint8_t dipswitch;
=======
<<<<<<< HEAD
static uint8 latch_data;
static uint32 latch_addr;
static uint8 dipswitch;
>>>>>>> 5926d713 (Update libretro.c)

static SFORMAT StateRegs[] =
{
	{ &latch_addr, 4 | FCEUSTATE_RLSB, "ADDR" },
	{ &latch_data, 1, "DATA" },
	{ &dipswitch,  1, "DIPS" },
=======
static uint8 dipsw;

static SFORMAT StateRegs[] = {
	{ &dipsw, 1, "DPSW" },
>>>>>>> 5d22892 (Update libretro.c)
	{ 0 }
};

static DECLFR(Mapper414_ReadOB) {
   return X.DB;
}

static void Sync(void) {
	if (latch.addr & 0x2000) { /* NROM-256 */
		setprg32(0x8000, latch.addr >> 2);
	} else { /* NROM-128 */
		setprg16(0x8000, latch.addr >> 1);
		setprg16(0xC000, latch.addr >> 1);
	}
<<<<<<< HEAD
	SetReadHandler(0xC000, 0xFFFF, ~latch_addr &0x100 && latch_addr &dipswitch? Mapper414_ReadOB: CartBR);
	setchr8(latch_data);
	setmirror((latch_addr & 1) ^ 1);
=======
	setchr8(latch.data);
	setmirror((latch.addr & 0x01) ^ 0x01);
>>>>>>> 5d22892 (Update libretro.c)
}

static DECLFR(M414Read) {
	if ((A >= 0xC000) && !(latch.addr & 0x100) && (latch.addr & (dipsw << 4))) {
		return cpu.openbus;
	}
	return CartBR(A);
}

<<<<<<< HEAD
static void M414Power(void) {
	dipswitch =0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M414Write);
}

static void M414Reset(void) {
	dipswitch =dipswitch ==0xF0? 0x00: (dipswitch <<1 | 0x10);
}

static void StateRestore(int version) {
=======
static void M414Reset(void) {
	dipsw++;
	dipsw &= 0x0F;
>>>>>>> 5d22892 (Update libretro.c)
	Sync();
}

void Mapper414_Init(CartInfo *info) {
<<<<<<< HEAD
	info->Power = M414Power;
	info->Reset = M414Reset;
	GameStateRestore = StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
=======
	Latch_Init(info, Sync, M414Read, FALSE, TRUE);
	info->Reset = M414Reset;
	AddExState(StateRegs, ~0, 0, NULL);
>>>>>>> 5d22892 (Update libretro.c)
}
