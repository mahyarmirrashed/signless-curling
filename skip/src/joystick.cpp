#include <stdbool.h>
#include <stdint.h>

#include "joystick.h"

void joystick_init(void) {
  return;
}

joystick_position_t joystick_get_position(const joystick_t joystick) {
  if (joystick.x < JOYSTICK_ONE_THIRD) {
    if (joystick.y < JOYSTICK_ONE_THIRD) {
      return JOYSTICK_BOTTOM_RIGHT;
    } else if (joystick.y < JOYSTICK_TWO_THIRD) {
      return JOYSTICK_MIDDLE_RIGHT;
    } else {
      return JOYSTICK_UPPER_RIGHT;
    }
  } else if (joystick.x < JOYSTICK_TWO_THIRD) {
    if (joystick.y < JOYSTICK_ONE_THIRD) {
      return JOYSTICK_BOTTOM_CENTER;
    } else if (joystick.y < JOYSTICK_TWO_THIRD) {
      return JOYSTICK_MIDDLE_CENTER;
    } else {
      return JOYSTICK_UPPER_CENTER;
    }
  } else {
    if (joystick.y < JOYSTICK_ONE_THIRD) {
      return JOYSTICK_BOTTOM_LEFT;
    } else if (joystick.y < JOYSTICK_TWO_THIRD) {
      return JOYSTICK_MIDDLE_LEFT;
    } else {
      return JOYSTICK_UPPER_LEFT;
    }
  }
}

bool joystick_switch_pressed(const joystick_t joystick) {
  return joystick.z == 0;
}
