#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint8_t sx[16]={0x0C, 0x00, 0x0F, 0x0A, 0x02, 0x0B, 0x09, 0x05, 0x08, 0x03, 0x0D, 0x07, 0x01, 0x0E, 0x06, 0x04};
int vh[16] ={5, 0, 1, 4, 7, 12, 3, 8, 13, 6, 9, 2, 15, 10, 11, 14};

uint32_t twine_fun1(uint32_t k, uint32_t m);
uint64_t twine_perm_z(uint64_t input);

uint32_t twine_fun1(uint32_t k, uint32_t m){

	uint64_t input = 0;
	input |= k;
	input = (input<<32) | m;
	
	uint64_t res = twine_perm_z(input);
	return (uint32_t) res;
}

uint64_t twine_perm_z(uint64_t input){

	unsigned int x[16];
	for (int i = 0; i < 16; i++){
		x[i] = (input >> (4*i)) & 0xF;
	}
	
	unsigned int rk [36][8]={{0}};
	
	for(int i = 0; i < 35; i++){
		for(int j = 0; j < 8; j++){
			x[2*j+1] ^= sx[x[2*j]^rk[i][j]];
		}

		unsigned int tmp[16];
		for(int h = 0; h < 16; h++) {
			tmp[vh[h]] = x[h];
		}
		memcpy(x, tmp, sizeof(unsigned int)*16) ;
	}
	
	for(int j = 0; j < 8; j++){
		x[2*j+1] ^= (sx[x[2*j]^rk[35][j]]);
	}
	
	uint64_t res = 0;
	for(int i = 0; i < 16; i++) {
		res |= (uint64_t)x[i] << i*4;
	}
	return res;
}

int main (int argc, char *argv[]){

	uint32_t res = twine_fun1(0x00000000, 0x00000000);
	printf("resultat : %#x\n", res) ;
	
	res = twine_fun1(0xcdef1234, 0xab123478);
	printf("resultat : %#x\n", res) ;
	return 0 ;
}


