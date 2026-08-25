/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
<<<<<<< HEAD
 *  Copyright (C) 2025 NewRisingSun
=======
 *  Copyright (C) 2023-2024 negativeExponent
>>>>>>> cd26ce4 (Update libretro_core_options.h)
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
<<<<<<< HEAD
#include "asic_latch.h"

static uint8_t submapper;
static uint8_t pad;
static uint8_t padSelect;

static DECLFR (interceptPRGRead_submapper0) {
	return Latch_address &0x200? CartBR(A &~0xF | pad &0xF): CartBR(A);
}

<<<<<<< HEAD
static DECLFR (readPad_submapper1) {
	return pad;
}

static DECLFR (interceptPRGRead_submapper2) {
	return padSelect &1? CartBR(A &~0x3 | pad &0x3): CartBR(A);
}

static void sync () {
	int prg = Latch_address >>2 &0x1F | Latch_address >>3 &0x20 | Latch_address >>4 &0x40;
	if (Latch_address &0x080) {
		if (Latch_address &0x001)
			setprg32(0x8000, prg >>1);
		else {
			setprg16(0x8000, prg);
			setprg16(0xC000, prg);
		}
		setmirror(Latch_data &0x10? MI_1: MI_0);
	} else {
		setprg16(0x8000, prg);
		setprg16(0xC000, prg | 7);
	}
	SetupCartCHRMapping(0, CHRptr[0], CHRsize[0], submapper == 0 && Latch_address &0x80? 0: 1);
	setchr8(Latch_data);
	setmirror(Latch_address &0x002? MI_H: MI_V);
<<<<<<< HEAD
=======
	SetReadHandler(0x8000, 0xFFFF, submapper == 0 && Latch_address &0x200? readPad_submapper0: submapper == 2 && padSelect &1? readPad_submapper2: CartBR);
=======
static uint8 Mapper449_Read(uint32 A)
{
   if (dipselect)
      return dipswitch &0x3;
   else
   if (latchAddr &0x200)
      return CartBR(A | dipswitch &0xF);
   return CartBR(A);
}

static void Mapper449_WriteDIPSelect(uint32 A, uint8 V) { dipselect = V & 1; }

static void Mapper449_WriteLatch(uint32 A, uint8 V)
{
   latchData =V;
   latchAddr =A &0xFFFF;
   Mapper449_Sync();
>>>>>>> 4d55ef8 (Update libretro_core_options.h)
>>>>>>> 83ce7510 (Update libretro_core_options.h)
}

static DECLFW (writePad_submapper2) {
	padSelect = V;
	sync();
}

static void power () {
	pad = padSelect = 0;
	Latch_power();
	switch(submapper) {
		case 0:
			SetReadHandler(0x8000, 0xFFFF, interceptPRGRead_submapper0);
			break;
		case 1:
			SetReadHandler(0x5000, 0x5FFF, readPad_submapper1);
			break;
		case 2:
			SetReadHandler(0x8000, 0xFFFF, interceptPRGRead_submapper2);
			SetWriteHandler(0x6000, 0x7FFF, writePad_submapper2);
			break;
	}
}

<<<<<<< HEAD
static void reset () {
	pad++;
	padSelect = 0;
	Latch_clear();
}

void Mapper449_Init (CartInfo *info) {
	submapper = info->submapper;
	Latch_init(info, sync, 0x8000, 0xFFFF, NULL);
	info->Power = power;
	info->Reset = reset;
	AddExState(&pad, 1, 0, "DIPS");
	if (submapper == 2) AddExState(&padSelect, 1, 0, "DIPE");
=======
void Mapper449_Init(CartInfo *info)
{
   info->Power       = Mapper449_Power;
   info->Reset       = Mapper449_Reset;
   AddExState(StateRegs, ~0, 0, 0);
>>>>>>> 4d55ef8 (Update libretro_core_options.h)
=======
#include "latch.h"

static uint8 dipsw;

static SFORMAT StateRegs[] = {
    { &dipsw,  1, "DPSW" },
    { 0 }
};

static void Sync(void) {
	uint32 prg = ((latch.addr >> 3) & 0x20) | ((latch.addr >> 2) & 0x1F);
	uint32 cpuA14 = latch.addr & 0x01;
	uint32 nrom = (latch.addr >> 7) & 0x01;

	setprg16(0x8000, prg & ~(cpuA14 * nrom));
	setprg16(0xC000, prg | (cpuA14 * nrom) | (0x07 * !nrom));

	setchr8(latch.data);
    setmirror((((latch.addr >> 1) & 0x01) ^ 0x01));
}

static DECLFR(M449Read) {
	if (latch.addr & 0x200) {
		A |= dipsw;
	}
	return CartBR(A);
}

static void M449Reset(void) {
	dipsw  = (dipsw + 1) & 0xF;
	Latch_RegReset();
}

void Mapper449_Init(CartInfo *info) {
	Latch_Init(info, Sync, M449Read, FALSE, FALSE);
	info->Reset = M449Reset;
	AddExState(StateRegs, ~0, 0, NULL);
>>>>>>> cd26ce4 (Update libretro_core_options.h)
}
