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

    //Create file pointer
    FILE* f;

    //Open file for reading (1st command line argument)
    f = fopen(argv[1], "r");

    //Loop until end of file
    while(!feof(f)){

    //Read in bits into 64 bits
    nobytes = fread(M.e, 1, 64, f);
    
    //Display how many bits are read in
    printf("%llu\n", nobytes);
    }

    //Close file
    fclose(f);

    return 0;
}