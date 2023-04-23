#ifndef JOYSTICK_H
#define JOYSTICK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef enum {
  JOYSTICK_BOTTOM_LEFT,
  JOYSTICK_BOTTOM_CENTER,
  JOYSTICK_BOTTOM_RIGHT,
  JOYSTICK_MIDDLE_LEFT,
  JOYSTICK_MIDDLE_CENTER,
  JOYSTICK_MIDDLE_RIGHT,
  JOYSTICK_UPPER_LEFT,
  JOYSTICK_UPPER_CENTER,
  JOYSTICK_UPPER_RIGHT
} joystick_position_t;

#define JOYSTICK_THREE_THIRDS (4096 - 1)
#define JOYSTICK_ONE_THIRD    (JOYSTICK_THREE_THIRDS / 3)
#define JOYSTICK_TWO_THIRD    (JOYSTICK_ONE_THIRD * 2)

typedef struct {
  uint16_t x;
  uint16_t y;
  uint16_t z;
} joystick_t;

void joystick_init(void);
joystick_position_t joystick_get_position(const joystick_t);
bool joystick_switch_pressed(const joystick_t);

#ifdef __cplusplus
}
#endif

#endif /* JOYSTICK_H */
