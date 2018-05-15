#ifndef UIODRIVER_H
#define UIODRIVER_H

#define IP   				"10.255.255.255"
#define PORT 				7891
#define PACKET_SIZE        	512

#define UIO_AXI_AUDIO		"/dev/uio0"
#define UIO_FILTER_LINE_IN	"/dev/uio1"
#define UIO_FILTER_NETWORK	"/dev/uio2"
#define UIO_VOLUME_LINE_IN	"/dev/uio3"
#define UIO_VOLUME_NETWORK	"/dev/uio4"
#define UIO_OLED			"/dev/uio5"

#define VOLUME_UP			1
#define VOLUME_DOWN			0

// audio line
#define LINE_IN				1
#define	NETWORK				2

#define LINE_OLED_PAGE		2
#define NETWORK_OLED_PAGE	3

// AXI_AUDIO registers
#define AUDIO_REGISTER 			*((unsigned *)(networkAudio + 0))

// Filter line in registers

// co-efficients  
#define FILTER_LINE_REG_0   	*((unsigned *)(filterLineIn + 0))
#define FILTER_LINE_REG_1   	*((unsigned *)(filterLineIn + 4))
#define FILTER_LINE_REG_2   	*((unsigned *)(filterLineIn + 8))
#define FILTER_LINE_REG_3   	*((unsigned *)(filterLineIn + 12)) 
#define FILTER_LINE_REG_4   	*((unsigned *)(filterLineIn + 16))
#define FILTER_LINE_REG_5   	*((unsigned *)(filterLineIn + 20))
#define FILTER_LINE_REG_6   	*((unsigned *)(filterLineIn + 24)) 
#define FILTER_LINE_REG_7   	*((unsigned *)(filterLineIn + 28))
#define FILTER_LINE_REG_8   	*((unsigned *)(filterLineIn + 32))
#define FILTER_LINE_REG_9   	*((unsigned *)(filterLineIn + 36)) 
#define FILTER_LINE_REG_10  	*((unsigned *)(filterLineIn + 40))
#define FILTER_LINE_REG_11  	*((unsigned *)(filterLineIn + 44))
#define FILTER_LINE_REG_12  	*((unsigned *)(filterLineIn + 48)) 
#define FILTER_LINE_REG_13  	*((unsigned *)(filterLineIn + 52))
#define FILTER_LINE_REG_14  	*((unsigned *)(filterLineIn + 56))
#define FILTER_LINE_RESET		*((unsigned *)(filterLineIn + 60))	
#define FILTER_LINE_TRIGGER		*((unsigned *)(filterLineIn + 64))
// 
#define FILTER_LINE_HIGH		*((unsigned *)(filterLineIn + 68))
#define FILTER_LINE_BAND		*((unsigned *)(filterLineIn + 72))
#define FILTER_LINE_LOW			*((unsigned *)(filterLineIn + 76))

// Filter network registers

// co-efficients  
#define FILTER_NETWORK_REG_0   	*((unsigned *)(filterNetwork + 0))
#define FILTER_NETWORK_REG_1   	*((unsigned *)(filterNetwork + 4))
#define FILTER_NETWORK_REG_2   	*((unsigned *)(filterNetwork + 8))
#define FILTER_NETWORK_REG_3   	*((unsigned *)(filterNetwork + 12)) 
#define FILTER_NETWORK_REG_4   	*((unsigned *)(filterNetwork + 16))
#define FILTER_NETWORK_REG_5   	*((unsigned *)(filterNetwork + 20))
#define FILTER_NETWORK_REG_6   	*((unsigned *)(filterNetwork + 24)) 
#define FILTER_NETWORK_REG_7   	*((unsigned *)(filterNetwork + 28))
#define FILTER_NETWORK_REG_8   	*((unsigned *)(filterNetwork + 32))
#define FILTER_NETWORK_REG_9   	*((unsigned *)(filterNetwork + 36)) 
#define FILTER_NETWORK_REG_10  	*((unsigned *)(filterNetwork + 40))
#define FILTER_NETWORK_REG_11  	*((unsigned *)(filterNetwork + 44))
#define FILTER_NETWORK_REG_12  	*((unsigned *)(filterNetwork + 48)) 
#define FILTER_NETWORK_REG_13  	*((unsigned *)(filterNetwork + 52))
#define FILTER_NETWORK_REG_14  	*((unsigned *)(filterNetwork + 56))
#define FILTER_NETWORK_RESET	*((unsigned *)(filterNetwork + 60))	
#define FILTER_NETWORK_TRIGGER	*((unsigned *)(filterNetwork + 64))
//
#define FILTER_NETWORK_RESET	*((unsigned *)(filterNetwork + 60))
#define FILTER_NETWORK_TRIGGER	*((unsigned *)(filterNetwork + 64))
//
#define FILTER_NETWORK_HIGH		*((unsigned *)(filterNetwork + 68))
#define FILTER_NETWORK_BAND		*((unsigned *)(filterNetwork + 72))
#define FILTER_NETWORK_LOW		*((unsigned *)(filterNetwork + 76))


// Volume line in registers
#define VOLUME_LINE_1   		*((unsigned *)(volumeLineIn + 0))
#define VOLUME_LINE_2   		*((unsigned *)(volumeLineIn + 4))

// Volume line in registers
#define VOLUME_NETWORK_1   		*((unsigned *)(volumeNetwork + 0))
#define VOLUME_NETWORK_2   		*((unsigned *)(volumeNetwork + 4))

// OLED registers
#define OLED_REG   				*((unsigned *)(oled + 0))

/*
 * Macro for mapping physical address to a virtual address. 
 */
#define GET_VIRTUAL_ADDRESS(uio, ptr)\
	fd = open(uio, O_RDWR);\
	if (fd < 1) \
	{ \
		printf("Error opening uio"); \
		programError = 1; \
	}\
	ptr = mmap(NULL, pageSize, (PROT_READ, PROT_WRITE), \
		MAP_SHARED, fd, pageSize * 0);\
		
/*
 * Receives data from the network and writes
 * to the FIFO pipe.
 */
void* receive_audio(void*);

void* receive_command();

int setVolume(unsigned*, int, int);

int setFilter(unsigned*);

void print_status_oled();

#endif
