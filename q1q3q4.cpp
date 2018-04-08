#include "q1q3q4.hpp"

uint64_t SipHash::rotation(uint64_t v,int n){
    uint64_t res =0;
    uint64_t tmp = v;
    res = (tmp<<n) | (tmp>>(64-n));
    return res;
}

void SipHash::sipRound(uint64_t *v, int n){
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

#define V0 0x736f6d6570736575
#define V1 0x646f72616e646f6d
#define V2 0x6c7967656e657261
#define V3 0x7465646279746573

uint64_t SipHash::siphash_2_4(uint64_t k[2], uint8_t *m, unsigned mlen){
    
    // Initialisation
    
    uint64_t v[4] ;
    v[0] = k[0] ^ V0;
    v[1] = k[1] ^ V1;
    v[2] = k[0] ^ V2;
    v[3] = k[1] ^ V3;
       
    // Découpage en Mots de 8 octets + ajout de l'entete
    
    uint64_t * m2 = (uint64_t*)calloc(mlen+8,sizeof(uint64_t)); 
    unsigned p = mlen / 8 ; // nombre de paquets
    unsigned r = mlen % 8 ; // taille du reste
    
    memcpy(m2, m, mlen) ;
	
    // ajout de la longueur sans reste
    
    if(!r){
        uint64_t tmp = 0;
        tmp = ((uint64_t)mlen % MOD)<<56 ;
        m2[p]=tmp;
    }else{
        // ajout de la longueur avec reste   	
        uint64_t tmp ;
        memcpy(&tmp, ((uint64_t *) m) + p, r) ;
    	tmp |= ((uint64_t)mlen % MOD)<<56 ;
    	m2[p] = tmp ;   	
    }
    p++ ;
    
    for(unsigned i = 0; i < p ;i++){
      v[3] ^= m2[i];
      sipRound(v,2);
      v[0] ^= m2[i];
    }
    free(m2) ;
    v[2] ^= 0xFF;
    sipRound(v,4);
    return v[0]^v[1]^v[2]^v[3] ;
}

void SipHash::test_siphash_2_4() {
    
    uint64_t k[2] = {0x0706050403020100,0x0f0e0d0c0b0a0908} ;
    uint64_t k2[2] = {0x0,0x0} ;
    uint8_t m[15] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe} ;
    uint8_t m2[8] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7} ;
    cout << endl ;
    cout << "clé k :" << endl ;
    cout << "k[0] = 0x" << hex << k[0] << endl ;
    cout << "k[1] = 0x" << k[1] << endl ;
    cout << endl ;
    cout << "clé k2 :" << endl ;
    cout << "k2[0] = 0x" << k2[0] << endl ;
    cout << "k2[1] = 0x" << k2[1] << endl ;
    cout << endl ;
    cout << "message m  = " ;
    for (unsigned i = 0 ; i < 15 ; i++) cout << unsigned(m[i]) ;
    cout << endl ;
    cout << "message m2 = ";
    for (unsigned i = 0 ; i < 8 ; i++) cout << unsigned(m2[i]) ;
    cout << endl << endl ;
    
    uint64_t res = siphash_2_4(k,m,15) ;
    cout << "resultat pour k et m          : 0x" << res << endl ;
    
    res = siphash_2_4(k,m2,8) ;
    cout << "resultat pour k et m2         : 0x" << res << endl ;
    
    res = siphash_2_4(k,NULL,0) ;
    cout << "resultat pour k et \"NULL\"     : 0x" << res << endl ;
    
    res = siphash_2_4(k2,NULL,0) ;
    cout << "resultat pour k2 et \"NULL\"    : 0x" << res << endl ;
    
    uint32_t message = 0xabababab ;
    res = siphash_2_4(k2,(uint8_t*)&message,4) ;
    cout << "resultat pour k et 0xabababab : 0x" << res << endl ;
}

uint32_t SearchColl::sip_hash_fix32(uint32_t k, uint32_t m){
    uint64_t knew[2] = {0};
    for (unsigned i = 0; i < sizeof(uint32_t) ; i++) {
	*((uint32_t*) knew + i) = *((uint8_t*) &k + i) ;
    }
    uint64_t hash = SipHash::siphash_2_4(knew,(uint8_t *)&m, 4);
    return hash ;
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
    //FILE * f = fopen("collisions3.txt", "w") ;
    srand(time(NULL)) ;
    for (; num < NBTEST ; num++) {
        ensemble[num].reserve(1 << 16) ;
        fprintf(stdout, "%u\n", SearchColl::coll_search(rand(), sip_hash_fix32)) ;
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
    /*cout << dec << (1U << 16) << endl ;
    cout << dec << (1U << 17) << endl ;
    cout << dec << ~0U << endl ;
    uint32_t  k = 0x12345678 ;
    uint64_t knew[2] = {0};
    for (unsigned i = 0; i < sizeof(uint32_t) ; i++) {
	*((uint32_t*) knew + i) = *((uint8_t*) &k + i) ;
    }
    printf("0x%016llx%016llx\n", knew[0], knew[1]) ;
    bool b = knew[0] ;
    cout << boolalpha << b << endl ;
    for (unsigned i = 0 ; i < 8 ; i++)
        printf("%02x", unsigned(*((uint8_t *)knew + i))) ;
    cout << endl ;
    uint64_t m[] = {V0,V1,V2,V3} ;
    for (int i = 0; i < 4; i++) {
        m[i] = reverse_endianess(m[i]) ;
    }

    for (unsigned i = 0 ; i < 8*4 ; i++)
        cout << ((char *)&m)[i] ;
    cout << endl ;*/
    return 0 ;
}



