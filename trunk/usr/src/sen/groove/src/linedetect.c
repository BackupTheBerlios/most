/*
 * linedetect.c
 *
 *  Created on: 04.08.2013
 *      Author: maik
 */

#include <ace/stdio.h>
#include <sen/linedetect.h>

extern void SEN_line_init(SEN_linedetect_t *line,
                          DEV_adc_t *adc, int ch1, int ch2,
                          DEV_digin_t *line1, DEV_digin_t *line2,
                          int div, int thres, int max,
                          enum SEN_line_mode mode)
{
    line->adc = adc;
    line->channel1 = ch1;
    line->channel2 = ch2;
    line->digin1 = line1;
    line->digin2 = line2;
    line->divisor = div;
    line->threshold = thres;
    line->max = max;
    line->state = SEN_LINE_LOST;
    line->mode = mode;
    line->diff = 0;
    line->av = 0;
    line->lastdiff = 0;
    line->lastav = 0;
    line->desc = 0;
}

static void
set_state(SEN_linedetect_t *line, int sens1, int sens2, int t)
{
    line->lastdiff = line->diff;
    line->lastav = line->av;
    line->diff = sens1 - sens2;
    line->av = (sens1 + sens2) / 2;
    if (sens1 < t && sens2 < t){
        line->state = SEN_LINE_LOST;
    } else if (sens1 >= t && sens2 < t){
        line->state = SEN_LINE_LEFT;
    } else if (sens1 < t && sens2 >= t){
        line->state = SEN_LINE_RIGHT;
    } else {
        line->state = SEN_LINE_FOUND;
    }
}

extern enum SEN_line_state SEN_line_get_state(SEN_linedetect_t *line)
{
    int sens1;
    int sens2;
    if (line->mode == SEN_LINE_ANALOG){
        sens1 = DEV_adc_get(line->adc, line->channel1) / line->divisor;
        sens2 = DEV_adc_get(line->adc, line->channel2) / line->divisor;
    } else {
        sens1 = DEV_digin_isset(line->digin1) == FALSE ? line->max : 0;
        sens2 = DEV_digin_isset(line->digin2) == FALSE ? line->max : 0;
    }
    set_state(line, sens1, sens2, line->threshold);
    return line->state;
}

extern int SEN_line_get_diff(SEN_linedetect_t *line)
{
    return line->diff;
}

extern int SEN_line_get_av(SEN_linedetect_t *line)
{
    return line->av;
}

extern int SEN_line_get_ddiff(SEN_linedetect_t *line)
{
    return line->diff - line->lastdiff;
}

extern int SEN_line_get_dav(SEN_linedetect_t *line)
{
    return line->av - line->lastav;
}

static void
info (MFS_descriptor_t * desc, int number, MFS_info_entry_t *entry)
{
    SEN_linedetect_t *line = (SEN_linedetect_t *) desc->represent;
    switch (number){
        case 0:{
            char *state;
            switch(line->state){
                case SEN_LINE_LOST:
                    state = "lost";
                    break;
                case SEN_LINE_LEFT:
                    state = "left";
                    break;
                case SEN_LINE_RIGHT:
                    state = "right";
                    break;
                case SEN_LINE_FOUND:
                    state = "found";
                    break;
                default:
                    state = "?";
                    break;
            }
            entry->type = MFS_INFO_STRING;
            entry->name = "state";
            entry->value.s = state;
            break;}
        case 1:
            entry->type = MFS_INFO_LONG;
            entry->name = "diff";
            entry->value.l = line->diff;
            break;
        case 2:
            entry->type = MFS_INFO_LONG;
            entry->name = "av";
            entry->value.l = line->av;
            break;
        default:
            entry->type = MFS_INFO_NOT_AVAIL;
            break;
    }
}

static struct MFS_descriptor_op line_descriptor_op = {
    .open = NULL,
    .close = NULL,
    .info = info,
    .control = NULL,
    .delete = NULL
};

extern void
SEN_line_install (SEN_linedetect_t *line, MFS_descriptor_t * dir, char *name)
{
    line->desc = MFS_descriptor_create (dir, name, MFS_SYS, &line_descriptor_op, (MFS_represent_t *) line);
}

