#include "dev/mmc.h"

// Tokens (necessary because at nop/idle (and CS active) only 0xff is on the data/command line)
#define MMC_START_DATA_BLOCK_TOKEN 					0xfe	// Data token start byte, Start Single Block Read
#define MMC_START_DATA_MULTIPLE_BLOCK_READ		 	0xfe	// Data token start byte, Start Multiple Block Read
#define MMC_START_DATA_BLOCK_WRITE					0xfe	// Data token start byte, Start Single Block Write
#define MMC_START_DATA_MULTIPLE_BLOCK_WRITE			0xfc	// Data token start byte, Start Multiple Block Write
#define MMC_STOP_DATA_MULTIPLE_BLOCK_WRITE			0xfd	// Data token stop byte, Stop Multiple Block Write

// an affirmative R1 response (no errors)
#define MMC_R1_RESPONSE       0x00

// commands: first bit 0 (start bit), second 1 (transmission bit); CMD-number + 0ffsett 0x40
#define MMC_GO_IDLE_STATE 			0x40    //CMD0
#define MMC_SEND_OP_COND 			0x41    //CMD1
#define MMC_READ_CSD 				0x49	//CMD9
#define MMC_SEND_CID 				0x4a    //CMD10
#define MMC_STOP_TRANSMISSION 		0x4c    //CMD12
#define MMC_SEND_STATUS 			0x4d    //CMD13
#define MMC_SET_BLOCKLEN 			0x50	//CMD16 Set block length for next read/write
#define MMC_READ_SINGLE_BLOCK 		0x51	//CMD17 Read block from memory
#define MMC_READ_MULTIPLE_BLOCK 	0x52    //CMD18
#define MMC_CMD_WRITEBLOCK 			0x54	//CMD20 Write block to memory
#define MMC_WRITE BLOCK 			0x58    //CMD25
#define MMC_WRITE_MULTIPLE_BLOCK 	0x59   	//CMD??
#define MMC_WRITE_CSD 				0x5b	//CMD27 PROGRAM_CSD
#define MMC_SET_WRITE_PROT 			0x5c    //CMD28
#define MMC_CLR_WRITE_PROT 			0x5d    //CMD29
#define MMC_SEND_WRITE_PROT 		0x5e    //CMD30
#define MMC_TAG_SECTOR_START 		0x60    //CMD32
#define MMC_TAG_SECTOR_END 			0x61    //CMD33
#define MMC_UNTAG_SECTOR 			0x62    //CMD34
#define MMC_TAG_EREASE_GROUP_START 	0x63 	//CMD35
#define MMC_TAG_EREASE_GROUP_END 	0x64  	//CMD36
#define MMC_UNTAG_EREASE_GROUP 		0x65    //CMD37
#define MMC_EREASE 					0x66    //CMD38
#define MMC_READ_OCR 				0x67    //CMD39
#define MMC_CRC_ON_OFF 				0x68    //CMD40




static DEV_spi_dev_t *spi;
char mmc_buffer[512] = { 0 };	// Buffer for mmc i/o for data and registers



static unsigned char spiSendByte(const unsigned char data)
{
	ACE_u32_t in;
	DEV_spi_exchange (spi, data, &in);
	return in;
}

static char mmcGetResponse(void)
{
	//Response comes 1-8bytes after command
	//the first bit will be a 0
	//followed by an error code
	//data will be 0xff until response
	int i=0;

	char response;

	while(i<=64)
	{
		response=spiSendByte(0xff);
		if(response==0x00)break;
		if(response==0x01)break;
		i++;
	}
	return response;
}

static char mmcGetXXResponse(const char resp)
{
	//Response comes 1-8bytes after command
	//the first bit will be a 0
	//followed by an error code
	//data will be 0xff until response
	int i=0;

	char response;

	while(i<=500)
	{
		response=spiSendByte(0xff);
		if(response==resp)break;
		i++;
	}
	return response;
}

static char mmcCheckBusy(void)
{
	//Response comes 1-8bytes after command
	//the first bit will be a 0
	//followed by an error code
	//data will be 0xff until response
	int i=0;

	char response;
	char err;
	while(i<=64)
	{
		response=spiSendByte(0xff);
		response &= 0x1f;
		switch(response)
		{
		case 0x05: err = MMC_SUCCESS;break;
		case 0x0b: return(MMC_CRC_ERROR);
		case 0x0d: return(MMC_WRITE_ERROR);
		default:
			err = MMC_OTHER_ERROR;
			break;
		}
		if(err == MMC_SUCCESS)break;
		i++;
	}
	i=0;
	do
	{
		response=spiSendByte(0xff);
		i++;
	}while(response==0);
	return response;
}

static void mmcSendCmd (const char cmd, unsigned long data, const char crc)
{
	char frame[6];
	char temp;
	int i;

	frame[0]=(cmd|0x40);
	for(i=3;i>=0;i--){
		temp=(char)(data>>(8*i));
		frame[4-i]=(temp);
	}
	frame[5]=(crc);
	for(i=0;i<6;i++)
		spiSendByte(frame[i]);
}

static char mmcSetBlockLength (const unsigned long blocklength)
{
	//MMC_SET_BLOCKLEN =CMD16
	mmcSendCmd(16, blocklength, 0xFF);
	// get response from MMC - make sure that its 0x00 (R1 ok response format)
	if(mmcGetResponse()!=0x00);
	// Send 8 Clock pulses of delay.
	spiSendByte(0xff);
	return MMC_SUCCESS;
}



extern int DEV_mmc_spi_init (DEV_spi_dev_t *dev)
{
	spi = dev;

	if (0 != DEV_spi_dev_config (spi,
			                     0x1F,      /* clock */
			                     0,         /* mode */
			                     8,         /* word size */
			                     0,         /* delay_b_sclk */
			                     0,         /* delay_b_ct */
			                     DEV_SPI_DEV_CS_ACTIVE))
		return (-1);
	return 0;
}

extern char DEV_mmc_init (void)
{
	//raise SS and MOSI for 80 clock cycles
	//SendByte(0xff) 10 times with SS high
	//RAISE SS
	int i;
	char response=0x01;
	char err = MMC_SUCCESS;

	DEV_spi_acquire (spi);

	//initialization sequence on PowerUp
	for(i=0;i<=9;i++)
		spiSendByte(0xff);
	//Send Command 0 to put MMC in SPI mode
	mmcSendCmd(0x00,0,0x95);
	//Now wait for READY RESPONSE
	if(mmcGetResponse()!=0x01)
	{
		err = MMC_RESPONSE_ERROR;
	} else {
		while(response==0x01)
		{
			spiSendByte(0xff);
			mmcSendCmd(0x01,0x00,0xff);
			response=mmcGetResponse();
		}
		spiSendByte(0xff);
	}

	DEV_spi_release (spi);

	return err;
}

// Reading the contents of the CSD and CID registers in SPI mode is a simple
// read-block transaction.
char DEV_mmc_read_register (const char cmd_register, const unsigned char length)
{
	char err = MMC_TIMEOUT_ERROR;

	DEV_spi_acquire (spi);

	if (mmcSetBlockLength (length) == MMC_SUCCESS)
	{
		// CRC not used: 0xff as last byte
		mmcSendCmd(cmd_register, 0x000000, 0xff);

		// wait for response
		// in the R1 format (0x00 is no errors)
		if (mmcGetResponse() == 0x00)
		{
			if (mmcGetXXResponse(0xfe)== 0xfe)
				for (int i = 0; i < length; i++)
					mmc_buffer[i] = spiSendByte(0xff);
			// get CRC bytes (not really needed by us, but required by MMC)
			spiSendByte(0xff);
			spiSendByte(0xff);
		}
		else
			err = MMC_RESPONSE_ERROR;

		// Send 8 Clock pulses of delay.
		spiSendByte(0xff);
	}

	DEV_spi_release (spi);

	return err;
}

// The card will respond with a standard response token followed by a data
// block suffixed with a 16 bit CRC.
extern char DEV_mmc_read_block(const unsigned long address, const unsigned long count)
{
	unsigned long i = 0;
	char err = MMC_RESPONSE_ERROR;

	DEV_spi_acquire (spi);

	// Set the block length to read
	if (mmcSetBlockLength (count) == MMC_SUCCESS)	// block length could be set
	{
		// send read command MMC_READ_SINGLE_BLOCK=CMD17
		mmcSendCmd (17,address, 0xFF);
		// Send 8 Clock pulses of delay, check if the MMC acknowledged the read block command
		// it will do this by sending an affirmative response
		// in the R1 format (0x00 is no errors)
		if (mmcGetResponse() == 0x00)
		{
			// now look for the data token to signify the start of
			// the data
			if (mmcGetXXResponse(MMC_START_DATA_BLOCK_TOKEN) == MMC_START_DATA_BLOCK_TOKEN)
			{
				// clock the actual data transfer and receive the bytes; spi_read automatically finds the Data Block
				for (i = 0; i < 512; i++)
					mmc_buffer[i] = spiSendByte(0xff);	// is executed with card inserted

				// get CRC bytes (not really needed by us, but required by MMC)
				spiSendByte(0xff);
				spiSendByte(0xff);
				err = MMC_SUCCESS;
			}
			else
			{
				// the data token was never received
				err = MMC_DATA_TOKEN_ERROR;	// 3
			}
		}
		else
		{
			// the MMC never acknowledge the read command
			err = MMC_RESPONSE_ERROR;	// 2
		}
	}
	else
    {
		err = MMC_BLOCK_SET_ERROR;	// 1
    }
	spiSendByte(0xff);

	DEV_spi_release (spi);

	return err;
}

extern char DEV_mmc_write_block (const unsigned long address)
{
	unsigned long i = 0;
	char err = MMC_RESPONSE_ERROR;

	DEV_spi_acquire (spi);

	// Set the block length to read
	if (mmcSetBlockLength (512) == MMC_SUCCESS)	// block length could be set
	{
		// send write command
		mmcSendCmd (24,address, 0xFF);

		// check if the MMC acknowledged the write block command
		// it will do this by sending an affirmative response
		// in the R1 format (0x00 is no errors)
		if (mmcGetXXResponse(MMC_R1_RESPONSE) == MMC_R1_RESPONSE)
		{
			spiSendByte(0xff);
			// send the data token to signify the start of the data
			spiSendByte(0xfe);
			// clock the actual data transfer and transmit the bytes
			for (i = 0; i < 512; i++)
				spiSendByte(mmc_buffer[i]);	// mmc_buffer[i];       Test: i & 0xff
			// put CRC bytes (not really needed by us, but required by MMC)
			spiSendByte(0xff);
			spiSendByte(0xff);
			// read the data response xxx0<status>1 : status 010: Data accected, status 101: Data
			//   rejected due to a crc error, status 110: Data rejected due to a Write error.
			mmcCheckBusy();
		}
		else
		{
			// the MMC never acknowledge the write command
			err = MMC_RESPONSE_ERROR;	// 2
		}
	}
	else
    {
		err = MMC_BLOCK_SET_ERROR;	// 1
    }
	//give the MMC the required clocks to finish up what ever it needs to do
	//for (i = 0; i < 9; ++i)
	//spiSendByte(0xff);

	// Send 8 Clock pulses of delay.
	spiSendByte(0xff);

	DEV_spi_release (spi);

	return err;
}
