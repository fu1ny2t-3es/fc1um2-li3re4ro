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
>>>>>>> 74114ad0 (Update libretro.c)
=======
extern uint8 *XBuf;
extern uint8 *XDBuf;

int FCEU_InitVirtualVideo(void);
void FCEU_KillVirtualVideo(void);
void FCEU_DrawNumberRow(uint8 *target, int *nstatus, int cur);
>>>>>>> 7b06ddc4 (Update libretro.c)

#endif
