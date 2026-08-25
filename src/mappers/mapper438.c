/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
<<<<<<< HEAD
 *  Copyright (C) 2025 NewRisingSun
=======
 *  Copyright (C) 2012 CaH4e3
 *  Copyright (C) 2002 Xodnizel
 *  Copyright (C) 2023-2024 negativeExponent
>>>>>>> 15c1ee6c (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
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

<<<<<<< HEAD
#include "mapinc.h"
#include "asic_latch.h"
#include "cartram.h"

static uint8_t unrom;
static uint8_t pad;
static uint8_t submapper;

static void sync0 () {
	if (Latch_address &0x01)
		setprg32(0x8000, Latch_address >>2);
	else {
		setprg16(0x8000, Latch_address >>1);
		setprg16(0xC000, Latch_address >>1);
	}
	setchr8(Latch_data >>1);
	setmirror(Latch_data &0x01? MI_H: MI_V);
}

<<<<<<< HEAD
static void sync1 () {
	int mask = ROM_size -9;
	if (unrom) {
		mask++;
		setprg16(0x8000, Latch_data &7 | mask);
		setprg16(0xC000,             7 | mask);
		setchr8r(0x10, 0);
		setmirror(MI_V);
	} else {
		if (Latch_address &0x01)
			setprg32(0x8000, Latch_address >>2 &(mask >>1));
		else {
			setprg16(0x8000, Latch_address >>1 &mask);
			setprg16(0xC000, Latch_address >>1 &mask);
		}
		setchr8(Latch_data >>1);
		setmirror(Latch_data &0x01? MI_H: MI_V);
	}
=======
static void Mapper438_WriteLatch(uint32 A, uint8 V) {
	latch[0] =A &0xFF;
	latch[1] =V;
	Mapper438_Sync();
>>>>>>> 38cb0e69 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
}

static DECLFR (interceptPRGRead) {
	return Latch_address &0x80 && ~Latch_address &0x20 && ~Latch_address &0x100 && pad &1? X.DB: CartBR(A);
}

static void power () {
	unrom = 0;
	pad = 0;
	Latch_power();
	if (submapper == 0) SetReadHandler(0x8000, 0xFFFF, interceptPRGRead);
}

static void reset () {
	unrom = !unrom;
	pad++;
	Latch_clear();
}

void Mapper438_Init (CartInfo *info) {
	submapper = info->submapper;
	if (info->submapper == 1) {
		Latch_init(info, sync1, 0x8000, 0xFFFF, NULL);
		CHRRAM_init(info, 8);
		AddExState(&unrom, 1, 0, "UNRO");
	} else {
		Latch_init(info, sync0, 0x8000, 0xFFFF, NULL);
		AddExState(&pad, 1, 0, "DIPS");
	}
<<<<<<< HEAD
	info->Power = power;
	info->Reset = reset;
=======
=======
/* K-3071 */

#include "mapinc.h"
#include "latch.h"

static void Sync(void) {
	uint16 prg = latch.addr >> 1;
	uint16 chr = latch.data >> 1;
	uint16 mirr = (latch.data & 0x01) ^ 0x01;

	if (latch.addr & 1)
		setprg32(0x8000, prg >> 1);
	else {
		setprg16(0x8000, prg);
		setprg16(0xC000, prg);
	}
	setchr8(chr);
	setmirror(mirr);
}

void Mapper438_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
	info->Reset = Latch_RegReset;
>>>>>>> 15c1ee6c (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
>>>>>>> c0a60450 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
}
