#ifndef UIODRIVER_H
#define UIODRIVER_H

#define IP   				"10.255.255.255"
#define PORT 				7891
#define PACKET_SIZE        	512
#define BYTES_RECEIVED 		1024
#define AUDIO_REGISTER 		*((int32_t *)(ptr + 0))

#define UIO_AXI_AUDIO		"/dev/uio0"
#define UIO_FILTER_LINE_IN	"/dev/uio1"
#define UIO_FILTER_NETWORK	"/dev/uio2"
#define UIO_VOLUME_LINE_IN	"/dev/uio3"
#define UIO_VOLUME_NETWORK	"/dev/uio4"
#define UIO_OLED			"/dev/uio5"

/*
 * Receives data from the network and writes
 * to the FIFO pipe.
 */
void* receive_audio(void*);

/*
 * Map the physical address to a virtual address. 
 */
void* get_virtual_address(const char*);

#endif
