#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint8_t sx[16]={0x0C, 0x00, 0x0F, 0x0A, 0x02, 0x0B, 0x09, 0x05, 0x08, 0x03, 0x0D, 0x07, 0x01, 0x0E, 0x06, 0x04};
int vh[16] ={5, 0, 1, 4, 7, 12, 3, 8, 13, 6, 9, 2, 15, 10, 11, 14};

uint32_t twine_fun1(uint32_t k, uint32_t m){

	uint64_t input = 0;
	input |= m;
	printf("test: %#lx\n",input) ;
	input = (input<<32) | k;
	printf("test: %#lx\n",input) ;
	
	unsigned char x[16];
	for (int i = 0; i < 16; i++){
		x[i] = (m >> (4*i)) & 0xF;
	}
	
	unsigned char rk [36][8]={0};
	
	for(int i = 0; i < 35; i++){
		for(int j = 0; j < 8; j++){
			x[2*j+1] ^= sx[x[2*j]^rk[i][j]];
		}

		unsigned char tmp[16];
		for(int h = 0; h < 16; h++) {
			tmp[vh[h]] = x[h];
		}
		memcpy(x, tmp, sizeof(unsigned char)*16) ;
	}
	
	for(int j = 0; j < 8; j++){
		x[2*j+1] ^= (sx[x[2*j]^rk[35][j]]);
	}
	
	unsigned char res[16]={0};
	for(int i = 0; i < 8; i++) {
		res[i] = x[2*i]<<4 | x[2*i+1];
	}
	return  *((uint32_t*)res);
}

int main (int argc, char *argv[]){

	uint32_t res = twine_fun1(0x00000000, 0x00000000);
	printf("resultat : %#x\n", res) ;
	
	res = twine_fun1(0xcdef1234, 0xab123478);
	printf("resultat : %#x\n", res) ;
	return 0 ;
}


