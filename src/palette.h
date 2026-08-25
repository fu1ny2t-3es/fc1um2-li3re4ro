#ifndef _FCEU_PALETTE_H
#define _FCEU_PALETTE_H


typedef struct {
	uint8_t r, g, b;
} pal;

extern pal *palo;
<<<<<<< HEAD
<<<<<<< HEAD
extern uint8_t palette_game_available;
=======
=======
>>>>>>> 5926d713 (Update libretro.c)
<<<<<<< HEAD
extern uint8 palette_game_available;
>>>>>>> 0f4fda22 (Update libretro.c)
void FCEU_ResetPalette(void);
=======
extern int ipalette;
>>>>>>> d2cd9a3 (Updates)
=======
extern int palette_nes_selected;
>>>>>>> d194c3a (Update libretro.c)

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
