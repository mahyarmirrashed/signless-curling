#include <errno.h>
#include <stdbool.h>

#include <Arduino.h>
#include <WiFi.h>

#include <driver/adc.h>
#include <esp_now.h>

#include "joystick.h"
#include "main.h"

#define BAUD_RATE 115200

#define JOYSTICK_X_PIN ADC1_CHANNEL_0
#define JOYSTICK_Y_PIN ADC1_CHANNEL_3
#define SLEEP_BTN_PIN  GPIO_NUM_34

#define SLEEP_BTN_DELAY 2000  // ms

static joystick_t joystick;
static joystick_position_t previous_position;
static msg_t msg;

static uint8_t peer_addr[ESP_NOW_ETH_ALEN] = {
  ADDR_BYTE0, ADDR_BYTE1, ADDR_BYTE2, ADDR_BYTE3, ADDR_BYTE4, ADDR_BYTE5};
static esp_now_peer_info_t peer_info;
static bool peer_setup = false;

static uint32_t prog_start_time = 0;

static bool btn_int_set = false;

void btn_int_cb(void) {
  if (millis() - prog_start_time > SLEEP_BTN_DELAY)
    btn_int_set = true;
}

int setup_adc(void) {
  adc1_config_width(ADC_WIDTH_12Bit);

  adc1_config_channel_atten(JOYSTICK_X_PIN, ADC_ATTEN_MAX);
  adc1_config_channel_atten(JOYSTICK_Y_PIN, ADC_ATTEN_MAX);

  return EXIT_SUCCESS;
}

int setup_peer(void) {
  if (esp_now_init() != ESP_OK) {
    errno = EPERM;
    return EXIT_FAILURE;
  }

  memcpy(peer_info.peer_addr, peer_addr, ESP_NOW_ETH_ALEN);

  peer_info.channel = 0;
  peer_info.encrypt = false;

  return EXIT_SUCCESS;
}

int setup_sleep_btn(void) {
  uint64_t bitmask;

  bitmask = 1ULL << SLEEP_BTN_PIN;

  if (esp_sleep_enable_ext1_wakeup(bitmask, ESP_EXT1_WAKEUP_ANY_HIGH)) {
    errno = EPERM;
    return EXIT_FAILURE;
  }

  pinMode(SLEEP_BTN_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(SLEEP_BTN_PIN), btn_int_cb, CHANGE);

  return EXIT_SUCCESS;
}

int setup_wifi(void) {
  WiFi.mode(WIFI_MODE_STA);

  return EXIT_SUCCESS;
}

void setup(void) {
  Serial.begin(BAUD_RATE);

  if (setup_adc() != EXIT_SUCCESS)
    exit(errno);

  if (setup_wifi() != EXIT_SUCCESS)
    exit(errno);

  if (setup_peer() != EXIT_SUCCESS)
    exit(errno);

  if (setup_sleep_btn() != EXIT_SUCCESS)
    exit(errno);

  pinMode(GPIO_NUM_5, OUTPUT);

  msg.position = JOYSTICK_MIDDLE_CENTER;
  previous_position = JOYSTICK_MIDDLE_CENTER;

  prog_start_time = millis();
}

void loop() {
  esp_err_t esp_err;

  if (btn_int_set) {
    for (int i = 0; i < 12; i += 1) {
      digitalWrite(GPIO_NUM_5, HIGH);
      delay(125);
      digitalWrite(GPIO_NUM_5, LOW);
      delay(125);
    }

    esp_deep_sleep_start();
  }

  if (!peer_setup) {
    esp_err = esp_now_add_peer(&peer_info);

    if (esp_err == ESP_OK) {
      peer_setup = true;
    } else if (esp_err == ESP_ERR_ESPNOW_FULL) {
      exit(ENOMEM);
    } else if (esp_err == ESP_ERR_ESPNOW_NO_MEM) {
      exit(ENOMEM);
    } else {
      return;
    }
  }

  joystick.x = adc1_get_raw(JOYSTICK_X_PIN);
  joystick.y = adc1_get_raw(JOYSTICK_Y_PIN);

  msg.position = joystick_get_position(joystick);

  if (msg.position != previous_position) {
    previous_position = msg.position;

    if (msg.position != JOYSTICK_MIDDLE_CENTER) {
      if (esp_now_send(NULL, (uint8_t *)&msg, sizeof(msg)) != ESP_OK)
        Serial.println("Message send failed.");
      else
        Serial.println("Message send succeeded.");
    }
  }
}
