/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2013 CaH4e3
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
 * DSOUNDV1/FL-TR8MA boards (32K WRAM, 8/16M), 178 mapper boards (8K WRAM, 4/8M)
 * Various Education Cartridges
 * 
 * mapper 551
 * Compared to INES Mapper 178, mirroring is hard-wired, and the chipset's internal CHR-RAM is not used in favor of CHR-ROM.
 *
 */

#include "mapinc.h"

static uint8 reg[4];

static SFORMAT StateRegs[] = {
	{ reg, 4, "REGS" },
	{ 0 }
};

static void Sync(void) {
	uint16 base = (reg[1] & 0x07) | (reg[2] << 3);

	if ((reg[0] & 0x02)) {
		setprg16(0x8000, base);
		setprg16(0xC000, base | ((reg[0] & 0x04) ? 0x06 : 0x07));
	} else {
		if (reg[0] & 0x04) {
			setprg16(0x8000, base);
			setprg16(0xC000, base);
		} else {
			setprg32(0x8000, base >> 1);
		}
	}

<<<<<<< HEAD
static uint8 decode(uint8 code) {
	acc += jedi_table[decstep + code];
	if ((acc & ~0x7ff) != 0)	/* acc is > 2047 */
		acc |= ~0xfff;
	else acc &= 0xfff;
	decstep += step_adj[code & 7] * 16;
	if (decstep < 0) decstep = 0;
	if (decstep > 48 * 16) decstep = 48 * 16;
	return (acc >> 8) & 0xff;
}

static void M178Sync(void) {
	uint32 sbank = reg[1] & 0x7;
	uint32 bbank = reg[2];
	setchr8(0);
	setprg8r(0x10, 0x6000, reg[3] & 3);
	if (reg[0] & 2) {	/* UNROM mode */
		setprg16(0x8000, (bbank << 3) | sbank);
		if (reg[0] & 4)
			setprg16(0xC000, (bbank << 3) | 6 | (reg[1] & 1));
		else
			setprg16(0xC000, (bbank << 3) | 7);
	} else {			/* NROM mode */
		uint32 bank = (bbank << 3) | sbank;
		if (reg[0] & 4) {
			setprg16(0x8000, bank);
			setprg16(0xC000, bank);
		} else
			setprg32(0x8000, bank >> 1);
	}

	setmirror((reg[0] & 1) ^ 1);
}

<<<<<<< HEAD
static void M551Sync(void) {
	uint32 sbank = reg[1] & 0x7;
	uint32 bbank = reg[2];
	if (reg[0] & 2) {	/* UNROM mode */
		setprg16(0x8000, (bbank << 3) | sbank);
		if (reg[0] & 4)
			setprg16(0xC000, (bbank << 3) | 6 | (reg[1] & 1));
		else
			setprg16(0xC000, (bbank << 3) | 7);
	} else {			/* NROM mode */
		uint32 bank = (bbank << 3) | sbank;
		if (reg[0] & 4) {
			setprg16(0x8000, bank);
			setprg16(0xC000, bank);
		} else
			setprg32(0x8000, bank >> 1);
	}

	setprg8r(0x10, 0x6000, 0);
	setchr8(reg[3]);
	setmirror(head.ROM_type & 1);
}

static DECLFW(M551Write) {
	reg[A & 3] = V;
	M551Sync();
}

static DECLFW(M178Write) {
	reg[A & 3] = V;
	M178Sync();
}

static DECLFW(M178WriteSnd) {
	if (A == 0x5800)
	{
=======
static void M178Write(uint32 A, uint8 V) {
=======
	if (iNESCart.mapper == 551) {
		setprg8r(0x10, 0x6000, 0);
		setchr8(reg[3]);
	} else {
		setchr8(0);
		setprg8r(0x10, 0x6000, reg[3] & 3);
		setmirror((reg[0] & 1) ^ 1);
	}
}

static DECLFW(M178Write) {
>>>>>>> 37453f6 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
	reg[A & 3] = V;
/*	FCEU_printf("cmd %04x:%02x\n", A, V); */
	Sync();
}

<<<<<<< HEAD
static void M178WriteSnd(uint32 A, uint8 V) {
	if (A == 0x5800) {
>>>>>>> ee43609 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
		if (V & 0xF0) {
			pcm_enable = 1;
			pcmwrite(0x4011, decode(V & 0xf));
		} else
			pcm_enable = 0;
	}
}

static uint8 M178ReadSnd(uint32 A) {
	if (A == 0x5800)
<<<<<<< HEAD
<<<<<<< HEAD
		return (X.DB & 0xBF) | ((pcm_enable ^ 1) << 6);
	return X.DB;
<<<<<<< HEAD
}

static void M551Power(void) {
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	M551Sync();
	pcmwrite = GetWriteHandler(0x4011);
	SetWriteHandler(0x4800, 0x4fff, M551Write);
	SetWriteHandler(0x5800, 0x5fff, M178WriteSnd);
	SetReadHandler(0x5800, 0x5fff, M178ReadSnd);
	SetReadHandler(0x6000, 0x7fff, CartBR);
	SetWriteHandler(0x6000, 0x7fff, CartBW);
	SetReadHandler(0x8000, 0xffff, CartBR);
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
=======
>>>>>>> ee43609 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
=======
		return (cpu.DB & 0xBF) | ((pcm_enable ^ 1) << 6);
	return cpu.DB;
>>>>>>> d7ecb21 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
=======
		return (cpu.openbus & 0xBF) | ((pcm_enable ^ 1) << 6);
	return cpu.openbus;
>>>>>>> 3597a61 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
}

static void M178Power(void) {
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	M178Sync();
	pcmwrite = GetWriteHandler(0x4011);
	SetWriteHandler(0x4800, 0x4fff, M178Write);
	SetWriteHandler(0x5800, 0x5fff, M178WriteSnd);
	SetReadHandler(0x5800, 0x5fff, M178ReadSnd);
	SetReadHandler(0x6000, 0x7fff, CartBR);
	SetWriteHandler(0x6000, 0x7fff, CartBW);
	SetReadHandler(0x8000, 0xffff, CartBR);
=======
static void M178Power(void) {
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	Sync();
	SetWriteHandler(0x4800, 0x4FFF, M178Write);
	SetReadHandler(0x6000, 0x7FFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
>>>>>>> 37453f6 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
}

static void M551Reset(void) {
	/* Always reset to menu */
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	M551Sync();
}
<<<<<<< HEAD

static void M178Reset(void)
{
	/* Always reset to menu */
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	M178Sync();
}

static void M178SndClk(int a)
{
	if (pcm_enable) {
		pcm_latch -= a;
		if (pcm_latch <= 0) {
			pcm_latch += pcm_clock;
			pcm_enable = 0;
		}
	}
}
=======
>>>>>>> 37453f6 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)

static void M178Close(void) {
}

static void M551StateRestore(int version) {
	M551Sync();
}

static void M178StateRestore(int version) {
	M178Sync();
}

void Mapper178_Init(CartInfo *info) {
	info->Power = M178Power;
	info->Reset = M178Reset;
	info->Close = M178Close;
<<<<<<< HEAD
	GameStateRestore = M178StateRestore;
	MapIRQHook = M178SndClk;
=======
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
>>>>>>> 37453f6 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)

	WRAMSIZE = 8192;
	WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
	if (info->battery) {
		info->SaveGame[0] = WRAM;
		info->SaveGameLen[0] = WRAMSIZE;
	}
<<<<<<< HEAD
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");

	AddExState(&StateRegs, ~0, 0, 0);

	if (info->mapper == 551)
	{
		info->Power      = M551Power;
		info->Reset      = M551Reset;
		GameStateRestore = M551StateRestore;
	}
=======
>>>>>>> 37453f6 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
}
