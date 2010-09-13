/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "dev/digin.h"


extern void
DEV_diginputs_init (DEV_diginputs_t* inputs)
{
	USO_list_init(&inputs->inputs);
}

extern void
DEV_digin_init (DEV_diginputs_t* inputs,
				DEV_digin_t * in,
                enum DEV_digio_logig logig,
                unsigned long (*sample) (void),
                unsigned int debounce_time)
{
    in->logig = logig;
    in->sample = sample;
    in->edge = DEV_DIGIO_NO;
    in->debounce_time = debounce_time;
    in->debounce_count = 0;
    if (sample ())
        in->state = DEV_DIGIO_HIGH;
 	else
        in->state = DEV_DIGIO_LOW;
    if (inputs != NULL && debounce_time > 0)
    	USO_enqueue (&inputs->inputs, (USO_node_t *) in);
}

static void
sample (USO_node_t *input)
{
	DEV_digin_t *in = (DEV_digin_t*)input;
    in->edge = DEV_DIGIO_NO;
    if (in->debounce_count == 0)
    {
        unsigned long s = in->sample ();
        if ((s && (in->state == DEV_DIGIO_LOW)) ||
            (!s && (in->state == DEV_DIGIO_HIGH)))
        {
            in->debounce_count = in->debounce_time;
        }
    }
    else
    {
        --in->debounce_count;
        if (in->debounce_count == 0)
        {
            unsigned long s = in->sample ();
            if (s && (in->state == DEV_DIGIO_LOW))
            {
                in->state = DEV_DIGIO_HIGH;
                in->edge = DEV_DIGIO_RAISE;
            }
            else if (!s && (in->state == DEV_DIGIO_HIGH))
            {
                in->state = DEV_DIGIO_LOW;
                in->edge = DEV_DIGIO_FALL;
            }
        }
    }
}

extern void
DEV_diginputs_sample (DEV_diginputs_t* inputs)
{
    USO_map (&inputs->inputs, sample);
}

extern ACE_bool_t
DEV_digin_isset (DEV_digin_t * in)
{
    if (in->debounce_time == 0){
		if(in->sample())
			in->state = DEV_DIGIO_HIGH;
		else	
			in->state = DEV_DIGIO_LOW;
    }
    if (in->logig == DEV_DIGIO_POS)
    {
        if (in->state == DEV_DIGIO_HIGH)
            return TRUE;
        else
            return FALSE;
    }
    else
    {
        if (in->state == DEV_DIGIO_HIGH)
            return FALSE;
        else
            return TRUE;
    }
}

extern enum DEV_digio_edge
DEV_digin_ischanged (DEV_digin_t * in)
{
    if (in->debounce_time == 0){
        in->edge = DEV_DIGIO_NO;
        unsigned long s = in->sample ();
        if (s && (in->state == DEV_DIGIO_LOW))
        {
            in->state = DEV_DIGIO_HIGH;
            in->edge = DEV_DIGIO_RAISE;
        }
        else if (!s && (in->state == DEV_DIGIO_HIGH))
        {
            in->state = DEV_DIGIO_LOW;
            in->edge = DEV_DIGIO_FALL;
        }
    }
    if (in->edge == DEV_DIGIO_NO)
    	return DEV_DIGIO_NO;
    if (in->logig == DEV_DIGIO_POS)
    {
        if (in->edge == DEV_DIGIO_RAISE)
            return DEV_DIGIO_RAISE;
        else
            return DEV_DIGIO_FALL;
    }
    else
    {
        if (in->edge == DEV_DIGIO_RAISE)
            return DEV_DIGIO_FALL;
        else
            return DEV_DIGIO_RAISE;
    }
}
