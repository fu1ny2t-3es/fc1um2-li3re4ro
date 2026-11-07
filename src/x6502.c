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

#include <string.h>

#include "fceu.h"
#include "fceu-types.h"
#include "x6502.h"
#include "fceu.h"
#include "sound.h"

<<<<<<< HEAD
X6502 X;
<<<<<<< HEAD
uint8_t encryptOpcodes = 0;
uint8_t encryptOpcodesConfig = 0;
=======
<<<<<<< HEAD
uint8_t encryptOpcodes =0;
=======
uint8 encryptOpcodes =0;
=======
X6502 cpu;
>>>>>>> 3dcd21b (Update libretro_core_options.h)
>>>>>>> a0dace0d (Update libretro_core_options.h)
>>>>>>> de069bf8 (Update libretro_core_options.h)

<<<<<<< HEAD
uint32_t timestamp;
uint32_t sound_timestamp;
void FP_FASTAPASS(1) (*MapIRQHook)(int a);
=======
uint32 timestamp;
uint32 sound_timestamp;
void (*MapIRQHook)(int a);
>>>>>>> d5085b8d (Update libretro_core_options.h)

#define CPU_PC     cpu.PC
#define CPU_A      cpu.A
#define CPU_X      cpu.X
#define CPU_Y      cpu.Y
#define CPU_S      cpu.S
#define CPU_P      cpu.P
#define CPU_PI     cpu.mooPI
#define CPU_DB        cpu.DB
#define CPU_count     cpu.count
#define CPU_tcount    cpu.tcount
#define CPU_IRQlow    cpu.IRQlow
#define CPU_jammed    cpu.jammed

#define ADDCYC(x) {									\
	int __x = x;									\
	CPU_tcount += __x;									\
	CPU_count -= __x * 48;								\
	timestamp += __x;  \
	if (!overclocked) sound_timestamp +=  __x; \
}

<<<<<<< HEAD
static INLINE uint8_t RdMemNorm(uint32_t A) {
	return(_DB = ARead[A](A));
=======
static INLINE uint8 RdMemNorm(uint32 A) {
	CPU_DB = ARead[A](A);
	return(CPU_DB);
>>>>>>> 8a159afe (Update libretro_core_options.h)
}

static INLINE void WrMemNorm(uint32_t A, uint8_t V) {
	BWrite[A](A, V);
}

<<<<<<< HEAD
static INLINE uint8_t RdRAMFast(uint32_t A) {
=======
static INLINE uint8 RdRAMFast(uint32 A) {
<<<<<<< HEAD
>>>>>>> d5085b8d (Update libretro_core_options.h)
	return(_DB = RAM[A]);
=======
	CPU_DB = RAM[A];
	return(CPU_DB);
>>>>>>> 8a159afe (Update libretro_core_options.h)
}

static INLINE void WrRAMFast(uint32_t A, uint8_t V) {
	RAM[A] = V;
	CPU_DB = V;
}

<<<<<<< HEAD
uint8_t FASTAPASS(1) X6502_DMR(uint32_t A) {
=======
uint8 X6502_DMR(uint32 A) {
>>>>>>> d5085b8d (Update libretro_core_options.h)
	ADDCYC(1);
	return(cpu.DB = ARead[A](A));
}

<<<<<<< HEAD
void FASTAPASS(2) X6502_DMW(uint32_t A, uint8_t V) {
=======
void X6502_DMW(uint32 A, uint8 V) {
>>>>>>> d5085b8d (Update libretro_core_options.h)
	ADDCYC(1);
	BWrite[A](A, V);
}

#define PUSH(V) {									\
<<<<<<< HEAD
	uint8_t VTMP = V;									\
	WrRAM(0x100 + _S, VTMP);						\
	_S--;											\
=======
	uint8 VTMP = V;									\
	WrRAM(0x100 + CPU_S, VTMP);						\
	CPU_S--;											\
>>>>>>> 8a159afe (Update libretro_core_options.h)
}

#define POP() RdRAM(0x100 + (++CPU_S))

static uint8_t ZNTable[256];
/* Some of these operations will only make sense if you know what the flag constants are. */

#define X_ZN(zort)  CPU_P &= ~(Z_FLAG | N_FLAG); CPU_P |= ZNTable[zort]
#define X_ZNT(zort) CPU_P |= ZNTable[zort]

#define JR(cond) {									\
	if (cond)										\
	{												\
<<<<<<< HEAD
		uint32_t tmp;									\
		int32_t disp;									\
		disp = (int8_t)RdMem(_PC);					\
		_PC++;										\
=======
		uint32 tmp;									\
		int32 disp;									\
		disp = (int8)RdMem(CPU_PC);					\
		CPU_PC++;										\
>>>>>>> 8a159afe (Update libretro_core_options.h)
		ADDCYC(1);									\
		tmp = CPU_PC;									\
		CPU_PC += disp;								\
		if ((tmp ^ CPU_PC) & 0x100)					\
			ADDCYC(1);								\
	} else CPU_PC++;									\
}

#define LDA     CPU_A = x; X_ZN(CPU_A)
#define LDX     CPU_X = x; X_ZN(CPU_X)
#define LDY     CPU_Y = x; X_ZN(CPU_Y)

/* All of the freaky arithmetic operations. */
#define AND     CPU_A &= x; X_ZN(CPU_A)
#define BIT     CPU_P &= ~(Z_FLAG | V_FLAG | N_FLAG); CPU_P |= ZNTable[x & CPU_A] & Z_FLAG; CPU_P |= x & (V_FLAG | N_FLAG)
#define EOR     CPU_A ^= x; X_ZN(CPU_A)
#define ORA     CPU_A |= x; X_ZN(CPU_A)

#define ADC {										\
<<<<<<< HEAD
	uint32_t l = _A + x + (_P & 1);					\
	_P &= ~(Z_FLAG | C_FLAG | N_FLAG | V_FLAG);		\
	_P |= ((((_A ^ x) & 0x80) ^ 0x80) & ((_A ^ l) & 0x80)) >> 1;  \
	_P |= (l >> 8) & C_FLAG;						\
	_A = l;											\
	X_ZNT(_A);										\
}

#define SBC {										\
	uint32_t l = _A - x - ((_P & 1) ^ 1);				\
	_P &= ~(Z_FLAG | C_FLAG | N_FLAG | V_FLAG);		\
	_P |= ((_A ^ l) & (_A ^ x) & 0x80) >> 1;		\
	_P |= ((l >> 8) & C_FLAG) ^ C_FLAG;				\
	_A = l;											\
	X_ZNT(_A);										\
=======
	uint32 l = CPU_A + x + (CPU_P & 1);					\
	CPU_P &= ~(Z_FLAG | C_FLAG | N_FLAG | V_FLAG);		\
	CPU_P |= ((((CPU_A ^ x) & 0x80) ^ 0x80) & ((CPU_A ^ l) & 0x80)) >> 1;  \
	CPU_P |= (l >> 8) & C_FLAG;						\
	CPU_A = l;											\
	X_ZNT(CPU_A);										\
}

#define SBC {										\
	uint32 l = CPU_A - x - ((CPU_P & 1) ^ 1);				\
	CPU_P &= ~(Z_FLAG | C_FLAG | N_FLAG | V_FLAG);		\
	CPU_P |= ((CPU_A ^ l) & (CPU_A ^ x) & 0x80) >> 1;		\
	CPU_P |= ((l >> 8) & C_FLAG) ^ C_FLAG;				\
	CPU_A = l;											\
	X_ZNT(CPU_A);										\
>>>>>>> 8a159afe (Update libretro_core_options.h)
}

#define CMPL(a1, a2) {								\
	uint32_t t = a1 - a2;								\
	X_ZN(t & 0xFF);									\
	CPU_P &= ~C_FLAG;									\
	CPU_P |= ((t >> 8) & C_FLAG) ^ C_FLAG;				\
}

/* Special undocumented operation.  Very similar to CMP. */
#define AXS {										\
<<<<<<< HEAD
	uint32_t t = (_A & _X) - x;						\
=======
	uint32 t = (CPU_A & CPU_X) - x;						\
>>>>>>> 8a159afe (Update libretro_core_options.h)
	X_ZN(t & 0xFF);									\
	CPU_P &= ~C_FLAG;									\
	CPU_P |= ((t >> 8) & C_FLAG) ^ C_FLAG;				\
	CPU_X = t;											\
}

#define CMP     CMPL(CPU_A, x)
#define CPX     CMPL(CPU_X, x)
#define CPY     CMPL(CPU_Y, x)

/* The following operations modify the byte being worked on. */
#define DEC     x--; X_ZN(x)
#define INC     x++; X_ZN(x)

#define ASL     CPU_P &= ~C_FLAG; CPU_P |= x >> 7; x <<= 1; X_ZN(x)
#define LSR     CPU_P &= ~(C_FLAG | N_FLAG | Z_FLAG); CPU_P |= x & 1; x >>= 1; X_ZNT(x)

/* For undocumented instructions, maybe for other things later... */
#define LSRA    CPU_P &= ~(C_FLAG | N_FLAG | Z_FLAG); CPU_P |= CPU_A & 1; CPU_A >>= 1; X_ZNT(CPU_A)

#define ROL {										\
	uint8_t l = x >> 7;								\
	x <<= 1;										\
	x |= CPU_P & C_FLAG;								\
	CPU_P &= ~(Z_FLAG | N_FLAG | C_FLAG);				\
	CPU_P |= l;										\
	X_ZNT(x);										\
}

#define ROR {										\
	uint8_t l = x & 1;								\
	x >>= 1;										\
	x |= (CPU_P & C_FLAG) << 7;						\
	CPU_P &= ~(Z_FLAG | N_FLAG | C_FLAG);				\
	CPU_P |= l;										\
	X_ZNT(x);										\
}

/* Icky icky thing for some undocumented instructions.  Can easily be
 * broken if names of local variables are changed.
 */

/* Absolute */
#define GetAB(target) {								\
	target = RdMem(CPU_PC);							\
	CPU_PC++;											\
	target |= RdMem(CPU_PC) << 8;						\
	CPU_PC++;											\
}

/* Absolute Indexed(for reads) */
#define GetABIRD(target, i) {						\
	uint32_t tmp;										\
	GetAB(tmp);										\
	target = tmp;									\
	target += i;									\
	if ((target ^ tmp) & 0x100) {					\
		target &= 0xFFFF;							\
		RdMem(target ^ 0x100);						\
		ADDCYC(1);									\
	}												\
}

/* Absolute Indexed(for writes and rmws) */
#define GetABIWR(target, i) {						\
	uint32_t rt;										\
	GetAB(rt);										\
	target = rt;									\
	target += i;									\
	target &= 0xFFFF;								\
	RdMem((target & 0x00FF) | (rt & 0xFF00));		\
}

/* Zero Page */
#define GetZP(target) {								\
	target = RdMem(CPU_PC);							\
	CPU_PC++;											\
}

/* Zero Page Indexed */
#define GetZPI(target, i) {							\
	target = i + RdMem(CPU_PC);						\
	CPU_PC++;											\
}

/* Indexed Indirect */
#define GetIX(target) {								\
<<<<<<< HEAD
	uint8_t tmp;										\
	tmp = RdMem(_PC);								\
	_PC++;											\
	tmp += _X;										\
=======
	uint8 tmp;										\
	tmp = RdMem(CPU_PC);								\
	CPU_PC++;											\
	tmp += CPU_X;										\
>>>>>>> 8a159afe (Update libretro_core_options.h)
	target = RdRAM(tmp);							\
	tmp++;											\
	target |= RdRAM(tmp) << 8;						\
}

/* Indirect Indexed(for reads) */
#define GetIYRD(target) {							\
<<<<<<< HEAD
	uint32_t rt;										\
	uint8_t tmp;										\
	tmp = RdMem(_PC);								\
	_PC++;											\
=======
	uint32 rt;										\
	uint8 tmp;										\
	tmp = RdMem(CPU_PC);								\
	CPU_PC++;											\
>>>>>>> 8a159afe (Update libretro_core_options.h)
	rt = RdRAM(tmp);								\
	tmp++;											\
	rt |= RdRAM(tmp) << 8;							\
	target = rt;									\
	target += CPU_Y;									\
	if ((target ^ rt) & 0x100) {					\
		target &= 0xFFFF;							\
		RdMem(target ^ 0x100);						\
		ADDCYC(1);									\
	}												\
}

/* Indirect Indexed(for writes and rmws) */
#define GetIYWR(target) {							\
<<<<<<< HEAD
	uint32_t rt;										\
	uint8_t tmp;										\
	tmp = RdMem(_PC);								\
	_PC++;											\
=======
	uint32 rt;										\
	uint8 tmp;										\
	tmp = RdMem(CPU_PC);								\
	CPU_PC++;											\
>>>>>>> 8a159afe (Update libretro_core_options.h)
	rt = RdRAM(tmp);								\
	tmp++;											\
	rt |= RdRAM(tmp) << 8;							\
	target = rt;									\
	target += CPU_Y;									\
	target &= 0xFFFF;								\
	RdMem((target & 0x00FF) | (rt & 0xFF00));		\
}

/* Now come the macros to wrap up all of the above stuff addressing mode functions
and operation macros.  Note that operation macros will always operate(redundant
redundant) on the variable "x".
*/

<<<<<<< HEAD
#define RMW_A(op)       { uint8_t x = _A; op; _A = x; break; }	/* Meh... */
#define RMW_AB(op)      { uint32_t A; uint8_t x; GetAB(A); x = RdMem(A); WrMem(A, x); op; WrMem(A, x); break; }
#define RMW_ABI(reg, op) { uint32_t A; uint8_t x; GetABIWR(A, reg); x = RdMem(A); WrMem(A, x); op; WrMem(A, x); break; }
#define RMW_ABX(op)     RMW_ABI(_X, op)
#define RMW_ABY(op)     RMW_ABI(_Y, op)
#define RMW_IX(op)      { uint32_t A; uint8_t x; GetIX(A); x = RdMem(A); WrMem(A, x); op; WrMem(A, x); break; }
#define RMW_IY(op)      { uint32_t A; uint8_t x; GetIYWR(A); x = RdMem(A); WrMem(A, x); op; WrMem(A, x); break; }
#define RMW_ZP(op)      { uint8_t A; uint8_t x; GetZP(A); x = RdRAM(A); op; WrRAM(A, x); break; }
#define RMW_ZPX(op)     { uint8_t A; uint8_t x; GetZPI(A, _X); x = RdRAM(A); op; WrRAM(A, x); break; }

#define LD_IM(op)       { uint8_t x; x = RdMem(_PC); _PC++; op; break; }
#define LD_ZP(op)       { uint8_t A; uint8_t x; GetZP(A); x = RdRAM(A); op; break; }
#define LD_ZPX(op)      { uint8_t A; uint8_t x; GetZPI(A, _X); x = RdRAM(A); op; break; }
#define LD_ZPY(op)      { uint8_t A; uint8_t x; GetZPI(A, _Y); x = RdRAM(A); op; break; }
#define LD_AB(op)       { uint32_t A; uint8_t x; GetAB(A); x = RdMem(A); op; break; }
#define LD_ABI(reg, op) { uint32_t A; uint8_t x; GetABIRD(A, reg); x = RdMem(A); op; break; }
#define LD_ABX(op)      LD_ABI(_X, op)
#define LD_ABY(op)      LD_ABI(_Y, op)
#define LD_IX(op)       { uint32_t A; uint8_t x; GetIX(A); x = RdMem(A); op; break; }
#define LD_IY(op)       { uint32_t A; uint8_t x; GetIYRD(A); x = RdMem(A); op; break; }

#define ST_ZP(r)        { uint8_t A; GetZP(A); WrRAM(A, r); break; }
#define ST_ZPX(r)       { uint8_t A; GetZPI(A, _X); WrRAM(A, r); break; }
#define ST_ZPY(r)       { uint8_t A; GetZPI(A, _Y); WrRAM(A, r); break; }
#define ST_AB(r)        { uint32_t A; GetAB(A); WrMem(A, r); break; }
#define ST_ABI(reg, r)  { uint32_t A; GetABIWR(A, reg); WrMem(A, r); break; }
#define ST_ABX(r)       ST_ABI(_X, r)
#define ST_ABY(r)       ST_ABI(_Y, r)
#define ST_IX(r)        { uint32_t A; GetIX(A); WrMem(A, r); break; }
#define ST_IY(r)        { uint32_t A; GetIYWR(A); WrMem(A, r); break; }
=======
#define RMWCPU_A(op)       { uint8 x = CPU_A; op; CPU_A = x; break; }	/* Meh... */
#define RMWCPU_AB(op)      { uint32 A; uint8 x; GetAB(A); x = RdMem(A); WrMem(A, x); op; WrMem(A, x); break; }
#define RMWCPU_ABI(reg, op) { uint32 A; uint8 x; GetABIWR(A, reg); x = RdMem(A); WrMem(A, x); op; WrMem(A, x); break; }
#define RMWCPU_ABX(op)     RMWCPU_ABI(CPU_X, op)
#define RMWCPU_ABY(op)     RMWCPU_ABI(CPU_Y, op)
#define RMW_IX(op)      { uint32 A; uint8 x; GetIX(A); x = RdMem(A); WrMem(A, x); op; WrMem(A, x); break; }
#define RMW_IY(op)      { uint32 A; uint8 x; GetIYWR(A); x = RdMem(A); WrMem(A, x); op; WrMem(A, x); break; }
#define RMW_ZP(op)      { uint8 A; uint8 x; GetZP(A); x = RdRAM(A); op; WrRAM(A, x); break; }
#define RMW_ZPX(op)     { uint8 A; uint8 x; GetZPI(A, CPU_X); x = RdRAM(A); op; WrRAM(A, x); break; }

#define LD_IM(op)       { uint8 x; x = RdMem(CPU_PC); CPU_PC++; op; break; }
#define LD_ZP(op)       { uint8 A; uint8 x; GetZP(A); x = RdRAM(A); op; break; }
#define LD_ZPX(op)      { uint8 A; uint8 x; GetZPI(A, CPU_X); x = RdRAM(A); op; break; }
#define LD_ZPY(op)      { uint8 A; uint8 x; GetZPI(A, CPU_Y); x = RdRAM(A); op; break; }
#define LDCPU_AB(op)       { uint32 A; uint8 x; GetAB(A); x = RdMem(A); op; break; }
#define LDCPU_ABI(reg, op) { uint32 A; uint8 x; GetABIRD(A, reg); x = RdMem(A); op; break; }
#define LDCPU_ABX(op)      LDCPU_ABI(CPU_X, op)
#define LDCPU_ABY(op)      LDCPU_ABI(CPU_Y, op)
#define LD_IX(op)       { uint32 A; uint8 x; GetIX(A); x = RdMem(A); op; break; }
#define LD_IY(op)       { uint32 A; uint8 x; GetIYRD(A); x = RdMem(A); op; break; }

#define ST_ZP(r)        { uint8 A; GetZP(A); WrRAM(A, r); break; }
#define ST_ZPX(r)       { uint8 A; GetZPI(A, CPU_X); WrRAM(A, r); break; }
#define ST_ZPY(r)       { uint8 A; GetZPI(A, CPU_Y); WrRAM(A, r); break; }
#define STCPU_AB(r)        { uint32 A; GetAB(A); WrMem(A, r); break; }
#define STCPU_ABI(reg, r)  { uint32 A; GetABIWR(A, reg); WrMem(A, r); break; }
#define STCPU_ABX(r)       STCPU_ABI(CPU_X, r)
#define STCPU_ABY(r)       STCPU_ABI(CPU_Y, r)
#define ST_IX(r)        { uint32 A; GetIX(A); WrMem(A, r); break; }
#define ST_IY(r)        { uint32 A; GetIYWR(A); WrMem(A, r); break; }
>>>>>>> 8a159afe (Update libretro_core_options.h)

static const uint8_t CycTable[256] =
{
/*0x00*/ 7, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 4, 4, 6, 6,
/*0x10*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
/*0x20*/ 6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 4, 4, 6, 6,
/*0x30*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
/*0x40*/ 6, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 3, 4, 6, 6,
/*0x50*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
/*0x60*/ 6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 5, 4, 6, 6,
/*0x70*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
/*0x80*/ 2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
/*0x90*/ 2, 6, 2, 6, 4, 4, 4, 4, 2, 5, 2, 5, 5, 5, 5, 5,
/*0xA0*/ 2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
/*0xB0*/ 2, 5, 2, 5, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4,
/*0xC0*/ 2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
/*0xD0*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
/*0xE0*/ 2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
/*0xF0*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
};

void X6502_IRQBegin(int w) {
	CPU_IRQlow |= w;
}

void X6502_IRQEnd(int w) {
	CPU_IRQlow &= ~w;
}

void TriggerNMI(void) {
	CPU_IRQlow |= FCEU_IQNMI;
}

void TriggerNMI2(void) {
	CPU_IRQlow |= FCEU_IQNMI2;
}

void X6502_Reset(void) {
	CPU_IRQlow = FCEU_IQRESET;
}

void X6502_Init(void) {
	int x;

	memset((void*)&cpu, 0, sizeof(cpu));
	for (x = 0; x < 256; x++)
		if (!x)
			ZNTable[x] = Z_FLAG;
		else if (x & 0x80)
			ZNTable[x] = N_FLAG;
		else
			ZNTable[x] = 0;
}

void X6502_Power(void) {
	CPU_count = CPU_tcount = CPU_IRQlow = CPU_PC = CPU_A = CPU_X = CPU_Y = CPU_P = CPU_PI = CPU_DB = CPU_jammed = 0;
	CPU_S = 0xFD;
	timestamp = sound_timestamp = 0;
	X6502_Reset();
}

<<<<<<< HEAD
void X6502_Run(int32_t cycles)
=======
<<<<<<< HEAD
#ifdef FCEUDEF_DEBUGGER
static void X6502_RunDebug(int32 cycles) {
	#define RdRAM RdMemHook
	#define WrRAM WrMemHook
	#define RdMem RdMemHook
	#define WrMem WrMemHook

	if (PAL)
		cycles *= 15;	/* 15*4=60 */
	else
		cycles *= 16;	/* 16*4=64 */

	_count += cycles;

	while (_count > 0) {
		int32 temp;
		uint8 b1;

		if (_IRQlow) {
			if (_IRQlow & FCEU_IQRESET) {
				_PC = RdMem(0xFFFC);
				_PC |= RdMem(0xFFFD) << 8;
				_jammed = 0;
				_PI = _P = I_FLAG;
				_IRQlow &= ~FCEU_IQRESET;
			} else if (_IRQlow & FCEU_IQNMI2) {
				_IRQlow &= ~FCEU_IQNMI2;
				_IRQlow |= FCEU_IQNMI;
			} else if (_IRQlow & FCEU_IQNMI) {
				if (!_jammed) {
					ADDCYC(7);
					PUSH(_PC >> 8);
					PUSH(_PC);
					PUSH((_P & ~B_FLAG) | (U_FLAG));
					_P |= I_FLAG;
					_PC = RdMem(0xFFFA);
					_PC |= RdMem(0xFFFB) << 8;
					_IRQlow &= ~FCEU_IQNMI;
				}
			} else {
				if (!(_PI & I_FLAG) && !_jammed) {
					ADDCYC(7);
					PUSH(_PC >> 8);
					PUSH(_PC);
					PUSH((_P & ~B_FLAG) | (U_FLAG));
					_P |= I_FLAG;
					_PC = RdMem(0xFFFE);
					_PC |= RdMem(0xFFFF) << 8;
				}
			}
			_IRQlow &= ~(FCEU_IQTEMP);
			if (_count <= 0) {
				_PI = _P;
				return;
			}	/* Should increase accuracy without a
				 * major speed hit.
				 */
		}

		if (X.CPUHook) X.CPUHook(&X);
		/* Ok, now the real fun starts.
		 * Do the pre-exec voodoo.
		 */
		if (X.ReadHook || X.WriteHook) {
			uint32 tsave = timestamp;
			XSave = X;

			fceuindbg = 1;
			X.preexec = 1;
			b1 = RdMem(_PC);
			_PC++;
			if (encryptOpcodes ==12) b1 =b1 &0x39 | b1 >>1 &0x42 | b1 <<1 &0x84;
			if (encryptOpcodes ==14) b1 =b1 &0x3F | b1 >>1 &0x40 | b1 <<1 &0x80;
			switch (b1) {
				#include "ops.h"
			}

			timestamp = tsave;

			/* In case an NMI/IRQ/RESET was triggered by the debugger.
			 * Should we also copy over the other hook variables?
			 */
			XSave.IRQlow = X.IRQlow;
			XSave.ReadHook = X.ReadHook;
			XSave.WriteHook = X.WriteHook;
			XSave.CPUHook = X.CPUHook;
			X = XSave;
			fceuindbg = 0;
		}

		_PI = _P;
		b1 = RdMem(_PC);
		ADDCYC(CycTable[b1]);

		temp = _tcount;
		_tcount = 0;
		if (MapIRQHook) MapIRQHook(temp);

      if (!overclocked)
         FCEU_SoundCPUHook(temp);

		_PC++;
		if (encryptOpcodes ==12) b1 =b1 &0x39 | b1 >>1 &0x42 | b1 <<1 &0x84;
		if (encryptOpcodes ==14) b1 =b1 &0x3F | b1 >>1 &0x40 | b1 <<1 &0x80;
		switch (b1) {
			#include "ops.h"
		}
	}
	#undef RdRAM
	#undef WrRAM
	#undef RdMem
	#undef WrMem
}

static void X6502_RunNormal(int32 cycles)
#else
=======
>>>>>>> dc718d2 (Update libretro_core_options.h)
void X6502_Run(int32 cycles)
>>>>>>> d5085b8d (Update libretro_core_options.h)
{
	#define RdRAM RdMemNorm
	#define WrRAM WrMemNorm
	#define RdMem RdMemNorm
	#define WrMem WrMemNorm

<<<<<<< HEAD
<<<<<<< HEAD
	#if (defined(C80x86) && defined(__GNUC__))
	/* Gives a nice little speed boost. */
	register uint16_t pbackus asm ("edi");
	#else
	uint16_t pbackus;
	#endif

	pbackus = _PC;
=======
	uint16 pbackus = _PC;
>>>>>>> d5085b8d (Update libretro_core_options.h)
=======
	uint16 pbackus = CPU_PC;
>>>>>>> 8a159afe (Update libretro_core_options.h)

	#undef CPU_PC
	#define CPU_PC pbackus

	if (PAL)
		cycles *= 15;	/* 15*4=60 */
	else
		cycles *= 16;	/* 16*4=64 */

	CPU_count += cycles;

<<<<<<< HEAD
	while (_count > 0) {
		int32_t temp;
		uint8_t b1;
=======
	while (CPU_count > 0) {
		int32 temp;
		uint8 b1;
>>>>>>> 8a159afe (Update libretro_core_options.h)

		if (CPU_IRQlow) {
			if (CPU_IRQlow & FCEU_IQRESET) {
				CPU_PC = RdMem(0xFFFC);
				CPU_PC |= RdMem(0xFFFD) << 8;
				CPU_jammed = 0;
				CPU_PI = CPU_P = I_FLAG;
				CPU_IRQlow &= ~FCEU_IQRESET;
			} else if (CPU_IRQlow & FCEU_IQNMI2) {
				CPU_IRQlow &= ~FCEU_IQNMI2;
				CPU_IRQlow |= FCEU_IQNMI;
			} else if (CPU_IRQlow & FCEU_IQNMI) {
				if (!CPU_jammed) {
					ADDCYC(7);
					PUSH(CPU_PC >> 8);
					PUSH(CPU_PC);
					PUSH((CPU_P & ~B_FLAG) | (U_FLAG));
					CPU_P |= I_FLAG;
					CPU_PC = RdMem(0xFFFA);
					CPU_PC |= RdMem(0xFFFB) << 8;
					CPU_IRQlow &= ~FCEU_IQNMI;
				}
			} else {
				if (!(CPU_PI & I_FLAG) && !CPU_jammed) {
					ADDCYC(7);
					PUSH(CPU_PC >> 8);
					PUSH(CPU_PC);
					PUSH((CPU_P & ~B_FLAG) | (U_FLAG));
					CPU_P |= I_FLAG;
					CPU_PC = RdMem(0xFFFE);
					CPU_PC |= RdMem(0xFFFF) << 8;
				}
			}
			CPU_IRQlow &= ~(FCEU_IQTEMP);
			if (CPU_count <= 0) {
				CPU_PI = CPU_P;
				cpu.PC = pbackus;
				return;
			}	/* Should increase accuracy without a
				 * major speed hit.
				 */
		}

		CPU_PI = CPU_P;
		b1 = RdMem(CPU_PC);

		ADDCYC(CycTable[b1]);

		temp = CPU_tcount;
		CPU_tcount = 0;
		if (MapIRQHook) MapIRQHook(temp);
		if (!overclocked)
			FCEU_SoundCPUHook(temp);
<<<<<<< HEAD
		X.PC = pbackus;
<<<<<<< HEAD
		_PC++;
<<<<<<< HEAD
		if (encryptOpcodes == 10) b1 = b1 &~0x36 | b1 >>1 &0x12 | b1 <<1 &0x24;
		if (encryptOpcodes == 11) {
			if (encryptOpcodesConfig &0x02) b1 = b1 &~0x30 | b1 >>1 &0x10 | b1 <<1 &0x20;
			if (encryptOpcodesConfig &0x40) b1 = b1 &~0xC6 | b1 >>1 &0x42 | b1 <<1 &0x84;
		}
		if (encryptOpcodes == 12) b1 = b1 &~0xC6 | b1 >>1 &0x42 | b1 <<1 &0x84;
		if (encryptOpcodes == 14) b1 = b1 &~0xC0 | b1 >>1 &0x40 | b1 <<1 &0x80;
=======
<<<<<<< HEAD
		if (encryptOpcodes ==12) b1 =b1 &0x39 | b1 >>1 &0x42 | b1 <<1 &0x84;
		if (encryptOpcodes ==14) b1 =b1 &0x3F | b1 >>1 &0x40 | b1 <<1 &0x80;
=======
		if (encryptOpcodes ==67) b1 =b1 &0x3F | b1 >>1 &0x40 | b1 <<1 &0x80;
=======
=======
		cpu.PC = pbackus;
>>>>>>> 3dcd21b (Update libretro_core_options.h)
		CPU_PC++;
>>>>>>> 8595c80 (Backport macro changes to X6502.c)
>>>>>>> f7b9a34 (Update libretro_core_options.h)
>>>>>>> 1646e5fe (Update libretro_core_options.h)
		switch (b1) {
			#include "ops.h"
		}
	}

	#undef CPU_PC
	#define CPU_PC cpu.PC
	CPU_PC = pbackus;
	#undef RdRAM
	#undef WrRAM
}
