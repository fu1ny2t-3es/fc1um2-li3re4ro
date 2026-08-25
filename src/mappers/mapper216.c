/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 CaH4e3
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
#include "latch.h"

<<<<<<< HEAD
#define CARD_EXTERNAL_INSERED 0x80

static uint8_t prg_reg;
static uint8_t chr_reg;
static SFORMAT StateRegs[] =
{
	{ &prg_reg, 1, "PREG" },
	{ &chr_reg, 1, "CREG" },
	{ 0 }
};

<<<<<<< HEAD
#if 0

cmd[0] = response on/off
				0x00 - on
				0x80 - off
cmd[1] = cmd


_GET_CHALLENGE:      .BYTE   0,$B4,  0,  0,$62

_SELECT_FILE_1_0200: .BYTE   0,$A4,  1,  0,  2,  2,  0
_SELECT_FILE_2_0201: .BYTE   0,$A4,  2,  0,  2,  2,  1
_SELECT_FILE_2_0203: .BYTE   0,$A4,  2,  0,  2,  2,  3
_SELECT_FILE_2_0204: .BYTE   0,$A4,  2,  0,  2,  2,  4
_SELECT_FILE_2_0205: .BYTE   0,$A4,  2,  0,  2,  2,  5
_SELECT_FILE_2_3F04: .BYTE   0,$A4,  2,  0,  2,$3F,  4
_SELECT_FILE_2_4F00: .BYTE   0,$A4,  2,  0,  2,$4F,  0

_READ_BINARY_5:      .BYTE   0,$B0,$85,  0,  2
_READ_BINARY_6:      .BYTE   0,$B0,$86,  0,  4
_READ_BINARY_6_0:    .BYTE   0,$B0,$86,  0,$18
_READ_BINARY_0:      .BYTE   0,$B0,  0,  2,  3
_READ_BINARY_0_0:    .BYTE   0,$B0,  0,  0,  4
_READ_BINARY_0_1:    .BYTE   0,$B0,  0,  0, $C
_READ_BINARY_0_2:    .BYTE   0,$B0,  0,  0,$10

_UPDATE_BINARY:      .BYTE   0,$D6,  0,  0,  4
_UPDATE_BINARY_0:    .BYTE   0,$D6,  0,  0,$10

_GET_RESPONSE:       .BYTE $80,$C0,  2,$A1,  8
_GET_RESPONSE_0:     .BYTE   0,$C0,  0,  0,  2
_GET_RESPONSE_1:     .BYTE   0,$C0,  0,  0,  6
_GET_RESPONSE_2:     .BYTE   0,$C0,  0,  0,  8
_GET_RESPONSE_3:     .BYTE   0,$C0,  0,  0, $C
_GET_RESPONSE_4:     .BYTE   0,$C0,  0,  0,$10

byte_8C0B:           .BYTE $80,$30,  0,  2, $A,  0,  1
byte_8C48:           .BYTE $80,$32,  0,  1,  4
byte_8C89:           .BYTE $80,$34,  0,  0,  8,  0,  0
byte_8D01:           .BYTE $80,$36,  0,  0, $C
byte_8CA7:           .BYTE $80,$38,  0,  2,  4
byte_8BEC:           .BYTE $80,$3A,  0,  3,  0

byte_89A0:           .BYTE   0,$48,  0,  0,  6
byte_8808:           .BYTE   0,$54,  0,  0,$1C
byte_89BF:           .BYTE   0,$58,  0,  0,$1C

_MANAGE_CHANNEL:     .BYTE   0,$70,  0,  0,  8
byte_8CE5:           .BYTE   0,$74,  0,  0,$12
byte_8C29:           .BYTE   0,$76,  0,  0,  8
byte_8CC6:           .BYTE   0,$78,  0,  0,$12
#endif

#if 0 /* Silenced since unused */
static const uint8_t sim0reset[0x1F] = {
	0x3B, 0xE9, 0x00, 0xFF, 0xC1, 0x10, 0x31, 0xFE,
	0x55, 0xC8, 0x10, 0x20, 0x55, 0x47, 0x4F, 0x53,
	0x56, 0x53, 0x43, 0xAD, 0x10, 0x10, 0x10, 0x10,
	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10
};

#endif

static void Sync(void) {
=======
static void M216Sync(void) {
>>>>>>> f6efdc94 (Update Makefile.libretro)
	setprg32(0x8000, prg_reg);
	setchr8(chr_reg);
=======
static void Sync(void) {
	setprg32(0x8000, latch.addr & 0x01);
	setchr8(latch.addr >> 1);
>>>>>>> ae14339e (Update Makefile.libretro)
}

void Mapper216_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
}
