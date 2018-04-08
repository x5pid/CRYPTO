#ifndef Q4_HPP
#define Q4_HPP

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <set>
#include <iostream>
#include <unordered_set>

#ifdef i386
#define MOD 256ULL
#else
#define MOD 256UL
#endif

using namespace std ;

class SipHash {
private:
    static uint64_t rotation(uint64_t v,int n) ;
    static void sipRound(uint64_t *v, int n) ;
public:
    static uint64_t siphash_2_4(uint64_t k[2], uint8_t *m, unsigned mlen) ;
    static void test_siphash_2_4();
};

#define NBTEST 1000

class SearchColl {
private:
    static unordered_set<uint32_t> ensemble[NBTEST] ;
    static unsigned num ;
    static uint32_t coll_search (uint32_t k, uint32_t (*fun)(uint32_t, uint32_t)) ;
    static uint32_t sip_hash_fix32(uint32_t k, uint32_t m) ;
public:
    static void test_coll_search () ;
};

#endif /* Q4_HPP */

