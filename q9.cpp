#include "q1q3q4.hpp"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define V0 0x736f6d6570736575
#define V1 0x646f72616e646f6d
#define V2 0x6c7967656e657261
#define V3 0x7465646279746573
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
	
	unsigned int rk [36][8]={0};
	
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


uint32_t SearchColl::coll_search (uint32_t k, uint32_t (*fun)(uint32_t, uint32_t)) {
    for (uint32_t i = 0 ; i < (uint32_t)((2 << 31) - 1) ; i++) {
        uint32_t hash = fun(k,i) ;
        unordered_set<uint32_t>::iterator it ;
        if ((it = ensemble[num].find(hash)) != ensemble[num].end())
            return i ;	
        else
            ensemble[num].insert(hash) ;
    }
    return 0 ;
}

unordered_set<uint32_t> SearchColl::ensemble[NBTEST] ;
unsigned SearchColl::num = 0 ;

void SearchColl::test_coll_search () {
    FILE * f = fopen("collisions4.txt", "w") ;
    srand(time(NULL)) ;
    for (; num < NBTEST ; num++) {
        ensemble[num].reserve(1 << 16) ;
        fprintf(f, "%u\n", SearchColl::coll_search(rand(), twine_fun1)) ;
    }
}

uint64_t reverse_endianess (uint64_t uint64) {
    char * m = (char *) &uint64 ;
    for (unsigned i = 0 ; i < 4 ; i++) {
        char tmp = m[i] ;
        m[i] = m[8-i-1] ;
        m[8-i-1] = tmp ;
    }
    return uint64 ;
}

int main () {
    
    SearchColl::test_coll_search() ;
    return 0 ;
}



