#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "graphics.h"
#include "imgwrite.h"

#define DIM_X 480
#define DIM_Y 320
#define BASE_R 0
#define BASE_G 0
#define BASE_B 0
#define TEXT_R 255
#define TEXT_G 255
#define TEXT_B 255

int16_t mar[256] = {0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xF800,0xF800,0xF800,0xF800,0xF800,0xF800,0xF800,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
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
 0xFFFF,0xFFFF,0x79E0,0x79E0,0x79E0,0x79E0,0x79E0,0xFFFF,0xFFFF,0x79E0,0x79E0,0x79E0,0x79E0,0x79E0,0xFFFF,0xFFFF,};

Image* createMenuScreen(char** strings, int num, int index){
	Image* img = createBlankScreen(BASE_R, BASE_G, BASE_B);
	
	int line = 0;
	writeText(img, 20, 20*line, "Available units: ");
	line++;
	for(int i = 0;i<num;i++){
		if(index == i){
			char pom[2] = {16, 0};
			writeText(img, 0, 20*line, pom);
		}
		writeText(img, 20, 20*line, strings[i]);
		line++;
	}
	return img;
}

Image* createDetailScreen(char* ip, char* name, int index, unsigned char* wallsRGB, unsigned char* ceilingRGB){
	char* pom = calloc(60, sizeof(char));
	char* title = calloc(55, sizeof(char));
	sprintf(title, "Settings of %s\n on adress %s", name, ip);
	char arrow[2] = {16,0};

	sprintf(pom, "Walls - R:%d G:%d B:%d", wallsRGB[0], wallsRGB[1], wallsRGB[2]);
	Image* img = createTextScreen(20, 20, title);
	writeText(img, 20, 60, pom);
	sprintf(pom, "Ceiling - R:%d G:%d B:%d", ceilingRGB[0], ceilingRGB[1], ceilingRGB[2]);
	writeText(img, 20, 80, pom);
	
	if(index == 0){
		writeText(img, 0, 60, arrow);
	} else {
		writeText(img, 0, 80, arrow);
	}

	return img;
}

Image* createResearchScreen(){
	char* title = "Don't panic! We are searching for others :)";
	Image* img = createTextScreen(30, 50,title);
	
	for(int y = 0;y<16;y++){
		for(int x = 0;x<16;x++){
			img = paintPixel(img, 100+x, 100+y, mar[(y*16 + x)]);
		}
	}
	return img;
}