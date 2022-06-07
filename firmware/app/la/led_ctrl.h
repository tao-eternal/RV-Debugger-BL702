#pragma once

#include "dev_cfg.h"

#define LED_SET(idx, sta) gpio_write(led_pins[(idx)], !(sta))
#define LED_TOGGLE(idx) gpio_toggle(led_pins[(idx)])

/************************  led ctrl functions  ************************/
static uint32_t led_pins[2] = {LED0_PIN, LED1_PIN};

static inline void led_gpio_init(void) {
  gpio_set_mode(led_pins[0], GPIO_OUTPUT_MODE);
  gpio_set_mode(led_pins[1], GPIO_OUTPUT_MODE);
  LED_SET(0, 0);
  LED_SET(1, 0);
}
