#include <stdint.h>
//read byte
//byte <<= 8-b;
uint8_t write_byte(uint8_t in, uint8_t w, uint8_t b){
	in &= 0xff<<b;
	w >>= 8-b;
	in |= w;
	return in;
}
uint16_t read_u16(uint8_t *buffer){
	int i;
	uint16_t out = 0;
	buffer+=7;
	for(i=0; i<8; i++){
		out <<= 2;
		out |= *buffer & 0x03;
		buffer--;
	}
	return out;
}
void write_u16(uint8_t *buffer, uint16_t u16){
	int i;
	for(i=0; i<8; i++){
		*buffer &= 0xff<<2;
		*buffer |= u16 & 0x0003;
		u16 >>= 2;
		buffer++;
	}
}
