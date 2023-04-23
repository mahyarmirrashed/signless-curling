#include <Adafruit_NeoPixel.h>

#include "color.h"
#include "joystick.h"
#include "ring.h"

#define RING_PIXELS 12

static uint32_t ring_color;
static Adafruit_NeoPixel *ring;

void ring_clear(void) {
  ring->clear();
}

void ring_init(const uint8_t pin) {
  ring = new Adafruit_NeoPixel(RING_PIXELS, pin, NEO_GRB + NEO_KHZ800);

  ring->begin();

  ring_clear();
}

uint32_t ring_get_color(void) {
  return ring_color;
}

void ring_set_brightness(const uint8_t brightness) {
  ring->setBrightness(brightness);
}

void ring_set_color(uint32_t color) {
  ring_color = color;
}

void ring_set_color_rgb(rgb_color_t *rgb) {
  ring_color = ring->Color(rgb->r, rgb->g, rgb->b);
}

void ring_set_pixel(uint8_t id) {
  ring->setPixelColor(id, ring_color);
}

void ring_set_pixel_pos(joystick_position_t position) {
  switch (position) {
    case JOYSTICK_UPPER_CENTER:
      ring_set_pixel(0);
      break;
    case JOYSTICK_UPPER_RIGHT:
      ring_set_pixel(1);
      ring_set_pixel(2);
      break;
    case JOYSTICK_MIDDLE_RIGHT:
      ring_set_pixel(3);
      break;
    case JOYSTICK_BOTTOM_RIGHT:
      ring_set_pixel(4);
      ring_set_pixel(5);
      break;
    case JOYSTICK_BOTTOM_CENTER:
      ring_set_pixel(6);
      break;
    case JOYSTICK_BOTTOM_LEFT:
      ring_set_pixel(7);
      ring_set_pixel(8);
      break;
    case JOYSTICK_MIDDLE_LEFT:
      ring_set_pixel(9);
      break;
    case JOYSTICK_UPPER_LEFT:
      ring_set_pixel(10);
      ring_set_pixel(11);
      break;
    case JOYSTICK_MIDDLE_CENTER:
      break;
    default:
      break;
  }
}

void ring_show(void) {
  ring->show();
}
