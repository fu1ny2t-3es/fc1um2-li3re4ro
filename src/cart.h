#ifndef _FCEU_CART_H
#define _FCEU_CART_H

#include "fceu-types.h"

typedef struct {
	/* Set by mapper/board code: */
	void (*Power)(void);
	void (*Reset)(void);
	void (*Close)(void);
	uint8_t *SaveGame[4];		/* Pointers to memory to save/load. */
	uint32_t SaveGameLen[4];	/* How much memory to save/load. */

	/* Set by iNES/UNIF loading code. */
	int iNES2;		/* iNES version */
	int mapper;		/* mapper used */
	int submapper;	/* submapper used */ /* TODO: */
	int mirror;		/* As set in the header or chunk.
					 * iNES/UNIF specific.  Intended
					 * to help support games like "Karnov"
					 * that are not really MMC3 but are
					 * set to mapper 4.
					 */
	int battery;	/* Presence of an actual battery. */
	int PRGRomSize;		/* prg rom size in bytes */
	int CHRRomSize;		/* chr rom size in bytes */
	int PRGRamSize;		/* prg ram size in bytes (volatile) */
	int CHRRamSize;		/* chr ram size in bytes (volatile) */
	int PRGRamSaveSize;	/* prg ram size in bytes (non-volatile or battery backed) */
	int CHRRamSaveSize;	/* chr ram size in bytes (non-volatile or battery backed) */
	int miscROMSize;
	int miscROMNumber;
	uint64_t totalFileSize;
	int region;			/* video system timing (ntsc, pal, dendy */

	uint8_t MD5[16];
	uint32_t PRGCRC32;
	uint32_t CHRCRC32;
	uint32_t CRC32;	/* Should be set by the iNES/UNIF loading
					 * code, used by mapper/board code, maybe
					 * other code in the future.
					 */
} CartInfo;

extern uint8_t *Page[32], *VPage[8], *MMC5SPRVPage[8], *MMC5BGVPage[8];

void ResetCartMapping(void);
void SetupCartPRGMapping(int chip, uint8_t *p, uint32_t size, int ram);
void SetupCartCHRMapping(int chip, uint8_t *p, uint32_t size, int ram);
void SetupCartMirroring(int m, int hard, uint8_t *extra);

uint8 CartBROB(uint32 A);
uint8 CartBR(uint32 A);
void CartBW(uint32 A, uint8 V);

extern uint8_t *PRGptr[32];
extern uint8_t *CHRptr[32];

extern uint32_t PRGsize[32];
extern uint32_t CHRsize[32];

extern uint32_t PRGmask2[32];
extern uint32_t PRGmask4[32];
extern uint32_t PRGmask8[32];
extern uint32_t PRGmask16[32];
extern uint32_t PRGmask32[32];

extern uint32_t CHRmask1[32];
extern uint32_t CHRmask2[32];
extern uint32_t CHRmask4[32];
extern uint32_t CHRmask8[32];

<<<<<<< HEAD
void FASTAPASS(2) setprg2(uint32_t A, uint32_t V);
void FASTAPASS(2) setprg4(uint32_t A, uint32_t V);
void FASTAPASS(2) setprg8(uint32_t A, uint32_t V);
void FASTAPASS(2) setprg16(uint32_t A, uint32_t V);
void FASTAPASS(2) setprg32(uint32_t A, uint32_t V);

void FASTAPASS(3) setprg2r(int r, uint32_t A, uint32_t V);
void FASTAPASS(3) setprg4r(int r, uint32_t A, uint32_t V);
void FASTAPASS(3) setprg8r(int r, uint32_t A, uint32_t V);
void FASTAPASS(3) setprg16r(int r, uint32_t A, uint32_t V);
void FASTAPASS(3) setprg32r(int r, uint32_t A, uint32_t V);

void FASTAPASS(3) setchr1r(int r, uint32_t A, uint32_t V);
void FASTAPASS(3) setchr2r(int r, uint32_t A, uint32_t V);
void FASTAPASS(3) setchr4r(int r, uint32_t A, uint32_t V);
void FASTAPASS(2) setchr8r(int r, uint32_t V);

void FASTAPASS(2) setchr1(uint32_t A, uint32_t V);
void FASTAPASS(2) setchr2(uint32_t A, uint32_t V);
void FASTAPASS(2) setchr4(uint32_t A, uint32_t V);
void FASTAPASS(2) setchr8(uint32_t V);
=======
void setprg2(uint32 A, uint32 V);
void setprg4(uint32 A, uint32 V);
void setprg8(uint32 A, uint32 V);
void setprg16(uint32 A, uint32 V);
void setprg32(uint32 A, uint32 V);

void setprg2r(int r, uint32 A, uint32 V);
void setprg4r(int r, uint32 A, uint32 V);
void setprg8r(int r, uint32 A, uint32 V);
void setprg16r(int r, uint32 A, uint32 V);
void setprg32r(int r, uint32 A, uint32 V);

void setchr1r(int r, uint32 A, uint32 V);
void setchr2r(int r, uint32 A, uint32 V);
void setchr4r(int r, uint32 A, uint32 V);
void setchr8r(int r, uint32 V);

void setchr1(uint32 A, uint32 V);
void setchr2(uint32 A, uint32 V);
void setchr4(uint32 A, uint32 V);
void setchr8(uint32 V);
>>>>>>> d5085b8d (Update libretro_core_options.h)

void setmirror(int t);
void setmirrorw(int a, int b, int c, int d);
<<<<<<< HEAD
void FASTAPASS(3) setntamem(uint8_t *p, int ram, uint32_t b);
=======
void setntamem(uint8 *p, int ram, uint32 b);
>>>>>>> d5085b8d (Update libretro_core_options.h)

#define MI_H 0
#define MI_V 1
#define MI_0 2
#define MI_1 3

extern int geniestage;

void FCEU_GeniePower(void);

void FCEU_OpenGenie(void);
void FCEU_CloseGenie(void);
void FCEU_KillGenie(void);

/* Helpers for the iNES1-vs-iNES2 sizing dichotomy.
 *
 * iNES1 headers don't carry explicit RAM-size fields, so mappers fall
 * back to a default (typically 8 KiB WRAM). iNES2 headers do, in which
 * case the runtime must use PRGRamSize + PRGRamSaveSize (resp.
 * CHRRamSize + CHRRamSaveSize). This pattern was hand-coded at every
 * site as `info->iNES2 ? (info->PRGRamSize + info->PRGRamSaveSize)
 * : default_bytes`, which is verbose and easy to write inconsistently.
 *
 * These inline helpers centralise the rule. They return byte counts
 * (caller divides by 1024 if it needs KiB). */
static INLINE uint32_t CartInfo_PRGRAM_bytes(const CartInfo *info, uint32_t default_bytes)
{
	if (info->iNES2)
		return (uint32_t)(info->PRGRamSize + info->PRGRamSaveSize);
	return default_bytes;
}

static INLINE uint32_t CartInfo_CHRRAM_bytes(const CartInfo *info, uint32_t default_bytes)
{
	if (info->iNES2)
		return (uint32_t)(info->CHRRamSize + info->CHRRamSaveSize);
	return default_bytes;
}

#endif
