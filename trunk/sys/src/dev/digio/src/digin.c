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
sample (USO_node_t * in)
{
    if (((DEV_digin_t *) in)->debounce_count == 0)
    {
        unsigned long s = ((DEV_digin_t *) in)->sample ();
        if ((s && (((DEV_digin_t *) in)->state == DEV_DIGIO_LOW)) ||
            (!s && (((DEV_digin_t *) in)->state == DEV_DIGIO_HIGH)))
        {
            ((DEV_digin_t *) in)->debounce_count =
                ((DEV_digin_t *) in)->debounce_time;
        }
    }
    else
    {
        --((DEV_digin_t *) in)->debounce_count;
        if (((DEV_digin_t *) in)->debounce_count == 0)
        {
            unsigned long s = ((DEV_digin_t *) in)->sample ();
            if (s && (((DEV_digin_t *) in)->state == DEV_DIGIO_LOW))
            {
                ((DEV_digin_t *) in)->state = DEV_DIGIO_HIGH;
            }
            else if (!s && (((DEV_digin_t *) in)->state == DEV_DIGIO_HIGH))
            {
                ((DEV_digin_t *) in)->state = DEV_DIGIO_LOW;
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
