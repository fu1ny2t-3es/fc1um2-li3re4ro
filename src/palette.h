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
>>>>>>> 29d840dc (Change PLATFORM_SUPPORTS_ references to FRONTEND_SUPPORTS_)
void FCEU_ResetPalette(void);
=======
extern int ipalette;
>>>>>>> 81ca407 (Updates)

void FCEU_ResetPalette(void);
void FCEU_LoadGamePalette(void);

#endif
