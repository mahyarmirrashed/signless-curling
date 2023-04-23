#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "joystick.h"

typedef enum {
  STOP_INSTRUCTION,
  CLEAN_INSTRUCTION,
  SWEEP_INSTRUCTION,
  HARD_INSTRUCTION
} instruction_t;

typedef enum {
  LEFT_SWEEPER,
  THIRD_SWEEPER,
  RIGHT_SWEEPER,
} sweeper_t;

typedef struct {
  joystick_position_t position;
} msg_t;

void btn_int_cb(void);
void esp_now_recv_cb(
  const uint8_t *mac_addr,
  const uint8_t *data,
  int data_len
);
void set_instruction(instruction_t instruction);
int setup_peer(void);
int setup_wifi(void);

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */
