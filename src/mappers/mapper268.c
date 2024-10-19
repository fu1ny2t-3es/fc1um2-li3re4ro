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

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg[8];

static uint8 *CHRRAM = NULL;
static uint32 CHRRAMSize = 0;

static void M268CW(uint16 A, uint16 V) {
	uint16 base = ((reg[0] << 4) & 0x380) | ((reg[2] << 3) & 0x078);
	uint16 mask = (reg[0] & 0x80) ? 0x7F : 0xFF;
	uint8  ram  = CHRRAM && (reg[4] & 0x01) && ((V & 0xFE) == (reg[4] & 0xFE));

	/* CHR-RAM write protect on submapper 8/9) */
	if ((iNESCart.submapper & ~1) == 8) {
		if (reg[0] & 0x10) {
			SetupCartCHRMapping(0, CHRptr[0], CHRsize[0], 0);
		} else {
			SetupCartCHRMapping(0, CHRptr[0], CHRsize[0], 1);
		}
	}
	if (reg[3] & 0x10) {
		/* GNROM */
		mask = 0x07;
		V = A >> 10;
	}
	setchr1r(ram ? 0x10 : 0x00, A, (base & ~mask) | (V & mask));
}

static void M268PW(uint16 A, uint16 V) {
	uint16 base;
	uint16 mask = 0x0F				/* PRG A13-A16 */
	    | ((~reg[0] >> 2) & 0x10)	/* PRG A17     */
	    | ((~reg[1] >> 2) & 0x20)	/* PRG A18     */
	    | ((reg[1] >> 0) & 0x40)	/* PRG A19     */
	    | ((reg[1] << 2) & 0x80)	/* PRG A20     */
	    ;

	switch (iNESCart.submapper & ~1) {
	default: /* Original implementation */
		base = (reg[3] & 0x0E) | ((reg[0] << 4) & 0x70) | ((reg[1] << 3) & 0x80) |
		    ((reg[1] << 6) & 0x300) | ((reg[0] << 6) & 0xC00);
		break;
	case 2: /* Later revision with different arrangement of register 1 */
		base = (reg[3] & 0x0E) | ((reg[0] << 4) & 0x70) | ((reg[1] << 4) & 0x80) |
		    ((reg[1] << 6) & 0x100) | ((reg[1] << 8) & 0x200) | ((reg[0] << 6) & 0xC00);
		break;
	case 4: /* LD622D: PRG A20-21 moved to register 0 */
		base = (reg[3] & 0x0E) | ((reg[0] << 4) & 0x70) | ((reg[0] << 3) & 0x180);
		break;
	case 6: /* J-852C: CHR A17 selects between two PRG chips */
		base = (reg[3] & 0x0E) | ((reg[0] << 4) & 0x70) | ((reg[1] << 3) & 0x80) |
		    ((reg[1] << 6) & 0x300) | ((reg[0] << 6) & 0xC00);
		base &= ROM.prg.size - 1;
		if ((reg[0] & 0x80) ? !!(reg[0] & 0x08) : !!(mmc3.reg[0] & 0x80)) {
			base |= ROM.prg.size;
		}
		break;
	}
	if (reg[3] & 0x10) {
		/* GNROM */
		switch (iNESCart.submapper & ~1) {
		default:
			mask = (reg[1] & 0x02) ? 0x03 : 0x01;
			break;
		case 2:
			mask = (reg[1] & 0x10) ? 0x01 : 0x03;
			break;
		}
		V = A >> 13;
	}
	setprg8(A, (base & ~mask) | (V & mask));
}

<<<<<<< HEAD
<<<<<<< HEAD
void Mapper268_MirrorWrap(uint8 V) {
	A000B =V;
	if ((submapper &~1) ==10 && ~EXPREGS[0] &0x20)
		setmirror(EXPREGS[0] &0x10? MI_1: MI_0);
	else
		setmirror(A000B &1? MI_H: MI_V);
}

static DECLFR(Mapper268_ReadWRAM) {
=======
static uint8 Mapper268_ReadWRAM(uint32 A) {
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> eefddd1 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
	return A001B &0xA0? CartBR(A): X.DB;
=======
	return A001B &0xA0? CartBR(A): cpu.DB;
>>>>>>> 4152142 (Rename X6502 X to cpu)
=======
	return A001B &0xA0? CartBR(A): cpu.openbus;
>>>>>>> d3918c4 (Rename CPU.DB to cpu.openbus and rename ops.h to x6502ops.inc)
}

static void Mapper268_WriteWRAM(uint32 A, uint8 V) {
	if (A001B &0x80 && ~A001B &0x40 || A001B &0x20) CartBW(A, V);
}

static void Mapper268_WriteReg(uint32 A, uint8 V) {
	int index =A &7;
	if (~submapper &1) Mapper268_WriteWRAM(A, V);
	if (~EXPREGS[3] &0x80 || index ==2) {
		if (index ==2) {
			if (EXPREGS[2] &0x80) V =V &0x0F | EXPREGS[2] &~0x0F;
			V &=~EXPREGS[2] >>3 &0xE |0xF1;
		}		
		EXPREGS[index] =V;
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
		Mapper268_MirrorWrap(A000B);
=======
static DECLFR(M268WramRead) {
	if (MMC3_WramIsWritable() || (mmc3.wram & 0x20)) {
		return CartBR(A);
	}
	return cpu.openbus;
}

static DECLFW(M268WramWrite) {
	if (MMC3_WramIsWritable() || (mmc3.wram & 0x20)) {
		CartBW(A, V);
>>>>>>> 96ae3bf (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
	}
}

static DECLFW(M268Write) {
	uint8 index = A & 0x07;

<<<<<<< HEAD
static void Mapper268_Power(void) {	
	EXPREGS[0] =EXPREGS[1] =EXPREGS[2] =EXPREGS[3] =EXPREGS[4] =EXPREGS[5] =0;
	GenMMC3Power();
	SetReadHandler(0x6000, 0x7FFF, Mapper268_ReadWRAM);
	if (submapper &1) {
		SetWriteHandler(0x5000, 0x5FFF, Mapper268_WriteReg);
		SetWriteHandler(0x6000, 0x7FFF, Mapper268_WriteWRAM);
	} else
		SetWriteHandler(0x6000, 0x7FFF, Mapper268_WriteReg);
}

static void Mapper268_close(void) {
	if (CHRRAM) FCEU_gfree(CHRRAM);
	CHRRAM =NULL;
	GenMMC3Close();
}

void Mapper268_Init(CartInfo *info) {
	submapper = info->submapper;
	GenMMC3_Init(info, 512, 256, (info->PRGRamSize +info->PRGRamSaveSize) >>10, info->battery);
	cwrap = Mapper268_CHRWrap;
	pwrap = Mapper268_PRGWrap;
	mwrap = Mapper268_MirrorWrap;
	info->Power = Mapper268_Power;
	info->Reset = Mapper268_Reset;
	info->Close = Mapper268_close;
	AddExState(EXPREGS, 8, 0, "EXPR");
	
	if (info->CHRRomSize && info->CHRRamSize + info->CHRRamSaveSize) {
		CHRRAM =(uint8 *)FCEU_gmalloc(info->CHRRamSize + info->CHRRamSaveSize);
		SetupCartCHRMapping(0x10, CHRRAM, info->CHRRamSize + info->CHRRamSaveSize, 1);
		AddExState(CHRRAM, info->CHRRamSize + info->CHRRamSaveSize, 0, "CRAM");
=======
	if (!(iNESCart.submapper & 0x01)) {
		M268WramWrite(A, V);
>>>>>>> 96ae3bf (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
	}
	if (!(reg[3] & 0x80) || (index == 2)) {
		if (index == 2) {
			if (reg[2] & 0x80) {
				V = (V & 0x0F) | (reg[2] & ~0x0F);
			}
			V &= ((~reg[2] >> 3) & 0x0E) | 0xF1;
		}
		reg[index] = V;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

static void M268Reset(void) {
	reg[0] = reg[1] = reg[2] = reg[3] = 0x00;
	reg[4] = reg[5] = reg[6] = reg[7] = 0x00;
	MMC3_Reset();
}

static void M268Power(void) {
	reg[0] = reg[1] = reg[2] = reg[3] = 0x00;
	reg[4] = reg[5] = reg[6] = reg[7] = 0x00;
	MMC3_Power();
	SetReadHandler(0x6000, 0x7FFF, M268WramRead);
	if (iNESCart.submapper & 1) {
		SetWriteHandler(0x5000, 0x5FFF, M268Write);
	} else {
		SetWriteHandler(0x6000, 0x7FFF, M268Write);
	}
}

static void M268Close(void) {
	MMC3_Close();
	if (CHRRAM) {
		FCEU_gfree(CHRRAM);
	}
	CHRRAM = NULL;
}

static void Common_Init(CartInfo *info, int _submapper) {
	MMC3_Init(info, (info->PRGRamSize + info->PRGRamSaveSize) >> 10, info->battery);
	if (info->submapper != _submapper) {
		info->submapper = _submapper;
	}
	MMC3_pwrap = M268PW;
	MMC3_cwrap = M268CW;
	info->Power = M268Power;
	info->Reset = M268Reset;
	info->Close = M268Close;
	CHRRAMSize = info->CHRRamSize + info->CHRRamSaveSize;
	if (!UNIFchrrama && CHRRAMSize) {
		CHRRAM = (uint8 *)FCEU_gmalloc(CHRRAMSize);
		SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSize, 1);
		AddExState(CHRRAM, CHRRAMSize, 0x00, "CRAM");
	}
	AddExState(reg, 8, 0, "EXPR");
}

void COOLBOY_Init(CartInfo *info) {
	Common_Init(info, 0);
}

void MINDKIDS_Init(CartInfo *info) { /* M224 */
	Common_Init(info, 1);
}

void Mapper268_Init(CartInfo *info) {
	Common_Init(info, info->submapper);
}
