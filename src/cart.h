#ifndef _FCEU_CART_H
#define _FCEU_CART_H

<<<<<<< HEAD
#include "fceu-types.h"
=======
enum DefaultValues { DEFAULT = -1, NOEXTRA = -1 };
enum ConsoleSystem { NES_NTSC = 0, NES_PAL = 1, MULTI = 2, DENDY = 3 };
>>>>>>> 8bf4e730 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)

typedef struct {
	/* Set by mapper/board code: */
	void (*Power)(void);
	void (*Reset)(void);
	void (*Close)(void);
<<<<<<< HEAD
	uint8_t *SaveGame[4];		/* Pointers to memory to save/load. */
	uint32_t SaveGameLen[4];	/* How much memory to save/load. */
=======

	uint8 *SaveGame[4];    /* Pointers to memory to save/load. */
	uint32 SaveGameLen[4]; /* How much memory to save/load. */
>>>>>>> 8bf4e730 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)

	/* Set by iNES/UNIF loading code. */
<<<<<<< HEAD
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
=======
	int format;
	int iNES2;   /* iNES version */
	int mapper; /* mapper used */
	int submapper;
	int mirror;      /* As set in the header or chunk.
	                    * iNES/UNIF specific.  Intended
	                    * to help support games like "Karnov"
	                    * that are not really MMC3 but are
	                    * set to mapper 4.
	                    */
	int mirror2bits; /* a 2bit representation for mirroring.
	                    * For use in nonstandard way like apply one-screen mirroring on
	                    * mapper 30.
	                    */
	int battery;     /* Presence of an actual battery. */
	int trainer;     /* Presense of trainer data */
	int MiscRoms;    /* Presense of misc roms */
	int region;      /* video system timing (NTSC, PAL, Dendy */
	int ConsoleType;
	int InputTypes;
	int VS_PPUTypes;
	int VS_HWType;

	int PRGRomSize;     /* actual prg rom size in bytes */
	int CHRRomSize;     /* actual chr rom size in bytes */
	int PRGRamSize;     /* prg ram size in bytes (volatile) */
	int CHRRamSize;     /* chr ram size in bytes (volatile) */
	int PRGRamSaveSize; /* prg ram size in bytes (non-volatile or battery backed) */
	int CHRRamSaveSize; /* chr ram size in bytes (non-volatile or battery backed) */
>>>>>>> e365e6e (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)

<<<<<<< HEAD
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
=======
	uint8 MD5[16];
	uint32 PRGCRC32;
	uint32 CHRCRC32;
	uint32 CRC32; /* Should be set by the iNES/UNIF loading
	               * code, used by mapper/board code, maybe
	               * other code in the future.
	               */
} CartInfo;

typedef struct mem_t {
	uint8 *data;
	uint32 size;
} mem_t;

typedef struct romData_t {
	mem_t prg; /* size in 16k banks */
	mem_t chr; /* size in 8k banks */
	mem_t trainer;
	mem_t misc;
	mem_t disk;
	mem_t disko;
} romData_t;

extern uint8 *Page[32], *VPage[8], *MMC5SPRVPage[8], *MMC5BGVPage[8];

void ResetCartMapping(void);
void SetupCartPRGMapping(int chip, uint8 *p, uint32 size, uint8 ram);
void SetupCartCHRMapping(int chip, uint8 *p, uint32 size, uint8 ram);
void SetupCartMirroring(int m, int hard, uint8 *extra);
>>>>>>> 8bf4e730 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)

DECLFR(CartBROB);
DECLFR(CartBR);
DECLFW(CartBW);

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
=======
void setprg2(uint16 A, uint16 V);
void setprg4(uint16 A, uint16 V);
void setprg8(uint16 A, uint16 V);
void setprg16(uint16 A, uint16 V);
void setprg32(uint16 A, uint16 V);
>>>>>>> 8bf4e730 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)

void setprg2r(int r, uint16 A, uint16 V);
void setprg4r(int r, uint16 A, uint16 V);
void setprg8r(int r, uint16 A, uint16 V);
void setprg16r(int r, uint16 A, uint16 V);
void setprg32r(int r, uint16 A, uint16 V);

void setchr1r(int r, uint16 A, uint16 V);
void setchr2r(int r, uint16 A, uint16 V);
void setchr4r(int r, uint16 A, uint16 V);
void setchr8r(int r, uint16 V);

<<<<<<< HEAD
void setchr1(uint32 A, uint32 V);
void setchr2(uint32 A, uint32 V);
void setchr4(uint32 A, uint32 V);
void setchr8(uint32 V);
>>>>>>> 09aa6a76 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)

void setmirror(int t);
void setmirrorw(int a, int b, int c, int d);
<<<<<<< HEAD
void FASTAPASS(3) setntamem(uint8_t *p, int ram, uint32_t b);
=======
void setntamem(uint8 *p, int ram, uint32 b);
>>>>>>> 09aa6a76 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
=======
void setchr1(uint16 A, uint16 V);
void setchr2(uint16 A, uint16 V);
void setchr4(uint16 A, uint16 V);
void setchr8(uint16 V);

void setmirror(int t);
void setmirrorw(int a, int b, int c, int d);
void setntamem(uint8 *p, int ram, int b);
>>>>>>> 8bf4e730 (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)

enum MirroringType {
	MI_H = 0, /* horizontal */
	MI_V = 1, /* vertical */
	MI_0 = 2, /* single-screen 0 */
	MI_1 = 3, /* single-screen 1 */
	MI_4 = 4  /* four-screen */
};

extern CartInfo iNESCart;

extern uint8 *WRAM;
extern uint32 WRAMSIZE;
extern romData_t ROM;

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
