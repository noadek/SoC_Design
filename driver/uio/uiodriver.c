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
#include "ZedboardOLED.h"
#include "keyboardinput.h" //to read getch data type from keyboard
#include "uiodriver.h"

volatile int programError = 0;
unsigned pageSize;
// get register variables
void *networkAudio;
void *filterLineIn;
void *filterNetwork;
void *volumeLineIn;
void *volumeNetwork;
void *oled;
int fd;

int main(int argc, char *argv[]){
	if (*argv[1] == 'p') {
        printf("::::START_USAGE::::\n");
        printf("FILTER EXAMPLE (high, band, low): %s f 1 0 0 \n", argv[0]);
        printf("VOLUME: Use 'q' and 'w' to control volume up ", argv[0]);
		printf("Values should be multiples of 2.\n");
        printf("::::END_USAGE::::\n");
    }
    else {
		printf("Audio mixer started\n\n");
		
		//get architecture specific page size
		pageSize = sysconf(_SC_PAGESIZE);

		// assign virtual addresses
		GET_VIRTUAL_ADDRESS(UIO_FILTER_LINE_IN, filterLineIn);
		GET_VIRTUAL_ADDRESS(UIO_FILTER_NETWORK, filterNetwork);
		GET_VIRTUAL_ADDRESS(UIO_VOLUME_LINE_IN, volumeLineIn);
		GET_VIRTUAL_ADDRESS(UIO_VOLUME_NETWORK, volumeNetwork);
		GET_VIRTUAL_ADDRESS(UIO_AXI_AUDIO, networkAudio);
		GET_VIRTUAL_ADDRESS(UIO_OLED, oled);
		
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
		FILTER_NETWORK_RESET = FILTER_LINE_RESET = 0;
		FILTER_NETWORK_TRIGGER = FILTER_LINE_TRIGGER = 1;
		
		// set the default filters
		FILTER_NETWORK_HIGH = FILTER_LINE_HIGH = 0;
		FILTER_NETWORK_BAND = FILTER_LINE_BAND = 0;
		FILTER_NETWORK_LOW = FILTER_LINE_LOW = 0;
		
		// set default volume
		VOLUME_LINE_1 = 256;
		VOLUME_LINE_2 = 256;
		
		VOLUME_NETWORK_1 = 2048;
		VOLUME_NETWORK_2 = 2048;
		
		// print message on OLED
		print_status_oled();
		
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
		
		// Create keyboard thread
		pthread_t keyboardThread;
		int err2 = pthread_create( &keyboardThread, NULL, receive_command, NULL);
		if (err2) {
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
		pthread_join(keyboardThread, NULL);
		
		// remove the FIFO
		unlink(audioPipe);
		
		//delete mapping
		munmap(networkAudio, pageSize);
		munmap(filterLineIn, pageSize);
		munmap(filterNetwork, pageSize);
		munmap(volumeLineIn, pageSize);
		munmap(volumeNetwork, pageSize);
		munmap(networkAudio, pageSize);
		munmap(oled, pageSize);
		
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
		if (programError || udp_client_recv(buffer, sizeof(buffer)) == 1) {
			printf("Error occured while receiving.\n");
            break;
        }
        // write to FIFO
        write(writeFd, buffer, sizeof(buffer));
    }
    close(writeFd);
    pthread_exit(NULL);
}

/*
 * Receives commands from keyboard
 */
void* receive_command()
{
	char key;
	while(1) {
		key = getch();
		switch(key) {
			// line in controls
			case 'w':
				// line in volume up for left side
				setVolume(&VOLUME_LINE_1, VOLUME_UP, LINE_IN);
				break;
			case 'q':
				// line in volume down for right side
				setVolume(&VOLUME_LINE_1, VOLUME_DOWN, LINE_IN);
				break;
			case 'r':
				// line network volume up for left
				setVolume(&VOLUME_LINE_2, VOLUME_UP, LINE_IN);
				break;
			case 'e':
				// line network volume up for left
				setVolume(&VOLUME_LINE_2, VOLUME_DOWN, LINE_IN);
				break;
			case 't':
				setFilter(&FILTER_LINE_HIGH);
				print_status_oled();
				break;
			case 'y':
				setFilter(&FILTER_LINE_BAND);
				print_status_oled();
				break;
			case 'u':
				setFilter(&FILTER_LINE_LOW);
				print_status_oled();
				break;
				
			// network controls
			case 's':
				// line in volume up for left side
				setVolume(&VOLUME_NETWORK_1, VOLUME_UP, NETWORK);
				break;
			case 'a':
				// line in volume down for right side
				setVolume(&VOLUME_NETWORK_1, VOLUME_DOWN, NETWORK);
				break;
			case 'f':
				// line network volume up for left
				setVolume(&VOLUME_NETWORK_2, VOLUME_UP, NETWORK);
				break;
			case 'd':
				// line network volume up for left
				setVolume(&VOLUME_NETWORK_2, VOLUME_DOWN, NETWORK);
				break;
			case 'g':
				setFilter(&FILTER_NETWORK_HIGH);
				print_status_oled();
				break;
			case 'h':
				setFilter(&FILTER_NETWORK_BAND);
				print_status_oled();
				break;
			case 'j':
				setFilter(&FILTER_NETWORK_LOW);
				print_status_oled();
				break;
			default:
				printf("Unknown command\n");
		}
	}
}

/*
 * Sets the volume
 * direction: VOLUME_UP(1) -up, VOLUME_DOWN(0) -down
 */
int setVolume(unsigned *volume, int direction, int audioLine)
{
	const int MAX = 14;
	int lineInRange[] = {0, 16, 32, 64, 128, 256, 512, 1024, \
		1536, 2048, 2560, 3072, 3584, 4096};
	int networkRange[] = {1536, 2048, 2560, 3072, 3584, 4096, 4352,\
		4608, 4864, 5376, 5632, 5888, 6144, 6400};
		
	int* volumeRange;
	volumeRange = (audioLine == LINE_IN) ? lineInRange : networkRange;
	
	// check index of current volume
	int index;
	for (int i=0; i < MAX; i++) {
		if (*volume == volumeRange[i]) {
			index = i;
		}
	}
	// increase or decrease volume
	if (direction == VOLUME_UP) {
		// check if max volume is reached
		if (index == MAX-1) {
			printf("Maximum volume reached.\n");
			return 0;
		}
	
		// set volume to next in line
		*volume = volumeRange[++index];
	} else { // VOLUME_DOWN
		// check if lowest volume is reached
		if (index == 0) {
			printf("Muted!\n");
			return 0;
		}
		// set volume to next in line
		*volume = volumeRange[--index];
	}	
	
	return 0;
}

/*
 * Toggle filters
 */
int setFilter(unsigned *filter)
{
	*filter = !*filter;
}

/*
 * prints the filter status on the OLED screen
 */
void print_status_oled()
{
	oled_clear(oled);
	if (!oled_print_message("AUDIO MIXER", 0, oled)){
		perror("Error: Failed to write in the OLED\n");
	}
	
	char lineBuffer[16];
	sprintf(lineBuffer, "Line H%d B%d L%d", 
		FILTER_LINE_HIGH, FILTER_LINE_BAND, FILTER_LINE_LOW);
	oled_print_message(lineBuffer, LINE_OLED_PAGE, oled);
		
	char networkBuffer[16];
	sprintf(networkBuffer, "Network H%d B%d L%d", 
		FILTER_NETWORK_HIGH, FILTER_NETWORK_BAND, FILTER_NETWORK_LOW);
	oled_print_message(networkBuffer, NETWORK_OLED_PAGE, oled);
}
