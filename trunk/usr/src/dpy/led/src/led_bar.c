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
            //DEV_cpudelay(DEV_NSEC_2_LOOPS(100));
        }
        else
        {
            DEV_digout_clear(led->data);
            //DEV_cpudelay(DEV_NSEC_2_LOOPS(100));
        }
        DEV_digout_toggle(led->clk);
        //DEV_cpudelay(DEV_NSEC_2_LOOPS(100));
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
        //DEV_cpudelay(DEV_NSEC_2_LOOPS(100));
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

static unsigned int
set_range (unsigned int mask, unsigned int range1, unsigned int range2)
{
    unsigned int i;
    unsigned int bit = 1;
    for (i = 0; i < 12; i++)
    {
        if (i >= range1 && i < range2)
            mask |= bit;
        bit = bit << 1;
    }
    return mask;
}

void DPY_led_bar_init(DPY_led_bar_t *led, DEV_digout_t *clk, DEV_digout_t *data)
{
    led->clk = clk;
    led->data = data;
}


//index: range 0b000000000000~0b001111111111, 1 stands light while 0 stands off;

void DPY_led_bar_set_mask(DPY_led_bar_t *led, unsigned int mask)
{
    set_mask (led, mask);
    latch_data (led);
}


//range : range 1~10.for example: 5, the 1~5 would be light 

void DPY_led_bar_set_range(DPY_led_bar_t *led, unsigned int range1, unsigned int range2)
{
    set_mask(led, set_range (0, range1, range1));
    latch_data (led);
}

void DPY_led_bar_set(DPY_led_bar_t *led, unsigned int green_range, unsigned int yellow, unsigned int red)
{
    unsigned int mask = 0;
    if (red) mask |= RED;
    if (yellow) mask |= YELLOW;
    set_mask(led, set_range (mask, 2, 2 + green_range));
    latch_data (led);
}

