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
//get architecture specific page size
unsigned pageSize;

int main(int argc, char *argv[]){
	printf("Ready to receive...\n");
	
	// get register variables
	void* ptr = get_virtual_address(UIO_AXI_AUDIO);
	
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
        fprintf(stderr, "Error - pthread_create() return code: %d\n", err1);
        return -1;
    }
    
    //Read audio data from the FIFO pipe and put data into audio register
    int16_t audioChunk;
    while (1) {
		if (programError) {
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
    munmap(ptr, pageSize);

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
            break;
        }
        // write to FIFO
        write(writeFd, buffer, sizeof(buffer));
    }
    close(writeFd);
    pthread_exit(NULL);
}

/*
 * Map the physical address to a virtual address. 
 */
void* get_virtual_address(const char* uio)
{
	pageSize = sysconf(_SC_PAGESIZE);
	//open dev/uio#
	int fd = open (uio, O_RDWR);
	if (fd < 1) { 
		perror("Error opening uio"); 
		programError = 1; 
	}
	
	//Redirect stdout/printf into /dev/kmsg file (so it will be printed using printk)
	freopen ("/dev/kmsg","w",stdout);
	
	return mmap(NULL, pageSize, (PROT_READ, PROT_WRITE), MAP_SHARED, fd, pageSize * 0);
}
