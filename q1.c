#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define V0 0x736f6d6570736575
#define V1 0x646f72616e646f6d
#define V2 0x6c7967656e657261
#define V3 0x7465646279746573


void rotation(uint64_t v,int n){
    uint64_t res =0;
    uint64_t tmp = v;
    res = (tmp<<n) | (tmp>>(64-n));
    return res;
}

void sipRound(uint64_t *v, int n){
    for(int i = 0 ; i < n;i++){
        v[0] += v[1];                v[2] += v[3];
        v[1] = rotation(v[1],13);    v[3] = rotation(v[3],16);
        v[1]^=v[0];                  v[3]^=v[2];
        v[0] = rotation(v[0],32);
        v[2] += v[1];                v[0] += v[3];
        v[1] = rotation(v[1],17);    v[3] = rotation(v[3],21);
        v[1]^=v[2];                  v[3]^=v[0];
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

    
    //Découpage en Mot de 8 octet
    uint64_t * m = calloc(mlen+1,sizeof(uint64_t));
    for(int i = 0;i< mlen;i=+8){
        m[i]= 0;
        for(int j = 0;j<8;j++){
            m[i]= m[i] + (m[i]<<(j*8));
        }
    }
    
    for(int i = 0; i < (mlen/8);i++){
      v[3] ^= m[i];
      sipRound(v,2);
      v[0] ^= m[i];
    }

    v[2] ^= 0xFF;
    sipRound(v,4);
    
    return (v[0]^v[1]^v[2]^v[3]);
}


int main (int argc, char *argv[]){

}
