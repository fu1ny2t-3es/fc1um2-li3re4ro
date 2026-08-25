/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
 *  Copyright (C) 2005 CaH4e3
 *  Copyright (C) 2019 Libretro Team
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
#include "jyasic.h"

<<<<<<< HEAD
<<<<<<< HEAD
void 	(*sync)(void);
static uint8_t	allowExtendedMirroring;
=======
void 	(*JYASIC_sync)(void);
uint8	allowExtendedMirroring;
>>>>>>> 6e3a1fff (Update Makefile.libretro)

static uint8_t	mode[4];
static uint8_t*	WRAM = NULL;
static uint32_t	WRAMSIZE;

static uint8_t	irqControl;
static uint8_t	irqEnabled;
static uint8_t	irqPrescaler;
static uint8_t	irqCounter;
static uint8_t	irqXor;
static uint32_t	lastPPUAddress;

static uint8_t	prg[4];
static uint16_t	chr[8];
static uint16_t	nt[4];
static uint8_t	latch[2];
static uint8_t	mul[2];
static uint8_t	adder;
static uint8_t	test;
static uint8_t    dipSwitch;
static uint8_t	submapper;

<<<<<<< HEAD
static uint8_t cpuWriteHandlersSet;
static writefunc cpuWriteHandlers[0x10000]; /* Actual write handlers for CPU write trapping as a method fo IRQ clocking */
=======
uint8 cpuWriteHandlersSet;
writefunc cpuWriteHandlers[0x10000]; /* Actual write handlers for CPU write trapping as a method fo IRQ clocking */
>>>>>>> 6e3a1fff (Update Makefile.libretro)
=======
JYASIC jyasic = { 0 };

static uint8 dipSwitch;
static uint8 allow_extended_mirroring;
static uint32 lastPPUAddress;

uint8 JYASIC_CPUWriteHandlersSet;
writefunc JYASIC_cpuWrite[0x10000]; /* Actual write handlers for CPU write trapping as a method fo IRQ clocking */

<<<<<<< HEAD
static uint8	prg[4];
static uint16	chr[8];
static uint16	nt[4];
static uint8	latch[2];
static uint8	mul[2];
static uint8	adder;
static uint8	test;
static uint8    dipSwitch;
static uint8	submapper;
=======
void (*JYASIC_pwrap)(uint16 A, uint16 V);
void (*JYASIC_wwrap)(uint16 A, uint16 V);
void (*JYASIC_cwrap)(uint16 A, uint16 V);
>>>>>>> ff5b809 (Update Makefile.libretro)

uint32 (*JYASIC_GetPRGBank)(uint32 V);
uint32 (*JYASIC_GetCHRBank)(uint32 V);
>>>>>>> ae14339e (Update Makefile.libretro)

static SFORMAT JYASIC_StateRegs[] = {
	{ jyasic.mode,           4,  "TKCO" },
	{ jyasic.prg,            4,  "PRGB" },
	{ jyasic.mul,            2,  "MUL"  },
	{ jyasic.latch,          2,  "CLTC" },
	{ jyasic.chr,            16, "CHRB" },
	{ jyasic.nt,             8,  "NMS0" },
	{ &jyasic.adder,         1,  "ADDE" },
	{ &jyasic.test,          1,  "REGI" },
	{ &jyasic.irq.control,   1,  "IRQM" },
	{ &jyasic.irq.prescaler, 1,  "IRQP" },
	{ &jyasic.irq.counter,   1,  "IRQC" },
	{ &jyasic.irq.xor,       1,  "IRQX" },
	{ &jyasic.irq.enable,    1,  "IRQA" },
	{ 0 }
};

<<<<<<< HEAD
static uint8_t rev (uint8_t val)
{
	return ((val <<6) &0x40) | ((val <<4) &0x20) | ((val <<2) &0x10) | (val &0x08) | ((val >>2) &0x04) | ((val >>4) &0x02) | ((val >>6) &0x01);
=======
static uint8 rev(uint8 val) {
	return (
		((val << 6) & 0x40) |
		((val << 4) & 0x20) |
		((val << 2) & 0x10) |
		((val << 0) & 0x08) |
		((val >> 2) & 0x04) |
		((val >> 4) & 0x02) |
		((val >> 6) & 0x01));
>>>>>>> ae14339e (Update Makefile.libretro)
}

static uint32 GENPRGBANK(uint32 V) { return 0; }
static uint32 GENCHRBANK(uint32 V) { return 0; }

static void GENPWRAP(uint16 A, uint16 V) {
	uint32 bank = JYASIC_GetPRGBank(V);
	setprg8(A, bank);
}

static void GENCWRAP(uint16 A, uint16 V) {
	uint32 bank = JYASIC_GetCHRBank(V);
	setchr1(A, bank);
}

static void GENWWRAP(uint16 A, uint16 V) {
	uint32 bank = JYASIC_GetPRGBank(V);
	setprg8(A, bank);
}

<<<<<<< HEAD
static void clockIRQ (void)
{
	uint8_t mask =irqControl &0x04? 0x07: 0xFF;
	if (irqEnabled)
      switch (irqControl &0xC0)
      {
         case 0x40: {
            uint8_t newp = (irqPrescaler + 1) & mask;
            irqPrescaler = (irqPrescaler & ~mask) | newp;
            if (newp == 0x00 && (irqControl &0x08? irqCounter: ++irqCounter) ==0x00)
               X6502_IRQBegin(FCEU_IQEXT);
            break;
         }
         case 0x80: {
            uint8_t newp = (irqPrescaler - 1) & mask;
            irqPrescaler = (irqPrescaler & ~mask) | newp;
            if (newp == mask && (irqControl &0x08? irqCounter: --irqCounter) ==0xFF)
               X6502_IRQBegin(FCEU_IQEXT);
            break;
         }
      }
}

void trapCPUWrite(uint32 A, uint8 V)
{
	if ((irqControl &0x03) ==0x03)
      clockIRQ(); /* Clock IRQ counter on CPU writes */
	cpuWriteHandlers[A](A, V);
}

<<<<<<< HEAD
static void FP_FASTAPASS(1) trapPPUAddressChange (uint32_t A)
=======
static void trapPPUAddressChange (uint32 A)
>>>>>>> f6efdc94 (Update Makefile.libretro)
{
   if ((irqControl &0x03) ==0x02 && lastPPUAddress !=A)
   {
      int i;
      for (i =0; i <2; i++)
         clockIRQ(); /* Clock IRQ counter on PPU "reads" */
   }
   if (mode[3] &0x80 && (mode[0] &0x18) ==0x08 && ((A &0x2FF0) ==0xFD0 || (A &0x2FF0) ==0xFE0))
   {
      /* If MMC4 mode[0] is enabled, and CHR mode[0] is 4 KiB, and tile FD or FE is being fetched ... */
      latch[A >>12 &1] =(A >>10 &4) | (A >>4 &2); /* ... switch the left or right pattern table's latch to 0 (FD) or 2 (FE), being used as an offset for the CHR register index. */
      JYASIC_sync();
   }
   lastPPUAddress =A;
}

static void ppuScanline(void)
{
	if ((irqControl &0x03) ==0x01)
   {
      int i;
      for (i =0; i <8; i++)
         clockIRQ(); /* Clock IRQ counter on A12 rises (eight per scanline). This should be done in trapPPUAddressChange, but would require more accurate PPU emulation for that. */
   }
}

static void cpuCycle(int a)
{
   if ((irqControl &0x03) ==0x00)
      while (a--)
         clockIRQ(); /* Clock IRQ counter on M2 cycles */
}

uint8 readALU_DIP(uint32 A)
{
   if ((A &0x3FF) ==0 && A !=0x5800) /* 5000, 5400, 5C00: read solder pad setting */
      return dipSwitch | cpu.openbus &0x3F;

   if (A &0x800)
      switch (A &3)
      {
         /* 5800-5FFF: read ALU */
         case 0:
            return (mul[0] *mul[1]) &0xFF;
         case 1:
            return (mul[0] *mul[1]) >>8;
         case 2:
            return adder;
         case 3:
            return test;
      }
   /* all others */
   return cpu.openbus;
}

void writeALU(uint32 A, uint8 V)
{
	switch (A &3)
   {
      case 0:
         mul[0] =V;
         break;
      case 1:
         mul[1] =V;
         break;
      case 2:
         adder +=V;
         break;
      case 3:
         test  = V;
         adder = 0;
         break;
   }
}

void writePRG(uint32 A, uint8 V)
{
	prg[A &3] = V;
	JYASIC_sync();
}

void writeCHRLow(uint32 A, uint8 V)
{
	chr[A &7] =chr[A &7] &0xFF00 | V;
	JYASIC_sync();
}

void writeCHRHigh(uint32 A, uint8 V)
{
	chr[A &7] =chr[A &7] &0x00FF | V <<8;
	JYASIC_sync();
}

void writeNT(uint32 A, uint8 V)
{
	if (~A &4)
		nt[A &3] =nt[A &3] &0xFF00 | V;
	else
		nt[A &3] =nt[A &3] &0x00FF | V <<8;
	JYASIC_sync();
}

void writeIRQ(uint32 A, uint8 V)
{
	switch (A &7)
   {
      case 0:
         irqEnabled =!!(V &1);
         if (!irqEnabled)
         {
            irqPrescaler =0;
            X6502_IRQEnd(FCEU_IQEXT);
         }
         break;
      case 1:
         irqControl =V;
         break;
      case 2:
         irqEnabled =0;
         irqPrescaler =0;
         X6502_IRQEnd(FCEU_IQEXT);
         break;
      case 3:
         irqEnabled =1;
         break;
      case 4:
         irqPrescaler =V ^irqXor;
         break;
      case 5:
         irqCounter =V ^irqXor;
         break;
      case 6:
         irqXor =V;
         break;
   }
}

void writeMode(uint32 A, uint8 V)
{
	switch (A &3)
   {
      case 0:
         mode[0] =V;
         if (!allowExtendedMirroring)
            mode[0] &=~0x20;
         break;
      case 1:
         mode[1] =V;
         if (!allowExtendedMirroring)
            mode[1] &=~0x08;
         break;
      case 2:
         mode[2] =V;
         break;
      case 3:
         mode[3] =V;
         break;
   }
   JYASIC_sync();
}

void JYASIC_restoreWriteHandlers(void)
{
   int i;
   if (cpuWriteHandlersSet) 
   {
	   for (i =0; i <0x10000; i++) SetWriteHandler(i, i, cpuWriteHandlers[i]);
	   cpuWriteHandlersSet =0;
   }
}

void JYASIC_power(void)
{
   unsigned int i;

   SetWriteHandler(0x5000, 0x5FFF, writeALU);
   SetWriteHandler(0x6000, 0x7fff, CartBW);
   SetWriteHandler(0x8000, 0x87FF, writePRG);     /* 8800-8FFF ignored */
   SetWriteHandler(0x9000, 0x97FF, writeCHRLow);  /* 9800-9FFF ignored */
   SetWriteHandler(0xA000, 0xA7FF, writeCHRHigh); /* A800-AFFF ignored */
   SetWriteHandler(0xB000, 0xB7FF, writeNT);      /* B800-BFFF ignored */
   SetWriteHandler(0xC000, 0xCFFF, writeIRQ);
   SetWriteHandler(0xD000, 0xD7FF, writeMode);    /* D800-DFFF ignored */

   JYASIC_restoreWriteHandlers();
   for (i =0; i <0x10000; i++) cpuWriteHandlers[i] =GetWriteHandler(i);
   SetWriteHandler(0x0000, 0xFFFF, trapCPUWrite); /* Trap all CPU writes for IRQ clocking purposes */
   cpuWriteHandlersSet =1;

   SetReadHandler(0x5000, 0x5FFF, readALU_DIP);
   SetReadHandler(0x6000, 0xFFFF, CartBR);

   mul[0]     = mul[1]    = adder   = test    = dipSwitch = 0;
   mode[0]    = mode[1]   = mode[2] = mode[3] = 0;
   irqControl =irqEnabled = irqPrescaler =irqCounter = irqXor = lastPPUAddress = 0;
   memset(prg, 0, sizeof(prg));
   memset(chr, 0, sizeof(chr));
   memset(nt, 0, sizeof(nt));
   latch[0] =0;
   latch[1] =4;

   JYASIC_sync();
}

static void JYASIC_reset (void)
{
	dipSwitch = (dipSwitch +0x40) &0xC0;
}

static void JYASIC_close (void)
{
	if (WRAM)
      FCEU_gfree(WRAM);
   WRAM = NULL;
}

static void JYASIC_restore (int version)
{
   JYASIC_sync();
}

<<<<<<< HEAD
static void JYASIC_init (CartInfo *info)
=======
void JYASIC_init(CartInfo *info)
>>>>>>> 6e3a1fff (Update Makefile.libretro)
{
   cpuWriteHandlersSet =0;
   info->Reset = JYASIC_reset;
   info->Power = JYASIC_power;
   info->Close = JYASIC_close;
   PPU_hook = trapPPUAddressChange;
   MapIRQHook = cpuCycle;
   GameHBIRQHook2 = ppuScanline;
   AddExState(JYASIC_stateRegs, ~0, 0, 0);
   GameStateRestore = JYASIC_restore;

   /* WRAM is present only in iNES mapper 35, or in mappers with numbers above 255 that require NES 2.0, which explicitly denotes WRAM size */
   if (info->iNES2)
      WRAMSIZE =info->PRGRamSize + info->PRGRamSaveSize;
   else
      WRAMSIZE =info->mapper ==35? 8192: 0;

   if (WRAMSIZE)
   {
      WRAM = (uint8_t*)FCEU_gmalloc(WRAMSIZE);
      SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
      FCEU_CheatAddRAM((WRAMSIZE >> 10) < 8 ? (WRAMSIZE >> 10) : 8, 0x6000, WRAM);
   }
}

static void syncSingleCart (void)
{
   syncPRG(0x3F, mode[3] <<5 &~0x3F);
   if (mode[3] &0x20)
   {
      syncCHR(0x1FF,                        mode[3] <<6 &0x600);
      syncNT (0x1FF,                        mode[3] <<6 &0x600);
   }
   else
   {
      syncCHR(0x0FF, mode[3] <<8 &0x100 | mode[3] <<6 &0x600);
      syncNT (0x0FF, mode[3] <<8 &0x100 | mode[3] <<6 &0x600);
   }
}
void Mapper35_Init(CartInfo *info)
{
   /* Basically mapper 90/209/211 with WRAM */
	allowExtendedMirroring =1;
	JYASIC_sync = syncSingleCart;
	JYASIC_init(info);
}
void Mapper90_Init(CartInfo *info)
{
   /* Single cart, extended mirroring and ROM nametables disabled */
	allowExtendedMirroring =0;
	JYASIC_sync = syncSingleCart;
	JYASIC_init(info);
}

void Mapper209_Init(CartInfo *info)
{
   /* Single cart, extended mirroring and ROM nametables enabled */
	allowExtendedMirroring =1;
	JYASIC_sync = syncSingleCart;
	JYASIC_init(info);
}

void Mapper211_Init(CartInfo *info)
{
   /* Duplicate of mapper 209 */
	allowExtendedMirroring =1;
	JYASIC_sync = syncSingleCart;
	JYASIC_init(info);
}

static void sync281 (void)
{
   syncPRG(0x1F, mode[3] <<5);
   syncCHR(0xFF, mode[3] <<8);
   syncNT (0xFF, mode[3] <<8);
}

void Mapper281_Init(CartInfo *info)
{
   /* Multicart */
	allowExtendedMirroring =1;
	JYASIC_sync = sync281;
	JYASIC_init(info);
}

static void sync282 (void)
{
	syncPRG(0x1F, mode[3] <<4 &~0x1F);
	if (mode[3] &0x20)
   {
		syncCHR(0x1FF,                            mode[3] <<6 &0x600);
		syncNT (0x1FF,                            mode[3] <<6 &0x600);
=======
void JYASIC_FixPRG(void) {
	uint8 prgLast = (jyasic.mode[0] & 0x04) ? jyasic.prg[3] : 0xFF;
	uint8 prg6000 = 0;

	switch (jyasic.mode[0] & 0x03) {
	case 0:
		JYASIC_pwrap(0x8000, (prgLast << 2) | 0);
		JYASIC_pwrap(0xA000, (prgLast << 2) | 1);
		JYASIC_pwrap(0xC000, (prgLast << 2) | 2);
		JYASIC_pwrap(0xE000, (prgLast << 2) | 3);
		prg6000 =      (jyasic.prg[3] << 2) | 3;
		break;
	case 1:
		JYASIC_pwrap(0x8000, (jyasic.prg[1] << 1) | 0);
		JYASIC_pwrap(0xA000, (jyasic.prg[1] << 1) | 1);
		JYASIC_pwrap(0xC000,       (prgLast << 1) | 0);
		JYASIC_pwrap(0xE000,       (prgLast << 1) | 1);
		prg6000 =            (jyasic.prg[3] << 1) | 1;
		break;
	case 2:
		JYASIC_pwrap(0x8000, jyasic.prg[0]);
		JYASIC_pwrap(0xA000, jyasic.prg[1]);
		JYASIC_pwrap(0xC000, jyasic.prg[2]);
		JYASIC_pwrap(0xE000, prgLast);
		prg6000 = jyasic.prg[3];
		break;
	case 3:
		JYASIC_pwrap(0x8000, rev(jyasic.prg[0]));
		JYASIC_pwrap(0xA000, rev(jyasic.prg[1]));
		JYASIC_pwrap(0xC000, rev(jyasic.prg[2]));
		JYASIC_pwrap(0xE000, rev(prgLast));
		prg6000 = rev(jyasic.prg[3]);
		break;
>>>>>>> ae14339e (Update Makefile.libretro)
	}
	if (jyasic.mode[0] & 0x80) { /* Map ROM */
		JYASIC_wwrap(0x6000, prg6000);
	} else if (WRAMSIZE) { /* Otherwise map WRAM if it exists */
		setprg8r(0x10, 0x6000, 0);
	}
}

<<<<<<< HEAD
void Mapper282_Init(CartInfo *info)
{
   /* Multicart */
	allowExtendedMirroring =1;
	JYASIC_sync = sync282;
	JYASIC_init(info);
}

static void sync295 (void)
{
   syncPRG(0x0F, mode[3] <<4);
   syncCHR(0x7F, mode[3] <<7);
   syncNT (0x7F, mode[3] <<7);
}

void Mapper295_Init(CartInfo *info)
{
   /* Multicart */
	allowExtendedMirroring =1;
	JYASIC_sync = sync295;
	JYASIC_init(info);
}

static void sync358 (void)
{
   syncPRG(0x1F, mode[3] <<4 &~0x1F);
   if (mode[3] &0x20)
   {
      syncCHR(0x1FF, mode[3] <<7 &0x600);
      syncNT (0x1FF, mode[3] <<7 &0x600);
   }
   else
   {
      syncCHR(0x0FF, mode[3] <<8 &0x100 | mode[3] <<7 &0x600);
      syncNT (0x0FF, mode[3] <<8 &0x100 | mode[3] <<7 &0x600);
   }
}

void Mapper358_Init(CartInfo *info)
{
   /* Multicart */
	allowExtendedMirroring =1;
	JYASIC_sync = sync358;
	JYASIC_init(info);
}

static void sync386 (void)
{
   syncPRG(0x1F, mode[3] <<4 &0x20 | mode[3] <<3 &0x40);
   if (mode[3] &0x20)
   {
      syncCHR(0x1FF, mode[3] <<7 &0x600);
      syncNT (0x1FF, mode[3] <<7 &0x600);
   }
   else
   {
      syncCHR(0x0FF, mode[3] <<8 &0x100 | mode[3] <<7 &0x600);
      syncNT (0x0FF, mode[3] <<8 &0x100 | mode[3] <<7 &0x600);
   }
}

void Mapper386_Init(CartInfo *info)
{
   /* Multicart */
	allowExtendedMirroring =1;
	JYASIC_sync = sync386;
	JYASIC_init(info);
}
	
static void sync387(void)
{
	syncPRG(0x0F, mode[3] <<3 &0x10 | mode[3] <<2 &0x20);
	if (mode[3] &0x20)
   {
		syncCHR(0x1FF, mode[3] <<7 &0x600);
		syncNT (0x1FF, mode[3] <<7 &0x600);
=======
void JYASIC_FixCHR(void) {
	/* MMC4 jyasic.mode[0] with 4 KiB CHR jyasic.mode[0] */
	if (jyasic.mode[3] & 0x80 && (jyasic.mode[0] & 0x18) == 0x08) {
		JYASIC_cwrap(0x0000, (jyasic.chr[(jyasic.latch[0] & 2) | 0] << 2) | 0);
		JYASIC_cwrap(0x0400, (jyasic.chr[(jyasic.latch[0] & 2) | 0] << 2) | 1);
		JYASIC_cwrap(0x0800, (jyasic.chr[(jyasic.latch[0] & 2) | 0] << 2) | 2);
		JYASIC_cwrap(0x0C00, (jyasic.chr[(jyasic.latch[0] & 2) | 0] << 2) | 3);
		JYASIC_cwrap(0x1000, (jyasic.chr[(jyasic.latch[1] & 2) | 4] << 2) | 0);
		JYASIC_cwrap(0x1400, (jyasic.chr[(jyasic.latch[1] & 2) | 4] << 2) | 1);
		JYASIC_cwrap(0x1800, (jyasic.chr[(jyasic.latch[1] & 2) | 4] << 2) | 2);
		JYASIC_cwrap(0x1C00, (jyasic.chr[(jyasic.latch[1] & 2) | 4] << 2) | 3);
	} else {
		switch (jyasic.mode[0] & 0x18) {
		case 0x00: /* 8 KiB CHR mode */
			JYASIC_cwrap(0x0000, (jyasic.chr[0] << 3) | 0);
			JYASIC_cwrap(0x0400, (jyasic.chr[0] << 3) | 1);
			JYASIC_cwrap(0x0800, (jyasic.chr[0] << 3) | 2);
			JYASIC_cwrap(0x0C00, (jyasic.chr[0] << 3) | 3);
			JYASIC_cwrap(0x1000, (jyasic.chr[0] << 3) | 4);
			JYASIC_cwrap(0x1400, (jyasic.chr[0] << 3) | 5);
			JYASIC_cwrap(0x1800, (jyasic.chr[0] << 3) | 6);
			JYASIC_cwrap(0x1C00, (jyasic.chr[0] << 3) | 7);
			break;
		case 0x08: /* 4 KiB CHR mode */
			JYASIC_cwrap(0x0000, (jyasic.chr[0] << 2) | 0);
			JYASIC_cwrap(0x0400, (jyasic.chr[0] << 2) | 1);
			JYASIC_cwrap(0x0800, (jyasic.chr[0] << 2) | 2);
			JYASIC_cwrap(0x0C00, (jyasic.chr[0] << 2) | 3);
			JYASIC_cwrap(0x1000, (jyasic.chr[4] << 2) | 0);
			JYASIC_cwrap(0x1400, (jyasic.chr[4] << 2) | 1);
			JYASIC_cwrap(0x1800, (jyasic.chr[4] << 2) | 2);
			JYASIC_cwrap(0x1C00, (jyasic.chr[4] << 2) | 3);
			break;
		case 0x10: /* 2 KiB CHR mode */
			JYASIC_cwrap(0x0000, (jyasic.chr[0] << 1) | 0);
			JYASIC_cwrap(0x0400, (jyasic.chr[0] << 1) | 1);
			JYASIC_cwrap(0x0800, (jyasic.chr[2] << 1) | 0);
			JYASIC_cwrap(0x0C00, (jyasic.chr[2] << 1) | 1);
			JYASIC_cwrap(0x1000, (jyasic.chr[4] << 1) | 0);
			JYASIC_cwrap(0x1400, (jyasic.chr[4] << 1) | 1);
			JYASIC_cwrap(0x1800, (jyasic.chr[6] << 1) | 0);
			JYASIC_cwrap(0x1C00, (jyasic.chr[6] << 1) | 1);
			break;
		case 0x18: /* 1 KiB CHR mode */
			JYASIC_cwrap(0x0000, jyasic.chr[0]);
			JYASIC_cwrap(0x0400, jyasic.chr[1]);
			JYASIC_cwrap(0x0800, jyasic.chr[2]);
			JYASIC_cwrap(0x0C00, jyasic.chr[3]);
			JYASIC_cwrap(0x1000, jyasic.chr[4]);
			JYASIC_cwrap(0x1400, jyasic.chr[5]);
			JYASIC_cwrap(0x1800, jyasic.chr[6]);
			JYASIC_cwrap(0x1C00, jyasic.chr[7]);
			break;
		}
>>>>>>> ae14339e (Update Makefile.libretro)
	}

	PPUCHRRAM = (jyasic.mode[2] & 0x40) ? 0xFF : 0x00; /* Write-protect or write-enable CHR-RAM */
}

void JYASIC_FixMIR(void) {
	if (jyasic.mode[0] & 0x20 || jyasic.mode[1] & 0x08) {
		/* ROM nametables or extended mirroring */
		/* First, set normal CIRAM pages using extended registers ... */
		setmirrorw(jyasic.nt[0] & 1, jyasic.nt[1] & 1, jyasic.nt[2] & 1, jyasic.nt[3] & 1);

		if (jyasic.mode[0] & 0x20) {
			int i;
			for (i = 0; i < 4; i++) {
				/* Then replace with ROM nametables if such are generally enabled */
				/* ROM nametables are used either when globally enabled via
				 * D000.6 or per-bank via B00x.7 vs. D002.7 */
				if (((jyasic.nt[i] & 0x80) ^ (jyasic.mode[2] & 0x80)) | (jyasic.mode[0] & 0x40)) {
					setntamem(CHRptr[0] + 0x400 * (JYASIC_GetCHRBank(jyasic.nt[i]) & CHRmask1[0]), 0, i);
				}
			}
		}
	} else {
		switch (jyasic.mode[1] & 0x03) {
		/* Regularly mirrored CIRAM */
		case 0: setmirror(MI_V); break;
		case 1: setmirror(MI_H); break;
		case 2: setmirror(MI_0); break;
		case 3: setmirror(MI_1); break;
		}
	}
}

static void clockIRQ(void) {
	uint8 mask            = jyasic.irq.control & 0x04 ? 0x07 : 0xFF;
	uint8 prescaler       = jyasic.irq.prescaler & mask;
	uint8 clockIrqCounter = FALSE;

	if (jyasic.irq.enable) {
		switch (jyasic.irq.control & 0xC0) {
		case 0x40:
			prescaler++;
			if ((prescaler & mask) == 0) {
				clockIrqCounter = TRUE;
			}
			break;
		case 0x80:
			if (--prescaler == 0) {
				clockIrqCounter = TRUE;
			}
			break;
		}

		jyasic.irq.prescaler = (jyasic.irq.prescaler & ~mask) | (prescaler & mask);

		if (clockIrqCounter) {
			switch (jyasic.irq.control & 0xC0) {
			case 0x40:
				if ((jyasic.irq.control & 0x08) == 0) {
					jyasic.irq.counter++;
				}
				if (jyasic.irq.counter == 0x00) {
					X6502_IRQBegin(FCEU_IQEXT);
				}
				break;
			case 0x80:
				if ((jyasic.irq.control & 0x08) == 0) {
					jyasic.irq.counter--;
				}
				if (jyasic.irq.counter == 0xFF) {
					X6502_IRQBegin(FCEU_IQEXT);
				}
				break;
			}
		}
	}
}

<<<<<<< HEAD
static void sync388 (void)
{
   syncPRG(0x1F, mode[3] <<3 &0x60);

   if (mode[3] &0x20)
   {
      syncCHR(0x1FF, mode[3] <<8 &0x200);
      syncNT (0x1FF, mode[3] <<8 &0x200);
   }
   else
   {
      syncCHR(0x0FF, mode[3] <<8 &0x100 | mode[3] <<8 &0x200);
      syncNT (0x0FF, mode[3] <<8 &0x100 | mode[3] <<8 &0x200);
   }
=======
DECLFW(JYASIC_trapCPUWrite) {
	if ((jyasic.irq.control & 0x03) == 0x03) {
		clockIRQ(); /* Clock IRQ counter on CPU writes */
	}
	JYASIC_cpuWrite[A](A, V);
>>>>>>> ae14339e (Update Makefile.libretro)
}

static void trapPPUAddressChange(uint32 A) {
	if (((jyasic.irq.control & 0x03) == 0x02) && (lastPPUAddress != A)) {
		int i;
		for (i = 0; i < 2; i++) {
			clockIRQ(); /* Clock IRQ counter on PPU "reads" */
		}
	}
	if ((jyasic.mode[3] & 0x80) && ((jyasic.mode[0] & 0x18) == 0x08) && (((A & 0x2FF0) == 0xFD0) || ((A & 0x2FF0) == 0xFE0))) {
		/* If MMC4 jyasic.mode[0] is enabled, and CHR jyasic.mode[0] is 4 KiB, and tile FD or FE is being fetched ... */
		jyasic.latch[(A >> 12) & 1] = ((A >> 10) & 4) | ((A >> 4) & 2); /* switch the left or right pattern table's latch to 0 (FD) or 2 (FE),
																  * being used as an offset for the CHR register index. */
		JYASIC_FixCHR();
	}
	lastPPUAddress = A;
}

<<<<<<< HEAD
static void sync397 (void)
{
	syncPRG(0x1F, mode[3] <<4 &~0x1F);
	syncCHR(0x7F, mode[3] <<7);
	syncNT (0x7F, mode[3] <<7);
=======
static void ppuScanline(void) {
	if ((jyasic.irq.control & 0x03) == 0x01) {
		int i;
		for (i = 0; i < 8; i++) {
			clockIRQ(); /* Clock IRQ counter on A12 rises (eight per scanline). This should be done in
						   trapPPUAddressChange, but would require more accurate PPU emulation for that. */
		}
	}
>>>>>>> ae14339e (Update Makefile.libretro)
}

static void cpuCycle(int a) {
	if ((jyasic.irq.control & 0x03) == 0x00) {
		while (a--) {
			clockIRQ(); /* Clock IRQ counter on M2 cycles */
		}
	}
}

<<<<<<< HEAD
static void sync421 (void)
{
   if (mode[3] &0x04)
      syncPRG(0x3F, mode[3] <<4 &~0x3F);
   else
      syncPRG(0x1F, mode[3] <<4 &~0x1F);
   syncCHR(0x1FF, mode[3] <<8 &0x300);
   syncNT (0x1FF, mode[3] <<8 &0x300);
=======
DECLFR(JYASIC_ReadALU_DIP) {
	if ((A & 0x3FF) == 0 &&
		A != 0x5800) { /* 5000, 5400, 5C00: read solder pad setting */
		return dipSwitch | (cpu.openbus & 0x3F);
	}

	if (A & 0x800) {
		switch (A & 3) {
		/* 5800-5FFF: read ALU */
		case 0: return (jyasic.mul[0] * jyasic.mul[1]) & 0xFF;
		case 1: return (jyasic.mul[0] * jyasic.mul[1]) >> 8;
		case 2: return jyasic.adder;
		case 3: return jyasic.test;
		}
	}
	/* all others */
	return cpu.openbus;
>>>>>>> ae14339e (Update Makefile.libretro)
}

DECLFW(JYASIC_WriteALU) {
	switch (A & 3) {
	case 0: jyasic.mul[0] = V; break;
	case 1: jyasic.mul[1] = V; break;
	case 2: jyasic.adder += V; break;
	case 3: jyasic.test   = V; jyasic.adder = 0; break;
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
/* Mapper 394: HSK007 circuit board that can simulate J.Y. ASIC, MMC3, and NROM. */
static uint8_t HSK007Reg[4];
static void sync394 (void) /* Called when J.Y. ASIC is active */
{
	int prgOR  =HSK007Reg[3] <<1 &0x010 | HSK007Reg[1] <<5 &0x060;
	int chrOR  =HSK007Reg[3] <<1 &0x080 | HSK007Reg[1] <<6 &0x100 | HSK007Reg[1] <<8 &0x200;
	syncPRG(0x1F, prgOR);
	syncCHR(0xFF, chrOR);
	syncNT (0xFF, chrOR);	
}
static void Mapper394_PWrap(uint32_t A, uint8_t V)
{
	int prgAND =HSK007Reg[3] &0x10? 0x1F: 0x0F;
	int prgOR  =HSK007Reg[3] <<1 &0x010 | HSK007Reg[1] <<5 &0x060;
	if (HSK007Reg[1] &0x08)
		setprg8(A, V &prgAND | prgOR &~prgAND);
	else
	if (A ==0x8000)
		setprg32(A, (prgOR | HSK007Reg[3] <<1 &0x0F) >>2);
	
}
static void Mapper394_CWrap(uint32_t A, uint8_t V)
{
	int chrAND =HSK007Reg[3] &0x80? 0xFF: 0x7F;
	int chrOR  =submapper ==1? (HSK007Reg[3] <<1 &0x080 | HSK007Reg[1] <<8 &0x200 | HSK007Reg[1] <<6 &0x100): (HSK007Reg[3] <<1 &0x080 | HSK007Reg[1] <<8 &0x300);	
	setchr1(A, V &chrAND | chrOR &~chrAND);
}
static void Mapper394_Write(uint32 A, uint8 V)
{
	uint8_t oldMode =HSK007Reg[1];
	A &=3;
	HSK007Reg[A] =V;
	if (A ==1)
	{		
		if (~oldMode &0x10 &&  V &0x10) JYASIC_power();
		if ( oldMode &0x10 && ~V &0x10)
		{
			JYASIC_restoreWriteHandlers();
			GenMMC3Power();
		}
	}
	else
	{
		if (HSK007Reg[1] &0x10)
			sync();
		else
		{
			FixMMC3PRG(MMC3_cmd);
			FixMMC3CHR(MMC3_cmd);
		}
			
	}
}
static void Mapper394_restore (int version)
{
	int i;
	JYASIC_restoreWriteHandlers();
	if (HSK007Reg[1] &0x10)
	{		
		SetWriteHandler(0x5000, 0x5FFF, writeALU);
		SetWriteHandler(0x6000, 0x7fff, CartBW);
		SetWriteHandler(0x8000, 0x87FF, writePRG);     /* 8800-8FFF ignored */
		SetWriteHandler(0x9000, 0x97FF, writeCHRLow);  /* 9800-9FFF ignored */
		SetWriteHandler(0xA000, 0xA7FF, writeCHRHigh); /* A800-AFFF ignored */
		SetWriteHandler(0xB000, 0xB7FF, writeNT);      /* B800-BFFF ignored */
		SetWriteHandler(0xC000, 0xCFFF, writeIRQ);
		SetWriteHandler(0xD000, 0xD7FF, writeMode);    /* D800-DFFF ignored */
		
		for (i =0; i <0x10000; i++) cpuWriteHandlers[i] =GetWriteHandler(i);
		SetWriteHandler(0x0000, 0xFFFF, trapCPUWrite); /* Trap all CPU writes for IRQ clocking purposes */
		cpuWriteHandlersSet =1;
		
		SetReadHandler(0x5000, 0x5FFF, readALU_DIP);
		SetReadHandler(0x6000, 0xFFFF, CartBR);
		sync();
	}
	else
	{
		SetWriteHandler(0x8000, 0xBFFF, MMC3_CMDWrite);
		SetWriteHandler(0xC000, 0xFFFF, MMC3_IRQWrite);
		SetReadHandler(0x8000, 0xFFFF, CartBR);
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
	}
}
static void Mapper394_power(void)
{
	HSK007Reg[0] =0x00;
	HSK007Reg[1] =0x0F;
	HSK007Reg[2] =0x00;
	HSK007Reg[3] =0x90;
	irqEnabled =0;
	GenMMC3Power();
	SetWriteHandler(0x5000, 0x5FFF, Mapper394_Write);
}

void Mapper394_Init(CartInfo *info)
{
	submapper =info->submapper;
	allowExtendedMirroring =1;
	sync =sync394;
	JYASIC_init(info);
	GenMMC3_Init(info, 128, 128, 0, 0);
	pwrap =Mapper394_PWrap;
	cwrap =Mapper394_CWrap;
	info->Reset = Mapper394_power;
	info->Power = Mapper394_power;
	AddExState(HSK007Reg, 4, 0, "HSK ");
	GameStateRestore = Mapper394_restore;
}
=======
>>>>>>> d091b8d (Split up more mappers)
=======
DECLFW(JYASIC_WritePRG) {
	jyasic.prg[A & 3] = V;
	JYASIC_FixPRG();
}

DECLFW(JYASIC_WriteCHRLow) {
	jyasic.chr[A & 7] = (jyasic.chr[A & 7] & 0xFF00) | V;
	JYASIC_FixCHR();
}

DECLFW(JYASIC_WriteCHRHigh) {
	jyasic.chr[A & 7] = (jyasic.chr[A & 7] & 0x00FF) | V << 8;
	JYASIC_FixCHR();
}

DECLFW(JYASIC_WriteNT) {
	if (~A & 4) {
		jyasic.nt[A & 3] = (jyasic.nt[A & 3] & 0xFF00) | V;
	} else {
		jyasic.nt[A & 3] = (jyasic.nt[A & 3] & 0x00FF) | V << 8;
	}
	JYASIC_FixMIR();
}

DECLFW(JYASIC_WriteIRQ) {
	switch (A & 7) {
	case 0:
		jyasic.irq.enable = !!(V & 1);
		if (!jyasic.irq.enable) {
			jyasic.irq.prescaler = 0;
			X6502_IRQEnd(FCEU_IQEXT);
		}
		break;
	case 1:
		jyasic.irq.control = V;
		break;
	case 2:
		jyasic.irq.enable   = 0;
		jyasic.irq.prescaler = 0;
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	case 3:
		jyasic.irq.enable = 1;
		break;
	case 4:
		jyasic.irq.prescaler = V ^ jyasic.irq.xor;
		break;
	case 5:
		jyasic.irq.counter = V ^ jyasic.irq.xor;
		break;
	case 6:
		jyasic.irq.xor = V;
		break;
	}
}

DECLFW(JYASIC_WriteMode) {
	switch (A & 3) {
	case 0:
		jyasic.mode[0] = V;
		if (!allow_extended_mirroring) {
			jyasic.mode[0] &= ~0x20;
		}
		break;
	case 1:
		jyasic.mode[1] = V;
		if (!allow_extended_mirroring) {
			jyasic.mode[1] &= ~0x08;
		}
		break;
	case 2:
		jyasic.mode[2] = V;
		break;
	case 3:
		jyasic.mode[3] = V;
		break;
	}
	JYASIC_FixPRG();
	JYASIC_FixCHR();
	JYASIC_FixMIR();
}

void JYASIC_restoreWriteHandlers(void) {
	int i;
	if (JYASIC_CPUWriteHandlersSet) {
		for (i = 0; i < 0x10000; i++) {
			SetWriteHandler(i, i, JYASIC_cpuWrite[i]);
		}
		JYASIC_CPUWriteHandlersSet = 0;
	}
}

void JYASIC_RegReset(void) {
	memset(jyasic.mode, 0, sizeof(jyasic.mode));
	memset(jyasic.prg, 0, sizeof(jyasic.prg));
	memset(jyasic.chr, 0, sizeof(jyasic.chr));
	memset(jyasic.nt, 0, sizeof(jyasic.nt));
	memset(jyasic.mul, 0, sizeof(jyasic.mul));

	jyasic.adder = jyasic.test = dipSwitch = 0;
	jyasic.irq.control = jyasic.irq.enable = 0;
	jyasic.irq.prescaler = jyasic.irq.counter = 0;
	jyasic.irq.xor = lastPPUAddress = 0;

	jyasic.latch[0] = 0;
	jyasic.latch[1] = 4;

	JYASIC_FixPRG();
	JYASIC_FixCHR();
	JYASIC_FixMIR();
}

void JYASIC_Power(void) {
	int i;

	SetWriteHandler(0x5000, 0x5FFF, JYASIC_WriteALU);
	SetWriteHandler(0x6000, 0x7fff, CartBW);
	SetWriteHandler(0x8000, 0x87FF, JYASIC_WritePRG); /* 8800-8FFF ignored */
	SetWriteHandler(0x9000, 0x97FF, JYASIC_WriteCHRLow); /* 9800-9FFF ignored */
	SetWriteHandler(0xA000, 0xA7FF, JYASIC_WriteCHRHigh); /* A800-AFFF ignored */
	SetWriteHandler(0xB000, 0xB7FF, JYASIC_WriteNT); /* B800-BFFF ignored */
	SetWriteHandler(0xC000, 0xCFFF, JYASIC_WriteIRQ);
	SetWriteHandler(0xD000, 0xD7FF, JYASIC_WriteMode); /* D800-DFFF ignored */

	JYASIC_restoreWriteHandlers();
	for (i = 0; i < 0x10000; i++) {
		JYASIC_cpuWrite[i] = GetWriteHandler(i);
	}
	SetWriteHandler(0x0000, 0xFFFF, JYASIC_trapCPUWrite); /* Trap all CPU writes for IRQ clocking purposes */
	JYASIC_CPUWriteHandlersSet = 1;

	SetReadHandler(0x5000, 0x5FFF, JYASIC_ReadALU_DIP);
	SetReadHandler(0x6000, 0xFFFF, CartBR);

	JYASIC_RegReset();
}

void JYASIC_Reset(void) {
	dipSwitch = (dipSwitch + 0x40) & 0xC0;
	JYASIC_FixPRG();
	JYASIC_FixCHR();
	JYASIC_FixMIR();
}

void JYASIC_Close(void) {
}

static void StateRestore(int version) {
	JYASIC_FixPRG();
	JYASIC_FixCHR();
	JYASIC_FixMIR();
}

void JYASIC_Init(CartInfo *info, int extended_mirr) {
	JYASIC_pwrap = GENPWRAP;
	JYASIC_wwrap = GENWWRAP;
	JYASIC_cwrap = GENCWRAP;

	JYASIC_GetPRGBank = GENPRGBANK;
	JYASIC_GetCHRBank = GENCHRBANK;

	allow_extended_mirroring = extended_mirr;

	JYASIC_CPUWriteHandlersSet = 0;
	info->Reset = JYASIC_Reset;
	info->Power = JYASIC_Power;
	info->Close = JYASIC_Close;

	PPU_hook = trapPPUAddressChange;
	MapIRQHook = cpuCycle;
	GameHBIRQHook2 = ppuScanline;

	AddExState(JYASIC_StateRegs, ~0, 0, 0);
	GameStateRestore = StateRestore;

	/* WRAM is present only in iNES mapper 35, or in mappers with numbers above 255 that require NES 2.0, which
	 * explicitly denotes WRAM size */
	if (info->iNES2) {
		WRAMSIZE = info->PRGRamSize + info->PRGRamSaveSize;
	} else {
		WRAMSIZE = info->mapper == 35 ? 8192 : 0;
	}

	if (WRAMSIZE) {
		WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
		SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
		FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
		AddExState(WRAM, WRAMSIZE, 0, "WRAM");
	}
}
>>>>>>> ff5b809 (Update Makefile.libretro)
