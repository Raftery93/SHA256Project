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

int main(int argc, char *argv[]){
    
    //Declare union
    union msgblock M;

    //Number of bytes
    uint64_t nobytes;
    uint64_t nobits = 0;

    //Create file pointer
    FILE* f;

    //Open file for reading (1st command line argument)
    f = fopen(argv[1], "r");

    //Loop until end of file
    while(!feof(f)){

        //Read in bits into 64 bits
        nobytes = fread(M.e, 1, 64, f);
        nobits = nobits + (nobytes + 8);
        if(nobytes < 56){
            printf("I've found a block with less than 55 bytes!\n");
            M.e[nobytes] = 0x80;
            while(nobytes < 56){
                nobytes = nobytes + 1;
                M.e[nobytes] = 0x00;
            }
            M.s[7] = nobits;
        }

    //Display how many bits are read in
    printf("%llu\n", nobytes);
    }

    //Close file
    fclose(f);

    return 0;
}