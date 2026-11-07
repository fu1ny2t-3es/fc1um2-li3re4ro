#ifndef _FCEUH
#define _FCEUH

#include "fceu-types.h"
#include "file.h"

#define RAM_SIZE 0x800
#define RAM_MASK (RAM_SIZE - 1)

#define NES_WIDTH  256
#define NES_HEIGHT 240
#define NTSC_WIDTH 602

extern int fceuindbg;
void ResetGameLoaded(void);

#define DECLFR(x) uint8 x(uint16 A)
#define DECLFW(x) void x(uint16 A, uint8 V)

<<<<<<< HEAD
void ResetGameLoaded(void);

<<<<<<< HEAD
#define DECLFR(x) uint8_t FP_FASTAPASS(1) x(uint32_t A)
#define DECLFW(x) void FP_FASTAPASS(2) x(uint32_t A, uint8_t V)

void FCEU_MemoryRand(uint8_t *ptr, uint32_t size);
void FCEU_MemoryRand_Reseed(uint32_t seed);
void FASTAPASS(3) SetReadHandler(int32_t start, int32_t end, readfunc func);
void FASTAPASS(3) SetWriteHandler(int32_t start, int32_t end, writefunc func);
writefunc FASTAPASS(1) GetWriteHandler(int32_t a);
readfunc FASTAPASS(1) GetReadHandler(int32_t a);
=======
=======
>>>>>>> ad14c351 (Simplify some functions)
void FCEU_MemoryRand(uint8 *ptr, uint32 size);
<<<<<<< HEAD
void SetReadHandler(int32 start, int32 end, readfunc func);
void SetWriteHandler(int32 start, int32 end, writefunc func);
writefunc GetWriteHandler(int32 a);
readfunc GetReadHandler(int32 a);
>>>>>>> f9553c43 (Update ppu.c)
=======
void SetReadHandler(uint16 start, uint16 end, readfunc func);
void SetWriteHandler(uint16 start, uint16 end, writefunc func);
writefunc GetWriteHandler(uint16 a);
readfunc GetReadHandler(uint16 a);
>>>>>>> e98261e5 (Update ppu.c)

void FCEU_ResetVidSys(void);

void ResetMapping(void);
void ResetNES(void);
void PowerNES(void);

<<<<<<< HEAD

extern uint64_t timestampbase;
extern uint32_t MMC5HackVROMMask;
extern uint8_t *MMC5HackExNTARAMPtr;
extern int MMC5Hack, PEC586Hack;
extern uint8_t *MMC5HackVROMPTR;
extern uint8_t MMC5HackCHRMode;
extern uint8_t MMC5HackSPMode;
extern uint8_t MMC50x5130;
extern uint8_t MMC5HackSPScroll;
extern uint8_t MMC5HackSPPage;

extern uint8_t RAM[0x800];
=======
extern uint64 timestampbase;
extern uint32 MMC5HackVROMMask;
extern uint8 *MMC5HackExNTARAMPtr;
extern uint8 MMC5Hack;
extern uint8 *MMC5HackVROMPTR;
extern uint8 MMC5HackCHRMode;
extern uint8 MMC5HackSPMode;
extern uint8 MMC50x5130;
extern uint8 MMC5HackSPScroll;
extern uint8 MMC5HackSPPage;

extern uint8 PEC586Hack;

extern uint8 QTAIHack;
extern uint8 qtramreg;
extern uint8 QTRAM[0x800];

extern uint8 *RAM;
>>>>>>> e98261e5 (Update ppu.c)

extern readfunc ARead[0x10000];
extern writefunc BWrite[0x10000];

extern void (*GameInterface)(int h);
extern void (*GameStateRestore)(int version);

#define GI_RESETM2 1
#define GI_POWER   2
#define GI_CLOSE   3

#include "git.h"
extern FCEUGI *GameInfo;

<<<<<<< HEAD
extern uint8_t PAL;
=======
extern uint8 isPAL;
<<<<<<< HEAD
>>>>>>> 77219ed2 (Update ppu.c)
=======
extern uint8 isDendy;
>>>>>>> e98261e5 (Update ppu.c)

#include "driver.h"

typedef struct {
	int PAL;
<<<<<<< HEAD
	int SoundVolume;
	int TriangleVolume;
	int SquareVolume[2];
	int NoiseVolume;
	int PCMVolume;
	int ExpVolume[6];	/* Per-channel expansion-audio volume,
	                     * indexed by SND_FDS..SND_MMC5 (see
	                     * src/sound.h).  Scale is 0-256 where 256 =
	                     * unscaled output and 0 = silence.  Default
	                     * 256 leaves output bit-identical to builds
	                     * predating issue #512. */
=======

	int volume[12]; /* master, nes apu and expansion audio */

>>>>>>> 964ecd96 (Update ppu.c)
	int GameGenie;

	/* Current first and last rendered scanlines. */
	int FirstSLine;
	int LastSLine;

	/* Driver code(user)-specified first and last rendered scanlines.
	 * Usr*SLine[0] is for NTSC, Usr*SLine[1] is for PAL.
	 */
	int UsrFirstSLine[2];
	int UsrLastSLine[2];
<<<<<<< HEAD
	uint32_t SndRate;
=======

	int SndRate;
>>>>>>> e98261e5 (Update ppu.c)
	int soundq;
	int lowpass;
<<<<<<< HEAD
	int RemoveTriangleNoise;	/* Mute triangle channel when its period
	                             * is low enough to produce only ultrasonic
	                             * output (period <= 3, > ~12 kHz at NTSC),
	                             * which the DAC reconstruction filter
	                             * folds back as audible popping in HQ
	                             * mode.  LQ already silences these
	                             * unconditionally; this option mirrors
	                             * that behaviour in HQ.  Default off to
	                             * preserve existing HQ output bit-exactly
	                             * when the option is not set. */
	int ReduceDMCPopping;		/* Smooth direct writes to the DMC DAC
	                             * register ($4011) by stepping the DAC
	                             * only halfway from its previous value
	                             * toward the newly written value, which
	                             * attenuates the audible click games
	                             * like Castlevania II generate when they
	                             * pulse the DAC for manual sample
	                             * playback.  Normal DPCM bit-decode
	                             * playback (the +/-2 per bit update) is
	                             * NOT affected.  Default off so the DAC
	                             * trajectory is bit-exact when the
	                             * option is not set. */
=======

	int SwapDutyCycles;
	int RamInitState;
	int ShowCrosshair;
	int ReplaceP2StartWithMicrophone;
	int PPUOverclockEnabled;
	int SkipDMC7BitOverclock;
	int ReduceDMCPopping;
>>>>>>> 964ecd96 (Update ppu.c)
} FCEUS;

extern FCEUS FSettings;

<<<<<<< HEAD
void FCEU_PrintError(const char *format, ...);
void FCEU_printf(const char *format, ...);
=======
void FCEU_PrintError(char *format, ...); /* warning level messages */
void FCEU_PrintDebug(char *format, ...); /* debug level messages */
void FCEU_printf(char *format, ...);     /* normal messages */
>>>>>>> e98261e5 (Update ppu.c)

<<<<<<< HEAD
void SetNESDeemph(uint8_t d, int force);
void DrawTextTrans(uint8_t *dest, uint32_t width, uint8_t *textmsg, uint8_t fgcolor);
void FCEU_PutImage(void);
=======
void SetNESDeemph(uint8 d, int force);
<<<<<<< HEAD
>>>>>>> a9752267 (Refactors)

<<<<<<< HEAD
extern uint8_t Exit;
extern uint8_t default_palette_selected;
extern uint8_t vsdip;
=======
=======
void DrawTextTrans(uint8 *dest, uint32 width, uint8 *textmsg, uint8 fgcolor);
void FCEU_PutImage(void);
#ifdef FRAMESKIP
void FCEU_PutImageDummy(void);
#endif

<<<<<<< HEAD
>>>>>>> e98261e5 (Update ppu.c)
<<<<<<< HEAD
extern uint8 Exit;
extern uint8 default_palette_selected;
=======
extern uint8 pale;
>>>>>>> ea3c9cc (Update ppu.c)
extern uint8 vsdip;
<<<<<<< HEAD
>>>>>>> f9553c43 (Update ppu.c)
=======
=======
#define JOY_A      0x01
#define JOY_B      0x02
#define JOY_SELECT 0x04
#define JOY_START  0x08
#define JOY_UP     0x10
#define JOY_DOWN   0x20
#define JOY_LEFT   0x40
#define JOY_RIGHT  0x80
>>>>>>> cba32ff (Update ppu.c)
>>>>>>> e98261e5 (Update ppu.c)

int UNIFLoad(const char *name, FCEUFILE *fp);
int iNESLoad(const char *name, FCEUFILE *fp);
int FDSLoad(const char *name, FCEUFILE *fp);
int NSFLoad(FCEUFILE *fp);

#endif
