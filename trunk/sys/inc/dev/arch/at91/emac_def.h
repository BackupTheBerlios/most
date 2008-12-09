#ifndef DEV_AT91C_EMAC_DEF_H
#define DEV_AT91C_EMAC_DEF_H

#define NB_RX_BUFFERS			24			//* Number of receive buffers
#define ETH_RX_BUFFER_SIZE		128         //*
#define ETH_BUFFER_SIZE         2048

#define NB_TX_BUFFERS			2		//* Number of Transmit buffers
#define ETH_TX_BUFFER_SIZE		2048    //*

#define SWAP16(x)	(((x & 0xff) << 8) | (x >> 8))

//* Receive Transfer descriptor structure
struct rx_descriptor {
	unsigned int addr;
	union
	{
		unsigned int status;
		struct {
			unsigned int Length:11;
			unsigned int Res0:1;
			unsigned int Rxbuf_off:2;
			unsigned int StartOfFrame:1;
			unsigned int EndOfFrame:1;
			unsigned int Cfi:1;
			unsigned int VlanPriority:3;
			unsigned int PriorityTag:1;
			unsigned int VlanTag:1;
			unsigned int TypeID:1;
			unsigned int Sa4Match:1;
			unsigned int Sa3Match:1;
			unsigned int Sa2Match:1;
			unsigned int Sa1Match:1;
			unsigned int Res1:1;
			unsigned int ExternalAdd:1;
			unsigned int UniCast:1;
			unsigned int MultiCast:1;
			unsigned int BroadCast:1;
		}S_Status;		
	}U_Status;
};

typedef struct rx_descriptor rx_descriptor_t;


//* Transmit Transfer descriptor structure
struct tx_descriptor {
	unsigned int addr;
	union
	{
		unsigned int status;
		struct {
			unsigned int Length:11;
			unsigned int Res0:4;
			unsigned int LastBuff:1;
			unsigned int NoCrc:1;
			unsigned int Res1:10;
			unsigned int BufExhausted:1;
			unsigned int TransmitUnderrun:1;
			unsigned int TransmitError:1;
			unsigned int Wrap:1;
			unsigned int BuffUsed:1;
		}S_Status;		
	}U_Status;
};

typedef struct tx_descriptor tx_descriptor_t;


#define AT91C_OWNERSHIP_BIT		0x00000001

/* Receive status defintion */
#define AT91C_BROADCAST_ADDR	((unsigned int) (1 << 31))	//* Broadcat address detected
#define AT91C_MULTICAST_HASH 	((unsigned int) (1 << 30))	//* MultiCast hash match
#define AT91C_UNICAST_HASH 	    ((unsigned int) (1 << 29))	//* UniCast hash match
#define AT91C_EXTERNAL_ADDR	    ((unsigned int) (1 << 28))	//* External Address match
#define AT91C_SA1_ADDR	    	((unsigned int) (1 << 26))	//* Specific address 1 match
#define AT91C_SA2_ADDR	    	((unsigned int) (1 << 25))	//* Specific address 2 match
#define AT91C_SA3_ADDR	    	((unsigned int) (1 << 24))	//* Specific address 3 match
#define AT91C_SA4_ADDR	    	((unsigned int) (1 << 23))	//* Specific address 4 match
#define AT91C_TYPE_ID	    	((unsigned int) (1 << 22))	//* Type ID match
#define AT91C_VLAN_TAG	    	((unsigned int) (1 << 21))	//* VLAN tag detected
#define AT91C_PRIORITY_TAG    	((unsigned int) (1 << 20))	//* PRIORITY tag detected
#define AT91C_VLAN_PRIORITY    	((unsigned int) (7 << 17))  //* PRIORITY Mask
#define AT91C_CFI_IND        	((unsigned int) (1 << 16))  //* CFI indicator
#define AT91C_EOF           	((unsigned int) (1 << 15))  //* EOF
#define AT91C_SOF           	((unsigned int) (1 << 14))  //* SOF
#define AT91C_RBF_OFFSET     	((unsigned int) (3 << 12))  //* Receive Buffer Offset Mask
#define AT91C_LENGTH_FRAME     	((unsigned int) 0x07FF)     //* Length of frame

/* Transmit Status definition */
#define AT91C_TRANSMIT_OK		((unsigned int) (1 << 31))	//*
#define AT91C_TRANSMIT_WRAP		((unsigned int) (1 << 30))	//* Wrap bit: mark the last descriptor
#define AT91C_TRANSMIT_ERR		((unsigned int) (1 << 29))	//* RLE:transmit error
#define AT91C_TRANSMIT_UND		((unsigned int) (1 << 28))	//* Transmit Underrun
#define AT91C_BUF_EX     		((unsigned int) (1 << 27))	//* Buffers exhausted in mid frame
#define AT91C_TRANSMIT_NO_CRC	((unsigned int) (1 << 16))	//* No CRC will be appended to the current frame
#define AT91C_LAST_BUFFER    	((unsigned int) (1 << 15))	//*

#endif
