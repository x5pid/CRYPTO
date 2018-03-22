#include "endian.h"

void swap (uint8_t * a, uint8_t * b) {
    uint8_t tmp = *a ;
	*a = *b ;
	*b = tmp ;
}
    

int is_little_endian(void) {

	int resultat = 42 ;
	
	resultat = ((char)resultat == (int)resultat) ;
	
	return resultat ;
}

void reverse_endianess(void * bloc, unsigned len) {
	
	for (unsigned i = 0; i < len >> 1 ; i++)
	    swap((uint8_t *)bloc + i, (uint8_t *)bloc + len - i - 1) ;
}

/*int main () {
    printf("Is little Endian ? %s\n", is_little_endian() ? "true" : "false") ;
    srand(time(NULL)) ;
    uint64_t i = 0x1234567887654321;
    printf("endian : %#lx\n", i) ;
    reverse_endianess(&i, 8) ;
    printf("reverse endian : %#lx\n", i) ; 
    return 0 ;
}*/
