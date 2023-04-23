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

joystick_t joystick;
joystick_position_t previous_position;
msg_t msg;

uint8_t peer_addr[ESP_NOW_ETH_ALEN] = {ADDR_BYTE0, ADDR_BYTE1, ADDR_BYTE2,
                                       ADDR_BYTE3, ADDR_BYTE4, ADDR_BYTE5};
esp_now_peer_info_t peer_info;
bool peer_setup = false;

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

  msg.position = JOYSTICK_MIDDLE_CENTER;
  previous_position = JOYSTICK_MIDDLE_CENTER;
}

void loop() {
  esp_err_t esp_err;

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
