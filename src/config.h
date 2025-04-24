#ifndef CONFIG_H
#define CONFIG_H

#include "base.h"

#define NUM_REGS (byte) 16
#define RAM_SIZE (word) 256
#define CODE_SIZE (word) 1024

#define PROGRAM_SIZE (word) 10 + NUM_REGS + RAM_SIZE + CODE_SIZE // PID + PC + SP + LR + FLAGS + regs + ram + code

#define STORAGE_START (word) 0xE000

#define vram1 (word) 0x9002
#define vram2 (word) 0x9602

#define PROGRAM_COUNT (word) 0x9000
#endif
