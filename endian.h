#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

void swap (uint8_t * a, uint8_t * b) ;

int is_little_endian(void) ;

void reverse_endianess(void * bloc, unsigned len) ;
