/*
 * linedetect.h
 *
 *  Created on: 04.08.2013
 *      Author: maik
 */

#ifndef SEN_LINEDETECT_H_
#define SEN_LINEDETECT_H_

#include <mfs/descriptor.h>
#include <dev/adc.h>
#include <dev/digin.h>


enum SEN_line_state
{
    SEN_LINE_LOST,      /**< We lost line. */
    SEN_LINE_LEFT,      /**< We have line on left side. */
    SEN_LINE_RIGHT,     /**< We have line on right side. */
    SEN_LINE_FOUND      /**< We have line. */
};

enum SEN_line_mode {
    SEN_LINE_ANALOG,
    SEN_LINE_DIGITAL
};


struct SEN_linedetect{
    DEV_adc_t *adc;
    int channel1;
    int channel2;
    DEV_digin_t *digin1;
    DEV_digin_t *digin2;
    int divisor;
    enum SEN_line_state state;
    enum SEN_line_mode mode;
    int diff;
    int av;
    int lastdiff;
    int lastav;
    int threshold;
    int max;
};

typedef struct SEN_linedetect SEN_linedetect_t;


extern void SEN_line_init(SEN_linedetect_t *line,
                          DEV_adc_t *adc, int ch1, int ch2,
                          DEV_digin_t *line1, DEV_digin_t *line2,
                          int div, int thres, int max,
                          enum SEN_line_mode mode);


extern enum SEN_line_state SEN_line_get_state(SEN_linedetect_t *line);
extern int SEN_line_get_diff(SEN_linedetect_t *line);
extern int SEN_line_get_av(SEN_linedetect_t *line);
extern int SEN_line_get_ddiff(SEN_linedetect_t *line);
extern int SEN_line_get_dav(SEN_linedetect_t *line);

extern void SEN_line_install (SEN_linedetect_t *line, MFS_descriptor_t * dir, char *name);

#endif /* LINEDETECT_H_ */
