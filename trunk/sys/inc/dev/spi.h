#ifndef DEV_SPI_H
#define DEV_SPI_H

#include <ace/stddef.h>
#include <uso/mutex.h>

typedef struct DEV_spi_dev DEV_spi_dev_t;

typedef int (*DEV_spi_exchange_func_t)(DEV_spi_dev_t *dev, ACE_u32_t out, ACE_u32_t* in);
typedef int (*DEV_spi_dev_config_func_t)(DEV_spi_dev_t *dev);

typedef struct DEV_spi DEV_spi_t;

enum DEV_spi_bus_ctrl{
	DEV_SPI_BUS_CTRL_SLAVE,
	DEV_SPI_BUS_CTRL_MASTER
};

enum DEV_spi_dev_cs{
	DEV_SPI_DEV_CS_PASSIVE,
	DEV_SPI_DEV_CS_ACTIVE
};

struct DEV_spi_bus{
	int number;                           /**< SPI bus number, 0,1,.. */
	enum DEV_spi_bus_ctrl ctrl;           /**< MASTER, SLAVE.         */
	int delay_b_cs;                       /**< Delay between two chip selects. */
	DEV_spi_exchange_func_t exchange;     /**< Exchange  function, registered by an spi implementation. */
	DEV_spi_dev_config_func_t dev_cfg;    /**< Device configuration function registert by a spi implementation. */
	USO_mutex_t lock;                     /**< Synchronization lock. */
};

typedef struct DEV_spi_bus DEV_spi_bus_t;

struct DEV_spi_dev{
	DEV_spi_bus_t *bus;                  /**< SPI bus used by the device. */
	int number;                          /**< Device number, 0,1,2,3 .    */
	int sclk;                            /**< Serial clock.               */
	int mode;                            /**< Mode 0(CPOL 0,CPHA 0),1(CPOL 0,CPHA 1),2(CPOL 1,CPHA 0),3(CPOL 1,CPHA 1). */
	int word_size;                       /**< Word can be from 8 bits to 16 bits. */
	int delay_b_sclk;                    /**< Delay before serial clock. */
	int delay_b_ct;                      /**< Delay between consecutive transfers. */
	enum DEV_spi_dev_cs cs_active;       /**< CS remains active after end of transfer. */
};

extern int DEV_spi_bus_init (DEV_spi_bus_t *bus,
							  int bus_nr,
							  enum DEV_spi_bus_ctrl ctrl,
							  int delay_b_cs);

extern int DEV_spi_dev_init (DEV_spi_dev_t* dev,
		                      DEV_spi_bus_t *bus,
		                      int dev_nr);

extern int DEV_spi_dev_config (DEV_spi_dev_t* dev,
		                        int sclk,
		                        int mode,
		                        int word_size,
		                        int delay_b_sclk,
		                        int delay_b_ct,
		                        enum DEV_spi_dev_cs cs_active);

extern void DEV_spi_acquire (DEV_spi_dev_t *dev);

extern void DEV_spi_release (DEV_spi_dev_t *dev);

extern int DEV_spi_exchange (DEV_spi_dev_t *dev, ACE_u32_t out, ACE_u32_t* in);

#endif /*DEV_SPI_H*/
