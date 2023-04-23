#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "joystick.h"

#define ADDR_BYTE0 0xFF
#define ADDR_BYTE1 0xFF
#define ADDR_BYTE2 0xFF
#define ADDR_BYTE3 0xFF
#define ADDR_BYTE4 0xFF
#define ADDR_BYTE5 0xFF

typedef struct {
  joystick_position_t position;
} msg_t;

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */
