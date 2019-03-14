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

int main(int argc, char *argv[]){
    
    //Declare union
    union msgblock M;

    //Number of bytes
    uint64_t nobytes;
    uint64_t nobits = 0;

    enum status S = READ;

    //Create file pointer
    FILE* f;

    //Open file for reading (1st command line argument)
    f = fopen(argv[1], "r");

    //Loop until end of file
    while(S == READ){

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
            S = FINISH;
        } else if (nobytes < 64){
            S = PAD0;
            M.e[nobytes] = 0x80;
            while(nobytes < 64){
                nobytes = nobytes + 1;
                M.e[nobytes] = 0x00;
            }
        } else if (feof(f)){
            S = PAD1;
        }
    }

    if(S == READ || S == PAD1){
        for(int i = 0; i < 56; i++){
            M.e[i] = 0x00;
            M.s[7] = nobits;
        }
    }
    
    if (S == PAD1){
        M.e[0] = 0x80;
    }

    //Close file
    fclose(f);

    //Output results
    for(int i = 0; i < 64; i++){
        printf("%x ", M.e[i]);
        printf("\n");
    }

    return 0;
}