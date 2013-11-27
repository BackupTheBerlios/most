/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/err.h>
#include <ace/string.h>
#include <ace/stdlib.h>
#include <cli/config.h>
#include <mfs/block.h>
#include <dev/mmc.h>

#include <init/config.h>


static ACE_err_t
config_read (void)
{
    ACE_err_t err = ACE_OK;
    ACE_ssize_t len;
    char *data;
    MFS_descriptor_t *desc = MFS_resolve("/bsp/mmc/cfg");
    if (desc == NULL || desc->type != MFS_BLOCK)
        return DEF_ERR_NOT_FOUND;
    MFS_block_t *p1 = (MFS_block_t *)desc;
    unsigned long block = p1->start;
    len = MFS_get (p1, &data, block);
    if (data != NULL)
    {
        if (len >= sizeof (CLI_config)){
            memcpy (&CLI_config, data, sizeof (CLI_config));
        } else {
            err = DEF_ERR_RANGE;
        }
        MFS_confirm(p1,block);
    }
    else
    {
        err = DEF_ERR_MEM;
    }
    MFS_close_desc (desc);
    return err;
}

static ACE_err_t
config_write (void)
{
    ACE_err_t err = ACE_OK;
    MFS_descriptor_t *desc = MFS_resolve("/bsp/mmc/cfg");
    if (desc == NULL || desc->type != MFS_BLOCK)
        return DEF_ERR_NOT_FOUND;
    MFS_block_t *p1 = (MFS_block_t *)desc;
    char *buffer = ACE_malloc (p1->size);
    if (buffer)
    {
        memset (buffer, '\0', p1->size);
        if (sizeof (CLI_config) <= p1->size)
        {
            memcpy (buffer, &CLI_config, sizeof (CLI_config));
            MFS_put  (p1, buffer, p1->size, p1->start);
        } else {
            err = DEF_ERR_RANGE;
        }
        ACE_free (buffer);
    }
    else
    {
        err = DEF_ERR_MEM;
    }
    MFS_close_desc (desc);
    return err;
}


static ACE_err_t
config_ip (void)
{
    return ACE_OK;
}


extern void
LA2_config_install (void)
{
    CLI_config_install (config_read, config_write, config_ip);
}
