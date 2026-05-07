#ifndef _FCEU_ENDIAN_H
#define _FCEU_ENDIAN_H

#include "fceu-memory.h"

<<<<<<< HEAD
uint32_t FCEU_de32lsb(const uint8_t *);
=======
int write32le_mem(uint32 b, memstream_t *mem);
int read32le_mem(uint32 *Bufo, memstream_t *mem);

void FCEU_en32lsb(uint8 *, uint32);
uint32 FCEU_de32lsb(const uint8 *);
>>>>>>> ea32266a (Refactors)

#endif
