#define _POSIX_C_SOURCE 200112L

#include <sys/mman.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <malloc.h>
#include <string.h>
#include <byteswap.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include "imgwrite.h"
#include "control.h"
#include "screen.h"
#include "utils.h"
#include "graphics.h"
#include "intercom.h"


#define DIM_X 480
#define DIM_Y 320
#define BASE_R 0
#define BASE_G 0
#define BASE_B 0
#define TEXT_R 255
#define TEXT_G 255
#define TEXT_B 255
#define INCREMENT_TYPE 1
#define SET_TYPE 2
#define MZAPO 0

struct timespec loop_delay = {.tv_sec = 1, .tv_nsec = 1000 * 1000 * 1000};

unsigned char* mem_base;
unsigned char* lcd_base;
unsigned char* thisWalls;
unsigned char* thisCeiling;
char* thisText;
int16_t* thisImage;

int16_t mario[256] = {0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xF800,0xF800,0xF800,0xF800,0xF800,0xF800,0xF800,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
 0xFFFF,0xFFFF,0xFFFF,0xF800,0xF800,0xF800,0xF800,0xF800,0xF800,0xF800,0xF800,0xF800,0xF800,0xF800,0xFFFF,0xFFFF,
 0xFFFF,0xFFFF,0xFFFF,0x79E0,0x79E0,0x79E0,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0x0000,0xFDEF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
 0xFFFF,0xFFFF,0x79E0,0xFDEF,0x79E0,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0x0000,0xFDEF,0xFDEF,0xFDEF,0xFFFF,0xFFFF,
 0xFFFF,0xFFFF,0x79E0,0xFDEF,0x79E0,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0x0000,0xFDEF,0xFDEF,0xFDEF,0xFFFF,
 0xFFFF,0xFFFF,0x79E0,0x79E0,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0x0000,0x0000,0x0000,0x0000,0xFFFF,0xFFFF,
 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0xFDEF,0xFFFF,0xFFFF,0xFFFF,
 0xFFFF,0xFFFF,0xFFFF,0xF800,0xF800,0xF800,0x5014,0xF800,0xF800,0xF800,0x5014,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
 0xFFFF,0xFFFF,0xF800,0xF800,0xF800,0xF800,0x5014,0xF800,0xF800,0x5014,0xF800,0xF800,0xF800,0xFFFF,0xFFFF,0xFFFF,
 0xFFFF,0xF800,0xF800,0xF800,0xF800,0xF800,0x5014,0x5014,0x5014,0x5014,0xF800,0xF800,0xF800,0xF800,0xFFFF,0xFFFF,
 0xFFFF,0xFFFF,0xFDEF,0xFDEF,0xF800,0x5014,0xFFE5,0x5014,0x5014,0xFFE5,0x5014,0xF800,0xFDEF,0xFDEF,0xFFFF,0xFFFF,
 0xFFFF,0xFDEF,0xFDEF,0xFDEF,0x5014,0x5014,0x5014,0x5014,0x5014,0x5014,0x5014,0xFDEF,0xFDEF,0xFDEF,0xFFFF,0xFFFF,
 0xFFFF,0xFDEF,0xFDEF,0x5014,0x5014,0x5014,0x5014,0x5014,0x5014,0x5014,0x5014,0x5014,0xFDEF,0xFDEF,0xFFFF,0xFFFF,
 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x5014,0x5014,0x5014,0xFFFF,0xFFFF,0x5014,0x5014,0x5014,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
 0xFFFF,0xFFFF,0xFFFF,0x79E0,0x79E0,0x79E0,0x79E0,0xFFFF,0xFFFF,0x79E0,0x79E0,0x79E0,0x79E0,0xFFFF,0xFFFF,0xFFFF,
 0xFFFF,0xFFFF,0x79E0,0x79E0,0x79E0,0x79E0,0x79E0,0xFFFF,0xFFFF,0x79E0,0x79E0,0x79E0,0x79E0,0x79E0,0xFFFF,0xFFFF,
};

int init(int mzapo){
	int socket = initCommunication();
	if(socket == 0) return 0;
	
	if(mzapo){
		lcd_base = initScreen();
		mem_base = initMemBase();
		if(lcd_base == NULL || mem_base == NULL) return 0;
	}
	
	thisCeiling = calloc(3, sizeof(char));
	thisWalls = calloc(3, sizeof(char));
	thisText = "bbb";
	
	thisWalls[0] = 0;
	thisWalls[1] = 2;
	thisWalls[2] = 4;
	
	
	thisCeiling[0] = 4;
	thisCeiling[1] = 2;
	thisCeiling[2] = 0;
	
	thisImage = malloc(512);
	
	if(thisCeiling == NULL || thisWalls == NULL || thisImage == NULL)
		return 0;
	
	memcpy(thisImage, mario, 512);
	return socket;
}

int main(){
	int socket = init(MZAPO);
	if(socket == 0){
		printf("[ERROR] Init failed\n");
		exit(1);
	} else{
		printf("[OK] Init\n");
	}
	long lastMilis = time(NULL);
	
	while(1){
		if((time(NULL) - lastMilis) >= 1){
			printf("Broadcasting...");
			printf("%d/n", broadcastInfo(socket, thisWalls, thisCeiling, thisText, thisImage));
			lastMilis = time(NULL);
		}
	}
	
	
	return 0;
}



