#ifndef _FCEU_PALETTE_H
#define _FCEU_PALETTE_H


typedef struct {
	uint8 r, g, b;
} pal;

extern pal *palo;
<<<<<<< HEAD
<<<<<<< HEAD
extern uint8 palette_game_available;
void FCEU_ResetPalette(void);
=======
extern int ipalette;
>>>>>>> 58d5196 (Updates)
=======
extern int palette_nes_selected;
>>>>>>> cba32ff (Update ppu.c)

void FCEU_ResetPalette(void);
void FCEU_LoadGamePalette(void);

#define PALETTE_ARRAY_SIZE 512 /* rgb palette size */

#define PAL_NES_DEFAULT    0
#define PAL_RP2C04_0001    1
#define PAL_RP2C04_0002    2
#define PAL_RP2C04_0003    3
#define PAL_RP2C04_0004    4
#define PAL_RP2C03         5

#endif
