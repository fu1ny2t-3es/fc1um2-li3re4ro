/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
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

/*
 * - Mapper 56 - UNL KS202
 *   FDS Conversion: Super Mario Bros. 3 (Pirate, Alt)
 *   similar to M142 but use WRAM instead? $D000 additional IRQ trigger
 * - fix IRQ counter, noticeable in status bars of both SMB2J(KS7032) and SMB3J(KS202)
 */

 #include "mapinc.h"
 #include "ks202.h"

<<<<<<< HEAD
<<<<<<< HEAD
static uint8_t reg[8], creg[8], mirr, cmd, IRQa = 0;
static int32_t IRQCount, IRQLatch;
static uint8_t KS7032;
static uint8_t *WRAM = NULL;
static uint32_t WRAMSIZE;
=======
static uint8 reg[8], creg[8], mirr, cmd, IRQa = 0;
static int32 IRQCount, IRQLatch;
static uint8 KS7032;
static uint8 *WRAM = NULL;

#ifndef WRAM_SIZE
#define WRAM_SIZE 8192
#endif
>>>>>>> d8c3ec5a (Update Makefile.libretro)

static SFORMAT StateRegsKS7032[] =
{
	{ &cmd, 1, "CMD" },
	{ reg, 8, "REGS" },
	{ &IRQa, 1, "IRQA" },
	{ &IRQCount, 4 | FCEUSTATE_RLSB, "IRQC" },
	{ 0 }
};

static SFORMAT StateRegsKS202[] =
{
	{ creg, 8, "CREG" },
=======
static uint8 prg[4];
static uint8 chr[8];
static uint8 mirr;

static SFORMAT M056StateRegs[] = {
	{ prg, 8, "PREG" },
	{ chr, 8, "CREG" },
>>>>>>> ae14339e (Update Makefile.libretro)
	{ &mirr, 1, "MIRR" },
	{ 0 }
};

static void Sync(void) {
	setprg8r(0x10, 0x6000, 0);

	setprg8(0x8000, (prg[0] & 0x10) | (ks202.reg[1] & 0x0F));
	setprg8(0xA000, (prg[1] & 0x10) | (ks202.reg[2] & 0x0F));
	setprg8(0xC000, (prg[2] & 0x10) | (ks202.reg[3] & 0x0F));
	setprg8(0xE000, (prg[3] & 0x10) | (          ~0 & 0x0F));

	setchr1(0x0000, chr[0]);
	setchr1(0x0400, chr[1]);
	setchr1(0x0800, chr[2]);
	setchr1(0x0C00, chr[3]);
	setchr1(0x1000, chr[4]);
	setchr1(0x1400, chr[5]);
	setchr1(0x1800, chr[6]);
	setchr1(0x1C00, chr[7]);

	setmirror(mirr & 1);
}

static DECLFW(M056Write) {
	switch (A & 0xC00) {
	case 0x000: prg[A & 0x03] = V; break;
	case 0x800: mirr = V; break;
	case 0xC00: chr[A & 0x07] = V; break;
	}
<<<<<<< HEAD
}

static void UNLKS7032Write(uint32 A, uint8 V) {
	switch (A & 0xF000) {
	case 0x8000: IRQLatch = (IRQLatch & 0xFFF0) | (V & 0x0F); break;
	case 0x9000: IRQLatch = (IRQLatch & 0xFF0F) | ((V & 0x0F) << 4); break;
	case 0xA000: IRQLatch = (IRQLatch & 0xF0FF) | ((V & 0x0F) << 8); break;
	case 0xB000: IRQLatch = (IRQLatch & 0x0FFF) | (V << 12); break;
	case 0xC000:
		IRQa = (V & 0xF);
		if (IRQa)
			IRQCount = IRQLatch;
		X6502_IRQEnd(FCEU_IQEXT); break;
	case 0xD000: X6502_IRQEnd(FCEU_IQEXT); break;
	case 0xE000: cmd = V & 7; break;
	case 0xF000: {
		uint8_t bank = (cmd - 1);
		if (bank < 3)
			reg[bank] = (reg[bank] & 0x10) | (V & 0x0F);
		else if (bank < 4)
			reg[bank] = V;
		Sync();
		switch (A & 0xFC00) {
		case 0xF000:
			A &= 3;
			if (A < 3)
				reg[bank] = (reg[bank] & 0x0F) | (V & 0x10);
			Sync();
			break;
		case 0xF800:
			mirr = (V & 1);
			Sync();
			break;
		case 0xFC00:
			creg[A & 7] = V;
			Sync();
			break;
		}
		}
		break;
	}
}

static void UNLSMB2JIRQHook(int a) {
	if (IRQa) {
		IRQCount += a;
		if (IRQCount >= 0xFFFF) {
			IRQCount = IRQLatch;
			X6502_IRQBegin(FCEU_IQEXT);
		}
	}
}

static void UNLKS7032Power(void) {
	Sync();
	SetReadHandler(0x6000, 0x7FFF, CartBR);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x4020, 0xFFFF, UNLKS7032Write);
	if (!KS7032) {
		SetWriteHandler(0x6000, 0x7FFF, CartBW);
		FCEU_CheatAddRAM(WRAM_SIZE >> 10, 0x6000, WRAM);
	}
}

static void UNLKS7032Close(void) {
	if (WRAM)
		FCEU_gfree(WRAM);
	WRAM = NULL;
}

static void StateRestore(int version) {
=======
	ks202.reg[ks202.cmd & 0x07] = V;
>>>>>>> ae14339e (Update Makefile.libretro)
	Sync();
}

static void M056Reset(void) {
	prg[0] = prg[1] = prg[2] = prg[3] = 0x10;
	chr[0] = chr[1] = chr[2] = chr[3] = 0;
	chr[4] = chr[5] = chr[6] = chr[7] = 0;
	mirr = 0;
	Sync();
}

<<<<<<< HEAD
void UNLKS202_Init(CartInfo *info) {
	KS7032 = 0;
	info->Power = UNLKS7032Power;
	info->Close = UNLKS7032Close;
	MapIRQHook = UNLSMB2JIRQHook;
	GameStateRestore = StateRestore;
	AddExState(&StateRegsKS7032, ~0, 0, 0);
	AddExState(&StateRegsKS202, ~0, 0, 0);

<<<<<<< HEAD
	WRAMSIZE = 8192;
	WRAM = (uint8_t*)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
=======
	WRAM = (uint8*)FCEU_gmalloc(WRAM_SIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAM_SIZE, 1);
>>>>>>> d8c3ec5a (Update Makefile.libretro)
	if (info->battery) {
		info->SaveGame[0] = WRAM;
		info->SaveGameLen[0] = WRAM_SIZE;
	}
	AddExState(WRAM, WRAM_SIZE, 0, "WRAM");
=======
static void M056Power(void) {
	prg[0] = prg[1] = prg[2] = prg[3] = 0x10;
	chr[0] = chr[1] = chr[2] = chr[3] = 0;
	chr[4] = chr[5] = chr[6] = chr[7] = 0;
	mirr = 0;
	KS202_Power();
	SetWriteHandler(0xF000, 0xFFFF, M056Write);
}

void Mapper056_Init(CartInfo *info) {
	KS202_Init(info, Sync, 1, 0);
	info->Power = M056Power;
	info->Reset = M056Reset;
	AddExState(&M056StateRegs, ~0, 0, 0);
>>>>>>> ae14339e (Update Makefile.libretro)
}
