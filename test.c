#include "sip.h"

int main () {
    uint8_t in[64], out[16], k[16];
    for (i = 0; i < 64; ++i) {
	     siphash(in, i, k, out, len);
    }
    printf("out[0] = %#lx", *(uint64_t*)out) ;
    printf("out[1] = %#lx", *((uint64_t*)out + 1)) ;
    return 0 ;
}
