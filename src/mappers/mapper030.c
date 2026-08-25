/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2014 CaitSith2, 2022 Cluster
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
 * Roms still using NES 1.0 format should be loaded as 8K CHR RAM.
 * Roms defined under NES 2.0 should use the VRAM size field, defining 7, 8 or 9, based on how much VRAM should be
 * present. UNIF doesn't have this problem, because unique board names can define this information. The UNIF names are
 * UNROM-512-8K, UNROM-512-16K and UNROM-512-32K
 *
 * The battery flag in the NES header enables flash,  Mirrror mode 2 Enables MI_0 and MI_1 mode.
 * Known games to use this board are:
 *    Battle Kid 2: Mountain of Torment (512K PRG, 8K CHR RAM, Horizontal Mirroring, Flash disabled)
 *    Study Hall (128K PRG (in 512K flash chip), 8K CHR RAM, Horizontal Mirroring, Flash enabled)
 *    Nix: The Paradox Relic (512 PRG, 8K CHR RAM, Vertical Mirroring, Flash enabled)
 * Although Xmas 2013 uses a different board, where LEDs can be controlled (with writes to the $8000-BFFF space),
 * it otherwise functions identically.
  */

#include "mapinc.h"
#include "latch.h"
#include "flashrom.h"

<<<<<<< HEAD
/* Workaround for Libretro API compatibility */
#define ROM_size_max                32
#define flashdata_size          (ROM_size_max * 0x4000)
#define flash_write_count_size  (ROM_size_max * 4 * sizeof(uint32_t))
static uint8_t fceumm_flash_buf[flashdata_size + flash_write_count_size];
static uint32_t fceumm_flash_buf_size = sizeof(fceumm_flash_buf);

static uint8_t submapper;
static uint8_t latche, latcheinit, bus_conflict, chrram_mask, software_id=0;
static uint16_t latcha;
static uint8_t *flashdata = fceumm_flash_buf + flash_write_count_size;
static uint32_t *flash_write_count = (uint32_t*)fceumm_flash_buf;
static uint8_t *FlashPage[32];
/* static uint32_t *FlashWriteCountPage[32]; */
/* static uint8_t flashloaded = 0; */

static uint8_t flash_save = 0, flash_state = 0, flash_mode = 0, flash_bank;
static void (*WLSync)(void);
static void (*WHSync)(void);

static INLINE void setfpageptr(int s, uint32_t A, uint8_t *p) {
	uint32_t AB = A >> 11;
	int x;

	if (p)
		for (x = (s >> 1) - 1; x >= 0; x--)
			FlashPage[AB + x] = p - A;
	else
		for (x = (s >> 1) - 1; x >= 0; x--)
			FlashPage[AB + x] = 0;
}

static void setfprg16(uint32_t A, uint32_t V) {
	if (PRGsize[0] >= 16384) {
		V &= PRGmask16[0];
		setfpageptr(16, A, flashdata ? (&flashdata[V << 14]) : 0);
	} else {
		int x;
		uint32_t VA = V << 3;

		for (x = 0; x < 8; x++)
			setfpageptr(2, A + (x << 11), flashdata ? (&flashdata[((VA + x) & PRGmask2[0]) << 11]) : 0);
	}
}

/* The flash write count region (the first flash_write_count_size bytes of
 * fceumm_flash_buf) is exposed verbatim to the frontend as battery save
 * RAM. To keep .srm files portable between LE and BE builds, we always
 * store the counters as little-endian on disk; on BE hosts that means
 * byte-swapping at every counter access. */
static INLINE uint32_t fwc_load(uint32_t idx) {
#ifdef MSB_FIRST
	uint8_t *p = (uint8_t *)&flash_write_count[idx];
	return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
#else
	return flash_write_count[idx];
#endif
}

static INLINE void fwc_store(uint32_t idx, uint32_t v) {
#ifdef MSB_FIRST
	uint8_t *p = (uint8_t *)&flash_write_count[idx];
	p[0] = (uint8_t)v;
	p[1] = (uint8_t)(v >> 8);
	p[2] = (uint8_t)(v >> 16);
	p[3] = (uint8_t)(v >> 24);
#else
	flash_write_count[idx] = v;
#endif
}

static void inc_flash_write_count(uint8_t bank, uint32_t A) {
	uint32_t idx = (bank * 4) + ((A & 0x3000) >> 12);
	uint32_t v = fwc_load(idx) + 1;
	if (v == 0) v = 1;	/* avoid wrap to 0 (which means "never written") */
	fwc_store(idx, v);
}

static uint32_t GetFlashWriteCount(uint8_t bank, uint32_t A) {
	return fwc_load((bank * 4) + ((A & 0x3000) >> 12));
}

static void StateRestore(int version) {
	/* erase_a/d/b[] are 5-element arrays indexed by flash_state.
	 * Legitimate values are 0..4; clamp savestate values that exceed
	 * the array to a safe value to avoid OOB reads. */
	if (flash_state >= 5)
		flash_state = 0;
	if (flash_mode > 2)
		flash_mode = 0;
	WHSync();
}

static void UNROM512LLatchWrite(uint32 A, uint8 V) {
	latche = V;
	latcha = A;
	WLSync();
}

static void UNROM512HLatchWrite(uint32 A, uint8 V) {
	if (bus_conflict)
		latche = (V == CartBR(A)) ? V : 0;
	else
		latche = V;
	latcha = A;
	WHSync();
}

<<<<<<< HEAD
static DECLFR(UNROM512LatchRead) {
	uint8_t flash_id[3] = { 0xB5, 0xB6, 0xB7 };
=======
static uint8 UNROM512LatchRead(uint32 A) {
	uint8 flash_id[3] = { 0xB5, 0xB6, 0xB7 };
>>>>>>> f9553c43 (Update ppu.c)
	if (software_id) {
		if (A & 1)
			return flash_id[ROM_size >> 4];
		return 0xBF;
=======
#define ROM_CHIP   0x00
#define FLASH_CHIP 0x10

<<<<<<< HEAD
static uint8 submapper;
static uint8 latche, latcheinit, bus_conflict, chrram_mask, software_id=0;
static uint16 latcha;
static uint8 *flashdata = fceumm_flash_buf + flash_write_count_size;
static uint32 *flash_write_count = (uint32*)fceumm_flash_buf;
static uint8 *FlashPage[32];
/* static uint32 *FlashWriteCountPage[32]; */
/* static uint8 flashloaded = 0; */
=======
static uint8 flash_save;
static uint8 *flash_data;
>>>>>>> 66cc868 (Update ppu.c)

static void M030Sync(void) {
	int chip = flash_save ? FLASH_CHIP : ROM_CHIP;

	setprg16r(chip, 0x8000, latch.data & 0x1F);
	setprg16r(chip, 0xC000, ~0);
	setchr8((latch.data >> 5) & 0x03);
	switch (iNESCart.submapper) {
	case 1:
		/* Mega Man II (30th Anniversary Edition) */
		setmirror((latch.data >> 7) & 0x01);	
		break;
	default:
		setmirror(MI_0 + ((latch.data >> 7) & 0x01));
		break;
	}
}

static void M030CPUHook(int a) {
	FlashROM_CPUCyle(a);
}

static DECLFR(M030Read) {
	if ((A < 0xC000) && flash_save) {
		return FlashROM_Read(A);
	}
	return CartBR(A);
}

static DECLFW(M030Write) {
	if ((A < 0xC000) && flash_save) {
		FlashROM_Write(A, V);
	} else {
		Latch_Write(A, V);
	}
}

static void M030Power(void) {
	Latch_Power();
	SetReadHandler(0x8000, 0xFFFF, M030Read);
	SetWriteHandler(0x8000, 0xBFFF, M030Write);
}

static void M030Close(void) {
	Latch_Close();
	if (flash_data) {
		FCEU_gfree(flash_data);
>>>>>>> e98261e5 (Update ppu.c)
	}
	flash_data = NULL;
}

<<<<<<< HEAD
static void UNROM512LatchPower(void) {
	latche = latcheinit;
	WHSync();
	SetReadHandler(0x8000, 0xFFFF, UNROM512LatchRead);
	if (submapper == 0 && !flash_save || submapper == 2)
		SetWriteHandler(0x8000, 0xFFFF, UNROM512HLatchWrite);
	else
	{
		if (submapper != 4) SetWriteHandler(0x8000, 0xBFFF, UNROM512LLatchWrite);
		SetWriteHandler(0xC000, 0xFFFF, UNROM512HLatchWrite);
	}
}

static void UNROM512LatchClose(void) { }

static void UNROM512LSync(void) {
	int erase_a[5] = { 0x9555, 0xAAAA, 0x9555, 0x9555, 0xAAAA };
	int erase_d[5] = { 0xAA, 0x55, 0x80, 0xAA, 0x55 };
	int erase_b[5] = { 1, 0, 1, 1, 0 };

	if (flash_mode==0) {
		if ((latcha == erase_a[flash_state]) && (latche == erase_d[flash_state]) && (flash_bank == erase_b[flash_state])) {
			flash_state++;
			if (flash_state == 5)
				flash_mode = 1;
		}
		else if ((flash_state == 2) && (latcha == 0x9555) && (latche == 0xA0) && (flash_bank == 1)) {
			flash_state++;
			flash_mode = 2;
		}
		else if ((flash_state == 2) && (latcha == 0x9555) && (latche == 0x90) && (flash_bank == 1)) {
			flash_state = 0;
			software_id = 1;
		} else {
			if (latche == 0xF0)
				software_id = 0;
			flash_state = 0;
		}
	}
	else if (flash_mode == 1) {	/* Chip Erase or Sector Erase */
		if (latche == 0x30) {
			inc_flash_write_count(flash_bank,latcha);
			memset(&FlashPage[(latcha & 0xF000) >> 11][latcha & 0xF000], 0xFF, 0x1000);
		}
		else if (latche == 0x10) {
			uint32_t i;
			for(i = 0; i < (ROM_size * 4); i++)
				inc_flash_write_count(i >> 2,i << 12);
			memset(flashdata, 0xFF, ROM_size * 0x4000);	/* Erasing the rom chip as instructed. Crash rate calulated to be 99.9% :) */
		}
		flash_state = 0;
		flash_mode = 0;
	}
	else if (flash_mode == 2) {	/* Byte Program */
		if (!GetFlashWriteCount(flash_bank, latcha)) {
			inc_flash_write_count(flash_bank, latcha);
			memcpy(&FlashPage[(latcha & 0xF000) >> 11][latcha & 0xF000], &Page[(latcha & 0xF000) >> 11][latcha & 0xF000], 0x1000);
		}
		FlashPage[latcha >> 11][latcha] &= latche;
		flash_state = 0;
		flash_mode = 0;
	}
}

static void UNROM512HSync(void) {
	flash_bank=latche&(ROM_size - 1);

	setprg16(0x8000, flash_bank);
	setprg16(0xc000, ~0);
	setfprg16(0x8000, flash_bank);
	setfprg16(0xC000, ~0);
	setchr8r(0, (latche & chrram_mask) >> 5);
	if (submapper == 3)
		setmirror(latche &0x80? MI_V: MI_H);
	else
		setmirror(MI_0 + (latche >> 7));
}

void UNROM512_Init(CartInfo *info) {
	int mirror;
	submapper = info->submapper;
	memset(fceumm_flash_buf, 0x00, fceumm_flash_buf_size);
	flash_state = 0;
	flash_bank = 0;
=======
void Mapper030_Init(CartInfo *info) {
>>>>>>> 66cc868 (Update ppu.c)
	flash_save = info->battery;
	Latch_Init(info, M030Sync, NULL, 0, !flash_save);

	if (!info->submapper && (info->PRGCRC32 == 0x891C14BC)) {
		info->submapper = 0x01;
	}

<<<<<<< HEAD
	mirror = (head.ROM_type & 1) | ((head.ROM_type & 8) >> 2);
	if (submapper == 3) /* Mega Man II (30th Anniversary Edition): switchable H/V */
		SetupCartMirroring(MI_V, 0, NULL);
	else
	switch (mirror) {
	case 0: /* hard horizontal, internal */
		SetupCartMirroring(MI_H, 1, NULL);
		break;
	case 1: /* hard vertical, internal */
		SetupCartMirroring(MI_V, 1, NULL);
		break;
	case 2: /* switchable 1-screen, internal (flags: 4-screen + horizontal) */
		SetupCartMirroring(MI_0, 0, NULL);
		break;
	case 3: /* hard four screen, last 8k of 32k RAM (flags: 4-screen + vertical) */
		SetupCartMirroring(4, 1, VROM + (info->CHRRamSize - 8192));
		break;
	}
	bus_conflict = submapper == 0 && !info->battery || submapper == 2;
	latcheinit = 0;
	WLSync = UNROM512LSync;
	WHSync = UNROM512HSync;
	info->Power = UNROM512LatchPower;
	info->Close = UNROM512LatchClose;
	GameStateRestore = StateRestore;
	if (flash_save)
	{
		info->SaveGame[0] = fceumm_flash_buf;
		info->SaveGameLen[0] = fceumm_flash_buf_size;
		AddExState(flash_write_count,ROM_size * 4 * sizeof(uint32_t), 0, "FLASH_WRITE_COUNT");
		AddExState(flashdata,ROM_size * 0x4000, 0, "FLASH_DATA");
		AddExState(&flash_state, 1, 0, "FLASH_STATE");
		AddExState(&flash_mode, 1, 0, "FLASH_MODE");
		AddExState(&flash_bank, 1, 0, "FLASH_BANK");
<<<<<<< HEAD
		AddExState(&latcha, 2, 1, "LATA");
=======
		AddExState(&latcha, 2, 0, "LATA");
=======
	if (!(info->submapper & 1)) {
		switch (info->mirror2bits) {
		case 0: /* hard horizontal, internal */
			SetupCartMirroring(MI_H, 1, NULL);
			break;
		case 1: /* hard vertical, internal */
			SetupCartMirroring(MI_V, 1, NULL);
			break;
		case 2: /* switchable 1-screen, internal (flags: 4-screen + horizontal) */
			SetupCartMirroring(MI_0, 0, NULL);
			break;
		case 3: /* hard four screen, last 8k of 32k RAM (flags: 4-screen + vertical) */
			SetupCartMirroring(4, 1, ROM.chr.data + (info->CHRRamSize - 8192));
			break;
		}
	}

	info->Power = M030Power;
	info->Close = M030Close;

	if (flash_save) {
		uint32 i, ssize;
		/* Allocate memory for flash */
		ssize = PRGsize[0];
		flash_data = (uint8 *)FCEU_gmalloc(ssize);
		/* Copy ROM to flash data */
		for (i = 0; i < ssize; i++) {
			flash_data[i] = PRGptr[ROM_CHIP][i % ssize];
		}
		SetupCartPRGMapping(FLASH_CHIP, flash_data, ssize, 1);
		AddExState(flash_data, ssize, 0, "FLSH");
		info->SaveGame[0] = flash_data;
		info->SaveGameLen[0] = ssize;

		FlashROM_Init(flash_data, ssize, 0xBF, 0xB7, 4096, 0x5555, 0x2AAA);
		MapIRQHook = M030CPUHook;
>>>>>>> 66cc868 (Update ppu.c)
>>>>>>> e98261e5 (Update ppu.c)
	}
}
