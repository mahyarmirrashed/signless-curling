#ifndef RING_H
#define RING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "color.h"
#include "joystick.h"

typedef struct {
  int32_t pos;
} esp_msg;

void ring_clear(void);
void ring_init(const uint8_t);
uint32_t ring_get_color(void);
void ring_set_brightness(const uint8_t);
void ring_set_color(uint32_t);
void ring_set_color_rgb(rgb_color_t *);
void ring_set_pixel(uint8_t);
void ring_set_pixel_pos(joystick_position_t);
void ring_show(void);

#ifdef __cplusplus
}
#endif

#endif /* RING_H */
