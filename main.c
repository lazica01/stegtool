#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "byte_operations.h"

uint8_t* offset(uint8_t *buffer){
	buffer+=3;
	while(*buffer!=0x0a)
		buffer++;
	buffer++;

	while(*buffer!=0x0a)
		buffer++;
	buffer++;
	return buffer;
}





void write_image(uint8_t *buffer, size_t bufferlen){
	FILE *fp;
	fp = fopen("temp.ppm", "ab");
	fwrite(buffer, 1, bufferlen, fp);
	fclose(fp);
	system("convert temp.ppm out.png");
	system("rm temp.ppm");
	
}
uint8_t* open_image(uint8_t *s, size_t *bufferlen){
	FILE *fp;
	char *command;
	uint8_t *buffer;
	if(strstr(s, ".ppm"))
		fp = fopen(s, "rb");
	
	else{
		command = malloc(strlen("convert  temp.ppm")+strlen(s)+1);
		strcpy(command, "convert ");
		strcat(command, s);
		strcat(command, " temp.ppm");
		system(command);
		printf(command);

		fp = fopen("temp.ppm", "rb");
	}
	fseek(fp, 0, SEEK_END);
	*bufferlen = ftell(fp);
	rewind(fp);
	
	buffer=(uint8_t *)malloc(*bufferlen+1);
	fread(buffer, *bufferlen, 1, fp);
	fclose(fp);
	system("rm temp.ppm > /dev/null 2>&1 &");
	return buffer;
}
void encrypt(uint8_t *buffer, size_t bufferlen, uint8_t *buffer2, uint8_t b){
	uint16_t h, w;
	uint8_t *write;
	int i, n;
	write=buffer;
	sscanf(buffer2+3, "%hu%hu", &h, &w);
	buffer2 = offset(buffer2);
	buffer = offset(buffer);

	
	write_u16(buffer, h);
	buffer+=9;
	write_u16(buffer, w);
	buffer+=9;

	
	n=h*w*3;
	for(i=0; i<n; i++){
		*buffer=write_byte(*buffer, *buffer2, b);
		buffer++; buffer2++;
	}
	write_image(write, bufferlen);
}
void decrypt(uint8_t *buffer, size_t bufferlen, uint8_t b){
	uint16_t h, w;
	int i, n;
	uint8_t *start, *write;
	FILE *out;
	write=buffer;
	buffer=offset(buffer);
	w=read_u16(buffer);
	buffer+=9;
	h=read_u16(buffer);
	buffer+=9;
	system("touch temp.ppm");
	out = fopen("temp.ppm", "w");
	fprintf(out, "P6\n%hu %hu\n255\n", w, h);
	fclose(out);
	write=buffer;
	n = w*h*3;
	for(i=0; i<n; i++){
		*buffer <<= 8-b;
		buffer++;
	}
	write_image(write, bufferlen-(buffer-start));
}
int main(int argc, char **argv){
	uint8_t *buffer, *buffer2;
	size_t bufferlen, buffer2len;
	
	if(argc == 4){
		buffer = open_image(argv[1], &bufferlen);
		buffer2 = open_image(argv[2], &buffer2len);
		encrypt(buffer, bufferlen, buffer2, atoi(argv[3]));
	}
	else if(argc == 3){
		buffer = open_image(argv[1], &bufferlen);
		decrypt(buffer, bufferlen, atoi(argv[2]));
	}
	else{
		printf("Izvini, al neradim tako\n");
		printf("stegtool [hide_in] [to_hide] [bits] //sakrij\n");
		printf("stegtool [try_to_extract] [bits] //dekriptuj\n");
	}
	return 0;
}
