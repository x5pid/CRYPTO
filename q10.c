#include <stdlib.h>#include <stdint.h>#include <stdio.h>#include <string.h>uint8_t sx[16]={0x0C, 0x00, 0x0F, 0x0A, 0x02, 0x0B, 0x09, 0x05, 0x08, 0x03, 0x0D, 0x07, 0x01, 0x0E, 0x06, 0x04};int vh[16] ={5, 0, 1, 4, 7, 12, 3, 8, 13, 6, 9, 2, 15, 10, 11, 14};uint32_t twine_fun1(uint32_t k, uint32_t m);uint64_t twine_perm_z(uint64_t input);uint32_t f(uint32_t k,uint16_t x, uint16_t y);uint32_t twine_fun2(uint32_t k,uint16_t *m, unsigned mlen);uint32_t twine_fun2_fix32(uint32_t k, uint32_t m);uint32_t f(uint32_t k,uint16_t x, uint16_t y){    return twine_fun1(k,(x<<16)^y);}uint32_t twine_fun2(uint32_t k,uint16_t *m, unsigned mlen){    if(mlen ==1 ){        return f(k,0xFFFF,m[mlen-1]);    }else{        return f(k,(uint16_t) twine_fun2(k,m,mlen-1), m[mlen-1]);    }}uint32_t twine_fun2_fix32(uint32_t k, uint32_t m){    uint16_t * mnew = malloc(sizeof(uint16_t)*2);	memcpy(mnew,&m, sizeof(uint16_t)*2) ;    return twine_fun2(k,mnew,2);}uint32_t twine_fun2_fix16(uint32_t k, uint32_t m){    uint16_t * mnew = malloc(sizeof(uint16_t));	memcpy(mnew,&m, sizeof(uint16_t)) ;    return twine_fun2(k,mnew,1);}uint32_t twine_fun1(uint32_t k, uint32_t m){	uint64_t input = 0;	input |= k;	input = (input<<32) | m;	uint64_t res = twine_perm_z(input);	return (uint32_t) res;}uint64_t twine_perm_z(uint64_t input){	unsigned int x[16];	for (int i = 0; i < 16; i++){		x[i] = (input >> (4*i)) & 0xF;	}	unsigned int rk [36][8]={{}};	for(int i = 0; i < 35; i++){		for(int j = 0; j < 8; j++){			x[2*j+1] ^= sx[x[2*j]^rk[i][j]];		}		unsigned int tmp[16];		for(int h = 0; h < 16; h++) {			tmp[vh[h]] = x[h];		}		memcpy(x, tmp, sizeof(unsigned int)*16) ;	}	for(int j = 0; j < 8; j++){		x[2*j+1] ^= (sx[x[2*j]^rk[35][j]]);	}	uint64_t res = 0;	for(int i = 0; i < 16; i++) {		res |= (uint64_t)x[i] << i*4;	}	return res;}int main (int argc, char *argv[]){    uint16_t m1[1]={0x67FC};    uint16_t m2[2]={0xEF12,0x5678};    uint16_t m3[4]={0xEF12,0x5678,0x31AA,0x7123};	uint32_t res = twine_fun2(0x00000000,m1, 1);	printf("resultat : %#x\n", res) ;    res = twine_fun2_fix16(0x00000000,*((uint32_t*) m1));    printf("resultat : %#x\n", res) ;   	res = twine_fun2(0x23AE90FF,m2, 2);	printf("resultat : %#x\n", res) ;       res = twine_fun2_fix32(0x23AE90FF,*((uint32_t*)m2));    printf("resultat : %#x\n", res) ;    res = twine_fun2(0xEEEEEEEE,m3, 4);	printf("resultat : %#x\n", res) ;	return 0 ;}