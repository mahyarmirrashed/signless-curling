#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#include "color.h"

static void validate_rgb(rgb_color_t *);
static void validate_hsl(hsl_color_t *);

int color_rgb2hsl(rgb_color_t *rgb, hsl_color_t *hsl) {
  assert(rgb != NULL);
  assert(hsl != NULL);

  validate_rgb(rgb);

  float_t r_prime = (float_t)rgb->r / MAX_RGB_VALUE;
  float_t g_prime = (float_t)rgb->g / MAX_RGB_VALUE;
  float_t b_prime = (float_t)rgb->b / MAX_RGB_VALUE;

  float_t c_max = fmaxf(r_prime, fmaxf(g_prime, b_prime));
  float_t c_min = fminf(r_prime, fminf(g_prime, b_prime));

  float_t delta = c_max - c_min;

  // hue calculation
  if (delta == 0.0f) {
    hsl->h = 0.0f;
  } else if (c_max == r_prime) {
    hsl->h = fmodf((g_prime - b_prime) / delta, 6);
  } else if (c_max == g_prime) {
    hsl->h = (b_prime - r_prime) / delta + 2;
  } else if (c_max == b_prime) {
    hsl->h = (r_prime - g_prime) / delta + 4;
  } else {
    errno = EINVAL;
    return -1;
  }
  hsl->h *= 60;

  // saturation calculation
  hsl->s = delta / (1 - fabsf(2 * hsl->l - 1));

  // lightness calculation
  hsl->l = (c_max + c_min) / 2;

  validate_hsl(hsl);

  return 0;
}

int color_hsl2rgb(hsl_color_t *hsl, rgb_color_t *rgb) {
  assert(hsl != NULL);
  assert(rgb != NULL);

  validate_hsl(hsl);

  float_t c = (1 - fabsf(2 * hsl->l - 1)) * hsl->s;
  float_t x = c * (1 - fabsf(fmodf(hsl->h / 60, 2) - 1));
  float_t m = hsl->l - (c / 2);

  float_t r_prime;
  float_t g_prime;
  float_t b_prime;

  if (0 <= hsl->h && hsl->h < 60) {
    r_prime = c;
    g_prime = x;
    b_prime = 0;
  } else if (60 <= hsl->h && hsl->h < 120) {
    r_prime = x;
    g_prime = c;
    b_prime = 0;
  } else if (120 <= hsl->h && hsl->h < 180) {
    r_prime = 0;
    g_prime = c;
    b_prime = x;
  } else if (180 <= hsl->h && hsl->h < 240) {
    r_prime = 0;
    g_prime = x;
    b_prime = c;
  } else if (240 <= hsl->h && hsl->h < 300) {
    r_prime = x;
    g_prime = 0;
    b_prime = c;
  } else if (300 <= hsl->h && hsl->h < 360) {
    r_prime = c;
    g_prime = 0;
    b_prime = x;
  } else {
    errno = EINVAL;
    return -1;
  }

  rgb->r = MAX_RGB_VALUE * (r_prime + m);
  rgb->g = MAX_RGB_VALUE * (g_prime + m);
  rgb->b = MAX_RGB_VALUE * (b_prime + m);

  validate_rgb(rgb);

  return 0;
}

static void validate_hsl(hsl_color_t *hsl) {
  assert(hsl != NULL);

  assert(hsl->h >= MIN_HUE_VALUE);
  assert(hsl->h < MAX_HUE_VALUE);
  assert(hsl->s >= MIN_SAT_VALUE);
  assert(hsl->s <= MAX_SAT_VALUE);
  assert(hsl->l >= MIN_LIGHT_VALUE);
  assert(hsl->l <= MAX_LIGHT_VALUE);
}

static void validate_rgb(rgb_color_t *rgb) {
  assert(rgb != NULL);

  assert(rgb->r >= MIN_RGB_VALUE);
  assert(rgb->r <= MAX_RGB_VALUE);
  assert(rgb->g >= MIN_RGB_VALUE);
  assert(rgb->g <= MAX_RGB_VALUE);
  assert(rgb->b >= MIN_RGB_VALUE);
  assert(rgb->b <= MAX_RGB_VALUE);
}
