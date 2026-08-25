/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 1998 BERO
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

#include "mapinc.h"
<<<<<<< HEAD

static void GenMMC1Power(void);
static void GenMMC1Init(CartInfo *info, int prg, int chr, int wram, int saveram);

static uint8_t submapper;
static uint8_t DRegs[4];
static uint8_t Buffer, BufferShift;

<<<<<<< HEAD
static uint32_t WRAMSIZE = 0;
=======
static uint32 WRAMSIZE = 0;
=======
#include "mmc1.h"
>>>>>>> dffcc139 (Update libretro.c)
>>>>>>> 5926d713 (Update libretro.c)

 /* size of non-battery-backed portion of WRAM */
 /* serves as starting offset for actual save ram from total wram size */
 /* returns 0 if entire work ram is battery backed ram */
static uint32_t NONSaveRAMSIZE = 0;

<<<<<<< HEAD
static void (*MMC1CHRHook4)(uint32_t A, uint8_t V);
static void (*MMC1PRGHook16)(uint32_t A, uint8_t V);
/* Used to override default wram behavior */
/* NULL uses default MMC1 wram. Set after GenMMC1Init() is called to override */
static void (*MMC1WRAMHook8)(void);

static uint8_t *WRAM = NULL;
static uint8_t *CHRRAM = NULL;
=======
MMC1Type mmc1_type = MMC1B;

<<<<<<< HEAD
static uint8 *WRAM = NULL;
static uint8 *CHRRAM = NULL;
>>>>>>> 5926d713 (Update libretro.c)
static int is155;
=======
void (*MMC1_pwrap)(uint16 A, uint16 V);
void (*MMC1_cwrap)(uint16 A, uint16 V);
void (*MMC1_mwrap)(uint8 V);
void (*MMC1_wwrap)(void);
>>>>>>> dffcc139 (Update libretro.c)

<<<<<<< HEAD
static uint32_t MMC1GetCHRBank (uint32_t bank) {
	if (DRegs[0] & 0x10)	/* 4 KiB mode */
		return (DRegs[1 + bank]);
	return ((DRegs[1] & ~1) | bank); /* 8 KiB mode */
}

static uint8_t MMC1WRAMEnabled(void) {
=======
MMC1 mmc1;

static void GENWRAMWRAP(void) {
	uint8 bank = 0;

	if (!WRAMSIZE) {
		return;
	}
	if (WRAMSIZE > 8192) {
		if (WRAMSIZE > 16384) {
			bank = (mmc1.reg[1] >> 2) & 3;
		} else {
			bank = (mmc1.reg[1] >> 3) & 1;
		}
	}
	setprg8r(0x10, 0x6000, bank);
}

static void GENPWRAP(uint16 A, uint16 V) {
	setprg16(A, V & 0x0F);
}

static void GENCWRAP(uint16 A, uint16 V) {
	setchr4(A, V & 0x1F);
}

static uint8 MMC1WRAMEnabled(void) {
<<<<<<< HEAD
>>>>>>> 5926d713 (Update libretro.c)
	return !(DRegs[3] & 0x10);
}

static void MBWRAM(uint32 A, uint8 V) {
	if (MMC1WRAMEnabled() || is155)
		Page[A >> 11][A] = V;	/* WRAM is enabled. */
}

static uint8 MAWRAM(uint32 A) {
	if (!MMC1WRAMEnabled() && !is155)
		return cpu.openbus;			/* WRAM is disabled */
	return(Page[A >> 11][A]);
}

static DECLFR(SFEXPROM_readBank) {
	return DRegs[3] <<3 &0x70;
}

static void MMC1CHR(void) {
	if (MMC1WRAMHook8)		/* Use custom wram hook, currently used for M543 */
		MMC1WRAMHook8();
	else {					/* Use default MMC1 wram behavior */
		if (WRAMSIZE > 8192) {
			if (WRAMSIZE > 16384)
				setprg8r(0x10, 0x6000, (DRegs[1] >> 2) & 3);
			else
				setprg8r(0x10, 0x6000, (DRegs[1] >> 3) & 1);
		}
=======
	if ((mmc1.reg[3] & 0x10) && (mmc1_type == MMC1B)) {
		return FALSE;
>>>>>>> dffcc139 (Update libretro.c)
	}

	return TRUE;
}

static DECLFW(MBWRAM) {
	if (MMC1WRAMEnabled()) {
		/* WRAM is enabled. */
		CartBW(A, V);
	}
}

<<<<<<< HEAD
static void MMC1PRG(void) {
	uint8_t offs = DRegs[1] & 0x10;
	if (MMC1PRGHook16) {
		switch (DRegs[0] & 0xC) {
		case 0xC:
			MMC1PRGHook16(0x8000, (DRegs[3] + offs));
			MMC1PRGHook16(0xC000, 0xF + offs);
			break;
		case 0x8:
			MMC1PRGHook16(0xC000, (DRegs[3] + offs));
			MMC1PRGHook16(0x8000, offs);
			break;
		case 0x0:
		case 0x4:
			MMC1PRGHook16(0x8000, ((DRegs[3] & ~1) + offs));
			MMC1PRGHook16(0xc000, ((DRegs[3] & ~1) + offs + 1));
			break;
		}
	} else {
		switch (DRegs[0] & 0xC) {
		case 0xC:
			setprg16(0x8000, (DRegs[3] + offs));
			setprg16(0xC000, 0xF + offs);
			break;
		case 0x8:
			setprg16(0xC000, (DRegs[3] + offs));
			setprg16(0x8000, offs);
			break;
		case 0x0:
		case 0x4:
			setprg16(0x8000, ((DRegs[3] & ~1) + offs));
			setprg16(0xc000, ((DRegs[3] & ~1) + offs + 1));
			break;
		}
=======
static DECLFR(MAWRAM) {
	if (!MMC1WRAMEnabled()) {
		/* WRAM is disabled */
		return cpu.openbus;
>>>>>>> 5926d713 (Update libretro.c)
	}

	return CartBR(A);
}

<<<<<<< HEAD
static void MMC1MIRROR(void) {
	if (submapper != 7)
		switch (DRegs[0] & 3) {
		case 2: setmirror(MI_V); break;
		case 3: setmirror(MI_H); break;
		case 0: setmirror(MI_0); break;
		case 1: setmirror(MI_1); break;
		}
=======
uint32 MMC1_GetPRGBank(int index) {
	uint32 bank;
	uint8 prg = mmc1.reg[3];

	switch (mmc1.reg[0] & 0xC) {
	case 0xC:
		bank = prg | (index * 0x0F);
		break;
	case 0x8:
		bank = (prg & (index * 0x0F));
		break;
	case 0x0:
	case 0x4:
	default:
		bank = (prg & ~1) | index;
		break;
	}

	if ((mmc1.reg[3] & 0x10) && (mmc1_type == MMC1A)) {
		return ((bank & 0x07) | (mmc1.reg[3] & 0x08));
	}

	return (bank & 0x0F);
}

uint32 MMC1_GetCHRBank(int index) {
	if (mmc1.reg[0] & 0x10) {
		return (mmc1.reg[1 + index]);
	}

	return ((mmc1.reg[1] & ~1) | index);
}

void MMC1_FixCHR(void) {
	if (MMC1_wwrap) {
		MMC1_wwrap();
	}

	MMC1_cwrap(0x0000, MMC1_GetCHRBank(0));
	MMC1_cwrap(0x1000, MMC1_GetCHRBank(1));
}

void MMC1_FixPRG(void) {
	MMC1_pwrap(0x8000, MMC1_GetPRGBank(0));
	MMC1_pwrap(0xC000, MMC1_GetPRGBank(1));
}

void MMC1_FixMIR(void) {
	switch (mmc1.reg[0] & 3) {
	case 2: setmirror(MI_V); break;
	case 3: setmirror(MI_H); break;
	case 0: setmirror(MI_0); break;
	case 1: setmirror(MI_1); break;
	}
>>>>>>> dffcc139 (Update libretro.c)
}

<<<<<<< HEAD
static uint64_t lreset;
static DECLFW(MMC1_write) {
=======
static uint64 lreset;
<<<<<<< HEAD
static void MMC1_write(uint32 A, uint8 V) {
>>>>>>> b8aebecd (Update libretro.c)
=======
DECLFW(MMC1_Write) {
>>>>>>> 5926d713 (Update libretro.c)
	int n = (A >> 13) - 4;

	/* The MMC1 is busy so ignore the write. */
	/* As of version FCE Ultra 0.81, the timestamp is only
		increased before each instruction is executed(in other words
		precision isn't that great), but this should still work to
		deal with 2 writes in a row from a single RMW instruction.
	*/
	if ((timestampbase + timestamp) < (lreset + 2)) {
		return;
	}

	/* FCEU_printf("Write %04x:%02x\n",A,V); */
	if (V & 0x80) {
		mmc1.reg[0] |= 0xC;
		mmc1.shift = mmc1.buffer = 0;
		MMC1_FixPRG();
		lreset = timestampbase + timestamp;
		return;
	}

	mmc1.buffer |= (V & 1) << (mmc1.shift++);

	if (mmc1.shift == 5) {
		/* FCEU_printf("REG[%d]=%02x\n",n,mmc1.buffer); */
		mmc1.reg[n] = mmc1.buffer;
		mmc1.shift = mmc1.buffer = 0;
		switch (n) {
		case 0:
			MMC1_FixMIR();
			MMC1_FixCHR();
			MMC1_FixPRG();
			break;
		case 1:
			MMC1_FixCHR();
			MMC1_FixPRG();
			break;
		case 2:
			MMC1_FixCHR();
			break;
		case 3:
			MMC1_FixPRG();
			break;
		}
	}
}

void MMC1_Restore(int version) {
	MMC1_FixMIR();
	MMC1_FixCHR();
	MMC1_FixPRG();
	lreset = 0; /* timestamp(base) is not stored in save states. */
}

void MMC1_Reset(void) {
	mmc1.reg[0] = 0x0C;
	mmc1.reg[1] = 0;
	mmc1.reg[2] = 0;
	mmc1.reg[3] = 0;

<<<<<<< HEAD
	for (i = 0; i < 4; i++)
		DRegs[i] = 0;
	Buffer = BufferShift = 0;
	DRegs[0] = 0x0C;
=======
	mmc1.buffer = mmc1.shift = 0;
>>>>>>> 5d22892 (Update libretro.c)

	MMC1_FixPRG();
	MMC1_FixCHR();
	MMC1_FixMIR();
}

<<<<<<< HEAD
static int DetectMMC1WRAMSize(CartInfo *info, int *saveRAM) {
	int workRAM = 8;
	if (info->iNES2) {
		workRAM = (info->PRGRamSize + info->PRGRamSaveSize) / 1024;
		*saveRAM = info->PRGRamSaveSize / 1024;
		/* we only support sizes between 8K and 32K */
		if (workRAM > 0 && workRAM < 8)
			workRAM = 8;
		if (workRAM > 32)
			workRAM = 32;
		if (*saveRAM > 0 && *saveRAM < 8)
			*saveRAM = 8;
		if (*saveRAM > 32)
			*saveRAM = 32;
		/* save ram cannot be bigger than workram */
		if (*saveRAM > workRAM) {
			*saveRAM = workRAM;
			workRAM = 0;
		}
	} else if (info->battery) {
		*saveRAM = 8;
	}
	return workRAM;
}

static uint32_t NWCIRQCount;
static uint8_t NWCRec;
static uint32_t nwcdip = 0x4;

static void NWCIRQHook(int a) {
	if (!(NWCRec & 0x10)) {
		NWCIRQCount += a;
		if (NWCIRQCount >= nwcdip) {
			NWCIRQCount = 0;
			X6502_IRQBegin(FCEU_IQEXT);
		}
	}
}

static void NWCCHRHook(uint32_t A, uint8_t V) {
	if ((V & 0x10)) {	/* && !(NWCRec&0x10)) */
		NWCIRQCount = 0;
		X6502_IRQEnd(FCEU_IQEXT);
	}

	NWCRec = V;
	if (V & 0x08)
		MMC1PRG();
	else
		setprg32(0x8000, (V >> 1) & 3);
}

static void NWCPRGHook(uint32_t A, uint8_t V) {
	if (NWCRec & 0x8)
		setprg16(A, 8 | (V & 0x7));
	else
		setprg32(0x8000, (NWCRec >> 1) & 3);
}

static void NWCPower(void) {
	GenMMC1Power();
	setchr8r(0, 0);
	nwcdip = 0x20000000 | ((uint32_t)GameInfo->cspecial << 25);
}

static void NWCReset(void) {
	nwcdip = 0x20000000 | ((uint32_t)GameInfo->cspecial << 25);
	MMC1CMReset();
}

void Mapper105_Init(CartInfo *info) {
	GenMMC1Init(info, 256, 256, 8, 0);
	MMC1CHRHook4 = NWCCHRHook;
	MMC1PRGHook16 = NWCPRGHook;
	MapIRQHook = NWCIRQHook;
	info->Power = NWCPower;
	info->Reset = NWCReset;
}

static void GenMMC1Power(void) {
=======
void MMC1_Power(void) {
>>>>>>> 5926d713 (Update libretro.c)
	lreset = 0;
	SetWriteHandler(0x8000, 0xFFFF, MMC1_Write);
	SetReadHandler(0x8000, 0xFFFF, CartBR);

	if (WRAMSIZE) {
		FCEU_CheatAddRAM(8, 0x6000, WRAM);

		/* clear non-battery-backed portion of WRAM */
		if (NONSaveRAMSIZE) {
			FCEU_MemoryRand(WRAM, NONSaveRAMSIZE);
		}

		SetReadHandler(0x6000, 0x7FFF, MAWRAM);
		SetWriteHandler(0x6000, 0x7FFF, MBWRAM);
		setprg8r(0x10, 0x6000, 0);
	} else
	if (submapper == 8)
		SetReadHandler(0x6000, 0x6FFF, SFEXPROM_readBank);

	MMC1_Reset();
}

void MMC1_Close(void) {
}

<<<<<<< HEAD
static void GenMMC1Init(CartInfo *info, int prg, int chr, int wram, int saveram) {
	submapper = info->submapper;
	is155 = 0;
=======
void MMC1_Init(CartInfo *info, int wram, int saveram) {
	MMC1_pwrap = GENPWRAP;
	MMC1_cwrap = GENCWRAP;
	MMC1_wwrap = GENWRAMWRAP;
>>>>>>> dffcc139 (Update libretro.c)

	WRAMSIZE = wram * 1024;
	NONSaveRAMSIZE = (wram - saveram) * 1024;

	if (WRAMSIZE) {
		WRAM = (uint8_t*)FCEU_gmalloc(WRAMSIZE);
		SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
		AddExState(WRAM, WRAMSIZE, 0, "WRAM");
		if (saveram) {
			info->SaveGame[0] = WRAM + NONSaveRAMSIZE;
			info->SaveGameLen[0] = saveram * 1024;
		}
	}
<<<<<<< HEAD
	if (!chr) {
		CHRRAM = (uint8_t*)FCEU_gmalloc(8192);
		SetupCartCHRMapping(0, CHRRAM, 8192, 1);
		AddExState(CHRRAM, 8192, 0, "CHRR");
	}
	AddExState(DRegs, 4, 0, "DREG");
=======
>>>>>>> 5926d713 (Update libretro.c)

	AddExState(mmc1.reg, 4, 0, "DREG");

	info->Power = MMC1_Power;
	info->Close = MMC1_Close;
	GameStateRestore = MMC1_Restore;
	AddExState(&lreset, 8, 1, "LRST");
<<<<<<< HEAD
	AddExState(&Buffer, 1, 1, "BFFR");
	AddExState(&BufferShift, 1, 1, "BFRS");
}

void Mapper1_Init(CartInfo *info) {
	int bs = info->battery ? 8 : 0;
	int ws = DetectMMC1WRAMSize(info, &bs);
	GenMMC1Init(info, 512, 256, ws, bs);
}

/* Same as mapper 1, without respect for WRAM enable bit. */
void Mapper155_Init(CartInfo *info) {
	GenMMC1Init(info, 512, 256, 8, info->battery ? 8 : 0);
	is155 = 1;
=======
	AddExState(&mmc1.buffer, 1, 1, "BFFR");
	AddExState(&mmc1.shift, 1, 1, "BFRS");
>>>>>>> dffcc139 (Update libretro.c)
}

void SAROM_Init(CartInfo *info) {
	MMC1_Init(info, 8, info->battery ? 8 : 0);
}

void SKROM_Init(CartInfo *info) {
	MMC1_Init(info, 8, info->battery ? 8 : 0);
}

<<<<<<< HEAD
void SLROM_Init(CartInfo *info) {
	GenMMC1Init(info, 256, 128, 0, 0);
}

void SL1ROM_Init(CartInfo *info) {
	GenMMC1Init(info, 128, 128, 0, 0);
}

/* Begin unknown - may be wrong - perhaps they use different MMC1s from the
	similarly functioning boards?
*/

static void SL2ROM_Init(CartInfo *info) {
	GenMMC1Init(info, 256, 256, 0, 0);
}

static void SFROM_Init(CartInfo *info) {
	GenMMC1Init(info, 256, 256, 0, 0);
}

static void SHROM_Init(CartInfo *info) {
	GenMMC1Init(info, 256, 256, 0, 0);
}

/* End unknown  */
/*              */
/*              */

=======
>>>>>>> 5926d713 (Update libretro.c)
void SNROM_Init(CartInfo *info) {
	MMC1_Init(info, 8, info->battery ? 8 : 0);
}

void SOROM_Init(CartInfo *info) {
<<<<<<< HEAD
	GenMMC1Init(info, 256, 0, 16, info->battery ? 8 : 0);
}

/* ----------------------- FARID_SLROM_8-IN-1 -----------------------*/

/* NES 2.0 Mapper 323 - UNIF FARID_SLROM_8-IN-1 */

static uint8_t reg, lock;

static void FARIDSLROM8IN1PRGHook(uint32_t A, uint8_t V) {
	setprg16(A, (V & 0x07) | (reg << 3));
}

static void FARIDSLROM8IN1CHRHook(uint32_t A, uint8_t V) {
	setchr4(A, (V & 0x1F) | (reg << 5));
}

static void FARIDSLROM8IN1Write(uint32 A, uint8 V) {
	if (MMC1WRAMEnabled() && !lock) {
		lock = (V & 0x08) >> 3;
		reg = (V & 0xF0) >> 4;
		MMC1MIRROR();
		MMC1CHR();
		MMC1PRG();
	}
}

static void FARIDSLROM8IN1Power(void) {
	reg = lock = 0;
	GenMMC1Power();
	SetWriteHandler(0x6000, 0x7FFF, FARIDSLROM8IN1Write);
}

static void FARIDSLROM8IN1Reset(void) {
	reg = lock = 0;
	MMC1CMReset();
}

void FARIDSLROM8IN1_Init(CartInfo *info) {
	GenMMC1Init(info, 1024, 256, 8, 0);
	MMC1CHRHook4 = FARIDSLROM8IN1CHRHook;
	MMC1PRGHook16 = FARIDSLROM8IN1PRGHook;
	info->Power = FARIDSLROM8IN1Power;
	info->Reset = FARIDSLROM8IN1Reset;
	AddExState(&lock, 1, 0, "LOCK");
	AddExState(&reg, 1, 0, "REG6");
}

/* ---------------------------- Mapper 374 -------------------------------- */
/* 1995 Super HiK 4-in-1 - 新系列機器戰警组合卡 (JY-022)
 * 1996 Super HiK 4-in-1 - 新系列超級飛狼組合卡 (JY-051)
 */
static uint8_t game = 0;
static void M374PRG(uint32_t A, uint8_t V) {
	setprg16(A, (V & 0x07) | (game << 3));
}

static void M374CHR(uint32_t A, uint8_t V) {
	setchr4(A, (V & 0x1F) | (game << 5));
}

static void M374Reset(void) {
	game = (game + 1) & 3;
	MMC1CMReset();
}

void Mapper374_Init(CartInfo *info) {
	GenMMC1Init(info, 128, 128, 0, 0);
	MMC1CHRHook4 = M374CHR;
	MMC1PRGHook16 = M374PRG;
	info->Reset = M374Reset;
	AddExState(&game, 1, 0, "GAME");
}

/* ---------------------------- Mapper 297 -------------------------------- */
/* NES 2.0 Mapper 297 - 2-in-1 Uzi Lightgun (MGC-002) */

static uint8_t mode;
static uint8_t latch;

static void M297PRG(uint32_t A, uint8_t V) {
	setprg16(A, (V & 0x07) | ((mode & 1) << 3));
}

static void M297CHR(uint32_t A, uint8_t V) {
	setchr4(A, (V & 0x1F) | ((mode & 1) << 5));
}

static void Sync(void) {
	if (mode & 1) {
		/* MMC1 */
		MMC1PRG();
		MMC1CHR();
		MMC1MIRROR();
	} else {
		/* Mapper 70 */
		setprg16(0x8000, ((mode & 2) << 1) | ((latch >> 4) & 3));
		setprg16(0xC000, ((mode & 2) << 1) | 3);
		setchr8(latch & 0xF);
		setmirror(1);
	}
}

static void M297Mode(uint32 A, uint8 V) {
	if (A & 0x100) {
		mode = V;
		Sync();
	}
}

static void M297Latch(uint32 A, uint8 V) {
	if (mode & 1) {
		MMC1_write(A, V);
	} else {
		latch = V;
		Sync();
	}
}

static void M297Power(void) {
	latch = 0;
	mode = 0;
	Sync();
	GenMMC1Power();
	SetWriteHandler(0x4120, 0x4120, M297Mode);
	SetWriteHandler(0x8000, 0xFFFF, M297Latch);
}

static void M297StateRestore(int version) {
	Sync();
}

void Mapper297_Init(CartInfo *info) {
	GenMMC1Init(info, 256, 256, 0, 0);
	info->Power = M297Power;
	MMC1CHRHook4 = M297CHR;
	MMC1PRGHook16 = M297PRG;
	GameStateRestore = M297StateRestore;
	AddExState(&latch, 1, 0, "LATC");
	AddExState(&mode, 1, 0, "MODE");
}

/* ---------------------------- Mapper 543 -------------------------------- */

/* NES 2.0 Mapper 543 - 1996 無敵智カ卡 5-in-1 (CH-501) */

static uint8_t outerBank;
static uint8_t bits;
static uint8_t shift;

static void M543PRG16(uint32_t A, uint8_t V) {
	setprg16(A, (V & 0x0F) | (outerBank << 4));
}

static void M543CHR4(uint32_t A, uint8_t V) {
	setchr4(A, (V & 7));
}

static void M543WRAM8(void) {
	uint32_t wramBank;
	if (outerBank & 2)
		wramBank = 4 | ((outerBank >> 1) & 2) | (outerBank & 1) ;
	else
		wramBank = ((outerBank << 1) & 2) | ((MMC1GetCHRBank(0) >> 3) & 1);
	setprg8r(0x10, 0x6000, wramBank);
}

static void M543Write(uint32 A, uint8 V) {
	bits |= ((V >> 3) & 1) << shift++;
	if (shift == 4) {
		outerBank = bits;
		bits = shift = 0;
		MMC1PRG();
		MMC1CHR();
	}
}

static void M543Reset(void) {
	bits = 0;
	shift = 0;
	outerBank = 0;
	MMC1CMReset();
	RAM[0] =0; /* BUG in cartridge's menu code: expects RAM location $000 to be <FA */
}

static void M543Power(void) {
	bits = 0;
	shift = 0;
	outerBank = 0;
	GenMMC1Power();
	SetWriteHandler(0x5000, 0x5FFF, M543Write);
	RAM[0] =0; /* BUG in cartridge's menu code: expects RAM location $000 to be <FA */
}

void Mapper543_Init(CartInfo *info) {
	/* M543 has 32K CHR RAM but only uses 8K, so its safe to set this chr to 0 */
	GenMMC1Init(info, 2048, 32, 64, info->battery ? 64 : 0);
	info->Power = M543Power;
	info->Reset = M543Reset;
	MMC1CHRHook4 = M543CHR4;
	MMC1PRGHook16 = M543PRG16;
	MMC1WRAMHook8 = M543WRAM8;
	AddExState(&bits, 1, 0, "BITS");
	AddExState(&shift, 1, 0, "SHFT");
	AddExState(&outerBank, 1, 0, "BANK");
}

/* ---------------------------- Mapper 550 -------------------------------- */

/* NES 2.0 Mapper 550 - 7-in-1 1993 Chess Series (JY-015) */

static uint8_t latch;
static uint8_t outerBank;

static void M550PRG16(uint32_t A, uint8_t V) {
	if ((outerBank & 6) == 6)
		setprg16(A, (V & 7) | (outerBank << 2));
	else
		setprg32(0x8000, (latch >> 4) | (outerBank << 1));
}

static void M550CHR4(uint32_t A, uint8_t V) {
	if ((outerBank & 6) == 6)
		setchr4(A, (V & 7) | ((outerBank << 2) & 0x18));
	else
		setchr8((latch & 3) | ((outerBank << 1) & 0x0C));
}

static void M550Write7(uint32 A, uint8 V) {
	if (!(outerBank & 8)) {
		outerBank = A & 15;
		MMC1PRG();
		MMC1CHR();
	}
}

static void M550Write8(uint32 A, uint8 V) {
	latch = V;
	if ((outerBank & 6) == 6)
		MMC1_write(A, V);
	MMC1PRG();
	MMC1CHR();
}

static void M550Reset(void) {
	latch = 0;
	outerBank = 0;
	MMC1CMReset();
}

static void M550Power(void) {
	latch = 0;
	outerBank = 0;
	GenMMC1Power();
	SetWriteHandler(0x7000, 0x7FFF, M550Write7);
	SetWriteHandler(0x8000, 0xFFFF, M550Write8);
}

void Mapper550_Init(CartInfo *info) {
	GenMMC1Init(info, 512, 128, 8, 0);
	info->Power = M550Power;
	info->Reset = M550Reset;
	MMC1CHRHook4 = M550CHR4;
	MMC1PRGHook16 = M550PRG16;
	AddExState(&latch, 1, 0, "LATC");
	AddExState(&outerBank, 1, 0, "BANK");
}

/* ---------------------------- Mapper 404 -------------------------------- */

/* NES 2.0 Mapper 404 - JY012005
 * 1998 Super HiK 8-in-1 (JY-021B)*/

static uint8_t outerBank;

static void M404PRG16(uint32_t A, uint8_t V) {
	uint8_t mask = outerBank & 0x40 ? 0x07 : 0x0F;
	setprg16(A, (V & mask) | (outerBank << 3) & ~mask);
}

static void M404CHR4(uint32_t A, uint8_t V) {
	setchr4(A, (V & 0x1F) | outerBank << 5);
}

static void M404Write(uint32 A, uint8 V) {
	if (!(outerBank & 0x80)) {
		outerBank = V;
		MMC1PRG();
		MMC1CHR();
	}
}

static void M404Reset(void) {
	outerBank = 0;
	MMC1CMReset();
}

static void M404Power(void) {
	outerBank = 0;
	GenMMC1Power();
	SetWriteHandler(0x6000, 0x7FFF, M404Write);
}

void Mapper404_Init(CartInfo *info) {
	GenMMC1Init(info, 256, 256, 0, 0);
	info->Power = M404Power;
	info->Reset = M404Reset;
	MMC1CHRHook4 = M404CHR4;
	MMC1PRGHook16 = M404PRG16;
	AddExState(&outerBank, 1, 0, "BANK");
=======
	MMC1_Init(info, 16, info->battery ? 8 : 0);
>>>>>>> 5d22892 (Update libretro.c)
}
