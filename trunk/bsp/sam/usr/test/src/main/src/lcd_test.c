#include <ace/stddef.h>
#include <ace/stdio.h>
#include <uso/stack.h>
#include <uso/sleep.h>
#include <uso/event.h>
#include <uso/thread.h>
#include <uso/log.h>
#include <dev/cpu.h>
#include <cli/exec.h>
#include <dpy/nokia6100/nokia6100.h>

#include <test.h>
#include <lcd_test.h>
#include <logo.h>
#include <arch/digio.h>
#include <arch/spi.h>
#include <arch/adc.h>
#include <arch/pwm.h>
#include <arch/ticks.h>
#include <init/events.h>

#define LCD_SLOT_SIZE 3

enum lcd_event
{
    LCD_event_start = SAM_LCD_EVENT_BASE,
    LCD_event_enter,
    LCD_event_logo,
    LCD_event_ad4,
    LCD_event_trim,
    LCD_event_temp,
    LCD_event_mic,
    LCD_event_audio
};

enum lcd_state
{
    LCD_STATE_IDLE,
    LCD_STATE_LOGO,
    LCD_STATE_AD4,
    LCD_STATE_TRIM,
    LCD_STATE_TEMP,
    LCD_STATE_MIC,
    LCD_STATE_AUDIO,
    LCD_STATE_CARD
};

static CLI_exec_t lcd_test;
static USO_slot_t lcd_slot;

static enum lcd_state lcd_state = LCD_STATE_IDLE;

static const long lcd_event_ids[] = {
    LCD_event_start,
    LCD_event_enter,
    LCD_event_logo,
    LCD_event_ad4,
    LCD_event_trim,
    LCD_event_temp,
    LCD_event_mic,
    LCD_event_audio,
    SAM_switch_1_pressed,
    SAM_switch_2_pressed,
    SAM_joystick_up_pressed,
    SAM_joystick_down_pressed,
    SAM_joystick_left_pressed,
    SAM_joystick_right_pressed,
    SAM_joystick_center_pressed,
    SAM_card_inserted,
    SAM_card_removed,
    0
};

static void
display_init (void)
{
    if (0 > DPY_nokia6100_spi_init ((DEV_spi_dev_t *) & SAM_display))
    {
        USO_log_puts (USO_LL_ERROR, "LCD spi init failed!\n");
    }
    DPY_nokia6100_reset (&SAM_lcd_reset);
    DPY_nokia6100_init ((DEV_spi_dev_t *) & SAM_display);
}

static void
display_logo (void)
{
    DPY_nokia6100_set_rectangle (0, 0, 131, 131, DPY_NOKIA6100_FILL, DPY_NOKIA6100_GREEN);
    DPY_nokia6100_put_string ("M O S T", 8, 37, DPY_NOKIA6100_LARGE, DPY_NOKIA6100_RED,
                              DPY_NOKIA6100_GREEN);
    DPY_nokia6100_put_string (SAM_APPLICATION, 102, 30, DPY_NOKIA6100_SMALL, DPY_NOKIA6100_RED,
                              DPY_NOKIA6100_GREEN);
    DPY_nokia6100_put_string (ACE_MOST_VERSION, 115, 30, DPY_NOKIA6100_SMALL, DPY_NOKIA6100_RED,
                              DPY_NOKIA6100_GREEN);
    DPY_nokia6100_set_rectangle (127, 0, 131, 131, DPY_NOKIA6100_FILL, DPY_NOKIA6100_WHITE);

    DPY_nokia6100_write_bitmap (logo, 30, 100, 30, 100, logo_get_size ());
}

#define LCD_LEN 131
#define LCD_BLUE_LEN 30
#define LCD_WHITE_LEN (LCD_LEN - LCD_BLUE_LEN)

static void
display_running (void)
{
    static int y1 = 0, y2 = LCD_WHITE_LEN;

    DPY_nokia6100_set_rectangle (127, y1, 131, y1 - 1, DPY_NOKIA6100_FILL, DPY_NOKIA6100_BLUE);
    DPY_nokia6100_set_rectangle (127, y2, 131, y2 - 1, DPY_NOKIA6100_FILL, DPY_NOKIA6100_WHITE);
    if (y1++ > LCD_LEN)
        y1 = 0;
    if (y2++ > LCD_LEN)
        y2 = 0;

    USO_sleep (USO_MSEC_2_TICKS (25));
}

static void
display_menu (char *s)
{
    DPY_nokia6100_set_rectangle (0, 0, 131, 131, DPY_NOKIA6100_FILL, DPY_NOKIA6100_BLUE);
    DPY_nokia6100_put_string (s, 10, 10, DPY_NOKIA6100_LARGE, DPY_NOKIA6100_RED,
                              DPY_NOKIA6100_WHITE);
}


static void
display_value (char *s)
{
    DPY_nokia6100_set_rectangle (40, 10, 50, 120, DPY_NOKIA6100_FILL, DPY_NOKIA6100_BLUE);
    DPY_nokia6100_put_string (s, 40, 10, DPY_NOKIA6100_SMALL, DPY_NOKIA6100_WHITE,
                              DPY_NOKIA6100_BLUE);
}

static char *
print_value (unsigned long v, char *text)
{
    static char *s = "ERR";
    static char buf[30];
    int i = ACE_sprintf (buf, "%lu %s", v, text);
    if (i >= 0)
    {
        s = buf;
    }
    return s;
}

static void
card_inserted (void)
{
    char *wp = "inserted w protect";
    if (DEV_digin_isset (&SAM_card_write))
        wp = "inserted w enable";
    display_value (wp);
}

static void
play_mic (void)
{
	SAM_tone ();
}

static void
play_audio (void)
{
    SAM_beep ();
}

static void
get_ad4 (void)
{
    ACE_u32_t v = DEV_adc_get (&adc, DEV_ADC_CHN_4);

    char *s = print_value (v, "AD 4");

    display_value (s);

    USO_sleep (USO_MSEC_2_TICKS (50));
}

static void
get_trim (void)
{
    ACE_u32_t v = DEV_adc_get (&adc, DEV_ADC_CHN_6);

    v /= 4;

    char *s = print_value (v, "Contrast");

    display_value (s);
    DPY_nokia6100_set_contrast (v);

    USO_sleep (USO_MSEC_2_TICKS (50));
}

static void
get_temp (void)
{
    ACE_u32_t v = DEV_adc_get (&adc, DEV_ADC_CHN_5);

    v *= 21;
    v /= 10;
    v -= 40;

    int grad = v / 10;
    int dezi = v % 10;

    char *s = "ERR";
    char buf[30];
    int i = ACE_sprintf (buf, "%i.%i Grad C", grad, dezi);
    if (i >= 0)
    {
        s = buf;
    }
    display_value (s);

    USO_sleep (USO_MSEC_2_TICKS (150));
}

static ACE_bool_t
change_state (USO_node_t * e, enum lcd_state state)
{
    lcd_state = state;
    e->data = LCD_event_enter;
    return USO_slots_put_event (&SAM_event_slots, NULL, e, FALSE);
}

static ACE_bool_t
send_event (USO_node_t * e, enum SAM_event event)
{
    e->data = event;
    return USO_slots_put_event (&SAM_event_slots, NULL, e, FALSE);
}


static void
lcd_test_exec (char *nix)
{
    SAM_event_create (LCD_event_start);
    for (;;)
    {
        USO_node_t *e = USO_slot_get_event (&lcd_slot);
        enum SAM_event event = e->data;
        ACE_bool_t free = FALSE;

        switch (lcd_state)
        {

        case LCD_STATE_IDLE:
            switch (event)
            {
            case LCD_event_enter:
                DPY_nokia6100_backlight (&SAM_lcd_light, FALSE);
                DPY_nokia6100_clear_screen ();
                break;
            case LCD_event_start:
            case SAM_joystick_center_pressed:
                DPY_nokia6100_clear_screen ();
                DPY_nokia6100_backlight (&SAM_lcd_light, TRUE);
                free = change_state (e, LCD_STATE_LOGO);
                break;
            default:
                /*
                 * unused event 
                 */
                break;
            }
            break;

        case LCD_STATE_LOGO:
            switch (event)
            {
            case LCD_event_enter:
                display_logo ();
                free = send_event (e, LCD_event_logo);
                break;
            case LCD_event_logo:
                display_running ();
                free = send_event (e, LCD_event_logo);
                break;
            case SAM_switch_1_pressed:
                free = change_state (e, LCD_STATE_AD4);
                break;
            case SAM_joystick_center_pressed:
                free = change_state (e, LCD_STATE_IDLE);
                break;
            default:
                /*
                 * unused event 
                 */
                break;
            }
            break;

        case LCD_STATE_AD4:
        	switch (event)
        	{
        	case LCD_event_enter:
        		display_menu ("AD 4");
        		free = send_event (e, LCD_event_ad4);
        		break;
        	case LCD_event_ad4:
        		get_ad4 ();
        		free = send_event (e, LCD_event_ad4);
        		break;
        	case SAM_switch_2_pressed:
        		free = change_state (e, LCD_STATE_LOGO);
        		break;
        	case SAM_joystick_left_pressed:
        		free = change_state (e, LCD_STATE_CARD);
        		break;
        	case SAM_switch_1_pressed:
        	case SAM_joystick_right_pressed:
        		free = change_state (e, LCD_STATE_TRIM);
        		break;
        	default:
                    /*
                     * unused event
                     */
        		break;
        	}
        	break;

        case LCD_STATE_TRIM:
            switch (event)
            {
            case LCD_event_enter:
                display_menu ("TRIM");
                free = send_event (e, LCD_event_trim);
                break;
            case LCD_event_trim:
                get_trim ();
                free = send_event (e, LCD_event_trim);
                break;
            case SAM_switch_2_pressed:
                free = change_state (e, LCD_STATE_LOGO);
                break;
            case SAM_joystick_left_pressed:
                free = change_state (e, LCD_STATE_AD4);
                break;
            case SAM_switch_1_pressed:
            case SAM_joystick_right_pressed:
                free = change_state (e, LCD_STATE_TEMP);
                break;
            default:
                /*
                 * unused event 
                 */
                break;
            }
            break;

        case LCD_STATE_TEMP:
            switch (event)
            {
            case LCD_event_enter:
                display_menu ("TEMP");
                free = send_event (e, LCD_event_temp);
                break;
            case LCD_event_temp:
                get_temp ();
                free = send_event (e, LCD_event_temp);
                break;
            case SAM_switch_2_pressed:
                free = change_state (e, LCD_STATE_LOGO);
                break;
            case SAM_joystick_left_pressed:
                free = change_state (e, LCD_STATE_TRIM);
                break;
            case SAM_switch_1_pressed:
            case SAM_joystick_right_pressed:
                free = change_state (e, LCD_STATE_MIC);
                break;
            default:
                /*
                 * unused event 
                 */
                break;
            }
            break;

        case LCD_STATE_MIC:
            switch (event)
            {
            case LCD_event_enter:
                display_menu ("MIC");
                free = send_event (e, LCD_event_mic);
                break;
            case LCD_event_mic:
                play_mic ();
                free = send_event (e, LCD_event_mic);
                break;
            case SAM_switch_2_pressed:
                free = change_state (e, LCD_STATE_LOGO);
                break;
            case SAM_joystick_left_pressed:
                free = change_state (e, LCD_STATE_TEMP);
                break;
            case SAM_switch_1_pressed:
            case SAM_joystick_right_pressed:
                free = change_state (e, LCD_STATE_AUDIO);
                break;
            default:
                /*
                 * unused event 
                 */
                break;
            }
            break;

        case LCD_STATE_AUDIO:
            switch (event)
            {
            case LCD_event_enter:
                display_menu ("AUDIO");
                free = send_event (e, LCD_event_audio);
                break;
            case LCD_event_audio:
                play_audio ();
                free = send_event (e, LCD_event_audio);
                break;
            case SAM_switch_2_pressed:
                free = change_state (e, LCD_STATE_LOGO);
                break;
            case SAM_joystick_left_pressed:
                free = change_state (e, LCD_STATE_MIC);
                break;
            case SAM_switch_1_pressed:
            case SAM_joystick_right_pressed:
                free = change_state (e, LCD_STATE_CARD);
                break;
            default:
                /*
                 * unused event 
                 */
                break;
            }
            break;

        case LCD_STATE_CARD:
            switch (event)
            {
            case LCD_event_enter:
                display_menu ("CARD");
                if (DEV_digin_isset (&SAM_card_insert))
                    free = send_event (e, SAM_card_inserted);
                else
                    free = send_event (e, SAM_card_removed);
                break;
            case SAM_card_inserted:
                card_inserted ();
                break;
            case SAM_card_removed:
                display_value ("removed");
                break;
            case SAM_switch_2_pressed:
                free = change_state (e, LCD_STATE_LOGO);
                break;
            case SAM_joystick_left_pressed:
                free = change_state (e, LCD_STATE_AUDIO);
                break;
            case SAM_switch_1_pressed:
            case SAM_joystick_right_pressed:
                free = change_state (e, LCD_STATE_TRIM);
                break;
            default:
                /*
                 * unused event 
                 */
                break;
            }
            break;

        default:
            /*
             * illegal state 
             */
            break;
        }

        if (free == FALSE)
            USO_buf_free (&SAM_event_pool, e);
    }
}

extern void
lcd_test_install (MFS_descriptor_t * test)
{
    display_init ();
    USO_slot_init (&SAM_event_slots, &lcd_slot, lcd_event_ids, LCD_SLOT_SIZE);
    CLI_exec_init (test, &lcd_test, "lcd_T", "LCD Test", lcd_test_exec);
}
