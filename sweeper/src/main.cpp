#include <stdbool.h>

#include <Arduino.h>
#include <WiFi.h>

#include <esp_now.h>

#include "color.h"
#include "main.h"
#include "ring.h"

#define BAUD_RATE 115200

#define RING_PIN GPIO_NUM_33
#define BTN_PIN  GPIO_NUM_34

#define BTN_DELAY 250  // ms

static rgb_color_t COLOR_STOP = {.r = 255, .g = 0, .b = 0};
static rgb_color_t COLOR_CLEAN = {.r = 255, .g = 255, .b = 0};
static rgb_color_t COLOR_SWEEP = {.r = 0, .g = 255, .b = 0};
static rgb_color_t COLOR_HARD = {.r = 255, .g = 0, .b = 255};
static rgb_color_t COLOR_OFF = {.r = 0, .g = 0, .b = 0};
static rgb_color_t COLOR_SWITCH = {.r = 0, .g = 0, .b = 255};

static msg_t msg;
static sweeper_t sweeper;

static uint32_t btn_int_time_curr = 0;
static uint32_t btn_int_time_prev = 0;
static bool btn_int_set = false;

void btn_int_cb(void) {
  btn_int_time_curr = millis();

  if (btn_int_time_curr - btn_int_time_prev > BTN_DELAY) {
    btn_int_time_prev = btn_int_time_curr;
    btn_int_set = true;
  }
}

void esp_now_recv_cb(
  const uint8_t *mac_addr,
  const uint8_t *data,
  int data_len
) {
  if (sweeper == THIRD_SWEEPER)
    return;

  memcpy(&msg, data, sizeof(msg));

  if (msg.position == JOYSTICK_UPPER_LEFT) {
    if (sweeper == LEFT_SWEEPER)
      set_instruction(SWEEP_INSTRUCTION);
    else
      set_instruction(STOP_INSTRUCTION);
  } else if (msg.position == JOYSTICK_UPPER_CENTER) {
    set_instruction(SWEEP_INSTRUCTION);
  } else if (msg.position == JOYSTICK_UPPER_RIGHT) {
    if (sweeper == RIGHT_SWEEPER)
      set_instruction(SWEEP_INSTRUCTION);
    else
      set_instruction(STOP_INSTRUCTION);
  } else if (msg.position == JOYSTICK_BOTTOM_LEFT) {
    set_instruction(HARD_INSTRUCTION);
  } else if (msg.position == JOYSTICK_BOTTOM_CENTER) {
    set_instruction(STOP_INSTRUCTION);
  } else if (msg.position == JOYSTICK_BOTTOM_RIGHT) {
    set_instruction(CLEAN_INSTRUCTION);
  }
}

void set_instruction(instruction_t instruction) {
  uint8_t id;
  uint32_t prev_color;

  prev_color = ring_get_color();

  if (instruction == STOP_INSTRUCTION) {
    ring_set_color_rgb(&COLOR_STOP);
  } else if (instruction == CLEAN_INSTRUCTION) {
    ring_set_color_rgb(&COLOR_CLEAN);
  } else if (instruction == SWEEP_INSTRUCTION) {
    ring_set_color_rgb(&COLOR_SWEEP);
  } else if (instruction == HARD_INSTRUCTION) {
    ring_set_color_rgb(&COLOR_HARD);
  } else {
    return;
  }

  for (id = 5; id <= 7; id += 1)
    ring_set_pixel(id);

  ring_show();

  ring_set_color(prev_color);
}

int setup_peer(void) {
  if (esp_now_init() != ESP_OK) {
    errno = EPERM;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int setup_wifi(void) {
  WiFi.mode(WIFI_MODE_STA);

  return EXIT_SUCCESS;
}

void setup() {
  Serial.begin(BAUD_RATE);

  if (setup_wifi() != EXIT_SUCCESS)
    exit(errno);

  if (setup_peer() != EXIT_SUCCESS)
    exit(errno);

  ring_init(RING_PIN);
  ring_set_brightness(10);

  if (esp_now_register_recv_cb(esp_now_recv_cb) != EXIT_SUCCESS)
    exit(EXIT_FAILURE);

  pinMode(BTN_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BTN_PIN), btn_int_cb, CHANGE);

  set_instruction(STOP_INSTRUCTION);
}

void loop() {
  if (btn_int_set) {
    ring_set_color_rgb(&COLOR_OFF);
    ring_set_pixel(0);
    ring_set_pixel(11);
    ring_set_pixel(1);

    ring_set_color_rgb(&COLOR_SWITCH);

    if (sweeper == LEFT_SWEEPER) {
      sweeper = THIRD_SWEEPER;

      ring_set_pixel(0);
    } else if (sweeper == THIRD_SWEEPER) {
      sweeper = RIGHT_SWEEPER;

      ring_set_pixel(1);
    } else if (sweeper == RIGHT_SWEEPER) {
      sweeper = LEFT_SWEEPER;

      ring_set_pixel(11);
    }

    ring_show();

    if (sweeper == THIRD_SWEEPER)
      set_instruction(STOP_INSTRUCTION);

    btn_int_set = false;
  }
}
