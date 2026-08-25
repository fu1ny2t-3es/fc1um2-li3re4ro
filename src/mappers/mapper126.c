/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2020
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

<<<<<<< HEAD
/* Multicarts based around the ING003C and TEC9719 enhanced MMC3 ASICs.
   ING003C's scanline counter is inverted from MMC3, while TEC9719 is normal.
   Both ASICs natively only support 256 KiB of CHR ROM/RAM; cartridges with more than that re-purpose higher PRG address as CHR address lines. There are two connection variants of this re-purposing.   
   Mapper 422: TEC9719, CHR A18=PRG A20, CHR A19=PRG A21
   Mapper 126: TEC9719, CHR A18=PRG A21, CHR A19=PRG A20
   Mapper 534: ING003C, CHR A18=PRG A20, CHR A19=PRG A21
   
   Cartridges with more than one PRG ROM chip are sometimes not connected to form one continuous address space. Submappers are used to avoid having to repeat data to accomodate them.
   Submapper 0: Normal connection
   Submapper 1: PRG A21 (2 MiB bank) selects between two 1 MiB chips
   Submapper 2: Register bit 6001.2 (undocumented in data sheet) selects between two 1 MiB chips
   Submapper 3: 6000.2 substitutes PRG A14 and CHR A14 with 6000.5
   Submapper 4: LD822 PCB - CHR A20..A18 = PRG A20..A18
   
   Both ASICs invert the register bit that selects PRG A21 (6000.5), hence "EXPREGS[0] ^0x20".
*/
=======
/* Mapper 422: "Normal" version of the mapper. Represents UNIF boards BS-400R and BS-4040R.
 * Mapper 126: Power Joy version of the mapper, connecting CHR A18 and A19 in reverse order.
 * Mapper 534: Waixing version of the mapper, inverting the reload V of the MMC3 scanline counter.
 */
>>>>>>> 30faeea0 (Update Makefile.libretro)

#include "mapinc.h"
#include "mmc3.h"

<<<<<<< HEAD
static uint8_t reverseCHR_A18_A19;
static uint8_t invertC000;
static uint8_t SL0;
static uint8_t submapper;

static uint8_t getMMC3Bank(int bank) {
	if (~bank &1 && MMC3_cmd &0x40) bank ^=2;
	return bank &2? 0xFE | bank &1: DRegBuf[6 | bank &1];
}

static void wrapPRG(uint32_t A, uint8_t V) {
=======
static uint8 reg[4];
static uint8 dipsw;
static uint8 oldump = FALSE;

static SFORMAT StateRegs[] = {
	{ reg, 4, "REGS" },
	{ &dipsw, 1, "DPSW" },
	{ 0 }
};

<<<<<<< HEAD
static void wrapPRG(uint32 A, uint8 V) {
>>>>>>> ae14339e (Update Makefile.libretro)
	int prgAND = EXPREGS[0] &0x40? 0x0F: 0x1F; /* 128 KiB or 256 KiB inner PRG bank selection */
	int prgOR  =(EXPREGS[0] <<4 &0x70 | (EXPREGS[0] ^0x20) <<3 &0x180) &~prgAND; /* Outer PRG bank */
	if (submapper ==1) prgOR =prgOR &0x7F | prgOR >>1 &0x80;    /* Submapper 1 uses PRG A21 as a chip select between two 1 MiB chips */
	if (submapper ==2) prgOR =prgOR &0x7F | EXPREGS[1] <<5 &0x80;   /* Submapper 2 uses 6001.2 (not documented in datasheet) as a chip select between two 1 MiB chips */
	if (submapper ==3 && EXPREGS[0] &0x04) { /* Submapper 3 replace PRG A14 with PRG A21 */
		prgAND &=~0x02;
		prgOR |= EXPREGS[0] &0x20? 0x00: 0x02;
	}
	for (A =0; A <4; A++) {
		/* In UNROM-like mode (CT3=1, CT2=1, CT0=1), MMC3 sees A13=0 and A14=CPU A14 during reads, making register 6 apply from $8000-$BFFF, and the fixed bank from $C000-$FFFF.
		   In NROM-128, NROM-256, ANROM and UNROM modes (CT0=1), MMC3 sees A13=0 and A14=0, making register 6 apply from $8000-$FFFF. */
		V =getMMC3Bank(A &((EXPREGS[3] &0x0D) ==0x0D? 2: EXPREGS[3] &0x01? 0: 3));
		
		/* UNROM and ANROM modes mean that MMC3 register 6 selects 16 and 32 KiB rather than 8 KiB banks. */
		if (EXPREGS[3] &0x08) switch(EXPREGS[3] &0x03) {
			case 0: V =V &3 | V <<1 &~3; break; /* PRG A14 appears twice */
			case 1: V =A &3 | V <<1 &~1; break; /* 16 KiB mode, bit 0 OR'd with CPU A14 */
			case 2: V =V &3 | V <<2 &~3; break; /* PRG A13 and PRG A14 appear twice */
			case 3: V =A &3 | V <<2 &~3; break; /* 32 KiB mode */
		} else
		if (EXPREGS[3] &0x01) { /* regular NROM modes */
			V =A &1 | V &~1;
			if (EXPREGS[3] &0x02) V =A &2 | V &~2; /* NROM-256 */
		}
		setprg8(0x8000 +A*0x2000, V &prgAND | prgOR &~prgAND);		
=======
static void M126PW(uint16 A, uint16 V) {
	uint8 reg0 = reg[0] ^ (oldump ? 0 : 0x20);
	uint16 mask = (reg0 & 0x40) ? 0x0F : 0x1F;
	uint16 base = (((reg0 << 4) & 0x70) | ((reg0 << 3) & 0x180)) & ~mask;

	switch (iNESCart.submapper) {
	case 1: base |= ((base & 0x100) >> 1); break;
	case 2: base |= ((reg[1] & 0x02) << 5); break;
>>>>>>> ff5b809 (Update Makefile.libretro)
	}

	if (reg[3] & 0x08) {
		uint8 b = (A >> 13) & 0x03;
		V = MMC3_GetPRGBank(b & (((reg[3] & 0x0D) == 0x0D) ? 0x02 : ((reg[3] & 0x01) ? 0 : 0x03)));
		switch (reg[3] & 0x03) {
		case 0: V = ((V << 1) & ~0x03) | (V & 0x03); break;
		case 1: V = ((V << 1) & ~0x01) | (b & 0x03); break;
		case 2: V = ((V << 2) & ~0x03) | (V & 0x03); break;
		case 3: V = ((V << 2) & ~0x03) | (b & 0x03); break;
		}
	} else {
		switch (reg[3] & 0x03) {
		case 1:
		case 2:
			base = base | (mmc3.reg[6] & mask);
			V = (A >> 13) & 0x01;
			mask = 0x01;
			break;
		case 3:
			base = base | (mmc3.reg[6] & mask);
			V = (A >> 13) & 0x03;
			mask = 0x03;
			break;
		}
	}
	setprg8(A, (base & ~mask) | (V & mask));
}

<<<<<<< HEAD
static void wrapCHR(uint32_t A, uint8_t V) {
=======
<<<<<<< HEAD
static void wrapCHR(uint32 A, uint8 V) {
>>>>>>> ae14339e (Update Makefile.libretro)
	int chrAND = EXPREGS[0] &0x80? 0x7F: 0xFF; /* 128 KiB or 256 KiB innter CHR bank selection */
	int chrOR; /* outer CHR bank */
	if (reverseCHR_A18_A19) /* Mapper 126 swaps CHR A18 and A19 */
		chrOR =(EXPREGS[0] <<4 &0x080 | (EXPREGS[0] ^0x20) <<3 &0x100 | EXPREGS[0] <<5 &0x200) &~chrAND;
	else
	if (submapper == 4) /* LD822 PCB - CHR A20..A18 = PRG A20..A18 */
		chrOR =(EXPREGS[0] <<4 &0x080 | EXPREGS[0] <<7 &0x100 | ~EXPREGS[0] <<4 &0x200 | EXPREGS[0] <<6 &0x400) &~chrAND;
	else
		chrOR =((EXPREGS[0] ^0x20) <<4 &0x380 | EXPREGS[0] <<8 &0x400) &~chrAND;
	
	if (submapper ==3 && EXPREGS[0] &0x04) { /* Submapper 3 replace CHR A14 with PRG A21 */
		chrAND &=~0x10;
		chrOR |= EXPREGS[0] &0x20? 0x00: 0x10;
	}
	if (EXPREGS[3] &0x10) /* CNROM mode: 8 KiB inner CHR bank comes from outer bank register #2 */
		setchr8(EXPREGS[2] &(chrAND >>3) | (chrOR &~chrAND) >>3);
	else /* MMC3 CHR mode */
		setchr1(A, (V & chrAND) | chrOR);
=======
static void M126CW(uint16 A, uint16 V) {
	uint8 reg0 = reg[0] ^ (oldump ? 0 : 0x20);
	uint16 mask = (reg0 & 0x80) ? 0x7F : 0xFF;
	uint16 base = (reg0 << 4) & 0x380;

	if (iNESCart.mapper == 126) {
		base = ((reg0 << 4) & 0x080) | ((reg0 << 3) & 0x100) | ((reg0 << 5) & 0x200);
	}
	if (reg[3] & 0x10) {
		base = (((base & ~mask) >> 3) | (reg[2] & (mask >> 3))) << 3;
		V = (A >> 10) & 0x07;
		mask = 0x07;
	}
	setchr1(A, (base & ~mask) | (V & mask));
>>>>>>> ff5b809 (Update Makefile.libretro)
}

<<<<<<< HEAD
static void wrapMirroring(uint8_t V) {
	A000B =V;
	if (EXPREGS[3] &0x20) { /* ANROM mirroring */
		if (DRegBuf[6] &0x10)
			setmirror(MI_1);
		else
			setmirror(MI_0);
	} else
	if (EXPREGS[1] &0x02) { /* Extended MMC3 mirroring */
		switch(A000B &3) {
=======
static void M126MIR(void) {
	if (reg[3] & 0x20) {
		setmirror(MI_0 + ((mmc3.reg[6] & 0x10) >> 4));
	} else if (reg[1] & 0x02) {
		switch (mmc3.mirr & 0x03) {
>>>>>>> ae14339e (Update Makefile.libretro)
		case 0: setmirror(MI_V); break;
		case 1: setmirror(MI_H); break;
		case 2: setmirror(MI_0); break;
		case 3: setmirror(MI_1); break;
		}
<<<<<<< HEAD
	} else              /* Normal MMC3 mirroring */
		setmirror(A000B &1? MI_H: MI_V);
	
}

static void writeWRAM(uint32 A, uint8 V) {
	CartBW(A, V);
	if ((A &3) ==2) { /* CNROM Bank (D0-D3), Bank Enable (D4-D6) and Bank Enable Lock (D7) */
		int latchMask =0xFF &~(EXPREGS[2] &0x80? 0xF0: 0x00) &~(EXPREGS [2] >>3 &0x0E);
		EXPREGS[2] =EXPREGS[2] &~latchMask | V &latchMask;
		FixMMC3CHR(MMC3_cmd);
	} else
	if (~EXPREGS[3] &0x80) {
		/* Lock bit clear: Update any outer bank register */
		EXPREGS[A &3] =V;
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
		mwrap(A000B); /* After 6001 or 6003 write */
	}
}

<<<<<<< HEAD
static DECLFW(MMC3_CMDWriteA) { /* In mmc3.c, MMC3_cmd is updated *after* FixMMC3PRG is called, but we need MMC3_cmd in wrapPRG, so work around this problem until the MMC3 core is properly rewritten to actually make sense. */
	if ((A &0xE001) ==0x8000) {
		MMC3_cmd =V;
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
	} else
		MMC3_CMDWrite(A, V);	
}
	
static DECLFW(writeCart) {
=======
static void writeCart(uint32 A, uint8 V) {
>>>>>>> ae063eb (Update Makefile.libretro)
	if ((EXPREGS[3] &0x09) ==0x09) /* UNROM and ANROM modes treat all writes to $8000-$FFFF as if they were going to $8000-$9FFF */
		MMC3_CMDWriteA(0x8000 | (EXPREGS[3] &0x08? 1: A) &1, V); /* A0 substitution only looks at bit 3 of register 3 */
	else
	if (A >=0xC000)
		MMC3_IRQWrite(A, V ^(invertC000? 0xFF: 0x00)); /* Mapper 534 inverts the MMC3 scanline counter reload value */
	else
		MMC3_CMDWriteA(A, V);	
}

static uint8 readPRG(uint32 A) {
	if (EXPREGS[1] &1) A =A &~1 | SL0 &1; /* Replace A0 with SL0 input */
=======
	} else {
		setmirror((mmc3.mirr & 0x01) ^ 0x01);
	}
}

static DECLFR(M126ReadDIP) {
	if (reg[1] & 0x01) {
		return CartBR((A & ~0x01) | (dipsw & 0x01));
	}
>>>>>>> 05517c6 (Update Makefile.libretro)
	return CartBR(A);
}

static DECLFW(M126WriteWRAM) {
	CartBW(A, V);
	if (!(reg[3] & 0x80)) {
		reg[A & 0x03] = V;
		MMC3_FixPRG();
		MMC3_FixCHR();
		MMC3_FixMIR();
	} else if ((A & 0x03) == 0x02) {
		const uint8 mask = 0xFF & ~(reg[2] & 0x80 ? 0xF0 : 0x00) & ~((reg[2]) >> 3 & 0x0E);
		reg[2] = (reg[2] & ~mask) | (V & mask);
		MMC3_FixCHR();
	}
}

static DECLFW(M534IRQWrite) {
	V ^= 0xFF;
	MMC3_IRQWrite(A, V);
}

static DECLFW(M126MMC3Write) {
	if ((reg[3] & 0x09) == 0x09) {
		MMC3_Write(0x8000 | (((reg[3] & 0x08) ? 0x01 : A) & 0x01), V);
	} else {
		MMC3_Write(A, V);
	}
}

static void M126Reset(void) {
	dipsw++;
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	MMC3_Reset();
}

static void M126Power(void) {
	dipsw = 0;
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M126WriteWRAM);
	SetWriteHandler(0x8000, 0xFFFF, M126MMC3Write);
	SetReadHandler(0x8000, 0xFFFF, M126ReadDIP);
	if (iNESCart.mapper == 534) {
		SetWriteHandler(0xC000, 0xDFFF, M534IRQWrite);
	}
}

static void InitCommon(CartInfo *info) {
	uint8 ws = 8;
	if (info->iNES2) {
		ws = (info->PRGRamSize + info->PRGRamSaveSize) / 1024;
	}
	MMC3_Init(info, ws, info->battery);
	MMC3_FixMIR = M126MIR;
	MMC3_cwrap = M126CW;
	MMC3_pwrap = M126PW;
	info->Power = M126Power;
	info->Reset = M126Reset;
	AddExState(StateRegs, ~0, 0, NULL);

	switch (iNESCart.CRC32) {
	case 0xEAD80031: /* Gamezone 118-in-1 (AT-207) */
	case 0x6FCBC309: /* Power Joy Classic TV Game 84-in-1 (PJ-008) */
	case 0x6D61FE21: /* 1998 4000000-in-1 (BS-400 PCB) */
	case 0x3FF46175:
	case 0xA3FF9D9B:
	case 0x2BDD0FC2:
	case 0x5789017D: /* (GD-106) 18-in-1 */
	case 0x46A01871: /* 3000000-in-1 (BS-300 PCB) */
	case 0x2466B80A: /* 700000-in-1 (BS-400 PCB) */
	case 0x871CFD16:
	case 0xB2724618:
	case 0x42A9219D:
		oldump = TRUE;
		break;
	}
}

void Mapper126_Init(CartInfo *info) {
	InitCommon(info);
}

void Mapper422_Init(CartInfo *info) {
	InitCommon(info);
}

void Mapper534_Init(CartInfo *info) {
	InitCommon(info);
}
