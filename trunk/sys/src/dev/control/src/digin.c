/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdio.h>
#include <mfs/sysfs.h>
#include <mfs/vfs.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>

#include <dev/digin.h>

static void
info (MFS_descriptor_t * desc, int number, MFS_info_entry_t *entry)
{
    DEV_digin_t *in = (DEV_digin_t *) desc->represent;

    char *signal;
    enum DEV_digio_edge edge = DEV_digin_ischanged (in);
    if (edge == DEV_DIGIO_RAISE) signal = "_|";
    else if (edge == DEV_DIGIO_FALL) signal = "|_";
    else signal = "--";

    switch (number){
        case 0:
            entry->type = MFS_INFO_STRING;
            entry->name = "State";
            entry->value.s = in->state == DEV_DIGIO_HIGH ? "1" : "0";
            break;
        case 1:
            entry->type = MFS_INFO_STRING;
            entry->name = "Logic";
            entry->value.s = in->logig == DEV_DIGIO_POS ? "+" : "-";
            break;
        case 2:
            entry->type = MFS_INFO_STRING;
            entry->name = "Edge";
            entry->value.s = signal;
            break;
        case 3:
            entry->type = MFS_INFO_LONG;
            entry->name = "Deb_time";
            entry->value.l = in->debounce_time;
            break;
        case 4:
            entry->type = MFS_INFO_LONG;
            entry->name = "Deb_cnt";
            entry->value.l = in->debounce_count;
            break;
        default:
            entry->type = MFS_INFO_NOT_AVAIL;
            break;
    }
}

static struct MFS_descriptor_op digin_descriptor_op = {
    .open = NULL,
    .close = NULL,
    .info = info,
    .control = NULL,
    .delete = NULL
};


extern void
DEV_diginputs_init (DEV_diginputs_t * inputs)
{
    USO_list_init (&inputs->inputs);
}

extern void
DEV_digin_init (DEV_diginputs_t * inputs,
                DEV_digin_t * in,
                enum DEV_digio_logig logig,
                unsigned long (*sample) (void), unsigned int debounce_time)
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

extern void
DEV_digin_install (DEV_digin_t * in, char *name)
{
    MFS_descriptor_t * dir = MFS_resolve("/sys/dev/control/in");
    MFS_descriptor_create (dir, name, MFS_SYS, &digin_descriptor_op, (MFS_represent_t *) in);
    MFS_close_desc(dir);
}

static void
sample (USO_node_t * input)
{
    DEV_digin_t *in = (DEV_digin_t *) input;
    in->edge = DEV_DIGIO_NO;
    if (in->debounce_count == 0)
    {
        unsigned long s = in->sample ();
        if ((s && (in->state == DEV_DIGIO_LOW)) || (!s && (in->state == DEV_DIGIO_HIGH)))
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
DEV_diginputs_sample (DEV_diginputs_t * inputs)
{
    USO_map (&inputs->inputs, sample);
}

extern ACE_bool_t
DEV_digin_isset (DEV_digin_t * in)
{
    if (in->debounce_time == 0)
    {
        if (in->sample ())
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
    if (in->debounce_time == 0)
    {
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

