/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
<<<<<<< HEAD
 *  Copyright (C) 2025 NewRisingSun
=======
 *  Copyright (C) 2005 CaH4e3
 *  Copyright (C) 2023-2024 negativeExponent
>>>>>>> f36364d (Update libretro_core_options.h)
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
 * iNES Mapper 222 - (CTC-31?)
 * (VRC2 mapper)
 *
>>>>>>> f36364d (Update libretro_core_options.h)
 */

#include "mapinc.h"
#include "asic_vrc2and4.h"

<<<<<<< HEAD
static uint8_t clockMode;
static uint8_t pending;
static uint8_t counter1;
static uint8_t counter2;
static uint8_t prescaler;
=======
<<<<<<< HEAD
static uint8 clockMode;
static uint8 pending;
static uint8 counter1;
static uint8 counter2;
static uint8 prescaler;
>>>>>>> a1c8c17c (Update libretro_core_options.h)

static SFORMAT stateRegs[] = {
	{ &clockMode, 1, "CLKM" },
	{ &pending, 1, "PEND" },
	{ &counter1, 1, "CNT1" },
	{ &counter2, 1, "CNT2" },
	{ &prescaler, 1, "PRES" },
=======
static uint8 IRQCount;
static uint8 IRQa;

static SFORMAT StateRegs[] = {
	{ &IRQCount, 1, "IRQC" },
	{ &IRQa, 1, "IRQA" },
>>>>>>> f36364d (Update libretro_core_options.h)
	{ 0 }
};

<<<<<<< HEAD
static void sync () {
	VRC24_syncPRG(0x01F, 0x000);
	VRC24_syncCHR(0x1FF, 0x000);
	VRC24_syncMirror();
}

static DECLFW (nibblizeCHR) {
	if (~A &1) {
		VRC24_writeReg(A,    V);
		VRC24_writeReg(A |1, V >>4);
	}
}

static DECLFW (writeIRQ) {
	switch(A &3) {
		case 0: clockMode = 0;
			break;
		case 1: pending = 0;
		        if (!clockMode) {
				counter1 = V &0xF;
				counter2 = V >>4;
			}
			break;
		case 2:	clockMode = 1;
			break;
	}
}

static void FP_FASTAPASS(1) cpuCycle (int a) {
	while (a--) {
		uint8_t previousPrescaler = prescaler;
		if (pending)
			prescaler = 0;
		else
			prescaler++;
		if (clockMode && ~previousPrescaler &0x40 && prescaler &0x40) {
			if (++counter1 == 0xF && ++counter2 == 0xF) pending = 1;
			counter1 &= 0xF;
			counter2 &= 0xF;
		}
		if (pending)
			X6502_IRQBegin(FCEU_IQEXT);
		else
			X6502_IRQEnd(FCEU_IQEXT);
=======
static void M222IRQ(void) {
	if (IRQa) {
		IRQCount++;
		if (IRQCount >= 238) {
			X6502_IRQBegin(FCEU_IQEXT);
<<<<<<< HEAD
>>>>>>> 553e13e (Update libretro_core_options.h)
	}
}

static void power (void) {
	clockMode = pending = counter1 = counter2 = prescaler = 0;
	VRC24_power();
	SetWriteHandler(0xB000, 0xEFFF, nibblizeCHR);
	SetWriteHandler(0xF000, 0xFFFF, writeIRQ);
}

<<<<<<< HEAD
void Mapper222_Init (CartInfo *info) {
<<<<<<< HEAD
	VRC2_init(info, sync, 0x01, 0x02, NULL, NULL, NULL, NULL);
	AddExState(stateRegs, ~0, 0, 0);
	info->Power =power;
	MapIRQHook = cpuCycle;
=======
	VRC24_init(info, sync, 0x01, 0x02, 0, 0, 0);
	AddExState(Mapper222_stateRegs, ~0, 0, 0);
	info->Power =Mapper222_power;
	MapIRQHook =Mapper222_cpuCycle;
=======
static void M222Write(uint32 A, uint8 V) {
	switch (A & 0xF003) {
	case 0x8000: prg_reg[0] = V; break;
	case 0x9000: mirr = V & 1; break;
	case 0xA000: prg_reg[1] = V; break;
	case 0xB000: chr_reg[0] = V; break;
	case 0xB002: chr_reg[1] = V; break;
	case 0xC000: chr_reg[2] = V; break;
	case 0xC002: chr_reg[3] = V; break;
	case 0xD000: chr_reg[4] = V; break;
	case 0xD002: chr_reg[5] = V; break;
	case 0xE000: chr_reg[6] = V; break;
	case 0xE002: chr_reg[7] = V; break;
	case 0xF000: IRQa = IRQCount = V; if (scanline < 240) IRQCount -= 8; else IRQCount += 4; X6502_IRQEnd(FCEU_IQEXT); break;
	}
	Sync();
=======
		}
	}
}

static DECLFW(M222WriteCHR) {
	if (!(A & 0x01)) {
		VRC24_Write(A, V);
		VRC24_Write(A | 0x01, V >> 4);
	}
}

static DECLFW(M222WriteIRQ) {
	/* TODO: Update this */
	IRQa = IRQCount = V;
	if (scanline < 240)
		IRQCount -= 8;
	else
		IRQCount += 4;
	X6502_IRQEnd(FCEU_IQEXT);
>>>>>>> f36364d (Update libretro_core_options.h)
}

static void M222Power(void) {
	VRC24_Power();
	SetWriteHandler(0xB000, 0xEFFF, M222WriteCHR);
	SetWriteHandler(0xF000, 0xFFFF, M222WriteIRQ);
}

void Mapper222_Init(CartInfo *info) {
	VRC24_Init(info, VRC2, 0x01, 0x02, 0, 1);
	info->Power = M222Power;
	MapIRQHook = NULL;
	GameHBIRQHook = M222IRQ;
<<<<<<< HEAD
	GameStateRestore = StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
>>>>>>> 553e13e (Update libretro_core_options.h)
<<<<<<< HEAD
>>>>>>> 4d55ef8 (Update libretro_core_options.h)
=======
=======
	AddExState(StateRegs, ~0, 0, NULL);
>>>>>>> f36364d (Update libretro_core_options.h)
>>>>>>> cd26ce4 (Update libretro_core_options.h)
}
