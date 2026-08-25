/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2007 CaH4e3
 *  Copyright (C) 2011 FCEUX team
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
 * INES Mapper 016
 * iNES Mapper 016 is used for some of the Bandai FCG boards, namely, boards with
 * the FCG-1 ASIC that supports no EEPROM, and the LZ93D50 ASIC with no or 256 bytes of EEPROM.
 *
 * INES Mapper 016 submapper table
 * Submapper #	Meaning											Note
 * 0			Unspecified										Emulate both FCG-1/2 and LZ93D50 chips in their respective CPU address ranges.
 * 1			LZ93D50 with 128 byte serial EEPROM (24C01)		Deprecated, use INES Mapper 159 instead.
 * 2			Datach 	Joint ROM System						Deprecated, use INES Mapper 157 instead.
 * 3			8 KiB of WRAM instead of serial EEPROM			Deprecated, use INES Mapper 153 instead.
 * 4			FCG-1/2											Responds only in the CPU $6000-$7FFF address range; IRQ counter is not latched.
 * 5			LZ93D50 with no or 256-byte serial EEPROM (24C02)		Responds only in the CPU $8000-$FFFF address range; IRQ counter is latched.
 *
 */

#include "mapinc.h"
#include "eeprom_x24c0x.h"
#include "bandai.h"

<<<<<<< HEAD
static uint8_t reg[16], is153, x24c02;
static uint8_t IRQa;
static int16_t IRQCount, IRQLatch;

<<<<<<< HEAD
static uint8_t *WRAM = NULL;
static uint32_t WRAMSIZE;
=======
static uint8 *WRAM = NULL;

#ifndef WRAM_SIZE
#define WRAM_SIZE 8192
#endif
>>>>>>> 99588a93 (Update libretro.c)
=======
EEPROM_TYPE eeprom_type = EEPROM_NONE;

static uint8 prg;
static uint8 chr[8];
static uint8 mirr;
static uint8 IRQa;
static int16 IRQCount, IRQLatch;

static uint8 isFCG12 = 0;

static uint8 eeprom[256];

void (*BANDAI_pwrap)(uint16 A, uint16 V);
void (*BANDAI_cwrap)(uint16 A, uint16 V);
>>>>>>> 7b06ddc4 (Update libretro.c)

static SFORMAT StateRegs[] =
{
	{ chr, 8, "CRGS" },
	{ &prg, 1, "PREG" },
	{ &mirr, 1, "MIRR" },
	{ &IRQa, 1, "IRQA" },
	{ &IRQCount, 2, "IRQC" },
	{ &IRQLatch, 2, "IRQL" },	/* need for Famicom Jump II - Saikyou no 7 Nin (J) [!] */
	{ 0 }
};

<<<<<<< HEAD
/* x24C0x interface */

#define X24C0X_STANDBY		0
#define X24C0X_ADDRESS		1
#define X24C0X_WORD			2
#define X24C0X_READ			3
#define X24C0X_WRITE		4

static uint8_t x24c0x_data[512];

static uint8_t x24c01_state;
static uint8_t x24c01_addr, x24c01_word, x24c01_latch, x24c01_bitcount;
static uint8_t x24c01_sda, x24c01_scl, x24c01_out;

static uint8_t x24c02_state;
static uint8_t x24c02_addr, x24c02_word, x24c02_latch, x24c02_bitcount;
static uint8_t x24c02_sda, x24c02_scl, x24c02_out;

static SFORMAT x24c01StateRegs[] =
{
	{ &x24c01_addr, 1, "ADDR" },
	{ &x24c01_word, 1, "WORD" },
	{ &x24c01_latch, 1, "LATC" },
	{ &x24c01_bitcount, 1, "BITC" },
	{ &x24c01_sda, 1, "SDA" },
	{ &x24c01_scl, 1, "SCL" },
	{ &x24c01_out, 1, "OUT" },
	{ &x24c01_state, 1, "STAT" },
	{ 0 }
};

static SFORMAT x24c02StateRegs[] =
{
	{ &x24c02_addr, 1, "ADDR" },
	{ &x24c02_word, 1, "WORD" },
	{ &x24c02_latch, 1, "LATC" },
	{ &x24c02_bitcount, 1, "BITC" },
	{ &x24c02_sda, 1, "SDA" },
	{ &x24c02_scl, 1, "SCL" },
	{ &x24c02_out, 1, "OUT" },
	{ &x24c02_state, 1, "STAT" },
	{ 0 }
};

static void x24c01_init(void) {
	x24c01_addr = x24c01_word = x24c01_latch = x24c01_bitcount = x24c01_sda = x24c01_scl = 0;
	x24c01_state = X24C0X_STANDBY;
=======
static void GENPWRAP(uint16 A, uint16 V) {
	setprg16(A, V & 0x0F);
>>>>>>> 7b06ddc4 (Update libretro.c)
}

static void GENCWRAP(uint16 A, uint16 V) {
	setchr1(A, V);
}

<<<<<<< HEAD
static void x24c01_write(uint8_t data) {
	uint8_t scl = (data >> 5) & 1;
	uint8_t sda = (data >> 6) & 1;

	if(x24c01_scl && scl) {
		if(x24c01_sda && !sda) {			/* START */
			x24c01_state = X24C0X_ADDRESS;
			x24c01_bitcount = 0;
			x24c01_addr = 0;
		} else if(!x24c01_sda && sda) {		/* STOP */
			x24c01_state = X24C0X_STANDBY;
		}
	} else if(!x24c01_scl && scl) {			/* RISING EDGE */
		switch(x24c01_state) {
		case X24C0X_ADDRESS:
			if(x24c01_bitcount < 7) {
				x24c01_addr <<= 1;
				x24c01_addr |= sda;
			} else {
				x24c01_word = x24c01_addr;
				if(sda) 					/* READ COMMAND */
					x24c01_state = X24C0X_READ;
				 else 						/* WRITE COMMAND */
					x24c01_state = X24C0X_WRITE;
			}
			x24c01_bitcount++;
			break;
		case X24C0X_READ:
			if (x24c01_bitcount == 8) {		/*  ACK */
				x24c01_out = 0;
				x24c01_latch = x24c0x_data[x24c01_word];
				x24c01_bitcount = 0;
			} else {						/* REAL OUTPUT */
				x24c01_out = x24c01_latch >> 7;
				x24c01_latch <<= 1;
				x24c01_bitcount++;
				if(x24c01_bitcount == 8) {
					x24c01_word++;
					x24c01_word &= 0xff;
				}
			}
			break;
		case X24C0X_WRITE:
			if (x24c01_bitcount == 8) {		/* ACK */
				x24c01_out = 0;
				x24c01_latch = 0;
				x24c01_bitcount = 0;
			} else {						/* REAL INPUT */
				x24c01_latch <<= 1;
				x24c01_latch |= sda;
				x24c01_bitcount++;
				if(x24c01_bitcount == 8) {
					x24c0x_data[x24c01_word] = x24c01_latch;
					x24c01_word++;
					x24c01_word &= 0xff;
				}
			}
			break;
		}
	}

	x24c01_sda = sda;
	x24c01_scl = scl;
}

static void x24c02_write(uint8_t data) {
	uint8_t scl = (data >> 5) & 1;
	uint8_t sda = (data >> 6) & 1;

	if (x24c02_scl && scl) {
		if (x24c02_sda && !sda) {			/* START */
			x24c02_state = X24C0X_ADDRESS;
			x24c02_bitcount = 0;
			x24c02_addr = 0;
		} else if (!x24c02_sda && sda) {	/* STOP */
			x24c02_state = X24C0X_STANDBY;
		}
	} else if (!x24c02_scl && scl) {		/* RISING EDGE */
		switch (x24c02_state) {
		case X24C0X_ADDRESS:
			if (x24c02_bitcount < 7) {
				x24c02_addr <<= 1;
				x24c02_addr |= sda;
			} else {
				if (sda)					/* READ COMMAND */
					x24c02_state = X24C0X_READ;
				else						/* WRITE COMMAND */
					x24c02_state = X24C0X_WORD;
			}
			x24c02_bitcount++;
			break;
		case X24C0X_WORD:
			if (x24c02_bitcount == 8) {		/* ACK */
				x24c02_word = 0;
				x24c02_out = 0;
			} else {						/* WORD ADDRESS INPUT */
				x24c02_word <<= 1;
				x24c02_word |= sda;
				if (x24c02_bitcount == 16) { /* END OF ADDRESS INPUT */
					x24c02_bitcount = 7;
					x24c02_state = X24C0X_WRITE;
				}
			}
			x24c02_bitcount++;
			break;
		case X24C0X_READ:
			if (x24c02_bitcount == 8) {		/* ACK */
				x24c02_out = 0;
				x24c02_latch = x24c0x_data[x24c02_word|0x100];
				x24c02_bitcount = 0;
			} else {						/* REAL OUTPUT */
				x24c02_out = x24c02_latch >> 7;
				x24c02_latch <<= 1;
				x24c02_bitcount++;
				if (x24c02_bitcount == 8) {
					x24c02_word++;
					x24c02_word &= 0xff;
				}
			}
			break;
		case X24C0X_WRITE:
			if (x24c02_bitcount == 8) {		/* ACK */
				x24c02_out = 0;
				x24c02_latch = 0;
				x24c02_bitcount = 0;
			} else {						/* REAL INPUT */
				x24c02_latch <<= 1;
				x24c02_latch |= sda;
				x24c02_bitcount++;
				if (x24c02_bitcount == 8) {
					x24c0x_data[x24c02_word|0x100] = x24c02_latch;
					x24c02_word++;
					x24c02_word &= 0xff;
				}
			}
			break;
		}
	}

	x24c02_sda = sda;
	x24c02_scl = scl;
=======
void BANDAI_FixPRG(void) {
	BANDAI_pwrap(0x8000, prg);
	BANDAI_pwrap(0xC000, ~0);
}

void BANDAI_FixCHR(void) {
	BANDAI_cwrap(0x0000, chr[0]);
	BANDAI_cwrap(0x0400, chr[1]);
	BANDAI_cwrap(0x0800, chr[2]);
	BANDAI_cwrap(0x0C00, chr[3]);
	BANDAI_cwrap(0x1000, chr[4]);
	BANDAI_cwrap(0x1400, chr[5]);
	BANDAI_cwrap(0x1800, chr[6]);
	BANDAI_cwrap(0x1C00, chr[7]);
>>>>>>> 7b06ddc4 (Update libretro.c)
}

void BANDAI_FixMIR(void) {
	switch (mirr & 0x03) {
	case 0: setmirror(MI_V); break;
	case 1: setmirror(MI_H); break;
	case 2: setmirror(MI_0); break;
	case 3: setmirror(MI_1); break;
	}
}

static DECLFR(BANDAI_Read) {
	if (eeprom_type == EEPROM_X24C01) {
		return (cpu.openbus & ~0x010) | (x24c01_read() << 4);
	}
	return (cpu.openbus & ~0x010) | (x24c02_read() << 4);
}

DECLFW(BANDAI_Write) {
	switch (A & 0x0F) {
	case 0x00:
	case 0x01:
	case 0x02:
	case 0x03:
	case 0x04:
	case 0x05:
	case 0x06:
	case 0x07:
		chr[A & 0x07] = V;
		BANDAI_FixCHR();
		break;
	case 0x08:
		prg = V;
		BANDAI_FixPRG();
		break;
	case 0x09:
		mirr = V;
		BANDAI_FixMIR();
		break;
	case 0x0A:
		IRQa = V & 1;
		IRQCount = IRQLatch;
		if (IRQa && !IRQLatch) {
			X6502_IRQBegin(FCEU_IQEXT);
		} else {
			X6502_IRQEnd(FCEU_IQEXT);
		}
		break;
	case 0x0B:
		IRQLatch &= 0xFF00;
		IRQLatch |= V;
		break;
	case 0x0C:
		IRQLatch &= 0x00FF;
		IRQLatch |= V << 8;
		break;
	case 0x0D:
		if (eeprom_type == EEPROM_X24C02) {
			x24c02_write(V);
		} else if (eeprom_type == EEPROM_X24C01) {
			x24c01_write(V);
		}
		break;
	}
}

void BANDAI_IRQHook(int a) {
	if (IRQa) {
		IRQCount -= a;
		if (IRQCount < 0) {
			X6502_IRQBegin(FCEU_IQEXT);
		}
	}
}

void BANDAI_Reset(void) {
	int x;

	prg = 0x00;
	for (x = 0; x < 8; x++) {
		chr[x] = x;
	}
	IRQCount = 0;
	IRQLatch = 0;
	IRQa = FALSE;

	BANDAI_FixPRG();
	BANDAI_FixCHR();
	BANDAI_FixMIR();
}

static void StateRestore(int version) {
	BANDAI_FixPRG();
	BANDAI_FixCHR();
	BANDAI_FixMIR();
}

void BANDAI_Power(void) {
	if (UNIFchrrama) {
		setchr8(0);
	}

	BANDAI_Reset();

	SetReadHandler(0x8000, 0xFFFF, CartBR);
<<<<<<< HEAD
	SetWriteHandler(0x8000, 0xFFFF, BandaiWrite);
	FCEU_CheatAddRAM(WRAM_SIZE >> 10, 0x6000, WRAM);
}


static void M153Close(void) {
	if (WRAM)
		FCEU_gfree(WRAM);
	WRAM = NULL;
}

void Mapper153_Init(CartInfo *info) {
	is153 = 1;
	info->Power = M153Power;
	info->Close = M153Close;
	MapIRQHook = BandaiIRQHook;

<<<<<<< HEAD
	WRAMSIZE = 8192;
	WRAM = (uint8_t*)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
=======
	WRAM = (uint8*)FCEU_gmalloc(WRAM_SIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAM_SIZE, 1);
	AddExState(WRAM, WRAM_SIZE, 0, "WRAM");
>>>>>>> 99588a93 (Update libretro.c)

	if (info->battery) {
		info->SaveGame[0] = WRAM;
		info->SaveGameLen[0] = WRAM_SIZE;
	}

	GameStateRestore = StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
}

/* Datach Barcode Battler */

static uint8_t BarcodeData[256];
static int BarcodeReadPos;
static int BarcodeCycleCount;
static uint32_t BarcodeOut;

/* #define INTERL2OF5 */

<<<<<<< HEAD
int FCEUI_DatachSet(uint8_t *rcode) {
=======
#if 0
static int FCEUI_DatachSet(uint8 *rcode) {
>>>>>>> f959f1b7 (Split up more mappers)
	int prefix_parity_type[10][6] = {
		{ 0, 0, 0, 0, 0, 0 }, { 0, 0, 1, 0, 1, 1 }, { 0, 0, 1, 1, 0, 1 }, { 0, 0, 1, 1, 1, 0 },
		{ 0, 1, 0, 0, 1, 1 }, { 0, 1, 1, 0, 0, 1 }, { 0, 1, 1, 1, 0, 0 }, { 0, 1, 0, 1, 0, 1 },
		{ 0, 1, 0, 1, 1, 0 }, { 0, 1, 1, 0, 1, 0 }
	};
	int data_left_odd[10][7] = {
		{ 0, 0, 0, 1, 1, 0, 1 }, { 0, 0, 1, 1, 0, 0, 1 }, { 0, 0, 1, 0, 0, 1, 1 }, { 0, 1, 1, 1, 1, 0, 1 },
		{ 0, 1, 0, 0, 0, 1, 1 }, { 0, 1, 1, 0, 0, 0, 1 }, { 0, 1, 0, 1, 1, 1, 1 }, { 0, 1, 1, 1, 0, 1, 1 },
		{ 0, 1, 1, 0, 1, 1, 1 }, { 0, 0, 0, 1, 0, 1, 1 }
	};
	int data_left_even[10][7] = {
		{ 0, 1, 0, 0, 1, 1, 1 }, { 0, 1, 1, 0, 0, 1, 1 }, { 0, 0, 1, 1, 0, 1, 1 }, { 0, 1, 0, 0, 0, 0, 1 },
		{ 0, 0, 1, 1, 1, 0, 1 }, { 0, 1, 1, 1, 0, 0, 1 }, { 0, 0, 0, 0, 1, 0, 1 }, { 0, 0, 1, 0, 0, 0, 1 },
		{ 0, 0, 0, 1, 0, 0, 1 }, { 0, 0, 1, 0, 1, 1, 1 }
	};
	int data_right[10][7] = {
		{ 1, 1, 1, 0, 0, 1, 0 }, { 1, 1, 0, 0, 1, 1, 0 }, { 1, 1, 0, 1, 1, 0, 0 }, { 1, 0, 0, 0, 0, 1, 0 },
		{ 1, 0, 1, 1, 1, 0, 0 }, { 1, 0, 0, 1, 1, 1, 0 }, { 1, 0, 1, 0, 0, 0, 0 }, { 1, 0, 0, 0, 1, 0, 0 },
		{ 1, 0, 0, 1, 0, 0, 0 }, { 1, 1, 1, 0, 1, 0, 0 }
	};
	uint8_t code[13 + 1];
	uint32_t tmp_p = 0;
	uint32_t csum = 0;
	int i, j;
	int len;

	for (i = len = 0; i < 13; i++) {
		if (!rcode[i]) break;
		if ((code[i] = rcode[i] - '0') > 9)
			return(0);
		len++;
	}
	if (len != 13 && len != 12 && len != 8 && len != 7) return(0);

	#define BS(x) BarcodeData[tmp_p] = x; tmp_p++

	for (j = 0; j < 32; j++) { /* delay before sending a code */
		BS(0x00);
	}

#ifdef INTERL2OF5
	
	BS(1); BS(1); BS(0); BS(0); /* 1 */
	BS(1); BS(1); BS(0); BS(0); /* 1 */
	BS(1); BS(1); BS(0); BS(0); /* 1 */
	BS(1); BS(1); BS(0); BS(0); /* 1 */
	BS(1); BS(1); BS(0); BS(0); /* 1 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 */
	BS(1);        BS(0); BS(0); /* 0 cs */
	BS(1); BS(1); BS(0); BS(0); /* 1 */

#else
	/* Left guard bars */
	BS(1); BS(0); BS(1);

	if (len == 13 || len == 12) {

		for (i = 0; i < 6; i++)
			if (prefix_parity_type[code[0]][i]) {
				for (j = 0; j < 7; j++) {
					BS(data_left_even[code[i + 1]][j]);
				}
			} else
				for (j = 0; j < 7; j++) {
					BS(data_left_odd[code[i + 1]][j]);
				}

		/* Center guard bars */
		BS(0); BS(1); BS(0); BS(1); BS(0);

		for (i = 7; i < 12; i++)
			for (j = 0; j < 7; j++) {
				BS(data_right[code[i]][j]);
			}
		/* Calc and write down the control code if not assigned, instead, send code as is
		   Battle Rush uses modified type of codes with different control code calculation */
		if (len == 12) {
			for (i = 0; i < 12; i++)
				csum += code[i] * ((i & 1) ? 3 : 1);
			csum = (10 - (csum % 10)) % 10;
			rcode[12] = csum + 0x30;	/* update check code to the input string as well */
			rcode[13] = 0;
			code[12] = csum;
		} 
		for (j = 0; j < 7; j++) {
			BS(data_right[code[12]][j]);
		}
	} else if (len == 8 || len == 7) {
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 7; j++) {
				BS(data_left_odd[code[i]][j]);
			}
		}

		/* Center guard bars */
		BS(0); BS(1); BS(0); BS(1); BS(0);

		for (i = 4; i < 7; i++)
			for (j = 0; j < 7; j++) {
				BS(data_right[code[i]][j]);
			}
		csum = 0;
		for (i = 0; i < 7; i++)
			csum += (i & 1) ? code[i] : (code[i] * 3);
		csum = (10 - (csum % 10)) % 10;
		rcode[7] = csum + 0x30;	/* update check code to the input string as well */
		rcode[8] = 0;
		for (j = 0; j < 7; j++) {
			BS(data_right[csum][j]);
		}
	}

	/* Right guard bars */
	BS(1); BS(0); BS(1);
#endif

	for (j = 0; j < 32; j++) {
		BS(0x00);
	}

	BS(0xFF);

	#undef BS

	BarcodeReadPos = 0;
	BarcodeOut = 0x8;
	BarcodeCycleCount = 0;
	return(1);
}
#endif

static void BarcodeSync(void) {
	setchr8(0);
	setprg16(0x8000, (reg[8] & 0x0F));
	setprg16(0xC000, 0x0F);
	SyncMirror();
}

static void BarcodeWrite(uint32 A, uint8 V) {
	A &= 0x0F;
	switch (A) {
	case 0x00: reg[0] = (V & 8) << 2; x24c01_write(reg[0xD] | reg[0]); break;		/* extra EEPROM x24C01 used in Battle Rush mini-cart */
	case 0x08: 
	case 0x09: reg[A] = V; BarcodeSync(); break;
	case 0x0A: X6502_IRQEnd(FCEU_IQEXT); IRQa = V & 1; IRQCount = IRQLatch; break;
	case 0x0B: IRQLatch &= 0xFF00; IRQLatch |= V; break;
	case 0x0C: IRQLatch &= 0xFF; IRQLatch |= V << 8; break;
	case 0x0D: reg[0xD] = V & (~0x20); x24c01_write(reg[0xD] | reg[0]);  x24c02_write(V); break;
	}
}

static void BarcodeIRQHook(int a) {
	BandaiIRQHook(a);

	BarcodeCycleCount += a;

	if (BarcodeCycleCount >= 1000) {
		BarcodeCycleCount -= 1000;
		if (BarcodeData[BarcodeReadPos] == 0xFF) {
			BarcodeOut = 0;
=======
	SetWriteHandler(0x8000, 0xFFFF, BANDAI_Write);
	if (isFCG12) {
		/* submapper 4 */
		SetWriteHandler(0x6000, 0x7FFF, BANDAI_Write);
	}

	if (eeprom_type != EEPROM_NONE) {
		SetReadHandler(0x6000, 0x7FFF, BANDAI_Read);
		if (EEPROM_X24C02) {
			x24c02_init(eeprom);
>>>>>>> 7b06ddc4 (Update libretro.c)
		} else {
			x24c01_init(eeprom);
		}
	}
}

void BANDAI_Init(CartInfo *info, EEPROM_TYPE _eeprom_type, int _isFCG) {
	BANDAI_pwrap = GENPWRAP;
	BANDAI_cwrap = GENCWRAP;

	eeprom_type = _eeprom_type;
	isFCG12 = _isFCG;

	info->Power = BANDAI_Power;
	MapIRQHook = BANDAI_IRQHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	if (eeprom_type != EEPROM_NONE) {
		info->battery = 1;
		memset(eeprom, 0, sizeof(eeprom));
		if (eeprom_type == EEPROM_X24C02) {
			info->SaveGame[0] = eeprom;
			info->SaveGameLen[0] = 256;
			AddExState(&x24c02_StateRegs, ~0, 0, 0);
		} else if (eeprom_type == EEPROM_X24C01) {
			info->SaveGame[0] = eeprom;
			info->SaveGameLen[0] = 128;
			AddExState(&x24c01_StateRegs, ~0, 0, 0);
		}
	}
}
