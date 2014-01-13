/*
* Groove Sensor Led Bar (MY9221)
*/

#ifndef LED_BAR_H
#define LED_BAR_H

#define LED_BAR_GREEN_MAX      255

#include <dev/digout.h>

struct DPY_led_bar
{
    DEV_digout_t *clk;
    DEV_digout_t *data;
    void (*delay) (void);
};

typedef struct DPY_led_bar DPY_led_bar_t;

void DPY_led_bar_init(DPY_led_bar_t *led, DEV_digout_t *clk, DEV_digout_t *data, void (*delay)(void));

void DPY_led_bar_set_mask(DPY_led_bar_t *led, unsigned int mask);

void DPY_led_bar_set(DPY_led_bar_t *led, ACE_u8_t green, unsigned int yellow, unsigned int red);

#endif

