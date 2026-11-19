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
>>>>>>> a1c8c17c (Update libretro_core_options.h)
<<<<<<< HEAD
extern uint8 palette_game_available;
>>>>>>> f31fc48c (Update libretro_core_options.h)
void FCEU_ResetPalette(void);
=======
extern int ipalette;
>>>>>>> 6638d4c (Updates)
=======
extern int palette_nes_selected;
>>>>>>> 593a216 (Update libretro_core_options.h)

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
