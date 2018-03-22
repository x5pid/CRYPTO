#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "endian.h"

#define V0 0x736f6d6570736575
#define V1 0x646f72616e646f6d
#define V2 0x6c7967656e657261
#define V3 0x7465646279746573

uint64_t rotation(uint64_t v,int n){
    uint64_t res =0;
    uint64_t tmp = v;
    res = (tmp<<n) | (tmp>>(64-n));
    return res;
}

void sipRound(uint64_t *v, int n){
    for(int i = 0 ; i < n;i++){
        v[0] += v[1];                
        v[2] += v[3];
        v[1] = rotation(v[1],13);
        v[3] = rotation(v[3],16);
        v[1]^=v[0];
        v[3]^=v[2];        
        v[0] = rotation(v[0],32);
        v[2] += v[1];
        v[0] += v[3];
        v[1] = rotation(v[1],17);
        v[3] = rotation(v[3],21);
        v[1]^=v[2];
        v[3]^=v[0];
        v[2] = rotation(v[2],32); 
    }
}

uint64_t siphash_2_4(uint64_t k[2], uint8_t *m, unsigned mlen){
    //Initialisation    
    uint64_t * v = malloc(4*sizeof(uint64_t));
    v[0] = k[0] ^ V0;
    v[1] = k[1] ^ V1;
    v[2] = k[0] ^ V2;
    v[3] = k[1] ^ V3;
       
    //Découpage en Mot de 8 octet + inversion + ajout de l'entete
    // alloc mémoire message inverse + entete	
	uint64_t * m2 = calloc(mlen+8,sizeof(uint64_t)); 
	unsigned p = mlen / 8 ; // nombre de paquets
    unsigned r = mlen % 8 ; // taille du reste
    
	memcpy(m2, m, mlen) ;
	
    // ajout de la longueur sans reste
    if(!r){
        uint64_t tmp= 0;
        tmp = (mlen % 256UL)<<56 ;
        m2[p]=tmp;
    }else{
        // ajout de la longueur avec reste   	
    	uint64_t tmp = ((uint64_t *) m)[p] ;
    	tmp &= ~((1 << (64 - (r+1)*8)) - 1) ;
    	tmp |= (mlen % 256UL)<<56 ; 	
    	m2[p] = tmp ;   	
    }
    p++ ;
    
    for(int i = 0; i < p ;i++){
      v[3] ^= m2[i];
      sipRound(v,2);
      v[0] ^= m2[i];
    }
    v[2] ^= 0xFF;
    sipRound(v,4);
    
    return (v[0]^v[1]^v[2]^v[3]);
}

int main (int argc, char *argv[]){
    uint64_t k[2] = {0x0706050403020100,0x0f0e0d0c0b0a0908} ;
    uint64_t k2[2] = {0x0,0x0} ;
    uint8_t m[15] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7,0x8,0x9,0xa,0xb,0xc,0xd,0xe} ;
    uint8_t m2[8] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7} ;
    printf("k[0] = %#lx\n", k[0]) ;    
    printf("k[1] = %#lx\n", k[1]) ;
    
    uint64_t res = siphash_2_4(k,m,15) ;
    printf("resultat : %#lx\n", res) ;
    
    res = siphash_2_4(k,m2,8) ;
    printf("resultat : %#lx\n", res) ;
    
    res = siphash_2_4(k2,NULL,0) ;
    printf("resultat : %#lx\n", res) ;
    
    return 0 ;
}


