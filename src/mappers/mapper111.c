<<<<<<< HEAD
<<<<<<< HEAD
/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
=======
/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
 *  Copyright (C) 2023-2024 negativeExponent
>>>>>>> 5926d713 (Update libretro.c)
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

/* mapper 111 - Cheapocabra board by Memblers
 * http://forums.nesdev.com/viewtopic.php?p=146039
 *
 * 512k PRG-ROM in 32k pages (flashable if battery backed is specified)
 * 32k CHR-RAM used as:
 *     2 x 8k pattern pages
 *     2 x 8k nametable pages
 *
 * Notes:
 * - CHR-RAM for nametables maps to $3000-3FFF as well, but FCEUX internally mirrors to 4k?
 */

#include "mapinc.h"
<<<<<<< HEAD

static uint8_t reg;
static uint8_t *CHRRAM = NULL;
static uint32_t CHRRAMSIZE;

static uint8_t flash = 0;
static uint8_t flash_mode;
static uint8_t flash_sequence;
static uint8_t flash_id;
static uint8_t *FLASHROM = NULL;
static uint32_t FLASHROMSIZE;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REG" },
	{ 0 }
};

static SFORMAT FlashRegs[] = {
	{ &flash_mode, 1, "FMOD" },
	{ &flash_sequence, 1, "FSEQ" },
	{ &flash_id, 1, "FMID" },
	{ 0 }
};

static void Sync(void) {
	/* bit 7 controls green LED */
	/* bit 6 controls red LED   */
	uint32_t prg_chip = flash ? 0x10 : 0;
	int nt = (reg & 0x20) ? 8192 : 0; /* bit 5 controls 8k nametable page */
	int chr = (reg & 0x10) ? 1 : 0; /* bit 4 selects 8k CHR page        */
	int prg = (reg & 0x0F); /* bits 0-3 select 32k PRG page     */
	int n = 0;

	nt += (16 * 1024);
	for (n = 0; n < 4; ++n) {
		setntamem(CHRRAM + nt + (1024 * n), 1, n);
	}
	setchr8r(0x10, chr);    
	setprg32r(prg_chip, 0x8000, prg);
}

static DECLFW(M111Write) {
	if ((A >= 0x5000 && A <= 0x5FFF) || (A >= 0x7000 && A <= 0x7FFF)) {
		reg = V;
		Sync();
	}
}

static DECLFR(M111FlashID) {
	/* Software ID mode is undefined by the datasheet for all but the lowest 2 addressable bytes,
	 * but some tests of the chip currently being used found it repeats in 512-byte patterns.
	 * http://forums.nesdev.com/viewtopic.php?p=178728#p178728
	 */

	uint32_t aid = A & 0x1FF;
	switch (aid) {
	case 0:
		return 0xBF;
	case 1:
		return 0xB7;
	default:
		return 0xFF;
	}
}

static void M111FlashIDEnter() {
	if (flash_id)
		return;
	flash_id = 1;
	SetReadHandler(0x8000, 0xFFFF, M111FlashID);
}

static void M111FlashIDExit() {
	if (!flash_id)
		return;
	flash_id = 0;
	SetReadHandler(0x8000, 0xFFFF, CartBR);
}

enum {
    FLASH_MODE_READY = 0,
    FLASH_MODE_COMMAND,
    FLASH_MODE_BYTE_WRITE,
    FLASH_MODE_ERASE,
};

static DECLFW(M111Flash) {
	uint32_t flash_addr = 0;
	uint32_t command_addr = 0;

	if (A < 0x8000 || A > 0xFFFF)
		return;

	flash_addr = ((reg & 0x0F) << 15) | (A & 0x7FFF);
	command_addr = flash_addr & 0x7FFF;

	switch (flash_mode) {
	default:
	case FLASH_MODE_READY:
		if (command_addr == 0x5555 && V == 0xAA) {
			flash_mode = FLASH_MODE_COMMAND;
			flash_sequence = 0;
		} else if (V == 0xF0) {
			M111FlashIDExit();
		}
		break;
	case FLASH_MODE_COMMAND:
		if (flash_sequence == 0) {
			if (command_addr == 0x2AAA && V == 0x55)
				flash_sequence = 1;
			else
				flash_mode = FLASH_MODE_READY;
		} else if (flash_sequence == 1) {
			if (command_addr == 0x5555) {
				flash_sequence = 0;
				switch (V) {
				default:
					flash_mode = FLASH_MODE_READY;
					break;
				case 0xA0:
					flash_mode = FLASH_MODE_BYTE_WRITE;
					break;
				case 0x80:
					flash_mode = FLASH_MODE_ERASE;
					break;
				case 0x90:
					M111FlashIDEnter();
					flash_mode = FLASH_MODE_READY;
					break;
				case 0xF0:
					M111FlashIDExit();
					flash_mode = FLASH_MODE_READY;
					break;
				}
			} else
				flash_mode = FLASH_MODE_READY;
		} else
			flash_mode = FLASH_MODE_READY; /* should be unreachable */
		break;
	case FLASH_MODE_BYTE_WRITE:
		FLASHROM[flash_addr] &= V;
		flash_mode = FLASH_MODE_READY;
		break;
	case FLASH_MODE_ERASE:
		if (flash_sequence == 0) {
			if (command_addr == 0x5555 && V == 0xAA)
				flash_sequence = 1;
			else
				flash_mode = FLASH_MODE_READY;
		} else if (flash_sequence == 1) {
			if (command_addr == 0x2AAA && V == 0x55)
				flash_sequence = 2;
			else
				flash_mode = FLASH_MODE_READY;
		} else if (flash_sequence == 2) {
			if (command_addr == 0x5555 && V == 0x10) /* erase chip */
			{
				memset(FLASHROM, 0xFF, FLASHROMSIZE);
			} else if (V == 0x30) /* erase 4k sector */
			{
				uint32_t sector = flash_addr & 0x7F000;
				memset(FLASHROM + sector, 0xFF, 1024 * 4);
			}
			flash_mode = FLASH_MODE_READY;
		} else
			flash_mode = FLASH_MODE_READY; /* should be unreachable */
		break;
	}
=======
#include "flashrom.h"
#include "mmc1.h"

static uint8 reg;

static uint8 *FLASHROM = NULL;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ 0 }
};

static void M111MMC1PW(uint16 A, uint16 V) {
	setprg16(A, V & 0x0F);
}

static void M111MMC1CW(uint16 A, uint16 V) {
	setchr4(A, V & 0x3F);
}

static DECLFW(M111Write_mmc1) {
	mmc1.reg[(A >> 13) & 0x03] =  V;
	MMC1_FixPRG();
	MMC1_FixCHR();
	MMC1_FixMIR();
}

static void M111Power_mmc1(void) {
	MMC1_Power();
	SetWriteHandler(0x8000, 0xFFFF, M111Write_mmc1);
}

#define VRAM_OFFSET(x) (0x4000 + (0x400 * (x)))

static void Sync(void) {
	/* 7  bit  0
	 * ---- ----
	 * GRNC PPPP
	 * |||| ||||
	 * |||| ++++- Select 32 KB PRG ROM bank for CPU $8000-$FFFF
	 * |||+------ Select 8 KB CHR RAM bank for PPU $0000-$1FFF
	 * ||+------- Select 8 KB nametable for PPU $2000-$3EFF
	 * |+-------- Red LED - 0=On; 1=Off
	 * +--------- Green LED - 0=On; 1=Off */
	int nt  = (reg & 0x20) >> 5;
	int chr = (reg & 0x10) >> 4;
	int prg = (reg & 0x0F);

	setprg32r(FLASHROM ? 0x10 : 0, 0x8000, prg);
	setchr8(chr);
	setntamem(CHRptr[0] + VRAM_OFFSET(0) + (nt << 13), 1, 0);
	setntamem(CHRptr[0] + VRAM_OFFSET(1) + (nt << 13), 1, 1);
	setntamem(CHRptr[0] + VRAM_OFFSET(2) + (nt << 13), 1, 2);
	setntamem(CHRptr[0] + VRAM_OFFSET(3) + (nt << 13), 1, 3);
}

static DECLFR(M111ReadOB) {
	reg = cpu.openbus;
	Sync();
	return reg;
}

static DECLFW(M111WriteReg) {
	reg = V;
	Sync();
}

static DECLFR(M111ReadFlash) {
	return FlashROM_Read(A);
}

static DECLFW(M111WriteFlash) {
	FlashROM_Write(A, V);
}

static void M111CPUCycle(int a) {
	FlashROM_CPUCyle(a);
>>>>>>> 5926d713 (Update libretro.c)
}

static void M111Power(void) {
	reg = 0xFF;
	Sync();
<<<<<<< HEAD
	SetReadHandler(0x8000, 0xffff, CartBR);
	SetWriteHandler(0x5000, 0x5fff, M111Write);
	SetWriteHandler(0x7000, 0x7fff, M111Write);

	if (flash) {
		flash_mode = 0;
		flash_sequence = 0;
		flash_id = 0;
		SetWriteHandler(0x8000, 0xFFFF, M111Flash);
	}
}

static void M111Close(void) {
	if (CHRRAM)
		FCEU_gfree(CHRRAM);
	CHRRAM = NULL;

	if (FLASHROM)
		FCEU_gfree(FLASHROM);
=======

	SetReadHandler(0x5000, 0x5FFF, M111ReadOB);
	SetReadHandler(0x7000, 0x7FFF, M111ReadOB);

	SetWriteHandler(0x5000, 0x5FFF, M111WriteReg);
	SetWriteHandler(0x7000, 0x7FFF, M111WriteReg);

	SetReadHandler(0x8000, 0xFFFF, M111ReadFlash);
	SetWriteHandler(0x8000, 0xFFFF, M111WriteFlash);
}

static void M111Close(void) {
	if (FLASHROM) {
		FCEU_gfree(FLASHROM);
	}
>>>>>>> 5926d713 (Update libretro.c)
	FLASHROM = NULL;
}

static void StateRestore(int version) {
<<<<<<< HEAD
	Sync();
}

void Mapper111_Init(CartInfo* info) {
	info->Power = M111Power;
	info->Close = M111Close;

	CHRRAMSIZE = 1024 * 32;
	CHRRAM = (uint8_t*)FCEU_gmalloc(CHRRAMSIZE);
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);

	GameStateRestore = StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
	AddExState(CHRRAM, CHRRAMSIZE, 0, "CRAM");

	flash = (info->battery != 0);
	if (flash) {
		uint32_t PRGSIZE = 0;
		uint32_t w = 0;
		uint32_t r = 0;

		FLASHROMSIZE = 1024 * 512;
		FLASHROM = (uint8_t*)FCEU_gmalloc(FLASHROMSIZE);
		info->SaveGame[0] = FLASHROM;
		info->SaveGameLen[0] = FLASHROMSIZE;
		AddExState(FLASHROM, FLASHROMSIZE, 0, "FROM");
		AddExState(&FlashRegs, ~0, 0, 0);

		/* copy PRG ROM into FLASHROM, use it instead of PRG ROM */
		PRGSIZE = ROM_size * 16 * 1024;
		for (w = 0, r = 0; w < FLASHROMSIZE; ++w) {
			FLASHROM[w] = ROM[r];
			++r;
			if (r >= PRGSIZE)
				r = 0;
		}
		SetupCartPRGMapping(0x10, FLASHROM, FLASHROMSIZE, 0);
	}
}
=======
/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
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

/* mapper 111 - Cheapocabra board by Memblers
 * http://forums.nesdev.com/viewtopic.php?p=146039
 *
 * 512k PRG-ROM in 32k pages (flashable if battery backed is specified)
 * 32k CHR-RAM used as:
 *     2 x 8k pattern pages
 *     2 x 8k nametable pages
 *
 * Notes:
 * - CHR-RAM for nametables maps to $3000-3FFF as well, but FCEUX internally mirrors to 4k?
 */

#include "mapinc.h"

static uint8 reg;
static uint8 *CHRRAM = NULL;
static uint32 CHRRAMSIZE;

static uint8 flash = 0;
static uint8 flash_mode;
static uint8 flash_sequence;
static uint8 flash_id;
static uint8 *FLASHROM = NULL;
static uint32 FLASHROMSIZE;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REG" },
	{ 0 }
};

static SFORMAT FlashRegs[] = {
	{ &flash_mode, 1, "FMOD" },
	{ &flash_sequence, 1, "FSEQ" },
	{ &flash_id, 1, "FMID" },
	{ 0 }
};

static void Sync(void) {
	/* bit 7 controls green LED */
	/* bit 6 controls red LED   */
	uint32 prg_chip = flash ? 0x10 : 0;
	int nt = (reg & 0x20) ? 8192 : 0; /* bit 5 controls 8k nametable page */
	int chr = (reg & 0x10) ? 1 : 0; /* bit 4 selects 8k CHR page        */
	int prg = (reg & 0x0F); /* bits 0-3 select 32k PRG page     */
	int n = 0;

	nt += (16 * 1024);
	for (n = 0; n < 4; ++n) {
		setntamem(CHRRAM + nt + (1024 * n), 1, n);
	}
	setchr8r(0x10, chr);    
	setprg32r(prg_chip, 0x8000, prg);
}

static void M111Write(uint32 A, uint8 V) {
	if ((A >= 0x5000 && A <= 0x5FFF) || (A >= 0x7000 && A <= 0x7FFF)) {
		reg = V;
		Sync();
	}
}

static uint8 M111FlashID(uint32 A) {
	/* Software ID mode is undefined by the datasheet for all but the lowest 2 addressable bytes,
	 * but some tests of the chip currently being used found it repeats in 512-byte patterns.
	 * http://forums.nesdev.com/viewtopic.php?p=178728#p178728
	 */

	uint32 aid = A & 0x1FF;
	switch (aid) {
	case 0:
		return 0xBF;
	case 1:
		return 0xB7;
	default:
		return 0xFF;
	}
}

void M111FlashIDEnter() {
	if (flash_id)
		return;
	flash_id = 1;
	SetReadHandler(0x8000, 0xFFFF, M111FlashID);
}

void M111FlashIDExit() {
	if (!flash_id)
		return;
	flash_id = 0;
	SetReadHandler(0x8000, 0xFFFF, CartBR);
}

enum {
    FLASH_MODE_READY = 0,
    FLASH_MODE_COMMAND,
    FLASH_MODE_BYTE_WRITE,
    FLASH_MODE_ERASE
};

static void M111Flash(uint32 A, uint8 V) {
	uint32 flash_addr = 0;
	uint32 command_addr = 0;

	if (A < 0x8000 || A > 0xFFFF)
		return;

	flash_addr = ((reg & 0x0F) << 15) | (A & 0x7FFF);
	command_addr = flash_addr & 0x7FFF;

	switch (flash_mode) {
	default:
	case FLASH_MODE_READY:
		if (command_addr == 0x5555 && V == 0xAA) {
			flash_mode = FLASH_MODE_COMMAND;
			flash_sequence = 0;
		} else if (V == 0xF0) {
			M111FlashIDExit();
		}
		break;
	case FLASH_MODE_COMMAND:
		if (flash_sequence == 0) {
			if (command_addr == 0x2AAA && V == 0x55)
				flash_sequence = 1;
			else
				flash_mode = FLASH_MODE_READY;
		} else if (flash_sequence == 1) {
			if (command_addr == 0x5555) {
				flash_sequence = 0;
				switch (V) {
				default:
					flash_mode = FLASH_MODE_READY;
					break;
				case 0xA0:
					flash_mode = FLASH_MODE_BYTE_WRITE;
					break;
				case 0x80:
					flash_mode = FLASH_MODE_ERASE;
					break;
				case 0x90:
					M111FlashIDEnter();
					flash_mode = FLASH_MODE_READY;
					break;
				case 0xF0:
					M111FlashIDExit();
					flash_mode = FLASH_MODE_READY;
					break;
				}
			} else
				flash_mode = FLASH_MODE_READY;
		} else
			flash_mode = FLASH_MODE_READY; /* should be unreachable */
		break;
	case FLASH_MODE_BYTE_WRITE:
		FLASHROM[flash_addr] &= V;
		flash_mode = FLASH_MODE_READY;
		break;
	case FLASH_MODE_ERASE:
		if (flash_sequence == 0) {
			if (command_addr == 0x5555 && V == 0xAA)
				flash_sequence = 1;
			else
				flash_mode = FLASH_MODE_READY;
		} else if (flash_sequence == 1) {
			if (command_addr == 0x2AAA && V == 0x55)
				flash_sequence = 2;
			else
				flash_mode = FLASH_MODE_READY;
		} else if (flash_sequence == 2) {
			if (command_addr == 0x5555 && V == 0x10) /* erase chip */
			{
				memset(FLASHROM, 0xFF, FLASHROMSIZE);
			} else if (V == 0x30) /* erase 4k sector */
			{
				uint32 sector = flash_addr & 0x7F000;
				memset(FLASHROM + sector, 0xFF, 1024 * 4);
			}
			flash_mode = FLASH_MODE_READY;
		} else
			flash_mode = FLASH_MODE_READY; /* should be unreachable */
		break;
	}
}

static void M111Power(void) {
	reg = 0xFF;
	Sync();
	SetReadHandler(0x8000, 0xffff, CartBR);
	SetWriteHandler(0x5000, 0x5fff, M111Write);
	SetWriteHandler(0x7000, 0x7fff, M111Write);

	if (flash) {
		flash_mode = 0;
		flash_sequence = 0;
		flash_id = 0;
		SetWriteHandler(0x8000, 0xFFFF, M111Flash);
	}
}

static void M111Close(void) {
	if (CHRRAM)
		FCEU_gfree(CHRRAM);
	CHRRAM = NULL;

	if (FLASHROM)
		FCEU_gfree(FLASHROM);
	FLASHROM = NULL;
}

static void StateRestore(int version) {
	Sync();
}

void Mapper111_Init(CartInfo* info) {
	info->Power = M111Power;
	info->Close = M111Close;

	CHRRAMSIZE = 1024 * 32;
	CHRRAM = (uint8*)FCEU_gmalloc(CHRRAMSIZE);
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);

	GameStateRestore = StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
	AddExState(CHRRAM, CHRRAMSIZE, 0, "CRAM");

	flash = (info->battery != 0);
	if (flash) {
		uint32 PRGSIZE = 0;
		uint32 w = 0;
		uint32 r = 0;

		FLASHROMSIZE = 1024 * 512;
		FLASHROM = (uint8*)FCEU_gmalloc(FLASHROMSIZE);
		info->SaveGame[0] = FLASHROM;
		info->SaveGameLen[0] = FLASHROMSIZE;
		AddExState(FLASHROM, FLASHROMSIZE, 0, "FROM");
		AddExState(&FlashRegs, ~0, 0, 0);

		/* copy PRG ROM into FLASHROM, use it instead of PRG ROM */
		PRGSIZE = ROM_size * 16 * 1024;
		for (w = 0, r = 0; w < FLASHROMSIZE; ++w) {
			FLASHROM[w] = ROM[r];
			++r;
			if (r >= PRGSIZE)
				r = 0;
		}
		SetupCartPRGMapping(0x10, FLASHROM, FLASHROMSIZE, 0);
	}
}
>>>>>>> b8aebecd (Update libretro.c)
=======
	if (!UNIFchrrama) {
		MMC1_FixPRG();
		MMC1_FixCHR();
		MMC1_FixMIR();
		return;
	}

	Sync();
}

void Mapper111_Init(CartInfo *info) {
	if (!UNIFchrrama) {
		/* Prior to the introduction of GTROM, Mapper 111 was assigned to a Chinese Fan Translation
		 * of Ninja Ryukenden (Japanese Ninja Gaiden). This translation uses a non-serialized
		 * version of MMC1 and supports 256KiB of CHR-ROM, whereas the official MMC1 is limited to
		 * 128KiB */
		info->Power = M111Power_mmc1;
		MMC1_pwrap = M111MMC1PW;
		MMC1_cwrap = M111MMC1CW;
		return;
	}

	info->Power = M111Power;
	info->Close = M111Close;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	if (info->battery) {
		int fsize = PRGsize[0];

		FLASHROM = (uint8 *)FCEU_gmalloc(fsize);
		info->SaveGame[0] = FLASHROM;
		info->SaveGameLen[0] = fsize;
		AddExState(FLASHROM, fsize, 0, "FROM");
		memcpy(FLASHROM, PRGptr[0], fsize);
		SetupCartPRGMapping(0x10, FLASHROM, fsize, 0);

		FlashROM_Init(FLASHROM, fsize, 0xB7, 0xBF, 4096, 0x5555, 0x2AAA);
		MapIRQHook = M111CPUCycle;
	}
}
>>>>>>> 5926d713 (Update libretro.c)
