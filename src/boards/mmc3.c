/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 1998 BERO
 *  Copyright (C) 2003 Xodnizel
 *  Mapper 12 code Copyright (C) 2003 CaH4e3
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

/*  Code for emulating iNES mappers 4,12,44,45,47,49,52,74,114,115,116,118,
 119,165,205,245,249,250,254,555
*/

#include "mapinc.h"
#include "mmc3.h"

uint8 MMC3_cmd;
static uint8 *WRAM;
static uint32 WRAMSIZE;
static uint8 *CHRRAM;
static uint32 CHRRAMSIZE;
uint8 DRegBuf[8];
uint8 EXPREGS[8];	/* For bootleg games, mostly. */
uint8 A000B, A001B;
uint8 mmc3opts = 0;

static uint8 IRQCount, IRQLatch, IRQa;
static uint8 IRQReload;

static SFORMAT MMC3_StateRegs[] =
{
	{ DRegBuf, 8, "REGS" },
	{ &MMC3_cmd, 1, "CMD" },
	{ &A000B, 1, "A000" },
	{ &A001B, 1, "A001" },
	{ &IRQReload, 1, "IRQR" },
	{ &IRQCount, 1, "IRQC" },
	{ &IRQLatch, 1, "IRQL" },
	{ &IRQa, 1, "IRQA" },
	{ 0 }
};

static int isRevB = 1;

void (*pwrap)(uint32 A, uint8 V);
void (*cwrap)(uint32 A, uint8 V);
void (*mwrap)(uint8 V);

void GenMMC3Power(void);
void FixMMC3PRG(int V);
void FixMMC3CHR(int V);

void GenMMC3_Init(CartInfo *info, int prg, int chr, int wram, int battery);

/* ----------------------------------------------------------------------
 * ------------------------- Generic MM3 Code ---------------------------
 * ----------------------------------------------------------------------
 */

int MMC3CanWriteToWRAM(void) {
	return ((A001B & 0x80) && !(A001B & 0x40));
}

void FixMMC3PRG(int V) {
	if (V & 0x40) {
		pwrap(0xC000, DRegBuf[6]);
		pwrap(0x8000, ~1);
	} else {
		pwrap(0x8000, DRegBuf[6]);
		pwrap(0xC000, ~1);
	}
	pwrap(0xA000, DRegBuf[7]);
	pwrap(0xE000, ~0);
}

void FixMMC3CHR(int V) {
	int cbase = (V & 0x80) << 5;

	cwrap((cbase ^ 0x000), DRegBuf[0] & (~1));
	cwrap((cbase ^ 0x400), DRegBuf[0] | 1);
	cwrap((cbase ^ 0x800), DRegBuf[1] & (~1));
	cwrap((cbase ^ 0xC00), DRegBuf[1] | 1);

	cwrap(cbase ^ 0x1000, DRegBuf[2]);
	cwrap(cbase ^ 0x1400, DRegBuf[3]);
	cwrap(cbase ^ 0x1800, DRegBuf[4]);
	cwrap(cbase ^ 0x1c00, DRegBuf[5]);

	if (mwrap) mwrap(A000B);
}

void MMC3RegReset(void) {
	IRQCount = IRQLatch = IRQa = MMC3_cmd = 0;

	DRegBuf[0] = 0;
	DRegBuf[1] = 2;
	DRegBuf[2] = 4;
	DRegBuf[3] = 5;
	DRegBuf[4] = 6;
	DRegBuf[5] = 7;
	DRegBuf[6] = 0;
	DRegBuf[7] = 1;

	FixMMC3PRG(0);
	FixMMC3CHR(0);
}

void MMC3_CMDWrite(uint32 A, uint8 V) {
	switch (A & 0xE001) {
	case 0x8000:
		if ((V & 0x40) != (MMC3_cmd & 0x40))
			FixMMC3PRG(V);
		if ((V & 0x80) != (MMC3_cmd & 0x80))
			FixMMC3CHR(V);
		MMC3_cmd = V;
		break;
	case 0x8001:
	{
		int cbase = (MMC3_cmd & 0x80) << 5;
		DRegBuf[MMC3_cmd & 0x7] = V;
		switch (MMC3_cmd & 0x07) {
		case 0:
			cwrap((cbase ^ 0x000), V & (~1));
			cwrap((cbase ^ 0x400), V | 1);
			break;
		case 1:
			cwrap((cbase ^ 0x800), V & (~1));
			cwrap((cbase ^ 0xC00), V | 1);
			break;
		case 2:
			cwrap(cbase ^ 0x1000, V);
			break;
		case 3:
			cwrap(cbase ^ 0x1400, V);
			break;
		case 4:
			cwrap(cbase ^ 0x1800, V);
			break;
		case 5:
			cwrap(cbase ^ 0x1C00, V);
			break;
		case 6:
			if (MMC3_cmd & 0x40)
				pwrap(0xC000, V);
			else
				pwrap(0x8000, V);
			break;
		case 7:
			pwrap(0xA000, V);
			break;
		}
		break;
	}
	case 0xA000:
		if (mwrap) mwrap(V);
		break;
	case 0xA001:
		A001B = V;
		break;
	}
}

void MMC3_IRQWrite(uint32 A, uint8 V) {
	switch (A & 0xE001) {
	case 0xC000: IRQLatch = V; break;
	case 0xC001: IRQReload = 1; break;
	case 0xE000: X6502_IRQEnd(FCEU_IQEXT); IRQa = 0; break;
	case 0xE001: IRQa = 1; break;
	}
}

static void ClockMMC3Counter(void) {
	int count = IRQCount;
	if (!count || IRQReload) {
		IRQCount = IRQLatch;
		IRQReload = 0;
	} else
		IRQCount--;
	if ((count | isRevB) && !IRQCount) {
		if (IRQa) {
			X6502_IRQBegin(FCEU_IQEXT);
		}
	}
}

static void MMC3_hb(void) {
	ClockMMC3Counter();
}

static void MMC3_hb_KickMasterHack(void) {
	if (scanline == 238) ClockMMC3Counter();
	ClockMMC3Counter();
}

static void MMC3_hb_PALStarWarsHack(void) {
	if (scanline == 240) ClockMMC3Counter();
	ClockMMC3Counter();
}

void GenMMC3Restore(int version) {
	FixMMC3PRG(MMC3_cmd);
	FixMMC3CHR(MMC3_cmd);
}

static void GENCWRAP(uint32 A, uint8 V) {
	setchr1(A, V);			/* Business Wars NEEDS THIS for 8K CHR-RAM */
}

static void GENPWRAP(uint32 A, uint8 V) {
   /* [NJ102] Mo Dao Jie (C) has 1024Mb MMC3 BOARD, maybe something other will be broken
    * also HengGe BBC-2x boards enables this mode as default board mode at boot up
    */
   setprg8(A, (V & 0x7F));
}

static void GENMWRAP(uint8 V) {
	A000B = V;
	setmirror((V & 1) ^ 1);
}

static void GENNOMWRAP(uint8 V) {
	A000B = V;
}

static void MBWRAMMMC6(uint32 A, uint8 V) {
	WRAM[A & 0x3ff] = V;
}

static uint8 MAWRAMMMC6(uint32 A) {
	return(WRAM[A & 0x3ff]);
}

void GenMMC3Power(void) {
	if (UNIFchrrama) setchr8(0);

	SetWriteHandler(0x8000, 0xBFFF, MMC3_CMDWrite);
	SetWriteHandler(0xC000, 0xFFFF, MMC3_IRQWrite);
	SetReadHandler(0x8000, 0xFFFF, CartBR);

	A001B = A000B = 0;
	setmirror(1);
	if (mmc3opts & 1) {
		if (WRAMSIZE == 1024) {
			FCEU_CheatAddRAM(1, 0x7000, WRAM);
			SetReadHandler(0x7000, 0x7FFF, MAWRAMMMC6);
			SetWriteHandler(0x7000, 0x7FFF, MBWRAMMMC6);
		} else {
         FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
			SetWriteHandler(0x6000, 0x6000 + ((WRAMSIZE - 1) & 0x1fff), CartBW);
			SetReadHandler(0x6000, 0x6000 + ((WRAMSIZE - 1) & 0x1fff), CartBR);
			setprg8r(0x10, 0x6000, 0);
		}
		if (!(mmc3opts & 2))
			FCEU_dwmemset(WRAM, 0, WRAMSIZE);
	}
	MMC3RegReset();
	if (CHRRAM)
		FCEU_dwmemset(CHRRAM, 0, CHRRAMSIZE);
}

void GenMMC3Close(void) {
	if (CHRRAM)
		FCEU_gfree(CHRRAM);
	if (WRAM)
		FCEU_gfree(WRAM);
	CHRRAM = WRAM = NULL;
}

void GenMMC3_Init(CartInfo *info, int prg, int chr, int wram, int battery) {
	pwrap = GENPWRAP;
	cwrap = GENCWRAP;
	mwrap = GENMWRAP;

	WRAMSIZE = wram << 10;

	PRGmask8[0] &= (prg >> 13) - 1;
	CHRmask1[0] &= (chr >> 10) - 1;
	CHRmask2[0] &= (chr >> 11) - 1;

	if (wram) {
		mmc3opts |= 1;
		WRAM = (uint8*)FCEU_gmalloc(WRAMSIZE);
		SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
		AddExState(WRAM, WRAMSIZE, 0, "WRAM");
	}

	if (battery) {
		mmc3opts |= 2;
		info->SaveGame[0] = WRAM;
		info->SaveGameLen[0] = WRAMSIZE;
	}

	AddExState(MMC3_StateRegs, ~0, 0, 0);

	info->Power = GenMMC3Power;
	info->Reset = MMC3RegReset;
	info->Close = GenMMC3Close;

	if (info->CRC32 == 0x5104833e)		/* Kick Master */
		GameHBIRQHook = MMC3_hb_KickMasterHack;
	else if (info->CRC32 == 0x5a6860f1 || info->CRC32 == 0xae280e20) /* Shougi Meikan '92/'93 */
		GameHBIRQHook = MMC3_hb_KickMasterHack;
	else if (info->CRC32 == 0xfcd772eb)	/* PAL Star Wars, similar problem as Kick Master. */
		GameHBIRQHook = MMC3_hb_PALStarWarsHack;
	else
		GameHBIRQHook = MMC3_hb;
	GameStateRestore = GenMMC3Restore;
}

/* ----------------------------------------------------------------------
 * -------------------------- MMC3 Based Code ---------------------------
 *  ----------------------------------------------------------------------
 */

/* ---------------------------- Mapper 4 -------------------------------- */

static int hackm4 = 0;	/* For Karnov, maybe others.  BLAH.  Stupid iNES format.*/

static void M4Power(void) {
	GenMMC3Power();
	A000B = (hackm4 ^ 1) & 1;
	setmirror(hackm4);
}

void Mapper4_Init(CartInfo *info) {
	int ws = 8;

	if ((info->CRC32 == 0x93991433 || info->CRC32 == 0xaf65aa84)) {
		FCEU_printf("Low-G-Man can not work normally in the iNES format.\nThis game has been recognized by its CRC32 value, and the appropriate changes will be made so it will run.\nIf you wish to hack this game, you should use the UNIF format for your hack.\n\n");
		ws = 0;
	}
	if (info->CRC32 == 0x97b6cb19)
		isRevB = 0;

	GenMMC3_Init(info, 512, 256, ws, info->battery);
	info->Power = M4Power;
	hackm4 = info->mirror;
}

/* ---------------------------- Mapper 12 ------------------------------- */

static void M12CW(uint32 A, uint8 V) {
	setchr1(A, (EXPREGS[(A & 0x1000) >> 12] << 8) + V);
}

static void M12Write(uint32 A, uint8 V) {
	EXPREGS[0] = V & 0x01;
	EXPREGS[1] = (V & 0x10) >> 4;
}

static uint8 M12Read(uint32 A) {
	return EXPREGS[2];
}

static void M12Power(void) {
	EXPREGS[0] = EXPREGS[1] = 0;
	EXPREGS[2] = 1; /* chinese is default */
	GenMMC3Power();
	SetWriteHandler(0x4100, 0x5FFF, M12Write);
	SetReadHandler(0x4100, 0x5FFF, M12Read);
}

static void M12Reset(void) {
	EXPREGS[0] = EXPREGS[1] = 0;
	EXPREGS[2] ^= 1;
	MMC3RegReset();
}

void Mapper12_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 256, 8, info->battery);
	cwrap = M12CW;
	isRevB = 0;

	info->Power = M12Power;
	info->Reset = M12Reset;
	AddExState(EXPREGS, 2, 0, "EXPR");
}

/* ---------------------------- Mapper 37 ------------------------------- */

static void M37PW(uint32 A, uint8 V) {
	if (EXPREGS[0] != 2)
		V &= 0x7;
	else
		V &= 0xF;
	V |= EXPREGS[0] << 3;
	setprg8(A, V);
}

static void M37CW(uint32 A, uint8 V) {
	uint32 NV = V;
	NV &= 0x7F;
	NV |= EXPREGS[0] << 6;
	setchr1(A, NV);
}

static void M37Write(uint32 A, uint8 V) {
	EXPREGS[0] = (V & 6) >> 1;
	FixMMC3PRG(MMC3_cmd);
	FixMMC3CHR(MMC3_cmd);
}

static void M37Reset(void) {
	EXPREGS[0] = 0;
	MMC3RegReset();
}

static void M37Power(void) {
	EXPREGS[0] = 0;
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x7FFF, M37Write);
}

void Mapper37_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 256, 8, info->battery);
	pwrap = M37PW;
	cwrap = M37CW;
	info->Power = M37Power;
	info->Reset = M37Reset;
	AddExState(EXPREGS, 1, 0, "EXPR");
}

/* ---------------------------- Mapper 44 ------------------------------- */

static void M44PW(uint32 A, uint8 V) {
	uint32 NV = V;
	if (EXPREGS[0] >= 6) NV &= 0x1F;
	else NV &= 0x0F;
	NV |= EXPREGS[0] << 4;
	setprg8(A, NV);
}

static void M44CW(uint32 A, uint8 V) {
	uint32 NV = V;
	if (EXPREGS[0] < 6) NV &= 0x7F;
	NV |= EXPREGS[0] << 7;
	setchr1(A, NV);
}

static void M44Write(uint32 A, uint8 V) {
	if (A & 1) {
		EXPREGS[0] = V & 7;
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
	} else
		MMC3_CMDWrite(A, V);
}

static void M44Power(void) {
	EXPREGS[0] = 0;
	GenMMC3Power();
	SetWriteHandler(0xA000, 0xBFFF, M44Write);
}

void Mapper44_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 256, 8, info->battery);
	cwrap = M44CW;
	pwrap = M44PW;
	info->Power = M44Power;
	AddExState(EXPREGS, 1, 0, "EXPR");
}

/* ---------------------------- Mapper 45 ------------------------------- */

static void M45CW(uint32 A, uint8 V) {
	if (CHRsize[0] ==8192)
		setchr1(A, V);
	else {
		int chrAND =0xFF >>(~EXPREGS[2] &0xF);
		int chrOR  =EXPREGS[0] | EXPREGS[2] <<4 &0xF00;
		setchr1(A, V &chrAND | chrOR &~chrAND);
	}
}

static uint8 M45ReadOB(uint32 A) {
	return X.DB;
}

static void M45PW(uint32 A, uint8 V) {
	int prgAND =~EXPREGS[3] &0x3F;
	int prgOR  =EXPREGS[1] | EXPREGS[2] <<2 &0x300;
	setprg8(A, V &prgAND | prgOR &~prgAND);

	/* Some multicarts select between five different menus by connecting one of the higher address lines to PRG /CE.
	   The menu code selects between menus by checking which of the higher address lines disables PRG-ROM when set. */
	if (PRGsize[0] <0x200000 && EXPREGS[5] ==1 && EXPREGS[1] &0x80 ||
	    PRGsize[0] <0x200000 && EXPREGS[5] ==2 && EXPREGS[2] &0x40 ||
	    PRGsize[0] <0x100000 && EXPREGS[5] ==3 && EXPREGS[1] &0x40 ||
	    PRGsize[0] <0x100000 && EXPREGS[5] ==4 && EXPREGS[2] &0x20)
		SetReadHandler(0x8000, 0xFFFF, M45ReadOB);
	else
		SetReadHandler(0x8000, 0xFFFF, CartBR);
}

static void M45Write(uint32 A, uint8 V) {
	if (EXPREGS[3] & 0x40) {
		WRAM[A - 0x6000] = V;
		return;
	}
	EXPREGS[EXPREGS[4]] = V;
	EXPREGS[4] = (EXPREGS[4] + 1) & 3;
	FixMMC3PRG(MMC3_cmd);
	FixMMC3CHR(MMC3_cmd);
}

static uint8 M45Read(uint32 A) {
	uint32 addr = 1 << (EXPREGS[5] + 4);
	if (A & (addr | (addr - 1)))
		return X.DB | 1;
	return X.DB;
}

static void M45Reset(void) {
	EXPREGS[0] = EXPREGS[1] = EXPREGS[3] = EXPREGS[4] = 0;
	EXPREGS[2] = 0x0F;
	EXPREGS[5]++;
	EXPREGS[5] &= 7;
	MMC3RegReset();
}

static void M45Power(void) {
	GenMMC3Power();
	EXPREGS[0] = EXPREGS[1] = EXPREGS[3] = EXPREGS[4] = EXPREGS[5] = 0;
	EXPREGS[2] = 0x0F;
	SetWriteHandler(0x6000, 0x7FFF, M45Write);
	SetReadHandler(0x5000, 0x5FFF, M45Read);
}

void Mapper45_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 256, 8, info->battery);
	cwrap = M45CW;
	pwrap = M45PW;
	info->Reset = M45Reset;
	info->Power = M45Power;
	AddExState(EXPREGS, 5, 0, "EXPR");
}

/* ---------------------------- Mapper 47 ------------------------------- */

static void M47PW(uint32 A, uint8 V) {
	V &= 0xF;
	V |= EXPREGS[0] << 4;
	setprg8(A, V);
}

static void M47CW(uint32 A, uint8 V) {
	uint32 NV = V;
	NV &= 0x7F;
	NV |= EXPREGS[0] << 7;
	setchr1(A, NV);
}

<<<<<<< HEAD
static DECLFW(M47Write) {
	EXPREGS[0] = V;
	FixMMC3PRG(MMC3_cmd);
	FixMMC3CHR(MMC3_cmd);
}

static void M47Reset(void) {
	EXPREGS[0] = 0;	
=======
static void M47Write(uint32 A, uint8 V) {
	EXPREGS[0] = V & 1;
>>>>>>> a2c232f (Update libretro_core_options.h)
	FixMMC3PRG(MMC3_cmd);
	FixMMC3CHR(MMC3_cmd);
}

static void M47Power(void) {
	EXPREGS[0] = 0;
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x7FFF, M47Write);
/*	SetReadHandler(0x6000,0x7FFF,0); */
}

void Mapper47_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 256, 8, 0);
	pwrap = M47PW;
	cwrap = M47CW;
	info->Reset = M47Reset;
	info->Power = M47Power;
	AddExState(EXPREGS, 1, 0, "EXPR");
}

/* ---------------------------- Mapper 49 ------------------------------- */
/* -------------------- BMC-STREETFIGTER-GAME4IN1 ----------------------- */
/* added 6-24-19:
 * BMC-STREETFIGTER-GAME4IN1 - Sic. $6000 set to $41 rather than $00 on power-up.
 */

static uint8 isUNIF = 0;

static void M49PW(uint32 A, uint8 V) {
	if (EXPREGS[0] & 1) {
		V &= 0xF;
		V |= (EXPREGS[0] & 0xC0) >> 2;
		setprg8(A, V);
	} else
		setprg32(0x8000, (EXPREGS[0] >> 4) & 15);
}

static void M49CW(uint32 A, uint8 V) {
	uint32 NV = V;
	NV &= 0x7F;
	NV |= (EXPREGS[0] & 0xC0) << 1;
	setchr1(A, NV);
}

static void M49Write(uint32 A, uint8 V) {
	if (A001B & 0x80) {
		EXPREGS[0] = V;
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
	}
}

static void M49Reset(void) {
	EXPREGS[0] = isUNIF ? 0x41 : 0;
	MMC3RegReset();
}

static void M49Power(void) {
	EXPREGS[0] = isUNIF ? 0x41 : 0;
	M49Reset();
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x7FFF, M49Write);
	SetReadHandler(0x6000, 0x7FFF, 0);
}

void Mapper49_Init(CartInfo *info) {
	isUNIF = 0;
	GenMMC3_Init(info, 512, 256, 0, 0);
	cwrap = M49CW;
	pwrap = M49PW;
	info->Reset = M49Reset;
	info->Power = M49Power;
	AddExState(EXPREGS, 1, 0, "EXPR");
}

void BMCSFGAME4IN1_Init(CartInfo *info) {
	isUNIF = 1;
	GenMMC3_Init(info, 512, 512, 0, 0);
	cwrap = M49CW;
	pwrap = M49PW;
	info->Reset = M49Reset;
	info->Power = M49Power;
	AddExState(EXPREGS, 1, 0, "EXPR");
}

/* ---------------------------- Mapper 52 ------------------------------- */
static void M52PW(uint32 A, uint8 V) {
	uint32 mask = 0x1F ^ ((EXPREGS[0] & 8) << 1);
	uint32 bank = ((EXPREGS[0] & 6) | ((EXPREGS[0] >> 3) & EXPREGS[0] & 1)) << 4;
	setprg8(A, bank | (V & mask));
}

static void M52CW(uint32 A, uint8 V) {
	uint32 mask = 0xFF ^ ((EXPREGS[0] & 0x40) << 1);
	uint32 bank = (((EXPREGS[0] >> 4) & 2) | (EXPREGS[0] & 4) | ((EXPREGS[0] >> 6) & (EXPREGS[0] >> 4) & 1)) << 7;	/* actually 256K CHR banks index bits is inverted! */
	if (CHRRAM && (EXPREGS[0] &3) ==3)
		setchr1r(0x10, A, bank | (V & mask));
	else
		setchr1(A, bank | (V & mask));
}

static void M52S14CW(uint32 A, uint8 V) {
	uint32 mask = 0xFF ^ ((EXPREGS[0] & 0x40) << 1);
	uint32 bank = EXPREGS[0] <<3 &0x80 | EXPREGS[0] <<7 &0x300;
	if (CHRRAM && EXPREGS[0] &0x20)
		setchr1r(0x10, A, bank | (V & mask));
	else
		setchr1(A, bank | (V & mask));
}

static void M52Write(uint32 A, uint8 V) {
	if (EXPREGS[1]) {
		WRAM[A - 0x6000] = V;
		return;
	}
	EXPREGS[1] = V & 0x80;
	EXPREGS[0] = V;
	FixMMC3PRG(MMC3_cmd);
	FixMMC3CHR(MMC3_cmd);
}

static void M52Reset(void) {
	EXPREGS[0] = EXPREGS[1] = 0;
	MMC3RegReset();
}

static void M52Power(void) {
	M52Reset();
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x7FFF, M52Write);
}

void Mapper52_Init(CartInfo *info) {
	GenMMC3_Init(info, 256, 256, 8, info->battery);
	isRevB = 0; /* For Aladdin in NT-8062 */
	cwrap = info->submapper ==14? M52S14CW: M52CW;
	pwrap = M52PW;
	info->Reset = M52Reset;
	info->Power = M52Power;
	AddExState(EXPREGS, 2, 0, "EXPR");
	if (info->iNES2 && info->CHRRomSize && info->CHRRamSize) {
		CHRRAMSIZE = 8192;
		CHRRAM = (uint8*)FCEU_gmalloc(CHRRAMSIZE);
		SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
		AddExState(CHRRAM, CHRRAMSIZE, 0, "CHRR");		
	}
}

/* ---------------------------- Mapper 76 ------------------------------- */

static void M76CW(uint32 A, uint8 V) {
	if (A >= 0x1000)
		setchr2((A & 0xC00) << 1, V);
}

void Mapper76_Init(CartInfo *info) {
	GenMMC3_Init(info, 128, 128, 0, 0);
	cwrap = M76CW;
}

/* ---------------------------- Mapper 74 ------------------------------- */

static void M74CW(uint32 A, uint8 V) {
	if ((V == 8) || (V == 9))	/* Di 4 Ci - Ji Qi Ren Dai Zhan (As).nes, Ji Jia Zhan Shi (As).nes */
		setchr1r(0x10, A, V);
	else
		setchr1r(0, A, V);
}

void Mapper74_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 256, 8, info->battery);
	cwrap = M74CW;
	CHRRAMSIZE = 2048;
	CHRRAM = (uint8*)FCEU_gmalloc(CHRRAMSIZE);
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
	AddExState(CHRRAM, CHRRAMSIZE, 0, "CHRR");
}

/* ---------------------------- Mapper 114 ------------------------------ */

static uint8 cmdin, type_Boogerman = 0;
uint8 boogerman_perm[8] = { 0, 2, 5, 3, 6, 1, 7, 4 };
uint8 m114_perm[8] = { 0, 3, 1, 5, 6, 7, 2, 4 };

static void M114PWRAP(uint32 A, uint8 V) {
	if (EXPREGS[0] & 0x80) {
		if (EXPREGS[0] & 0x20)
			setprg32(0x8000, (EXPREGS[0] & 0x0F) >> 1);
		else {
			setprg16(0x8000, (EXPREGS[0] & 0x0F));
			setprg16(0xC000, (EXPREGS[0] & 0x0F));
		}
	} else
		setprg8(A, V);
}

static void M114CWRAP(uint32 A, uint8 V) {
	setchr1(A, (uint32)V | ((EXPREGS[1] & 1) << 8));
}

static void M114Write(uint32 A, uint8 V) {
	switch (A & 0xE001) {
	case 0x8001: MMC3_CMDWrite(0xA000, V); break;
	case 0xA000: MMC3_CMDWrite(0x8000, (V & 0xC0) | (m114_perm[V & 7])); cmdin = 1; break;
	case 0xC000: if (!cmdin) break; MMC3_CMDWrite(0x8001, V); cmdin = 0; break;
	case 0xA001: IRQLatch = V; break;
	case 0xC001: IRQReload = 1; break;
	case 0xE000: X6502_IRQEnd(FCEU_IQEXT); IRQa = 0; break;
	case 0xE001: IRQa = 1; break;
	}
}

static void BoogermanWrite(uint32 A, uint8 V) {
	switch (A & 0xE001) {
	case 0x8001: if (!cmdin) break; MMC3_CMDWrite(0x8001, V); cmdin = 0; break;
	case 0xA000: MMC3_CMDWrite(0x8000, (V & 0xC0) | (boogerman_perm[V & 7])); cmdin = 1; break;
	case 0xA001: IRQReload = 1; break;
	case 0xC000: MMC3_CMDWrite(0xA000, V); break;
	case 0xC001: IRQLatch = V; break;
	case 0xE000: X6502_IRQEnd(FCEU_IQEXT); IRQa = 0; break;
	case 0xE001: IRQa = 1; break;
	}
}

static void M114ExWrite(uint32 A, uint8 V) {
	if (A <= 0x7FFF) {
		if (A & 1)
			EXPREGS[1] = V;
		else
			EXPREGS[0] = V;
		FixMMC3PRG(MMC3_cmd);
	}
}

static void M114Power(void) {
	EXPREGS[0] = EXPREGS[1] = 0;
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x7FFF, M114ExWrite);
	SetWriteHandler(0x8000, 0xFFFF, M114Write);
	if (type_Boogerman)
		SetWriteHandler(0x8000, 0xFFFF, BoogermanWrite);
}

static void M114Reset(void) {
	EXPREGS[0] = EXPREGS[1] = 0;
	MMC3RegReset();
}

void Mapper114_Init(CartInfo *info) {
	isRevB = 0;
	/* Use NES 2.0 submapper to identify scrambling pattern, otherwise CRC32 for Boogerman and test rom */
	type_Boogerman = info->iNES2? (info->submapper ==1): (info->CRC32 ==0x80eb1839 || info->CRC32 ==0x071e4ee8);

	GenMMC3_Init(info, 256, 256, 0, 0);
	pwrap = M114PWRAP;
	cwrap = M114CWRAP;
	info->Power = M114Power;
	info->Reset = M114Reset;
	AddExState(EXPREGS, 1, 0, "EXPR");
	AddExState(&cmdin, 1, 0, "CMDI");
}

/* ---------------------------- Mapper 115 KN-658 board ------------------------------ */

static void M115PW(uint32 A, uint8 V) {
	int prgOR =EXPREGS[0] &0xF | EXPREGS[0] >>2 &0x10;
	if (EXPREGS[0] & 0x80) {
		if (EXPREGS[0] & 0x20)
			setprg32(0x8000, prgOR >> 1);
		else {
			setprg16(0x8000, prgOR);
			setprg16(0xC000, prgOR);
		}
	} else
		setprg8(A, V &0x1F | prgOR <<1 &~0x1F);
}

static void M115CW(uint32 A, uint8 V) {
<<<<<<< HEAD
	setchr1(A, V | EXPREGS[1] <<8);
}

static DECLFR(M115Read) {
	return (A &3) ==2? EXPREGS[2]: X.DB;
}

static DECLFW(M115Write) {
	EXPREGS[A &3] =V;
	FixMMC3PRG(MMC3_cmd);
	FixMMC3CHR(MMC3_cmd);
}

static void M115Reset(void) {
	EXPREGS[2]++;
=======
	setchr1(A, (uint32)V | ((EXPREGS[1] & 1) << 8));
}

static void M115Write(uint32 A, uint8 V) {
	if (A == 0x5080)
		EXPREGS[2] = V;	/* Extra prot hardware 2-in-1 mode */
	else if (A == 0x6000)
		EXPREGS[0] = V;
	else if (A == 0x6001)
		EXPREGS[1] = V;
	FixMMC3PRG(MMC3_cmd);
}

static uint8 M115Read(uint32 A) {
	return EXPREGS[2];
>>>>>>> a2c232f (Update libretro_core_options.h)
}

static void M115Power(void) {
	EXPREGS[2] =0;
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x7FFF, M115Write);
	SetReadHandler(0x6000, 0x7FFF, M115Read);
}

void Mapper115_Init(CartInfo *info) {
	GenMMC3_Init(info, 128, 512, 0, 0);
	cwrap = M115CW;
	pwrap = M115PW;
	info->Reset = M115Reset;
	info->Power = M115Power;
	AddExState(EXPREGS, 3, 0, "EXPR");
}

/* ---------------------------- Mapper 118 ------------------------------ */

static uint8 PPUCHRBus;
static uint8 TKSMIR[8];

static void TKSPPU(uint32 A) {
	A &= 0x1FFF;
	A >>= 10;
	PPUCHRBus = A;
	setmirror(MI_0 + TKSMIR[A]);
}

static void TKSWRAP(uint32 A, uint8 V) {
	TKSMIR[A >> 10] = V >> 7;
	setchr1(A, V & 0x7F);
	if (PPUCHRBus == (A >> 10))
		setmirror(MI_0 + (V >> 7));
}

/* ---------------------------- Mapper 119 ------------------------------ */

static void TQWRAP(uint32 A, uint8 V) {
	setchr1r((V & 0x40) >> 2, A, V & 0x3F);
}

void Mapper119_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 64, 0, 0);
	cwrap = TQWRAP;
	CHRRAMSIZE = 8192;
	CHRRAM = (uint8*)FCEU_gmalloc(CHRRAMSIZE);
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
   AddExState(CHRRAM, CHRRAMSIZE, 0, "CHRR");
}

/* ---------------------------- Mapper 165 ------------------------------ */

static void M165CW(uint32 A, uint8 V) {
	if (V == 0)
		setchr4r(0x10, A, 0);
	else
		setchr4(A, V >> 2);
}

static void M165PPUFD(void) {
	if (EXPREGS[0] == 0xFD) {
		M165CW(0x0000, DRegBuf[0]);
		M165CW(0x1000, DRegBuf[2]);
	}
}

static void M165PPUFE(void) {
	if (EXPREGS[0] == 0xFE) {
		M165CW(0x0000, DRegBuf[1]);
		M165CW(0x1000, DRegBuf[4]);
	}
}

static void M165CWM(uint32 A, uint8 V) {
	if (((MMC3_cmd & 0x7) == 0) || ((MMC3_cmd & 0x7) == 2))
		M165PPUFD();
	if (((MMC3_cmd & 0x7) == 1) || ((MMC3_cmd & 0x7) == 4))
		M165PPUFE();
}

static void M165PPU(uint32 A) {
	if ((A & 0x1FF0) == 0x1FD0) {
		EXPREGS[0] = 0xFD;
		M165PPUFD();
	} else if ((A & 0x1FF0) == 0x1FE0) {
		EXPREGS[0] = 0xFE;
		M165PPUFE();
	}
}

static void M165Power(void) {
	EXPREGS[0] = 0xFD;
	GenMMC3Power();
}

void Mapper165_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 128, 8, info->battery);
	cwrap = M165CWM;
	PPU_hook = M165PPU;
	info->Power = M165Power;
	CHRRAMSIZE = 4096;
	CHRRAM = (uint8*)FCEU_gmalloc(CHRRAMSIZE);
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
	AddExState(CHRRAM, CHRRAMSIZE, 0, "CHRR");
	AddExState(EXPREGS, 4, 0, "EXPR");
}

/* ---------------------------- Mapper 191 ------------------------------ */

static void M191CW(uint32 A, uint8 V) {
	setchr1r((V & 0x80) >> 3, A, V);
}

void Mapper191_Init(CartInfo *info) {
	GenMMC3_Init(info, 256, 256, 8, info->battery);
	cwrap = M191CW;
	CHRRAMSIZE = 2048;
	CHRRAM = (uint8*)FCEU_gmalloc(CHRRAMSIZE);
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
	AddExState(CHRRAM, CHRRAMSIZE, 0, "CHRR");
}

/* ---------------------------- Mapper 192 ------------------------------- */

static void M192CW(uint32 A, uint8 V) {
	/* Ying Lie Qun Xia Zhuan (Chinese),
	* You Ling Xing Dong (China) (Unl) [this will be mistakenly headered as m074 sometimes]
	*/
	if ((V == 8) || (V == 9) || (V == 0xA) || (V == 0xB))
		setchr1r(0x10, A, V);
	else
		setchr1r(0, A, V);
}

void Mapper192_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 256, 8, info->battery);
	cwrap = M192CW;
	CHRRAMSIZE = 4096;
	CHRRAM = (uint8*)FCEU_gmalloc(CHRRAMSIZE);
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
	AddExState(CHRRAM, CHRRAMSIZE, 0, "CHRR");
}

/* ---------------------------- Mapper 194 ------------------------------- */

static void M194CW(uint32 A, uint8 V) {
	if (V <= 1)	/* Dai-2-Ji - Super Robot Taisen (As).nes */
		setchr1r(0x10, A, V);
	else
		setchr1r(0, A, V);
}

void Mapper194_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 256, 8, info->battery);
	cwrap = M194CW;
	CHRRAMSIZE = 2048;
	CHRRAM = (uint8*)FCEU_gmalloc(CHRRAMSIZE);
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
	AddExState(CHRRAM, CHRRAMSIZE, 0, "CHRR");
}

/* ---------------------------- Mapper 196 ------------------------------- */
/* MMC3 board with optional command address line connection, allows to
 * make three-four different wirings to IRQ address lines and separately to
 * CMD address line, Mali Boss additionally check if wiring are correct for
 * game
 */

static void M196PW(uint32 A, uint8 V) {
	if (EXPREGS[0])
		setprg32(0x8000, EXPREGS[1]);
	else
		setprg8(A, V);
}

static void Mapper196Write(uint32 A, uint8 V) {
	A =A &0xF000 | (!!(A &0xE) ^(A &1));
	if (A >= 0xC000)
		MMC3_IRQWrite(A, V);
	else
		MMC3_CMDWrite(A, V);
}

static void Mapper196WriteLo(uint32 A, uint8 V) {
	EXPREGS[0] = 1;
	EXPREGS[1] = (V & 0xf) | (V >> 4);
	FixMMC3PRG(MMC3_cmd);
}

static void Mapper196Power(void) {
	GenMMC3Power();
	EXPREGS[0] = EXPREGS[1] = 0;
	SetWriteHandler(0x6000, 0x6FFF, Mapper196WriteLo);
	SetWriteHandler(0x8000, 0xFFFF, Mapper196Write);
}

void Mapper196_Init(CartInfo *info) {
	GenMMC3_Init(info, 128, 128, 0, 0);
	pwrap = M196PW;
	info->Power = Mapper196Power;
}

/* ---------------------------- Mali Splash Bomb---------------------------- */
/* The same board as for 196 mapper games, but with additional data bit swap
 * Also, it is impossible to work on the combined 196 mapper source with
 * all data bits merged, because it's using one of them as 8000 reg...
 */

static void UNLMaliSBPW(uint32 A, uint8 V) {
	setprg8(A, (V & 3) | ((V & 8) >> 1) | ((V & 4) << 1));
}

static void UNLMaliSBCW(uint32 A, uint8 V) {
	setchr1(A, (V & 0xDD) | ((V & 0x20) >> 4) | ((V & 2) << 4));
}

static void UNLMaliSBWrite(uint32 A, uint8 V) {
	if (A >= 0xC000) {
		A = (A & 0xFFFE) | ((A >> 2) & 1) | ((A >> 3) & 1);
		MMC3_IRQWrite(A, V);
	} else {
		A = (A & 0xFFFE) | ((A >> 3) & 1);
		MMC3_CMDWrite(A, V);
	}
}

static void UNLMaliSBPower(void) {
	GenMMC3Power();
	SetWriteHandler(0x8000, 0xFFFF, UNLMaliSBWrite);
}

void UNLMaliSB_Init(CartInfo *info) {
	GenMMC3_Init(info, 128, 128, 0, 0);
	pwrap = UNLMaliSBPW;
	cwrap = UNLMaliSBCW;
	info->Power = UNLMaliSBPower;
}

/* ---------------------------- Mapper 197 ------------------------------- */

static void M197S0CW(uint32 A, uint8 V) {
	switch(A) {
	case 0x0000: setchr2(0x0000, V); break;
	case 0x0400: setchr2(0x0800, V); break;
	case 0x1000: setchr2(0x1000, V); break;
	case 0x1400: setchr2(0x1800, V); break;
	}
}

static void M197S1CW(uint32 A, uint8 V) {
	switch(A) {
	case 0x0800: setchr2(0x0000, V); break;
	case 0x0C00: setchr2(0x0800, V); break;
	case 0x1800: setchr2(0x1000, V); break;
	case 0x1C00: setchr2(0x1800, V); break;
	}
}

static void M197S2CW(uint32 A, uint8 V) {
	switch(A) {
	case 0x0000: setchr2(0x0000, V); break;
	case 0x0C00: setchr2(0x0800, V); break;
	case 0x1000: setchr2(0x1000, V); break;
	case 0x1C00: setchr2(0x1800, V); break;
	}
}

static void M197S3CW(uint32 A, uint8 V) {
	switch(A) {
	case 0x0000: setchr2(0x0000, V | EXPREGS[0] <<7 &0x100); break;
	case 0x0400: setchr2(0x0800, V | EXPREGS[0] <<7 &0x100); break;
	case 0x1000: setchr2(0x1000, V | EXPREGS[0] <<7 &0x100); break;
	case 0x1400: setchr2(0x1800, V | EXPREGS[0] <<7 &0x100); break;
	}
}

static void M197S3PW(uint32 A, uint8 V) {
	setprg8(A, V &(EXPREGS[0] &8? 0x0F: 0x1F) | EXPREGS[0] <<4);
}

static void Mapper197S3Write(uint32 A, uint8 V) {
	if (A001B &0x80) {
		EXPREGS[0] =V;
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
	}
}

static void Mapper197S3_Reset(void) {
	EXPREGS[0] =0;
	FixMMC3PRG(MMC3_cmd);
	FixMMC3CHR(MMC3_cmd);
}

static void Mapper197S3_Power(void) {
	EXPREGS[0] =0;
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x7fff, Mapper197S3Write);
}

void Mapper197_Init(CartInfo *info) {
	GenMMC3_Init(info, 128, 512, 8, 0);	
	switch(info->submapper) {
		case 0: cwrap =M197S0CW; break;
		case 1: cwrap =M197S1CW; break;
		case 2: cwrap =M197S2CW; break;
		case 3: cwrap =M197S3CW;
		        pwrap =M197S3PW;
			info->Power = Mapper197S3_Power;
			info->Reset = Mapper197S3_Reset;
			break;
	}
	AddExState(EXPREGS, 1, 0, "EXPR");
}

/* ---------------------------- Mapper 198 ------------------------------- */

static void M198Power(void) {
	GenMMC3Power();
	setprg4r(0x10, 0x5000, 2);
	SetWriteHandler(0x5000, 0x5fff, CartBW);
	SetReadHandler(0x5000, 0x5fff, CartBR);
}

static void M198PW(uint32 A, uint8 V) {
	if (V >= 0x50)	/* Tenchi o Kurau II - Shokatsu Koumei Den (J) (C).nes */
		setprg8(A, V & 0x4F);
	else
		setprg8(A, V);
}

void Mapper198_Init(CartInfo *info) {
	GenMMC3_Init(info, 1024, 0, 16, info->battery);
	pwrap = M198PW;
	info->Power = M198Power;
}

/* ---------------------------- Mapper 205 ------------------------------ */
/* UNIF boardname BMC-JC-016-2
https://wiki.nesdev.com/w/index.php/INES_Mapper_205 */

static void M205PW(uint32 A, uint8 V) {
	uint8 bank = V & ((EXPREGS[0] & 0x02) ? 0x0F : 0x1F);
	setprg8(A, EXPREGS[0] << 4 | bank);
}

static void M205CW(uint32 A, uint8 V) {
	uint8 bank = V & ((EXPREGS[0] & 0x02) ? 0x7F : 0xFF);
	setchr1(A, (EXPREGS[0] << 7) | bank);
}

static void M205Write0(uint32 A, uint8 V) {
	if (EXPREGS[1] == 0) {
		EXPREGS[0] = V & 0x03;
		EXPREGS[1] = A & 0x80;
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
	} else
		CartBW(A, V);
}

static void M205Write1(uint32 A, uint8 V) {
	if (EXPREGS[1] == 0) {
		EXPREGS[0] = V & 0xF0;
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
	} else
		CartBW(A, V);
}

static void M205Reset(void) {
	EXPREGS[0] = EXPREGS[1] = 0;
	MMC3RegReset();
}

static void M205Power(void) {
	EXPREGS[0] = EXPREGS[1] = 0;
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x6fff, M205Write0);
	SetWriteHandler(0x7000, 0x7fff, M205Write1);	/* OK-411 boards, the same logic, but data latched, 2-in-1 frankenstein */
}

void Mapper205_Init(CartInfo *info) {
	GenMMC3_Init(info, 256, 128, 8, 0);
	pwrap = M205PW;
	cwrap = M205CW;
	info->Power = M205Power;
	info->Reset = M205Reset;
	AddExState(EXPREGS, 1, 0, "EXPR");
}

/* --------------------------- GN-45 BOARD ------------------------------ */

/* Mapper 361 and 366, previously assigned as Mapper 205 */
static void GN45PW(uint32 A, uint8 V) {
/* GN-30A - \ED\E0\F7\E0\EB\FC\ED\E0\FF \EC\E0\F1\EA\E0 \E4\EE\EB\E6\ED\E0 \E1\FB\F2\FC 1F + \E0\EF\EF\E0\F0\E0\F2\ED\FB\E9 \EF\E5\F0\E5\EA\EB\FE\F7\E0\F2\E5\EB\FC \ED\E0 \F8\E8\ED\E5 \E0\E4\F0\E5\F1\E0 */
	setprg8(A, (V & 0x0f) | EXPREGS[0]);
}

static void GN45CW(uint32 A, uint8 V) {
/* GN-30A - \ED\E0\F7\E0\EB\FC\ED\E0\FF \EC\E0\F1\EA\E0 \E4\EE\EB\E6\ED\E0 \E1\FB\F2\FC FF */
	setchr1(A, (V & 0x7F) | (EXPREGS[0] << 3));
}

static void GN45Write0(uint32 A, uint8 V) {
	if (EXPREGS[2] == 0) {
		EXPREGS[0] = A & 0x30;
		EXPREGS[2] = A & 0x80;
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
	} else
		CartBW(A, V);
}

static void GN45Write1(uint32 A, uint8 V) {
	if (EXPREGS[2] == 0) {
		EXPREGS[0] = V & 0x30;
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
	} else
		CartBW(A, V);
}

static void GN45Reset(void) {
	EXPREGS[0] = EXPREGS[2] = 0;
	MMC3RegReset();
}

static void GN45Power(void) {
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x6fff, GN45Write0);
	SetWriteHandler(0x7000, 0x7fff, GN45Write1);	/* OK-411 boards, the same logic, but data latched, 2-in-1 frankenstein */
}

void GN45_Init(CartInfo *info) {
	GenMMC3_Init(info, 128, 128, 8, 0);
	pwrap = GN45PW;
	cwrap = GN45CW;
	info->Power = GN45Power;
	info->Reset = GN45Reset;
	AddExState(EXPREGS, 1, 0, "EXPR");
}

/* ---------------------------- Mapper 245 ------------------------------ */

static void M245CW(uint32 A, uint8 V) {
	if (!UNIFchrrama)	/* Yong Zhe Dou E Long - Dragon Quest VI (As).nes NEEDS THIS for RAM cart */
		setchr1(A, V & 7);
	EXPREGS[0] = V;
	FixMMC3PRG(MMC3_cmd);
}

static void M245PW(uint32 A, uint8 V) {
	setprg8(A, (V & 0x3F) | ((EXPREGS[0] & 2) << 5));
}

static void M245Power(void) {
	EXPREGS[0] = 0;
	GenMMC3Power();
}

void Mapper245_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 256, 8, info->battery);
	cwrap = M245CW;
	pwrap = M245PW;
	info->Power = M245Power;
	AddExState(EXPREGS, 1, 0, "EXPR");
}

/* ---------------------------- Mapper 249 ------------------------------ */

static void M249PW(uint32 A, uint8 V) {
	if (EXPREGS[0] & 0x2) {
		if (V < 0x20)
			V = (V & 1) | ((V >> 3) & 2) | ((V >> 1) & 4) | ((V << 2) & 8) | ((V << 2) & 0x10);
		else {
			V -= 0x20;
			V = (V & 3) | ((V >> 1) & 4) | ((V >> 4) & 8) | ((V >> 2) & 0x10) | ((V << 3) & 0x20) | ((V << 2) & 0xC0);
		}
	}
	setprg8(A, V);
}

static void M249CW(uint32 A, uint8 V) {
	if (EXPREGS[0] & 0x2)
		V = (V & 3) | ((V >> 1) & 4) | ((V >> 4) & 8) | ((V >> 2) & 0x10) | ((V << 3) & 0x20) | ((V << 2) & 0xC0);
	setchr1(A, V);
}

static void M249Write(uint32 A, uint8 V) {
	EXPREGS[0] = V;
	FixMMC3PRG(MMC3_cmd);
	FixMMC3CHR(MMC3_cmd);
}

static void M249Power(void) {
	EXPREGS[0] = 0;
	GenMMC3Power();
	SetWriteHandler(0x5000, 0x5000, M249Write);
}

void Mapper249_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 256, 8, info->battery);
	cwrap = M249CW;
	pwrap = M249PW;
	info->Power = M249Power;
	AddExState(EXPREGS, 1, 0, "EXPR");
}

/* ---------------------------- Mapper 250 ------------------------------ */

static void M250Write(uint32 A, uint8 V) {
	MMC3_CMDWrite((A & 0xE000) | ((A & 0x400) >> 10), A & 0xFF);
}

static void M250IRQWrite(uint32 A, uint8 V) {
	MMC3_IRQWrite((A & 0xE000) | ((A & 0x400) >> 10), A & 0xFF);
}

static void M250_Power(void) {
	GenMMC3Power();
	SetWriteHandler(0x8000, 0xBFFF, M250Write);
	SetWriteHandler(0xC000, 0xFFFF, M250IRQWrite);
}

void Mapper250_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 256, 8, info->battery);
	info->Power = M250_Power;
}

/* ---------------------------- Mapper 254 ------------------------------ */

static uint8 MR254WRAM(uint32 A) {
	if (EXPREGS[0])
		return WRAM[A - 0x6000];
	return WRAM[A - 0x6000] ^ EXPREGS[1];
}

static void M254Write(uint32 A, uint8 V) {
	switch (A) {
	case 0x8000: EXPREGS[0] = 0xff; break;
	case 0xA001: EXPREGS[1] = V; break;
	}
	MMC3_CMDWrite(A, V);
}

static void M254_Power(void) {
	GenMMC3Power();
	SetWriteHandler(0x8000, 0xBFFF, M254Write);
	SetReadHandler(0x6000, 0x7FFF, MR254WRAM);
}

void Mapper254_Init(CartInfo *info) {
	GenMMC3_Init(info, 128, 128, 8, info->battery);
	info->Power = M254_Power;
	AddExState(EXPREGS, 2, 0, "EXPR");
}

/* ---------------------------- Mapper 555 ------------------------------ */

static uint8 m555_reg[2];
static uint8 m555_count_expired;
static uint32 m555_count;
static uint32 m555_count_target = 0x20000000;

static SFORMAT M555StateRegs[] = {
	{ m555_reg, 2, "REGS" },
	{ &m555_count, 2, "CNTR" },
	{ &m555_count_expired, 2, "CNTE" },
	{ 0 }
};

static void M555CW(uint32 A, uint8 V)
{
	uint16 base = (m555_reg[0] << 5) & 0x80;

	if ((m555_reg[0] & 0x06) == 0x02) {
		if (V & 0x40) {
			setchr1r(0x10, A, base | (V & 0x07));
		} else {
			setchr1(A, base | (V & 0xFF));
		}
	} else {
		setchr1(A, base | (V & 0x7F));
	}
}

static void M555PW(uint32 A, uint8 V) {
	uint16 mask = ((m555_reg[0] << 3) & 0x18) | 0x07;
	uint16 base = ((m555_reg[0] << 3) & 0x20);

	setprg8(A, base | (V & mask));
}

static DECLFR(M555Read5)
{
	if (A & 0x800)
		return (0x5C | (m555_count_expired ? 0x80 : 0));
	return WRAM[0x2000 | (A & 0xFFF)];
}

static DECLFW(M555Write5) {
	if (A & 0x800) {
		m555_reg[(A >> 10) & 0x01] = V;
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
	} else {
		WRAM[0x2000 | (A & 0xFFF)] = V;
	}
}

static void M555Reset(void) {
	m555_count_target = 0x20000000 | ((uint32)GameInfo->cspecial << 25);
	m555_count = 0;
	memset(m555_reg, 0, sizeof(m555_reg));
	MMC3RegReset();
}

static void M555Power(void) {
	m555_count_target = 0x20000000 | ((uint32)GameInfo->cspecial << 25);
	m555_count = 0;
	memset(m555_reg, 0, sizeof(m555_reg));
	GenMMC3Power();

	SetReadHandler(0x5000, 0x5FFF, M555Read5);
	SetWriteHandler(0x5000, 0x5FFF, M555Write5);

	setprg8r(0x10, 0x6000, 0);
	SetReadHandler(0x6000, 0x7FFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
}

static void M555CPUIRQHook(int a) {
	while (a--) {
		if (!(m555_reg[0] & 0x08))
		{
			m555_count = 0;
			m555_count_expired = false;
		}
		else
		{
			if (++m555_count == m555_count_target)
				m555_count_expired = true;
		}
	}
}

void Mapper555_Init(CartInfo *info) {
	GenMMC3_Init(info, 0, 0, 0, info->battery);
	cwrap       = M555CW;
	pwrap       = M555PW;
	info->Power = M555Power;
	info->Reset = M555Reset;
	MapIRQHook  = M555CPUIRQHook;
	AddExState(M555StateRegs, ~0, 0, NULL);

	WRAMSIZE = 16 * 1024;
	WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");

	CHRRAMSIZE = 8 * 1024;
	CHRRAM = (uint8 *)FCEU_gmalloc(CHRRAMSIZE);
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
	AddExState(CHRRAM, CHRRAMSIZE, 0, "CHRR");
}

/* ---------------------------- UNIF Boards ----------------------------- */

void TBROM_Init(CartInfo *info) {
	GenMMC3_Init(info, 64, 64, 0, 0);
}

void TEROM_Init(CartInfo *info) {
	GenMMC3_Init(info, 32, 32, 0, 0);
}

void TFROM_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 64, 0, 0);
}

void TGROM_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 0, 0, 0);
}

void TKROM_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 256, 8, info->battery);
}

void TLROM_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 256, 0, 0);
}

void TSROM_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 256, 8, 0);
}

void TLSROM_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 256, 8, 0);
	cwrap = TKSWRAP;
	mwrap = GENNOMWRAP;
	PPU_hook = TKSPPU;
	AddExState(&PPUCHRBus, 1, 0, "PPUC");
}

void TKSROM_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 256, 8, info->battery);
	cwrap = TKSWRAP;
	mwrap = GENNOMWRAP;
	PPU_hook = TKSPPU;
	AddExState(&PPUCHRBus, 1, 0, "PPUC");
}

void TQROM_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 64, 0, 0);
	cwrap = TQWRAP;
	CHRRAMSIZE = 8192;
	CHRRAM = (uint8*)FCEU_gmalloc(CHRRAMSIZE);
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
   AddExState(CHRRAM, CHRRAMSIZE, 0, "CHRR");
}

void HKROM_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 512, 1, info->battery);
}
