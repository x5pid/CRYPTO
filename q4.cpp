#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "endian.h"

#define V0 0x736f6d6570736575
#define V1 0x646f72616e646f6d
#define V2 0x6c7967656e657261
#define V3 0x7465646279746573

#include <set>
#include <iostream>

#define K 0x0

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
    uint64_t * v = (uint64_t*)malloc(4*sizeof(uint64_t));
    v[0] = k[0] ^ V0;
    v[1] = k[1] ^ V1;
    v[2] = k[0] ^ V2;
    v[3] = k[1] ^ V3;
       
    //Découpage en Mot de 8 octet + inversion + ajout de l'entete
    // alloc mémoire message inverse + entete	
    uint64_t * m2 = (uint64_t*)calloc(mlen+8,sizeof(uint64_t)); 
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


uint32_t sip_hash_fix32(uint32_t k, uint32_t m){
    uint64_t knew[2] = {0};
    uint8_t *mnew = (uint8_t*)malloc(sizeof(uint8_t)*4);    
    memcpy(mnew, &m, 4) ;
    memcpy(knew, &k, 4) ;
    
    return (uint32_t) siphash_2_4(knew,mnew, 4);
}

uint32_t sip_hash_fix32N2(uint32_t k, uint32_t m){
    uint64_t knew[2] = {0};
    for (unsigned i = 0; i < sizeof(uint32_t) ; i++) {
	*((uint32_t*) knew + i) = *((uint8_t*) &k + i) ;
    }
    uint64_t hash = siphash_2_4(knew,(uint8_t *)&m, 4);
    printf("k : %0#x, key : %0#lx %0lx, hash : %0#lx, message : %u\n", k, knew[0], knew[1], hash, m) ;
    return hash ;
}

uint32_t coll_search (uint32_t k, uint32_t (*fun)(uint32_t, uint32_t)) {
    
    /*printf("k:%x\n",k);
    printf("%x\n",sip_hash_fix32(k,0x1978411));
    
    printf("%x\n",sip_hash_fix32(k,1));*/
    
    std::set<uint32_t> ensemble ;

    for (uint32_t i = 0 ; i < (2 << 31) - 1 ; i++) {
	uint32_t hash = fun(k,i) ;
	std::set<uint32_t>::iterator it ;
	if ((it = ensemble.find(hash)) != ensemble.end()) {
	    printf("hash : %#x0, hash trouve : %#x0\n", hash, *it) ;
	    return i ;	
	}
	else
	    ensemble.insert(hash) ;

    }
    
    return 0 ;
}

int main ()  {
    printf("Indice de collision : %u\n", coll_search(K, sip_hash_fix32N2)) ;
}



