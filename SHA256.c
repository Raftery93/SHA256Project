//Input/output header file
#include <stdio.h>
//Fixed bit length integers
#include <stdint.h>


void sha256();

uint32_t sig0(uint32_t x);
uint32_t sig1(uint32_t x);

uint32_t rotr(uint32_t n, uint32_t x);
uint32_t shr(uint32_t n, uint32_t x);

int main(int argc, char *argv[]){
    
    sha256();

    return 0;
}


void sha256(){

    int t;

    //Message schedule
    uint32_t W[64];
    //Working variables
    uint32_t a, b, c, d, e, f, g, h;
    //The hash value
    uint32_t H[8];
    //Two temporary variables
    uint32_t T1, T2;

    
    uint32_t h[8] = {
        0x6a09e667,
        0xbb67ae85,
        0x3c6ef372,
        0xa54ff53a,
        0x510e527f,
        0x9b05688c,
        0x1f83d9ab,
        0x5be0cd19,
    };

    //The current message block
    uint32_t M[16];

    for(t = 0; t<16;t++){
        W[t] = M[t];
    } 

    for(t = 16; t<64;t++){
        sig1(W[t-2]) + W[t-7] + sig0(W[t-15]) + W[t-16];
    }

    //Step 2
    //Initialize a, b, c, d, e, g, h
    a = H[0];
    b = H[1];
    c = H[2];
    d = H[3];
    e = H[4];
    f = H[5];
    g = H[6];
    h = H[7];

    
}