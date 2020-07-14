#ifndef __STM32F10x_H
#define __STM32F10x_H

// Stub header replacing the original for unit tests

#include <stdint.h>

// Normally passed through commandline when compiling
#define GLOBAL_VID 0x20a0
#define GLOBAL_PID 0x4108

#define __IO volatile

#ifndef __cplusplus
typedef enum
{ FALSE = 0, TRUE = !FALSE } bool;
#endif

// typedef enum
// { DISABLE = 0, ENABLE = !DISABLE } FunctionalState;

#endif