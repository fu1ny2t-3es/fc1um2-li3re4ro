/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2019 CaH4e3
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
 * Family Network System controller
 *
 */

#include <string.h>
#include "share.h"

<<<<<<< HEAD:src/input/quiz.c
static uint8_t QZVal, QZValR;
static uint8_t FunkyMode;

<<<<<<< HEAD
static uint8_t FP_FASTAPASS(2) QZ_Read(int w, uint8_t ret) {
=======
static uint8 QZ_Read(int w, uint8 ret) {
>>>>>>> b8aebecd (Update libretro.c)
	if (w) {
		ret |= (QZValR & 0x7) << 2;
		QZValR = QZValR >> 3;

		if (FunkyMode)
			QZValR |= 0x28;
		else
			QZValR |= 0x38;
=======
static int readbit;
static int32 readdata;

static uint8 Read(int w, uint8 ret) {
	if (!w) {
		if (readbit < 24) {
			ret |= ((readdata >> readbit) & 1) << 1;
			readbit++;
		} else
			ret |= 2; /* sense! */
>>>>>>> 5926d713 (Update libretro.c):src/input/fns.c
	}
	return ret;
}

<<<<<<< HEAD:src/input/quiz.c
<<<<<<< HEAD
static void QZ_Strobe(void) {
	QZValR = QZVal;
}

static void FP_FASTAPASS(1) QZ_Write(uint8_t V) {
	FunkyMode = V & 4;
}

static void FP_FASTAPASS(2) QZ_Update(void *data, int arg) {
	QZVal = *(uint8_t*)data;
}
=======
static void QZ_Strobe(void) { QZValR = QZVal; }
static void QZ_Write(uint8 V) { FunkyMode = V & 4; }
static void QZ_Update(void *data, int arg) { QZVal = *(uint8*)data; }
>>>>>>> b8aebecd (Update libretro.c)

static INPUTCFC QuizKing = { QZ_Read, QZ_Write, QZ_Strobe, QZ_Update, 0, 0 };

INPUTCFC *FCEU_InitQuizKing(void) {
	QZVal = QZValR = 0;
	return(&QuizKing);
=======
static void Strobe(void) {
	readbit = 0;
}

static void Update(void *data, int arg) {
	readdata = *(uint32 *)data;
}

static INPUTCFC FamiNetSys = { Read, 0, Strobe, Update, 0, 0 };

INPUTCFC *FCEU_InitFamiNetSys(void) {
	return (&FamiNetSys);
>>>>>>> 5926d713 (Update libretro.c):src/input/fns.c
}
