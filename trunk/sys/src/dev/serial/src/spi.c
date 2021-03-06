#include "dev/spi.h"

extern int
DEV_spi_bus_init (DEV_spi_bus_t * bus,
				  int bus_nr,
				  enum DEV_spi_bus_ctrl ctrl,
				  int delay_btn_cs)
{
    bus->number = bus_nr;
    bus->ctrl = ctrl;
    bus->delay_btn_cs = delay_btn_cs;
    USO_mutex_init (&bus->lock);
    return 0;
}

extern int
DEV_spi_dev_init (DEV_spi_dev_t * dev, DEV_spi_bus_t * bus, int dev_nr)
{
    dev->bus = bus;
    if (dev_nr < 0 || dev_nr > 3)
        return -1;
    dev->number = dev_nr;
    return 0;
}

extern int
DEV_spi_dev_config (DEV_spi_dev_t * dev,
                    int sclk,
                    int mode,
                    int word_size,
                    int delay_bfe_sclk,
                    int delay_btn_ct,
                    enum DEV_spi_dev_cs cs)
{
    if (dev->bus != NULL && dev->bus->dev_cfg != NULL)
    {
        int ret, i;
        ACE_u32_t mask = 0;
        USO_lock (&dev->bus->lock);
        dev->sclk = sclk;
        dev->mode = mode;
        dev->word_size = word_size;
        dev->delay_bfe_sclk = delay_bfe_sclk;
        dev->delay_btn_ct = delay_btn_ct;
        dev->cs = cs;
        ret = dev->bus->dev_cfg (dev);
        USO_unlock (&dev->bus->lock);
        for (i = dev->word_size; i; i--)
        {
            mask <<= 1;
            mask |= 1;
        }
        dev->word_mask = mask;
        return ret;
    }
    return -1;
}

extern void
DEV_spi_acquire (DEV_spi_dev_t * dev)
{
    USO_lock (&dev->bus->lock);
}

extern void
DEV_spi_release (DEV_spi_dev_t * dev)
{
    USO_unlock (&dev->bus->lock);
}

extern int
DEV_spi_exchange (DEV_spi_dev_t * dev, ACE_u32_t out, ACE_u32_t * in)
{
    return dev->bus->exchange (dev, out, in);
}
