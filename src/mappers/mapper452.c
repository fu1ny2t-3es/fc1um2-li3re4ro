/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2012 CaH4e3
 *  Copyright (C) 2002 Xodnizel
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

/* DS-9-27. Absolutely insane PCB that overlays 8 KiB of WRAM into a selectable position between $8000 and $E000. */

#include "mapinc.h"
#include "latch.h"

<<<<<<< HEAD
static uint8_t *WRAM;
static uint32_t WRAMSIZE;
static uint16_t latch[2];
static uint8_t submapper;

static void SetWRAM(uint16_t A) {
	setprg8r(0x10, A, 0);
	SetWriteHandler(A, A |0x1FFF, CartBW);
}

static void Mapper452_WriteLatch(uint32 A, uint8 V); /* forward declaration */

static void Mapper452_Sync(void) {
	SetWriteHandler(0x8000, 0xFFFF, Mapper452_WriteLatch);
	if (submapper ==1) {
		switch(latch[0] &0xF000) {
		case 0xA000:
			setprg16(0x8000, latch[0] >>1);
			setprg8 (0xC000, 0);
			SetWRAM(latch[0] <<4 &0x6000 |0x8000);
			break;
		case 0xC000:
			setprg16(0x8000, latch[0] >>1 |0);
			setprg16(0xC000, latch[0] >>1 |1);
			SetWRAM(latch[0] <<4 &0x6000 |0x8000);
			break;
		case 0xD000:
			setprg8(0x8000, latch[0]);
			setprg8(0xA000, latch[0]);
			setprg8(0xC000, latch[0]);
			setprg8(0xE000, latch[0]);
			SetWRAM(latch[0] <<4 &0x2000 |0x8000);
			SetWRAM(latch[0] <<4 &0x2000 |0xC000);
			break;
		case 0xE000:
			setprg16(0x8000, latch[0] >>1);
			setprg16(0xC000, latch[0] &0x100? (latch[0] >>1 |7): 0);
			SetWRAM(latch[0] <<4 &0x6000 |0x8000);
			break;
		default:
			setprg16(0x8000, latch[0] >>1);
			setprg16(0xC000, 0);
			break;
		}
		setchr8(0);
		setmirror(latch[0] &0x800? MI_H: MI_V);
	} else {
		uint8_t wramBank = latch[1] >>3 &6 |8;
		if (latch[1] &2) {
			setprg8(0x8000, latch[0] >>1);
			setprg8(0xA000, latch[0] >>1);
			setprg8(0xC000, latch[0] >>1);
			setprg8(0xE000, latch[0] >>1);
			SetWRAM((wramBank ^4) <<12);
		} else
		if (latch[1] &8) {
			setprg8(0x8000, latch[0] >>1 &~1 |0);
			setprg8(0xA000, latch[0] >>1 &~1 |1);
			setprg8(0xC000, latch[0] >>1 &~1 |2);
			setprg8(0xE000, latch[0] >>1 &~1 |3 | latch[1] &4 | (latch[1] &0x04 && latch[1] &0x40? 8: 0));
		} else {
			setprg16(0x8000, latch[0] >>2);
			setprg16(0xC000, 0);
		}
		SetWRAM(wramBank <<12);
		setchr8(0);
		setmirror(latch [1] &1 ^1);
=======
static void Sync(void) {
	uint16 prgbank = latch.addr >> 1;
	uint16 prgram_addr = 0x8000 | ((latch.data << 9) & 0x6000);

	if (latch.data & 0x02) {
		setprg8(0x8000, prgbank);
		setprg8(0xA000, prgbank);
		setprg8(0xC000, prgbank);
		setprg8(0xE000, prgbank);
	} else if (latch.data & 0x08) {
		setprg8(0x8000, (prgbank & ~0x01) | 0);
		setprg8(0xA000, (prgbank & ~0x01) | 1);
		setprg8(0xC000, (prgbank & ~0x01) | 2);
		setprg8(0xE000, (prgbank & ~0x01) | 3
			| (latch.data & 0x04)
			| ((latch.data & 0x04) && (latch.data & 0x40) ? 0x08 : 0x00));
	} else {
		setprg16(0x8000, latch.addr >> 2);
		setprg16(0xC000, 0);
	}

	setchr8(0);
	setmirror((latch.data & 0x01) ^ 0x01);

	setprg8r(0x10, prgram_addr, 0);
	if (latch.data & 0x02) {
		setprg8r(0x10, prgram_addr ^ 0x4000, 0);
>>>>>>> e98261e5 (Update ppu.c)
	}
}

static DECLFW(M452Write) {
	switch (A & 0xE000) {
	case 0x8000:
	case 0xA000:
	case 0xC000:
		Latch_Write(A, V);
		break;
	case 0xE000:
		CartBW(A, V);
		break;
	}
}

static void M452Power(void) {
	Latch_Power();
	SetWriteHandler(0x8000, 0xFFFF, M452Write);
}

void Mapper452_Init(CartInfo *info) {
<<<<<<< HEAD
	submapper =info->submapper;
	info->Reset = Mapper452_Reset;
	info->Power = Mapper452_Power;
	info->Close = Mapper452_Close;
	GameStateRestore = StateRestore;

	WRAMSIZE =8192;
	WRAM =(uint8_t*) FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
	AddExState(&latch, 4, 0, "LATC");
=======
	Latch_Init(info, Sync, NULL, TRUE, FALSE);
	info->Reset = Latch_RegReset;
	info->Power = M452Power;
>>>>>>> e98261e5 (Update ppu.c)
}
