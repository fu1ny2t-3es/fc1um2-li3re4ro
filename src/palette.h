#ifndef _FCEU_PALETTE_H
#define _FCEU_PALETTE_H

typedef struct {
	uint8 r, g, b;
} pal;

extern pal *palo;
<<<<<<< HEAD
extern uint8 palette_game_available;
void FCEU_ResetPalette(void);
=======
extern int ipalette;
>>>>>>> b9a594b (Updates)

void FCEU_ResetPalette(void);
void FCEU_LoadGamePalette(void);

#endif
