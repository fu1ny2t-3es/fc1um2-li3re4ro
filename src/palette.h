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
>>>>>>> 3841c9b0 (Update libretro.c)
void FCEU_ResetPalette(void);
=======
extern int ipalette;
>>>>>>> 74ed8e6 (Updates)

void FCEU_ResetPalette(void);
void FCEU_LoadGamePalette(void);

#endif
