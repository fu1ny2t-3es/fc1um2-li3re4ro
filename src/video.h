#ifndef _FCEU_VIDEO_H
#define _FCEU_VIDEO_H

<<<<<<< HEAD
int FCEU_InitVirtualVideo(void);
void FCEU_KillVirtualVideo(void);
extern uint8_t *XBuf;
extern uint8_t *XDBuf;
extern int show_crosshair;
<<<<<<< HEAD
void FCEU_DrawNumberRow(uint8_t *XBuf, int *nstatus, int cur);
=======
>>>>>>> d5085b8d (Update libretro_core_options.h)
=======
extern uint8 *XBuf;
extern uint8 *XDBuf;

int FCEU_InitVirtualVideo(void);
void FCEU_KillVirtualVideo(void);
void FCEU_DrawNumberRow(uint8 *target, int *nstatus, int cur);
>>>>>>> a1c8c17c (Update libretro_core_options.h)

#endif
