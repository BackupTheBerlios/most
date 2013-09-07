/*
* Groove Sensor Led Bar (MY9221)
*/

#ifndef LED_BAR_H
#define LED_BAR_H

#include <dev/digout.h>

struct DPY_led_bar
{
    DEV_digout_t *clk;
    DEV_digout_t *data;
};

typedef struct DPY_led_bar DPY_led_bar_t;

void DPY_led_bar_init(DPY_led_bar_t *led, DEV_digout_t *clk, DEV_digout_t *data);

void DPY_led_bar_set_mask(DPY_led_bar_t *led, unsigned int mask);

void DPY_led_bar_set_range(DPY_led_bar_t *led, unsigned int range1,unsigned int range2);

void DPY_led_bar_set(DPY_led_bar_t *led, unsigned int green_range, unsigned int yellow, unsigned int red);

#endif

