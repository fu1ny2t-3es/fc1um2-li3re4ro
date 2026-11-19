#ifndef _FCEU_PALETTE_H
#define _FCEU_PALETTE_H

typedef struct {
	uint8_t r, g, b;
} pal;

extern pal *palo;
<<<<<<< HEAD
extern uint8_t palette_game_available;
=======
<<<<<<< HEAD
extern uint8 palette_game_available;
>>>>>>> f31fc48c (Update libretro_core_options.h)
void FCEU_ResetPalette(void);
=======
extern int ipalette;
>>>>>>> 6638d4c (Updates)

void FCEU_ResetPalette(void);
void FCEU_LoadGamePalette(void);

#endif
