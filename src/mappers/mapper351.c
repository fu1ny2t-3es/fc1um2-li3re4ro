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
<<<<<<< HEAD
#include "asic_mmc1.h"
#include "asic_mmc3.h"
#include "asic_vrc2and4.h"

static uint8_t reg[4], dip;
static uint8_t *CHRRAM = NULL;
static uint8_t *PRGCHR = NULL;
static int prgMask_CHRROM; /* PRG-ROM bank mask when CHR-ROM is active (outside of PRG address space */
static int prgMask_CHRRAM; /* PRG-ROM bank mask when CHR-RAM is active (CHR-ROM becomes part of PRG address space) */

static SFORMAT stateRegs[] = {
	{ reg,  4, "REGS" },
	{ &dip, 1, "DIPS" },
	{ 0 }
};

static void sync () {
	int prgAND = reg[2] &0x10? 0x00: reg[2] &0x04? 0x0F: 0x1F; /* No inner bank in NROM mode, 128K or 256K for others */
	int chrAND = reg[2] &0x40? 0x00: reg[2] &0x20? 0x7F: reg[2] &0x10? 0x1F: 0xFF; /* No inner bank in (C)NROM mode, 128K or 256K for others */
	int prgOR  = reg[1] >>1 &(reg[2] &0x01 && CHRRAM? prgMask_CHRRAM: prgMask_CHRROM) &~prgAND;
	int chrOR  = reg[0] <<1 &~chrAND;

	if (reg[2] &0x10) { /* NROM mode */
		if (reg[2] &0x08) { /* NROM-64 */
			setprg8(0x8000, prgOR);
			setprg8(0xA000, prgOR);
			setprg8(0xC000, prgOR);
			setprg8(0xE000, prgOR);
		} else
		if (reg[2] &0x04) { /* NROM-128 */
			setprg16(0x8000, prgOR >>1);
			setprg16(0xC000, prgOR >>1);
		} else      /* NROM-256 */
			setprg32(0x8000, prgOR >>2);
	} else
	if (~reg[0] &0x02)
		MMC3_syncPRG(prgAND, prgOR);
	else
	if (reg[0] &0x01)
		VRC24_syncPRG(prgAND, prgOR);
	else
		MMC1_syncPRG(prgAND >>1, prgOR >>1);

	if (reg[2] &0x01 && CHRRAM)
		setchr8r(0x10, 0);
	else
	if (reg[2] &0x40)
		setchr8(chrOR >>3);
	else
	if (~reg[0] &0x02)
		MMC3_syncCHR(chrAND, chrOR);
	else
	if (reg[0] &0x01)
		VRC24_syncCHR(chrAND, chrOR);
	else
		MMC1_syncCHR(chrAND >>2, chrOR >>2);

	if (~reg[0] &0x02)
		MMC3_syncMirror();
	else
	if (reg[0] &0x01)
		VRC24_syncMirror();
	else
		MMC1_syncMirror();
}

<<<<<<< HEAD
static DECLFW (VRC24_trapWriteReg) { /* When A11 is set, VRC4's A0 and A1 are swapped */
=======
<<<<<<< HEAD
DECLFW (VRC24_trapWriteReg) { /* When A11 is set, VRC4's A0 and A1 are swapped */
>>>>>>> 3cd7a223 (Update libretro_core_options.h)
	if (A &0x800) A = A &~0xF | A >>1 &0x5 | A <<1 &0xA;
	VRC24_writeReg(A, V);
}

static void applyMode (uint8_t clear) {
	PPU_hook = NULL;
	MapIRQHook = NULL;
	GameHBIRQHook = NULL;
	if (~reg[0] &0x02)
		MMC3_activate(clear, sync, MMC3_TYPE_SHARP, NULL, NULL, NULL, NULL);
	else
	if (reg[0] &0x01) {
		if (reg[2] &0x05)
			VRC4_activate(clear, sync, 0x05, 0x0A, 1, NULL, NULL, NULL, NULL, NULL);
		else
			VRC4_activate(clear, sync, 0x02, 0x04, 1, NULL, NULL, NULL, NULL, NULL);
		SetWriteHandler(0x8000, 0xFFFF, VRC24_trapWriteReg);
=======
static void writeMMC3(uint32 A, uint8 V) {
	switch(A &0xE001) {
	case 0x8000: MMC3_index =V;              sync();    break;
	case 0x8001: MMC3_reg[MMC3_index &7] =V; sync();    break;
	case 0xA000: MMC3_mirroring =V;          sync();    break;
	case 0xA001: MMC3_wram =V;               sync();    break;
	case 0xC000: MMC3_reload =V;                        break;
	case 0xC001: MMC3_count =0;                         break;
	case 0xE000: MMC3_irq =0; X6502_IRQEnd(FCEU_IQEXT); break;
	case 0xE001: MMC3_irq =1;                           break;
	}
}

static void writeMMC1(uint32 A, uint8 V) {
	if (V &0x80) {
		MMC1_shift =MMC1_count =0;
		MMC1_reg[0] |=0x0C;
		sync();
>>>>>>> e94122c (Update libretro_core_options.h)
	} else
		MMC1_activate(clear, sync, MMC1_TYPE_MMC1B, NULL, NULL, NULL, NULL);
}

<<<<<<< HEAD
static void restore (int version) {
	applyMode(0);
	sync();
}

static DECLFR (readDIP) {
	return dip;
}

static DECLFW (writeReg) {
	reg[A &3] = V;
	applyMode(A == 2);
}

static DECLFW (writeFDSMirroring) {
	MMC3_writeReg(0xA000, V >>3 &1);
=======
=======
#include "mmc1.h"
#include "mmc3.h"
#include "vrc2and4.h"

static uint8 reg[4], dipsw;

static uint8 *CHRRAM = NULL;
static uint8 *PRGCHR = NULL;

static SFORMAT stateRegs[] = {
	{ reg, 4, "REGS" },
	{ &dipsw, 1, "DIPS" },
	{ 0 },
};

static uint32 GetPRGChip(void) {
	return (((reg[2] & 0x01) && CHRRAM) ? 0x10 : 0x00);
}

static uint32 GetPRGMask(void) {
	return ((reg[2] & 0x04) ? 0x0F : 0x1F);
}

static uint32 GetPRGBase(void) {
	return (reg[1] >> 1);
}

static uint32 GetCHRMask(void) {
	if ((reg[2] & 0x10) && !(reg[2] & 0x20)) {
		return 0x1F;
	}
	return ((reg[2] & 0x20) ? 0x7F : 0xFF);
}

static uint32 GetCHRBase(void) {
	return (reg[0] << 1);
}

static void M351MMC1PW(uint16 A, uint16 V) {
	uint8 mask = GetPRGMask() >> 1;
	uint8 bank = GetPRGBase() >> 1;

	setprg16r(GetPRGChip(), A, (bank & ~mask) | (V & mask));
}

static void M351MMC1CW(uint16 A, uint16 V) {
	uint16 mask = GetCHRMask() >> 2;
	uint16 bank = GetCHRBase() >> 2;

	setchr4(A, (bank & ~mask) | (V & mask));
}

static void M351MMC3PW(uint16 A, uint16 V) {
	uint8 mask = GetPRGMask();
	uint8 bank = GetPRGBase();

	setprg8r(GetPRGChip(), A, (bank & ~mask) | (V & mask));
}

static void M351MMC3CW(uint16 A, uint16 V) {
	uint16 mask = GetCHRMask();
	uint16 bank = GetCHRBase();

	setchr1(A, (bank & ~mask) | (V & mask));
}

static void M351VRC24PW(uint16 A, uint16 V) {
	uint8 mask = GetPRGMask();
	uint8 bank = GetPRGBase();

	setprg8r(GetPRGChip(), A, (bank & ~mask) | (V & mask));
}

static void M351VRC24CW(uint16 A, uint16 V) {
	uint16 mask = GetCHRMask();
	uint16 bank = GetCHRBase();

	setchr1(A, (bank & ~mask) | (V & mask));
}

static void SyncPRG(void) {
	if (reg[2] & 0x10) { /* NROM mode */
		uint32 bank = GetPRGBase();
		uint32 chip = GetPRGChip();

		if (reg[2] & 0x08) { /* NROM-64 */
			setprg8r(chip, 0x8000, bank);
			setprg8r(chip, 0xA000, bank);
			setprg8r(chip, 0xC000, bank);
			setprg8r(chip, 0xE000, bank);
		} else {
			if (reg[2] & 0x04) { /* NROM-128 */
				setprg16r(chip, 0x8000, bank >> 1);
				setprg16r(chip, 0xC000, bank >> 1);
			} else { /* NROM-256 */
				setprg32r(chip, 0x8000, bank >> 2);
			}
		}
	} else {
		switch (reg[0] & 0x03) {
		default:
		case 1: MMC3_FixPRG(); break;
		case 2: MMC1_FixPRG(); break;
		case 3: VRC24_FixPRG(); break;
		}
	}
}

static void SyncCHR(void) {
	if (reg[2] & 0x01) { /* CHR RAM mode */
		setchr8r(0x10, 0);
	} else if (reg[2] & 0x40) { /* CNROM mode */
		setchr8(GetCHRBase() >> 3);
	} else {
		switch (reg[0] & 0x03) {
		default:
		case 1: MMC3_FixCHR(); break;
		case 2: MMC1_FixCHR(); break;
		case 3: VRC24_FixCHR(); break;
		}
	}
}

<<<<<<< HEAD
>>>>>>> 46df7ab (Update libretro_core_options.h)
static void writeVRC4(uint32 A, uint8 V) {
	uint8 index;
	if (~reg[2] &4) A =A &0xF800 | A >>1 &0x3FF; /* A2,A1 -> A1,A0 if 5002.2=1 */
	A |=A >>2 &3; /* A3,A2 -> A1,A0 */
	if (A &0x800) A =A >>1 &1 | A <<1 &2 | A &~3; /* A8==1 => Swap A1,A0 */
	switch (A &0xF000) {
	case 0x8000: case 0xA000:
		VRC4_prg[A >>13 &1] =V;
		sync();
		break;
	case 0x9000:
		if (~A &2)
			VRC4_mirroring =V;
		else
		if (~A &1)
			VRC4_misc =V;
		sync();
		break;
	case 0xF000:
		switch (A &3) {
		case 0: VRCIRQ_latch =VRCIRQ_latch &0xF0 | V &0x0F; break;
		case 1: VRCIRQ_latch =VRCIRQ_latch &0x0F | V <<4;   break;
		case 2: VRCIRQ_mode =V;
		        if (VRCIRQ_mode &0x02) {
				VRCIRQ_count =VRCIRQ_latch;
				VRCIRQ_cycles =341;
			}
			X6502_IRQEnd(FCEU_IQEXT);
			break;
		case 3: VRCIRQ_mode =VRCIRQ_mode &~0x02 | VRCIRQ_mode <<1 &0x02;
			X6502_IRQEnd(FCEU_IQEXT);
			break;
		}
		break;
	default:
		index =(A -0xB000) >>11 &~1 | A >>1 &1;
		if (A &1)
			VRC4_chr[index] =VRC4_chr[index] & 0x0F | V <<4;
		else
			VRC4_chr[index] =VRC4_chr[index] &~0x0F | V &0x0F;
		sync();
=======
static void SyncMIR(void) {
	switch (reg[0] & 0x03) {
	default:
	case 1: MMC3_FixMIR(); break;
	case 2: MMC1_FixMIR(); break;
	case 3: VRC24_FixMIR(); break;
	}
}

static void Sync(void) {
	SyncPRG();
	SyncCHR();
	SyncMIR();
}

static void M351CPUHook(int a) {
	if ((reg[0] & 0x03) == 0x03) {
		VRC24_IRQCPUHook(a);
	}
}

static void M351HBHook(void) {
	if (!(reg[0] & 0x02)) { /* MMC3 mode */
		MMC3_IRQHBHook();
	}
}

static DECLFR(M351ReadDIP) {
	return dipsw;
}

static DECLFW(M351WriteMirr) {
	/* FDS mirroring */
	mmc3.mirr = (V >> 3) & 0x01;
	SyncMIR();
}

static DECLFW(M351WriteReg) {
	reg[A & 0x03] = V;
	Sync();
}

static DECLFW(M351Write) {
	switch (reg[0] & 0x03) {
	default:
	case 1:
		MMC3_Write(A, V);
		Sync();
		break;
	case 2:
		MMC1_Write(A, V);
		break;
	case 3:
		if (A & 0x800) {
			A = (A & 0xFFF3) | ((A << 1) & 0x08) | ((A >> 1) & 0x04);
		}
		VRC24_Write(A, V);
>>>>>>> 9023b1d (Update libretro_core_options.h)
		break;
	}
}

static void M351Power(void) {
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	dipsw = 0;

	VRC24_Reset();
	MMC1_Reset();
	MMC3_Reset();

<<<<<<< HEAD
static void Mapper351_restore (int version) {
	applyMode();
	sync();
}

static uint8 readDIP(uint32 A) { return dip; }

static void writeReg(uint32 A, uint8 V) {
	uint8 previousMode =reg[0] &3;
	reg[A &3] =V;
	if ((reg[0] &3) !=previousMode) applyMode();
	sync();
}

static void writeFDSMirroring(uint32 A, uint8 V) {
	MMC3_mirroring =V >>3 &1;
	sync();
>>>>>>> e94122c (Update libretro_core_options.h)
}

static void power (void) {
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	dip = 0;
	SetReadHandler(0x5000, 0x5FFF, readDIP);
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x5000, 0x5FFF, writeReg);
	SetWriteHandler(0x4025, 0x4025, writeFDSMirroring);
	applyMode(1);
}

static void reset (void) {
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	dip++;
	applyMode(1);
}

static void close (void) {
	if (CHRRAM) FCEU_gfree(CHRRAM);
	if (PRGCHR) FCEU_gfree(PRGCHR);
=======
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetReadHandler(0x5000, 0x5FFF, M351ReadDIP);
	SetWriteHandler(0x4025, 0x4025, M351WriteMirr);
	SetWriteHandler(0x5000, 0x5FFF, M351WriteReg);
	SetWriteHandler(0x8000, 0xFFFF, M351Write);
}

static void M531Reset(void) {
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	dipsw++;

	VRC24_Reset();
	MMC1_Reset();
	MMC3_Reset();

	FCEU_printf(" Mapper Reset! dpsw:%d\n", dipsw);
}

static void M351Close(void) {
	if (CHRRAM) {
		FCEU_gfree(CHRRAM);
	}
	if (PRGCHR) {
		FCEU_gfree(PRGCHR);
	}
>>>>>>> 46df7ab (Update libretro_core_options.h)
	CHRRAM = NULL;
	PRGCHR = NULL;
}

<<<<<<< HEAD
void Mapper351_Init (CartInfo *info) {
	int CHRRAMSIZE = info->CHRRamSize + info->CHRRamSaveSize;
	MMC1_addExState();
	MMC3_addExState();
	VRC24_addExState();
	info->Reset = reset;
	info->Power = power;
	info->Close = close;
	GameStateRestore = restore;
	AddExState(stateRegs, ~0, 0, 0);

 	/* When CHR-RAM is enabled, CHR-ROM becomes part of PRG-ROM address space. */
	prgMask_CHRROM = prgMask_CHRRAM = PRGsize[0] /8192 -1;
	if (CHRRAMSIZE) {
		uint8_t* newROM;
		CHRRAM = (uint8_t *)FCEU_gmalloc(CHRRAMSIZE);
		SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
		AddExState(CHRRAM, CHRRAMSIZE, 0, "CRAM");
		prgMask_CHRRAM = (PRGsize[0] +CHRsize[0]) /8192 -1;
		newROM = (uint8_t*)FCEU_gmalloc(PRGsize[0] +CHRsize[0]);
		memcpy(newROM,              ROM, info->PRGRomSize);
		memcpy(newROM +PRGsize[0], VROM, info->CHRRomSize);
		FCEU_gfree(ROM);
		ROM = newROM;
		SetupCartPRGMapping(0, ROM, PRGsize[0] +CHRsize[0], 0);
=======
static void StateRestore(int version) {
	Sync();
}

void Mapper351_Init(CartInfo *info) {
	int CHRRAMSIZE = info->CHRRamSize + info->CHRRamSaveSize;

	VRC24_Init(info, VRC2, 0x04, 0x08, FALSE, TRUE);
	VRC24_pwrap = M351VRC24PW;
	VRC24_cwrap = M351VRC24CW;

	MMC1_Init(info, FALSE, FALSE);
	MMC1_pwrap = M351MMC1PW;
	MMC1_cwrap = M351MMC1CW;

	MMC3_Init(info, FALSE, FALSE);
	MMC3_pwrap = M351MMC3PW;
	MMC3_cwrap = M351MMC3CW;

	info->Reset = M531Reset;
	info->Power = M351Power;
	info->Close = M351Close;

	MapIRQHook = M351CPUHook;
	GameHBIRQHook = M351HBHook;

	GameStateRestore = StateRestore;
	AddExState(stateRegs, ~0, 0, 0);

	if (!UNIFchrrama && CHRRAMSIZE) {
		CHRRAM = (uint8 *)FCEU_gmalloc(CHRRAMSIZE);
		SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
		AddExState(CHRRAM, CHRRAMSIZE, 0, "CRAM");

		/* This crazy thing can map CHR-ROM into CPU address space. Allocate a combined PRG+CHR address space and treat
		 * it a second "chip". */
		PRGCHR = (uint8 *)FCEU_gmalloc(PRGsize[0] + CHRsize[0]);
		memcpy(PRGCHR, PRGptr[0], PRGsize[0]);
		memcpy(PRGCHR + PRGsize[0], CHRptr[0], CHRsize[0]);
		SetupCartPRGMapping(0x10, PRGCHR, PRGsize[0] + CHRsize[0], 0);
>>>>>>> 46df7ab (Update libretro_core_options.h)
	}
}
