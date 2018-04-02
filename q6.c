#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint8_t sx[16]={0x0C, 0x00, 0x0F, 0x0A, 0x02, 0x0B, 0x09, 0x05, 0x08, 0x03, 0x0D, 0x07, 0x01, 0x0E, 0x06, 0x04};
int vh[16] ={5, 0, 1, 4, 7, 12, 3, 8, 13, 6, 9, 2, 15, 10, 11, 14};

uint64_t twine_perm_z(uint64_t input){

	unsigned x[16];
	for (int i = 0; i < 16; i++){
		x[i] = (input >> (4*i)) & 0xF;
	}
	

	unsigned rk [36][8]={0};
	
	for(int i = 0; i < 35; i++){
		for(int j = 0; j < 8; j++){
			x[2*j+1] ^= sx[x[2*j]^rk[i][j]];
		}

		unsigned tmp[16];
		for(int h = 0; h < 16; h++) {
			tmp[vh[h]] = x[h];
		}
		memcpy(x, tmp, sizeof(unsigned)*16) ;
	}
	
	for(int j = 0; j < 8; j++){
		x[2*j+1] ^= (sx[x[2*j]^rk[35][j]]);
	}
	
	unsigned res[16]={0};
	for(int i = 0; i < 8; i++) {
		res[i] = x[2*i]<<4 | x[2*i+1];
	}
	
	//Probleme tout juste avant et probleme pour affecter 
	printf("res2:");
	uint64_t resultat = 0;
	for(int i = 0; i < 8;i++){
		printf("%#x ",res[i]) ;
		resultat |= res[i]<<(i*8);
		//printf("%#lx\n",resultat) ;
	}
	printf("\n");
	return  resultat;
}

int main (int argc, char *argv[]){

	uint64_t res = twine_perm_z(0x0000000000000000ULL);
	printf("resultat : %#lx\n", res) ;
	
	//res = twine_perm_z(0x123456789abcdef1ULL);
	//printf("resultat : %#lx\n", res) ;
	
	//res = twine_perm_z(0xb4329ed38453aac8ULL);
	//printf("resultat : %#lx\n", res) ;
	
	return 0 ;
}


