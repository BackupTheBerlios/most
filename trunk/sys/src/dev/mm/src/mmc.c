#include <ace/string.h>
#include <mfs/sysfs.h>
#include <mfs/vfs.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>
#include <dev/mmc.h>

/* Response token */
#define MMC_RESPONSE  0xff

/* an affirmative R1 response
 */
#define MMC_R1_RESPONSE_OK         0x00 /* no errors */
#define MMC_R1_RESPONSE_IDLE       0x01 /* in idle state and doing initialization */

/* Response comes 1-8bytes after command
 * the first bit will be a 0 followed by an error code
 * data will be 0xff until response
 */
#define MMC_MAX_BYTES_TO_RESPONSE   64  /* ??? 8 is not enough ? */
#define MMC_MAX_BYTES_TO_XX_RESPONSE   500      /* ??? 8 is not enough ? */

/* Data Tokens (necessary because at nop/idle (and CS active) only 0xff is on the data/command line)
 */
#define MMC_DATA_START_SINGLE_BLOCK_READ        0xfe    /* Data token start byte, Start Single Block Read */
#define MMC_DATA_START_MULTIPLE_BLOCK_READ      0xfe    /* Data token start byte, Start Multiple Block Read */
#define MMC_DATA_START_SINGLE_BLOCK_WRITE       0xfe    /* Data token start byte, Start Single Block Write */
#define MMC_DATA_START_MULTIPLE_BLOCK_WRITE     0xfc    /* Data token start byte, Start Multiple Block Write */
#define MMC_DATA_STOP_MULTIPLE_BLOCK_WRITE      0xfd    /* Data token stop byte, Stop Multiple Block Write */

#define MMC_DATA_ACEPPTED      0x05
#define MMC_DATA_CRC_ERROR     0x0b
#define MMC_DATA_WRITE_ERROR   0x0d

/* command tokens: first bit 0 (start bit), second 1 (transmission bit)-> CMD-number + 0ffset 0x40
 */
#define MMC_START_BIT          0x80
#define MMC_TRANSMISSION_BIT   0x40

#define MMC_CMD_GO_IDLE_STATE           0x00    /* CMD0 */
#define MMC_CMD_SEND_OP_COND            0x01    /* CMD1 */
#define MMC_CMD_SEND_CSD                0x09    /* CMD9  Read Card Specific Data Register */
#define MMC_CMD_SEND_CID                0x0A    /* CMD10 Read Card IDentification Register */
#define MMC_CMD_STOP_TRANSMISSION       0x0C    /* CMD12 */
#define MMC_CMD_SEND_STATUS             0x0D    /* CMD13 */
#define MMC_CMD_SET_BLOCKLEN            0x10    /* CMD16 Set block length for next read/write */
#define MMC_CMD_READ_SINGLE_BLOCK       0x11    /* CMD17 Read block from memory */
#define MMC_CMD_READ_MULTIPLE_BLOCK     0x12    /* CMD18 */
#define MMC_CMD_WRITE_SINGLE_BLOCK      0x18    /* CMD24 */
#define MMC_CMD_WRITE_MULTIPLE_BLOCK    0x19    /* CMD25 */
#define MMC_CMD_WRITE_CSD               0x1b    /* CMD27 Program CSD */
#define MMC_CMD_SET_WRITE_PROT          0x1c    /* CMD28 */
#define MMC_CMD_CLR_WRITE_PROT          0x1d    /* CMD29 */
#define MMC_CMD_SEND_WRITE_PROT         0x1e    /* CMD30 */
#define MMC_CMD_TAG_SECTOR_START        0x20    /* CMD32 */
#define MMC_CMD_TAG_SECTOR_END          0x21    /* CMD33 */
#define MMC_CMD_UNTAG_SECTOR            0x22    /* CMD34 */
#define MMC_CMD_TAG_EREASE_GROUP_START  0x23    /* CMD35 */
#define MMC_CMD_TAG_EREASE_GROUP_END    0x24    /* CMD36 */
#define MMC_CMD_UNTAG_EREASE_GROUP      0x25    /* CMD37 */
#define MMC_CMD_EREASE                  0x26    /* CMD38 */
#define MMC_CMD_READ_OCR                0x27    /* CMD39 */
#define MMC_CMD_CRC_ON_OFF              0x28    /* CMD40 */

#define MMC_BUSY           0

#define MMC_CRC_INITIAL    0x95
#define MMC_CRC_NOT_USED   0xff

#define MMC_CID_LENGTH                  16      /**< CID Register Length */
#define MMC_CSD_LENGTH                  16      /**< CSD Register Length */



static DEV_spi_dev_t *spi;
static struct DEV_mmc_CID cid;
static struct DEV_mmc_CSD csd;

static void
info (MFS_descriptor_t * desc, int number, MFS_info_entry_t *entry)
{
    switch (number){
        case 0:
            entry->type = MFS_INFO_SIZE;
            entry->name = "mid";
            entry->value.z = cid.mid;
            break;
        case 1:
            entry->type = MFS_INFO_STRING;
            entry->name = "name";
            entry->value.s = cid.name;
            break;
        case 2:
            entry->type = MFS_INFO_SIZE;
            entry->name = "serial_nr";
            entry->value.z = cid.serial_nr;
            break;
        case 3:
            entry->type = MFS_INFO_LONG;
            entry->name = "R block len";
            entry->value.l = csd.read_block_len;
            break;
        case 4:
            entry->type = MFS_INFO_LONG;
            entry->name = "R block part";
            entry->value.l = csd.read_block_partial;
            break;
        case 5:
            entry->type = MFS_INFO_LONG;
            entry->name = "W block len";
            entry->value.l = csd.write_block_len;
            break;
        case 6:
            entry->type = MFS_INFO_LONG;
            entry->name = "W block part";
            entry->value.l = csd.write_block_partial;
            break;
        case 7:
            entry->type = MFS_INFO_LONG;
            entry->name = "Nr. of blocks";
            entry->value.l = csd.block_nr;
            break;
        case 8:
            entry->type = MFS_INFO_SIZE;
            entry->name = "Card capacity";
            entry->value.z = (csd.block_nr * csd.read_block_len) / (((unsigned long)1024) * 1024);
            break;
        default:
            entry->type = MFS_INFO_NOT_AVAIL;
            break;
    }
}

static struct MFS_descriptor_op mmc_descriptor_op = {
    .open = NULL,
    .close = NULL,
    .info = info,
    .control = NULL,
    .delete = NULL
};


static unsigned char
spi_send_byte (const unsigned char data)
{
    ACE_u32_t in;
    DEV_spi_exchange (spi, data, &in);
    return in;
}

static unsigned char
mmc_get_response (void)
{
    unsigned char response;
    for (int i = 0; i <= MMC_MAX_BYTES_TO_RESPONSE; ++i)
    {
        response = spi_send_byte (MMC_RESPONSE);
        if (response == MMC_R1_RESPONSE_OK)
            break;
        if (response == MMC_R1_RESPONSE_IDLE)
            break;
    }
    return response;
}

static unsigned char
mmc_get_XX_response (const char resp)
{
    unsigned char response;

    for (int i = 0; i <= MMC_MAX_BYTES_TO_XX_RESPONSE; ++i)
    {
        response = spi_send_byte (MMC_RESPONSE);
        if (response == resp)
            break;
    }
    return response;
}

static ACE_err_t
mmc_check_busy (void)
{
    unsigned char response;
    ACE_err_t err;

    /* read the data response xxx0<status>1 : status 010: Data accepted, status 101: Data
     * rejected due to a crc error, status 110: Data rejected due to a Write error.
     */
    for (int i = 0; i <= MMC_MAX_BYTES_TO_RESPONSE; ++i)
    {
        response = spi_send_byte (MMC_RESPONSE);
        response &= 0x1f;
        switch (response)
        {
        case MMC_DATA_ACEPPTED:
            err = ACE_OK;
            break;
        case MMC_DATA_CRC_ERROR:
            return (DEV_ERR_MMC_CRC);
        case MMC_DATA_WRITE_ERROR:
            return (DEV_ERR_MMC_WRITE);
        default:
            err = DEV_ERR_MMC_OTHER;
            break;
        }
        if (err == ACE_OK)
            break;
    }

    do
    {
        response = spi_send_byte (MMC_RESPONSE);
    }
    while (response == MMC_BUSY);

    return err;
}

static void
mmc_send_cmd (const char cmd, unsigned long data, const char crc)
{
    char frame[6];
    char temp;

    frame[0] = (cmd | MMC_TRANSMISSION_BIT);
    for (int i = 3; i >= 0; i--)
    {
        temp = (char)(data >> (8 * i));
        frame[4 - i] = (temp);
    }
    frame[5] = (crc);
    for (int i = 0; i < 6; i++)
        spi_send_byte (frame[i]);
}

static ACE_err_t
mmc_set_block_length (const unsigned long blocklength)
{
    ACE_err_t err = DEV_ERR_MMC_RESPONSE;
    mmc_send_cmd (MMC_CMD_SET_BLOCKLEN, blocklength, MMC_CRC_NOT_USED);

    if (mmc_get_response () == MMC_R1_RESPONSE_OK)
        err = ACE_OK;

    /* Send 8 Clock pulses of delay. */
    spi_send_byte (MMC_RESPONSE);

    return err;
}

/* Reading the contents of the CSD and CID registers in SPI mode is a simple
 * read-block transaction.
 */
static ACE_err_t
mmc_read_register (const char cmd, const unsigned int length, unsigned char buffer[])
{
    ACE_err_t err = DEV_ERR;

    DEV_spi_acquire (spi);

    if (mmc_set_block_length (length) == ACE_OK)
    {
        mmc_send_cmd (cmd, 0, MMC_CRC_NOT_USED);

        if (mmc_get_response () == MMC_R1_RESPONSE_OK)
        {
            if (mmc_get_XX_response (MMC_DATA_START_SINGLE_BLOCK_READ) ==
                MMC_DATA_START_SINGLE_BLOCK_READ)
            {
                for (int i = 0; i < length; i++)
                    buffer[i] = spi_send_byte (MMC_RESPONSE);
                /* get CRC bytes (not really needed by us, but required by MMC) */
                spi_send_byte (MMC_RESPONSE);
                spi_send_byte (MMC_RESPONSE);
                err = ACE_OK;
            }
            else
            {
                err = DEV_ERR_MMC_DATA_TOKEN;
            }
        }
        else
        {
            err = DEV_ERR_MMC_RESPONSE;
        }
        /* Send 8 Clock pulses of delay. */
        spi_send_byte (MMC_RESPONSE);
    }
    else
    {
        err = DEV_ERR_MMC_BLOCK_SET;
    }

    DEV_spi_release (spi);

    return err;
}


extern ACE_err_t
DEV_mmc_spi_init (DEV_spi_dev_t * dev)
{
    spi = dev;
    ACE_err_t err = DEV_ERR;

    if (0 == DEV_spi_dev_config (spi, 0x1F,     /* clock */
                                 0,     /* mode */
                                 8,     /* word size */
                                 0,     /* delay_b_sclk */
                                 0,     /* delay_b_ct */
                                 DEV_SPI_DEV_CS_ACTIVE))
    {
        err = ACE_OK;
    }
    return err;
}

extern ACE_err_t
DEV_mmc_init (void)
{
    /* raise SS and MOSI for 80 clock cycles
     * send_byte(0xff) 10 times with SS high
     * raise SS
     */
    unsigned char response = MMC_R1_RESPONSE_IDLE;
    ACE_err_t err = DEV_ERR;

    DEV_spi_acquire (spi);

    /* initialization sequence on Power Up */
    for (int i = 0; i <= 9; i++)
        spi_send_byte (MMC_RESPONSE);

    mmc_send_cmd (MMC_CMD_GO_IDLE_STATE, 0, MMC_CRC_INITIAL);

    /* Now wait for READY RESPONSE */
    if (mmc_get_response () != MMC_R1_RESPONSE_IDLE)
    {
        err = DEV_ERR_MMC_INIT;
    }
    else
    {
        while (response == MMC_R1_RESPONSE_IDLE)
        {
            spi_send_byte (MMC_RESPONSE);
            mmc_send_cmd (MMC_CMD_SEND_OP_COND, 0, MMC_CRC_NOT_USED);
            response = mmc_get_response ();
        }
        spi_send_byte (MMC_RESPONSE);
        err = ACE_OK;
    }

    DEV_spi_release (spi);

    if (err == ACE_OK){
        err = DEV_mmc_read_CID (&cid);
    }

    if (err == ACE_OK){
        err = DEV_mmc_read_CSD (&csd);
    }

    return err;
}

extern void
DEV_mmc_install (void)
{
    MFS_descriptor_t *dir = MFS_resolve("/sys/dev/mm"); 
    MFS_descriptor_create (dir, "mmc", MFS_SYS, &mmc_descriptor_op, NULL);
    MFS_close_desc(dir);
}

/* The card will respond with a standard response token followed by a data
 * block suffixed with a 16 bit CRC.
 */
extern ACE_err_t
DEV_mmc_read_block (const unsigned long address, const unsigned long count, char buffer[])
{
    ACE_err_t err = DEV_ERR;

    DEV_spi_acquire (spi);

    /* Set the block length to read */
    if (mmc_set_block_length (count) == ACE_OK)
    {
        mmc_send_cmd (MMC_CMD_READ_SINGLE_BLOCK, address, MMC_CRC_NOT_USED);

        /* Send 8 Clock pulses of delay, check if the MMC acknowledged the read block command
         * it will do this by sending an affirmative response
         */
        if (mmc_get_response () == MMC_R1_RESPONSE_OK)
        {
            /* now look for the data token to signify the start of the data */
            if (mmc_get_XX_response (MMC_DATA_START_SINGLE_BLOCK_READ) ==
                MMC_DATA_START_SINGLE_BLOCK_READ)
            {
                /* clock the actual data transfer and receive the bytes; spi_read automatically finds the Data Block */
                for (int i = 0; i < count; i++)
                    buffer[i] = spi_send_byte (MMC_RESPONSE);

                /* get CRC bytes (not really needed by us, but required by MMC) */
                spi_send_byte (MMC_RESPONSE);
                spi_send_byte (MMC_RESPONSE);
                err = ACE_OK;
            }
            else
            {
                /* the data token was never received */
                err = DEV_ERR_MMC_DATA_TOKEN;
            }
        }
        else
        {
            /* the MMC never acknowledge the read command */
            err = DEV_ERR_MMC_RESPONSE;
        }
    }
    else
    {
        err = DEV_ERR_MMC_BLOCK_SET;
    }
    spi_send_byte (MMC_RESPONSE);

    DEV_spi_release (spi);

    return err;
}

extern ACE_err_t
DEV_mmc_write_block (const unsigned long address, char buffer[])
{
    ACE_err_t err = DEV_ERR;

    DEV_spi_acquire (spi);

    /* Set the block length to read */
    if (mmc_set_block_length (DEV_MMC_BLOCK_SIZE) == ACE_OK)
    {
        mmc_send_cmd (MMC_CMD_WRITE_SINGLE_BLOCK, address, MMC_CRC_NOT_USED);

        /* check if the MMC acknowledged the write block command
         * it will do this by sending an affirmative response
         */
        if (mmc_get_XX_response (MMC_R1_RESPONSE_OK) == MMC_R1_RESPONSE_OK)
        {
            spi_send_byte (MMC_RESPONSE);
            /* send the data token to signify the start of the data */
            spi_send_byte (MMC_DATA_START_SINGLE_BLOCK_WRITE);
            /* clock the actual data transfer and transmit the bytes */
            for (int i = 0; i < DEV_MMC_BLOCK_SIZE; i++)
                spi_send_byte (buffer[i]);
            /* put CRC bytes (not really needed by us, but required by MMC) */
            spi_send_byte (MMC_RESPONSE);
            spi_send_byte (MMC_RESPONSE);

            err = mmc_check_busy ();
        }
        else
        {
            /* the MMC never acknowledge the write command */
            err = DEV_ERR_MMC_RESPONSE;
        }
    }
    else
    {
        err = DEV_ERR_MMC_BLOCK_SET;
    }

    /* Send 8 Clock pulses of delay. */
    spi_send_byte (MMC_RESPONSE);

    DEV_spi_release (spi);

    return err;
}

extern ACE_err_t
DEV_mmc_read_CID (struct DEV_mmc_CID *cid)
{
    ACE_err_t err;
    unsigned char buffer[MMC_CID_LENGTH];
    err = mmc_read_register (MMC_CMD_SEND_CID, sizeof (buffer), buffer);
    if (err == ACE_OK)
    {
        cid->mid = buffer[0];
        memcpy (cid->name, buffer + 3, 6);
        cid->name[6] = '\0';
        memcpy (&cid->serial_nr, buffer + 10, 4);       /* @revise : byte order little endian */
    }
    return err;
}

extern ACE_err_t
DEV_mmc_read_CSD (struct DEV_mmc_CSD *csd)
{
    ACE_err_t err;
    unsigned char buffer[MMC_CSD_LENGTH];
    err = mmc_read_register (MMC_CMD_SEND_CSD, sizeof (buffer), buffer);
    if (err == ACE_OK)
    {
        csd->read_block_len = 1 << (buffer[5] & 0x0f);
        csd->read_block_partial = (buffer[6] & 0x80) ? TRUE : FALSE;
        csd->write_block_len = 1 << (((buffer[12] & 0x03) << 2) | ((buffer[13] & 0xc0) >> 6));
        csd->write_block_partial = (buffer[13] & 0x20) ? TRUE : FALSE;
        unsigned int c_size = ((buffer[6] & 0x03) << 10) |
            ((buffer[7]) << 2) | ((buffer[8] & 0xc0) >> 6);
        unsigned int c_size_mul = 4 << (((buffer[9] & 0x03) << 1) | ((buffer[10] & 0x80) >> 7));
        csd->block_nr = (c_size + 1) * c_size_mul;
    }
    return err;
}

