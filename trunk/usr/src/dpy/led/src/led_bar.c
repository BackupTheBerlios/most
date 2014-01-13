#include <ace/stddef.h>
#include <dev/cpu.h>
#include <dpy/led_bar.h>

#define CMD_MODE  0x0000   //Work on 8-bit mode
#define ON        0x00ff   //8-bit 1 data
#define SHUT      0x0000   //8-bit 0 data
#define RED       0x0001
#define YELLOW    0x0002


static void
send_16bit_data(DPY_led_bar_t *led, unsigned int data)
{
    for(unsigned int i=0;i<16;i++)
    {
        if(data&0x8000)
        {
            DEV_digout_set(led->data);
            if (led->delay) led->delay();
        }
        else
        {
            DEV_digout_clear(led->data);
            if (led->delay) led->delay();
        }
        DEV_digout_toggle(led->clk);
        if (led->delay) led->delay();
        data <<= 1;
    }
}

static void
latch_data(DPY_led_bar_t *led)
{
    DEV_digout_clear(led->data);
    DEV_cpudelay(DEV_USEC_2_LOOPS(10));
    for(unsigned int i=0; i < 8; i++)
    {
        DEV_digout_toggle(led->data);
        if (led->delay) led->delay();
    }
}

static void
set_mask (DPY_led_bar_t *led, unsigned int mask)
{
    unsigned int i;
    send_16bit_data(led, CMD_MODE); 
    for (i = 0; i < 12; i++)
    {
        if (mask & 0x0001)
            send_16bit_data(led, ON);
        else
            send_16bit_data(led, SHUT);
        mask = mask >> 1;
    }
}


void DPY_led_bar_init(DPY_led_bar_t *led, DEV_digout_t *clk, DEV_digout_t *data, void (*delay)(void))
{
    led->clk = clk;
    led->data = data;
    led->delay = delay;
}


//index: range 0b000000000000~0b001111111111, 1 stands light while 0 stands off;

void DPY_led_bar_set_mask(DPY_led_bar_t *led, unsigned int mask)
{
    set_mask (led, mask);
    latch_data (led);
}


void DPY_led_bar_set(DPY_led_bar_t *led, ACE_u8_t green, unsigned int yellow, unsigned int red)
{
    unsigned int mask = 0;
    if (red) mask |= RED;
    if (yellow) mask |= YELLOW;
    mask |= (green << 2);
    set_mask(led, mask);
    latch_data (led);
}

