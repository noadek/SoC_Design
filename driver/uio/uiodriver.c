#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // to get pagesize
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <errno.h>

#include "udpclient.h"
#include "uiodriver.h"

volatile int programError = 0;
unsigned pageSize;
// get register variables
void *networkAudio;
void *filterLineIn;
void *filterNetwork;
void *volumeLineIn;
void *volumeNetwork;
int fd;

int main(int argc, char *argv[]){
	if (*argv[1] == 'p') {
        printf("::::START_USAGE::::\n");
        printf("FILTER EXAMPLE (high, band, low): %s f 1 0 0 \n", argv[0]);
        printf("VOLUME EXAMPLE (left, right): %s v 4 -2 \n", argv[0]);
		printf("Values should be multiples of 2.\n");
        printf("::::END_USAGE::::\n");
    }
    else {
		printf("Ready to receive...\n");
		
		//get architecture specific page size
		pageSize = sysconf(_SC_PAGESIZE);

		// assign virtual addresses
		GET_VIRTUAL_ADDRESS(UIO_FILTER_LINE_IN, filterLineIn);
		GET_VIRTUAL_ADDRESS(UIO_FILTER_NETWORK, filterNetwork);
		GET_VIRTUAL_ADDRESS(UIO_VOLUME_LINE_IN, volumeLineIn);
		GET_VIRTUAL_ADDRESS(UIO_VOLUME_NETWORK, volumeNetwork);
		GET_VIRTUAL_ADDRESS(UIO_AXI_AUDIO, networkAudio);
		
		FILTER_NETWORK_REG_0 = FILTER_LINE_REG_0 = 0x00002CB6;
		FILTER_NETWORK_REG_1 = FILTER_LINE_REG_1 = 0x0000596C;
		FILTER_NETWORK_REG_2 = FILTER_LINE_REG_2 = 0x00002CB6;
		FILTER_NETWORK_REG_3 = FILTER_LINE_REG_3 = 0x8097A63A;
		FILTER_NETWORK_REG_4 = FILTER_LINE_REG_4 = 0x3F690C9D;
		FILTER_NETWORK_REG_5 = FILTER_LINE_REG_5 = 0x074D9236;
		FILTER_NETWORK_REG_6 = FILTER_LINE_REG_6 = 0x00000000;
		FILTER_NETWORK_REG_7 = FILTER_LINE_REG_7 = 0xF8B26DCA;
		FILTER_NETWORK_REG_8 = FILTER_LINE_REG_8 = 0x9464B81B;
		FILTER_NETWORK_REG_9 = FILTER_LINE_REG_9 = 0x3164DB93;
		FILTER_NETWORK_REG_10 = FILTER_LINE_REG_10 = 0x12BEC333;
		FILTER_NETWORK_REG_11 = FILTER_LINE_REG_11 = 0xDA82799A;
		FILTER_NETWORK_REG_12 = FILTER_LINE_REG_12 = 0x12BEC333;
		FILTER_NETWORK_REG_13 = FILTER_LINE_REG_13 = 0x00000000;
		FILTER_NETWORK_REG_14 = FILTER_LINE_REG_14 = 0x0AFB0CCC;
		
		// reset and trigger filters
		FILTER_LINE_RESET = 0;
		FILTER_LINE_TRIGGER = 1;
		
		FILTER_NETWORK_RESET = 0;
		FILTER_NETWORK_TRIGGER = 1;
		
		// set the default filters
		FILTER_LINE_HIGH = 0;
		FILTER_LINE_BAND = 0;
		FILTER_LINE_LOW = 0;
		
		FILTER_NETWORK_HIGH = 0;
		FILTER_NETWORK_BAND = 0;
		FILTER_NETWORK_LOW = 0;
		
		// set default volume
		VOLUME_LINE_1 = 256;
		VOLUME_LINE_2 = 256;
		
		VOLUME_NETWORK_1 = 1024;
		VOLUME_NETWORK_2 = 1024;
		
		// create data pipe
		char* audioPipe = "/tmp/audiopipe";
		int ret = mkfifo(audioPipe, 0666);
		if ((ret == -1) && (errno != EEXIST)) {
			perror("Error creating the named pipe");
			return -1;
		}
		
		// open pipe in read and write mode
		int readFd = open(audioPipe, O_RDONLY|O_NONBLOCK);
		int writeFd = open(audioPipe, O_WRONLY|O_NONBLOCK);

		// Create receiver thread
		pthread_t receiverThread;
		int err1 = pthread_create( &receiverThread, NULL, receive_audio, (void*) &writeFd);
		if (err1) {
			printf("Error - pthread_create() return code: %d\n", err1);
			return -1;
		}
		
		//Read audio data from the FIFO pipe and put data into audio register
		int16_t audioChunk;
		while (1) {
			if (programError) {
				printf("Exiting....\n");
				break;
			}
			int bytes_read = read(readFd, &audioChunk, sizeof(int16_t));
			if (bytes_read < 0) {
				continue;
			}
			// sign extend 16 bit data and put in register
			AUDIO_REGISTER = (int32_t) audioChunk;
		}
		close(readFd);
		
		// Wait till threads complete before main exits.
		pthread_join(receiverThread, NULL);
		
		// remove the FIFO
		unlink(audioPipe);
		
		//delete mapping
		munmap(networkAudio, pageSize);
		munmap(filterLineIn, pageSize);
		munmap(filterNetwork, pageSize);
		munmap(volumeLineIn, pageSize);
		munmap(volumeNetwork, pageSize);
		munmap(networkAudio, pageSize);
		
		printf("Closed!\n");
	}
	
    return 0;
}

/*
 * Receives data from the network and writes
 * to the FIFO pipe.
 */
void *receive_audio(void* fd)
{
    int writeFd = (*(int*)fd);
    int16_t buffer[PACKET_SIZE];
    
    programError = udp_client_setup(IP, PORT);
    
    while (1) {
		if (programError || udp_client_recv(buffer, BYTES_RECEIVED) == 1) {
			printf("Error occured while receiving.\n");
            break;
        }
        // write to FIFO
        write(writeFd, buffer, sizeof(buffer));
    }
    close(writeFd);
    pthread_exit(NULL);
}
