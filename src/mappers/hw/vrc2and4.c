/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
<<<<<<< HEAD
 *  Copyright (C) 2025 NewRisingSun
=======
 *  Copyright (C) 2007 CaH4e3
 *  Copyright (C) 2023-2024 negativeExponent
>>>>>>> 8bc4f46 (Update libretro.c)
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
 * VRC-2/VRC-4 Konami
 * VRC-4 Pirate
 *
 * VRC2
 * Nickname	PCB		A0	A1	Registers					iNES mapper	submapper
 * VRC2a	351618	A1	A0	$x000, $x002, $x001, $x003	22			0
 * VRC2b	many†	A0	A1	$x000, $x001, $x002, $x003	23			3
 * VRC2c	351948	A1	A0	$x000, $x002, $x001, $x003	25			3
 * VRC4
 * Nickname	PCB		A0	A1	Registers					iNES mapper	submapper
 * VRC4a	352398	A1	A2	$x000, $x002, $x004, $x006	21			1
 * VRC4b	351406	A1	A0	$x000, $x002, $x001, $x003	25			1
 * VRC4c	352889	A6	A7	$x000, $x040, $x080, $x0C0	21			2
 * VRC4d	352400	A3	A2	$x000, $x008, $x004, $x00C	25			2
 * VRC4e	352396	A2	A3	$x000, $x004, $x008, $x00C	23			2
 * VRC4f	-		A0	A1	$x000, $x001, $x002, $x003	23			1
 *
>>>>>>> 8bc4f46 (Update libretro.c)
 */

#include "mapinc.h"
#include "vrc2and4.h"
<<<<<<< HEAD

int    VRC24_A0, VRC24_A1; /* What cartridge connector address pins are connected to the chip's A0 and A1 input pins? */
uint8  VRC24_isVRC4;       /* VRC2 or VRC4? VRC2 has no single-screen mirroring, no PRG A14 swap and no IRQ counter */
uint8  VRC24_useRepeatBit; /* VRC4 clones ignore the "repeat" bit in the IRQ Mode register */
uint8  VRC24_prg[2];
uint16 VRC24_chr[8];
uint8  VRC24_mirroring;
uint8  VRC24_misc;
uint8  VRC2_pins;          /* EEPROM interface */
uint8  VRC4_latch;
uint8  VRC4_mode;
uint8  VRC4_count;
signed short int VRC4_cycles;

static uint8 *WRAM;
static uint32 WRAMSize;

int    (*VRC24_GetPRGBank)(int);
int    (*VRC24_GetCHRBank)(int);
void   (*VRC24_Sync)();
DECLFW((*VRC24_ExternalSelect));
DECLFR((*VRC24_WRAMRead));
DECLFW((*VRC24_WRAMWrite));

static SFORMAT VRC24_stateRegs[] ={
	{ VRC24_prg, 2, "PREG" },
	{ &VRC24_chr[0], 2 | FCEUSTATE_RLSB, "CRH0" },
	{ &VRC24_chr[1], 2 | FCEUSTATE_RLSB, "CRH1" },
	{ &VRC24_chr[2], 2 | FCEUSTATE_RLSB, "CRH2" },
	{ &VRC24_chr[3], 2 | FCEUSTATE_RLSB, "CRH3" },
	{ &VRC24_chr[4], 2 | FCEUSTATE_RLSB, "CRH4" },
	{ &VRC24_chr[5], 2 | FCEUSTATE_RLSB, "CRH5" },
	{ &VRC24_chr[6], 2 | FCEUSTATE_RLSB, "CRH6" },
	{ &VRC24_chr[7], 2 | FCEUSTATE_RLSB, "CRH7" },
	{ &VRC24_mirroring, 1, "MIRR" },
	{ &VRC24_misc, 1, "MISC" },
	{ 0 }
};

static SFORMAT VRC2_stateRegs[] ={
	{ &VRC2_pins, 1, "PINS" },
};

static SFORMAT VRC4_stateRegs[] ={
	{ &VRC4_latch, 1, "LATC" },
	{ &VRC4_mode, 1, "MODE" },
	{ &VRC4_count, 1, "COUN" },
	{ &VRC4_cycles, 2 | FCEUSTATE_RLSB, "CYCL" },
	{ 0 }
};

void VRC24_syncPRG(int AND, int OR) {
	setprg8(0x8000, VRC24_GetPRGBank(0) &AND | OR);
	setprg8(0xA000, VRC24_GetPRGBank(1) &AND | OR);
	setprg8(0xC000, VRC24_GetPRGBank(2) &AND | OR);
	setprg8(0xE000, VRC24_GetPRGBank(3) &AND | OR);
}

<<<<<<< HEAD
void VRC24_syncCHR(int AND, int OR) {
	setchr1(0x0000, VRC24_GetCHRBank(0) &AND | OR);
	setchr1(0x0400, VRC24_GetCHRBank(1) &AND | OR);
	setchr1(0x0800, VRC24_GetCHRBank(2) &AND | OR);
	setchr1(0x0C00, VRC24_GetCHRBank(3) &AND | OR);
	setchr1(0x1000, VRC24_GetCHRBank(4) &AND | OR);
	setchr1(0x1400, VRC24_GetCHRBank(5) &AND | OR);
	setchr1(0x1800, VRC24_GetCHRBank(6) &AND | OR);
	setchr1(0x1C00, VRC24_GetCHRBank(7) &AND | OR);
}

void VRC24_syncWRAM(int OR) {
	setprg8r(0x10, 0x6000, OR);
}

int VRC24_getPRGBank(int bank) {
	if (~bank &1 && VRC24_misc &2) bank ^=2;
	return bank &2? (0xFE | bank &1): VRC24_prg[bank &1];
}

int VRC24_getCHRBank(int bank) {
	return VRC24_chr[bank &7];
}

void VRC24_syncMirror() {
	setmirror(VRC24_isVRC4 && VRC24_mirroring &2? (VRC24_mirroring &1? MI_1: MI_0): (VRC24_mirroring &1? MI_H: MI_V));
}

DECLFR(VRC24_wramRead) {
	if (VRC24_WRAMRead)
		VRC24_WRAMRead(A);
	else
	if (WRAMSize)
		return CartBR(((A -0x6000) &(WRAMSize -1)) +0x6000);
	else
		return 0xFF;
}

DECLFW(VRC24_wramWrite) {
	if (WRAMSize)
		CartBW(((A -0x6000) &(WRAMSize -1)) +0x6000, V);
	else
	if (!VRC24_isVRC4) {
		VRC2_pins =V;
		VRC24_Sync();
	}
	
	if (VRC24_WRAMWrite)
		VRC24_WRAMWrite(A, V);
}

DECLFW(VRC24_writeReg) {
	unsigned int index, addr;
	addr =A &0xF000 | (A &VRC24_A0? 1: 0) | (A &VRC24_A1? 2: 0); /* address as the chip sees it */
	switch (addr &0xF000) {
	case 0x8000: case 0xA000:
		VRC24_prg[addr >>13 &1] =V;
		VRC24_Sync();
		break;
	case 0x9000:
		if (!VRC24_isVRC4 || (addr &3) ==0) {
			VRC24_mirroring =V;
			VRC24_Sync();
		} else
		if (VRC24_isVRC4 && (addr &3) ==2) {
			VRC24_misc =V;
			VRC24_Sync();
		} 
		if (VRC24_isVRC4 && (addr &3) ==3 && VRC24_ExternalSelect) {
			VRC24_ExternalSelect(A, V); /* Provide original address */
		}
		break;
	case 0xF000:
		if (VRC24_isVRC4) switch (addr &3) {
			case 0: VRC4_latch =VRC4_latch &0xF0 | V &0x0F; break;
			case 1: VRC4_latch =VRC4_latch &0x0F | V <<4;   break;
			case 2: VRC4_mode =V;
				if (VRC4_mode &0x02) {
					VRC4_count =VRC4_latch;
					VRC4_cycles =341;
=======
static void VRC24Write(uint32 A, uint8 V) {
	A &= 0xF003;
	if ((A >= 0xB000) && (A <= 0xE003)) {
		if (UNIFchrrama)
			big_bank = (V & 8) << 2;							/* my personally many-in-one feature ;) just for support pirate cart 2-in-1 */
		else {
			uint16 i = ((A >> 1) & 1) | ((A - 0xB000) >> 11);
			uint16 nibble = ((A & 1) << 2);
			chrreg[i] = (chrreg[i] & (0xF0 >> nibble)) | ((V & 0xF) << nibble);
			if (nibble)
				chrhi[i] = (V & 0x10) << 4;						/* another one many in one feature from pirate carts */
=======
#include "vrcirq.h"

uint8 vrc2and4_VRC4;
uint32 vrc2and4_A0;
uint32 vrc2and4_A1;

void (*VRC24_FixPRG)(void);
void (*VRC24_FixCHR)(void);
void (*VRC24_FixMIR)(void);

void (*VRC24_pwrap)(uint16 A, uint16 V);
void (*VRC24_cwrap)(uint16 A, uint16 V);
void (*VRC24_miscWrite)(uint16 A, uint8 V);

VRC24 vrc24;

static SFORMAT StateRegs[] = {
	{ vrc24.prg, 2, "PREG" },
	{ vrc24.chr, 16, "CREG" },
	{ &vrc24.cmd, 1, "CMDR" },
	{ &vrc24.mirr, 1, "MIRR" },
	{ &vrc24.latch, 1, "LATC" },

	{ 0 }
};

static void GENFIXPRG(void) {
	if (vrc24.cmd & 2) {
		VRC24_pwrap(0x8000, ~1);
		VRC24_pwrap(0xC000, vrc24.prg[0]);
	} else {
		VRC24_pwrap(0x8000, vrc24.prg[0]);
		VRC24_pwrap(0xC000, ~1);
	}
	VRC24_pwrap(0xA000, vrc24.prg[1]);
	VRC24_pwrap(0xE000, ~0);
}

static void GENFIXCHR(void) {
	VRC24_cwrap(0x0000, vrc24.chr[0]);
	VRC24_cwrap(0x0400, vrc24.chr[1]);
	VRC24_cwrap(0x0800, vrc24.chr[2]);
	VRC24_cwrap(0x0C00, vrc24.chr[3]);
	VRC24_cwrap(0x1000, vrc24.chr[4]);
	VRC24_cwrap(0x1400, vrc24.chr[5]);
	VRC24_cwrap(0x1800, vrc24.chr[6]);
	VRC24_cwrap(0x1C00, vrc24.chr[7]);
}

static void GENFIXMIR(void) {
	switch (vrc24.mirr & (vrc2and4_VRC4 ? 0x03 : 0x01)) {
	case 0: setmirror(MI_V); break;
	case 1: setmirror(MI_H); break;
	case 2: setmirror(MI_0); break;
	case 3: setmirror(MI_1); break;
	}
}

static DECLFR(VRC24WRAMRead) {
	A = 0x6000 | (A & (WRAMSIZE - 1));
	return CartBR(A);
}

static DECLFW(VRC24WRAMWrite) {
	A = 0x6000 | (A & (WRAMSIZE - 1));
	CartBW(A, V);
}

static DECLFR(VRC24LatchRead) {
	return (cpu.openbus & ~0x01) | (vrc24.latch & 0x01);
}

static DECLFW(VRC24LatchWrite) {
	vrc24.latch = V;
	VRC24_FixPRG();
	VRC24_FixCHR();
}

DECLFW(VRC24_Write) {
	uint8 index;

	switch (A & 0xF000) {
	case 0x8000:
	case 0xA000:
		vrc24.prg[(A >> 13) & 0x01] = V & 0x1F;
		VRC24_FixPRG();
		break;

	case 0x9000:
		index = ((A & vrc2and4_A1) ? 0x02 : 0x00) | ((A & vrc2and4_A0) ? 0x01 : 0x00);
		switch (index & (vrc2and4_VRC4 ? 0x03 : 0x00)) {
		case 0:
		case 1:
			if (V != 0xFF) {
				vrc24.mirr = V;
				VRC24_FixMIR();
			}
			break;
		case 2:
			vrc24.cmd = V;
			VRC24_FixPRG();
			break;
		case 3:
			if (VRC24_miscWrite) {
				VRC24_miscWrite(A, V);
			}
			break;
>>>>>>> 8bc4f46 (Update libretro.c)
		}
		break;

	case 0xF000:
		index = ((A & vrc2and4_A1) ? 0x02 : 0x00) | ((A & vrc2and4_A0) ? 0x01 : 0x00);
		switch (index) {
		case 0x00: VRCIRQ_LatchNibble(V, 0); break;
		case 0x01: VRCIRQ_LatchNibble(V, 1); break;
		case 0x02: VRCIRQ_Control(V); break;
		case 0x03: VRCIRQ_Acknowledge(); break;
		}
		break;

	default:
		index = (((A - 0xB000) >> 11) & 0x06) | ((A & vrc2and4_A1) ? 0x01 : 0x00);
		if (A & vrc2and4_A0) {
			/* m25 can be 512K, rest are 256K or less */
			vrc24.chr[index] = (vrc24.chr[index] & 0x000F) | (V << 4);
		} else {
			vrc24.chr[index] = (vrc24.chr[index] & 0x0FF0) | (V & 0x0F);
		}
		VRC24_cwrap(index << 10, vrc24.chr[index]);
		break;
	}
}

static void GENPWRAP(uint16 A, uint16 V) {
	setprg8(A, V & 0x1F);
}

static void GENCWRAP(uint16 A, uint16 V) {
	setchr1(A, V & 0x1FF);
}

void VRC24_IRQCPUHook(int a) {
	VRCIRQ_CPUHook(a);
}

void VRC24_Reset(void) {
	vrc24.prg[0] = 0;
	vrc24.prg[1] = 1;

	vrc24.chr[0] = 0;
	vrc24.chr[1] = 1;
	vrc24.chr[2] = 2;
	vrc24.chr[3] = 3;
	vrc24.chr[4] = 4;
	vrc24.chr[5] = 5;
	vrc24.chr[6] = 6;
	vrc24.chr[7] = 7;

	vrc24.cmd = vrc24.mirr = 0;

	VRC24_FixPRG();
	VRC24_FixCHR();
}

void VRC24_Power(void) {
	VRC24_Reset();

	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, VRC24_Write);

	if (WRAMSIZE) {
		setprg8r(0x10, 0x6000, 0);
		SetReadHandler(0x6000, 0x7FFF, VRC24WRAMRead);
		SetWriteHandler(0x6000, 0x7FFF, VRC24WRAMWrite);
		FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
	} else if (!vrc2and4_VRC4) {
		/* microwire interface */
		SetReadHandler(0x6000, 0x6FFF, VRC24LatchRead);
		SetWriteHandler(0x6000, 0x6FFF, VRC24LatchWrite);
	}

<<<<<<< HEAD
static void M22Power(void) {
	VRC24PowerCommon(M22Write);
}

static void M23Power(void) {
	big_bank = 0x20;
	if ((prgreg[2] == 0x30) && (prgreg[3] == 0x31))
		big_bank = 0x00;
	VRC24PowerCommon(M23Write);
}

static void M25Power(void) {
	big_bank = 0x20;
	VRC24PowerCommon(M22Write);
}

void VRC24IRQHook(int a) {
	#define LCYCS 341
	if (IRQa) {
		acount += a * 3;
		if (acount >= LCYCS) {
			while (acount >= LCYCS) {
				acount -= LCYCS;
				IRQCount++;
				if (IRQCount & 0x100) {
					X6502_IRQBegin(FCEU_IQEXT);
					IRQCount = IRQLatch;
>>>>>>> bc0be5f (Update libretro.c)
				}
				X6502_IRQEnd(FCEU_IQEXT);
				break;
			case 3: if (VRC24_useRepeatBit) VRC4_mode =VRC4_mode &~0x02 | VRC4_mode <<1 &0x02;
				X6502_IRQEnd(FCEU_IQEXT);
				break;
		}
		break;
	default:
		index =(addr -0xB000) >>11 | addr >>1 &1;
		if (addr &1)
			VRC24_chr[index] =VRC24_chr[index] & 0x0F | V <<4;
		else
			VRC24_chr[index] =VRC24_chr[index] &~0x0F | V &0x0F;
		VRC24_Sync();
		break;
	}
}

void FP_FASTAPASS(1) VRC4_cpuCycle(int a) {
	while (a--) {
		if (VRC4_mode &0x02 && (VRC4_mode &0x04 || (VRC4_cycles -=3) <=0)) {
			if (~VRC4_mode &0x04) VRC4_cycles +=341;
			if (!++VRC4_count) {
				VRC4_count =VRC4_latch;
				X6502_IRQBegin(FCEU_IQEXT);
			}
		}
	}
}

void VRC24_power(void) {
	VRC24_prg[0] =0; VRC24_prg[1] =0;
	VRC24_chr[0] =0; VRC24_chr[1] =1; VRC24_chr[2] =2; VRC24_chr[3] =3; VRC24_chr[4] =4; VRC24_chr[5] =5; VRC24_chr[6] =6; VRC24_chr[7] =7;
	VRC24_mirroring =VRC24_misc =VRC2_pins =VRC4_latch =VRC4_mode =VRC4_count =VRC4_cycles =0;
	SetReadHandler(0x6000, 0x7FFF,  VRC24_wramRead);
	SetWriteHandler(0x6000, 0x7FFF, VRC24_wramWrite);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, VRC24_writeReg);
	VRC24_Sync();
}

void VRC24_close(void) {
	if (WRAM) {
		FCEU_gfree(WRAM);
		WRAM =NULL;
	}
}

static void StateRestore(int version) {
	VRC24_Sync();
}

void VRC24_init(CartInfo *info, void (*_sync)(), int _A0, int _A1, uint8 _isVRC4, uint8 _useRepeatBit, uint8 defaultWRAMSizeKiB) {
	VRC24_Sync =_sync;
	VRC24_A0 =_A0;
	VRC24_A1 =_A1;
	VRC24_isVRC4 =_isVRC4;
	VRC24_useRepeatBit =_useRepeatBit;
	VRC24_GetPRGBank =VRC24_getPRGBank;
	VRC24_GetCHRBank =VRC24_getCHRBank;
	VRC24_WRAMRead =NULL;
	VRC24_WRAMWrite =NULL;
	VRC24_ExternalSelect =NULL;
	info->Power =VRC24_power;
	info->Close =VRC24_close;
	GameStateRestore =StateRestore;
	AddExState(VRC24_stateRegs, ~0, 0, 0);
	if (VRC24_isVRC4) {
		MapIRQHook =VRC4_cpuCycle;
		AddExState(VRC4_stateRegs, ~0, 0, 0);
	} else {
		AddExState(VRC2_stateRegs, ~0, 0, 0);
	}
	
	WRAMSize =info->iNES2? (info->PRGRamSize +info->PRGRamSaveSize): (defaultWRAMSizeKiB *1024);
	if (WRAMSize) {
		WRAM =(uint8*)FCEU_gmalloc(WRAMSize);
		SetupCartPRGMapping(0x10, WRAM, WRAMSize, 1);
		AddExState(WRAM, WRAMSize, 0, "WRAM");
		if (info->battery) {
			info->SaveGame[0] =WRAM;
			info->SaveGameLen[0] =WRAMSize;
=======
	if (UNIFchrrama) {
		setchr8(0);
	}
}

static void StateRestore(int version) {
	VRC24_FixPRG();
	VRC24_FixCHR();
	VRC24_FixMIR();
}

void VRC24_Close(void) {
}

void VRC24_Init(CartInfo *info, uint8 vrc4, uint32 A0, uint32 A1, int wram, int irqRepeated) {
	VRC24_FixPRG = GENFIXPRG;
	VRC24_FixCHR = GENFIXCHR;
	VRC24_FixMIR = GENFIXMIR;

	VRC24_pwrap = GENPWRAP;
	VRC24_cwrap = GENCWRAP;
	VRC24_miscWrite = NULL;

	vrc2and4_A0 = A0;
	vrc2and4_A1 = A1;
	vrc2and4_VRC4 = vrc4;

	if (wram) {
		if (info->iNES2) {
			WRAMSIZE = info->PRGRamSize + info->PRGRamSaveSize;
		} else {
			WRAMSIZE = 8192;
		}
		if (WRAMSIZE) {
			WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
			SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
			AddExState(WRAM, WRAMSIZE, 0, "WRAM");

			if (info->battery) {
				info->SaveGame[0] = WRAM;
				info->SaveGameLen[0] = WRAMSIZE;
			}
>>>>>>> 8bc4f46 (Update libretro.c)
		}
	}
<<<<<<< HEAD
=======

<<<<<<< HEAD
	AddExState(&StateRegs, ~0, 0, 0);
}

void Mapper21_Init(CartInfo *info) {
	isPirate = 0;
	is22 = 0;
	info->Power = M21Power;
	VRC24_Init(info, 1);
}

void Mapper22_Init(CartInfo *info) {
	isPirate = 0;
	is22 = 1;
	info->Power = M22Power;
	VRC24_Init(info, 0);
}

void Mapper23_Init(CartInfo *info) {
	isPirate = 0;
	is22 = 0;
	info->Power = M23Power;
	VRC24_Init(info, 1);
}

void Mapper25_Init(CartInfo *info) {
	isPirate = 0;
	is22 = 0;
	info->Power = M25Power;
	VRC24_Init(info, 1);
}

void UNLT230_Init(CartInfo *info) {
	isPirate = 1;
	is22 = 0;
	info->Power = M23Power;
	VRC24_Init(info, 1);
}

/* -------------------- UNL-TH2131-1 -------------------- */
/* https://wiki.nesdev.com/w/index.php/NES_2.0_Mapper_308
 * NES 2.0 Mapper 308 is used for a bootleg version of the Sunsoft game Batman
 * similar to Mapper 23 Submapper 3) with custom IRQ functionality.
 * UNIF board name is UNL-TH2131-1.
 */

static void TH2131Write(uint32 A, uint8 V) {
	switch (A & 0xF003) {
	case 0xF000: X6502_IRQEnd(FCEU_IQEXT); IRQa = 0; IRQCount = 0; break;
	case 0xF001: IRQa = 1; break;
	case 0xF003: IRQLatch = (V & 0xF0) >> 4; break;
	}
}

void TH2131IRQHook(int a) {
	int count;

	if (!IRQa)
		return;

	for (count = 0; count < a; count++) {
		IRQCount++;
		if ((IRQCount & 0x0FFF) == 2048)
			IRQLatch--;
		if (!IRQLatch && (IRQCount & 0x0FFF) < 2048)
			X6502_IRQBegin(FCEU_IQEXT);
	}
}

static void TH2131Power(void) {
	VRC24PowerCommon(VRC24Write);
	SetWriteHandler(0xF000, 0xFFFF, TH2131Write);
}

void UNLTH21311_Init(CartInfo *info) {
	info->Power = TH2131Power;
	VRC24_Init(info, 1);
	MapIRQHook = TH2131IRQHook;
}

/* -------------------- UNL-KS7021A -------------------- */
/* http://wiki.nesdev.com/w/index.php/NES_2.0_Mapper_525
 * NES 2.0 Mapper 525 is used for a bootleg version of versions of Contra and 月風魔伝 (Getsu Fūma Den).
 * Its similar to Mapper 23 Submapper 3) with non-nibblized CHR-ROM bank registers.
 */

static void KS7021AWrite(uint32 A, uint8 V) {
	switch (A & 0xB000) {
	case 0xB000: chrreg[A & 0x07] = V; Sync(); break;
	}
}

static void KS7021APower(void) {
	VRC24PowerCommon(VRC24Write);
	SetWriteHandler(0xB000, 0xBFFF, KS7021AWrite);
}

void UNLKS7021A_Init(CartInfo *info) {
	info->Power = KS7021APower;
	VRC24_Init(info, 1);
}

/* -------------------- BTL-900218 -------------------- */
/* http://wiki.nesdev.com/w/index.php/UNIF/900218
 * NES 2.0 Mapper 524 describes the PCB used for the pirate port Lord of King or Axe of Fight.
 * UNIF board name is BTL-900218.
 */

static void BTL900218Write(uint32 A, uint8 V) {
	switch (A & 0xF00C) {
	case 0xF008: IRQa = 1; break;
	case 0xF00C: X6502_IRQEnd(FCEU_IQEXT); IRQa = 0; IRQCount = 0; break;
	}
}

void BTL900218IRQHook(int a) {
	if (!IRQa)
		return;

	IRQCount += a;
	if (IRQCount & 1024)
		X6502_IRQBegin(FCEU_IQEXT);
}

static void BTL900218Power(void) {
	VRC24PowerCommon(VRC24Write);
	SetWriteHandler(0xF000, 0xFFFF, BTL900218Write);
}

void BTL900218_Init(CartInfo *info) {
	info->Power = BTL900218Power;
	VRC24_Init(info, 1);
	MapIRQHook = BTL900218IRQHook;
>>>>>>> bc0be5f (Update libretro.c)
=======
	AddExState(StateRegs, ~0, 0, NULL);

	info->Power = VRC24_Power;
	info->Close = VRC24_Close;
	GameStateRestore = StateRestore;

	VRCIRQ_Init(irqRepeated);
	MapIRQHook = VRCIRQ_CPUHook;
	AddExState(&VRCIRQ_StateRegs, ~0, 0, 0);
>>>>>>> 8bc4f46 (Update libretro.c)
}
