#ifndef BOARD_LED_H
#define BOARD_LED_H		1

#include "hal_common_includes.h"

typedef enum {
	LED9,
	LED6,
	LED8
} led;

void board_led_init(void);
void board_led_on(led led_id);
void board_led_off(led led_id);
void board_led_all_off();
void board_led_toggle(led led_id);

#endif
