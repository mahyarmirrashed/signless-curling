#ifndef HSLLIB_H
#define HSLLIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define MIN_RGB_VALUE 0U
#define MAX_RGB_VALUE 255U

#define MIN_HUE_VALUE   0.0F
#define MAX_HUE_VALUE   360.0F
#define MIN_SAT_VALUE   0.0F
#define MAX_SAT_VALUE   1.0F
#define MIN_LIGHT_VALUE 0.0F
#define MAX_LIGHT_VALUE 1.0F

typedef struct {
  float_t h;
  float_t s;
  float_t l;
} hsl_color_t;

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} rgb_color_t;

int color_rgb2hsl(rgb_color_t *, hsl_color_t *);
int color_hsl2rgb(hsl_color_t *, rgb_color_t *);

#ifdef __cplusplus
}
#endif

#endif /* HSLLIB_H */