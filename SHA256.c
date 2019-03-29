//Conor Raftery G00274094

//Input/output header file
#include <stdio.h>
//Fixed bit length integers
#include <stdint.h>

//Creates a union for storing certain size bits
union msgblock {
    uint8_t e[64];
    uint32_t t[16];
    uint64_t s[8];
};

enum status {READ, PAD0, PAD1, FINISH};

void sha256();

uint32_t sig0(uint32_t x);
uint32_t sig1(uint32_t x);

uint32_t rotr(uint32_t n, uint32_t x);
uint32_t shr(uint32_t n, uint32_t x);

uint32_t SIG0(uint32_t x);
uint32_t SIG1(uint32_t x);

uint32_t Ch(uint32_t x, uint32_t y, uint32_t z);
uint32_t Maj(uint32_t x, uint32_t y, uint32_t z);