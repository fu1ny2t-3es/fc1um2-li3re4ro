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
>>>>>>> c7dde5ce (Update libretro_core_options.h)
void FCEU_ResetPalette(void);
=======
extern int ipalette;
>>>>>>> a2f6bdc (Updates)

void FCEU_ResetPalette(void);
void FCEU_LoadGamePalette(void);

#endif
