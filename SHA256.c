//Conor Raftery G00274094
//References:
//              Documentation: https://www.nist.gov/publications/secure-hash-standard
//              Video Demonstrations: https://web.microsoftstream.com/user/20b32719-41e8-4560-9f7f-c83ba751229c

//Input/output header file
#include <stdio.h>
//Fixed bit length integers
#include <stdint.h>

//Defines a method for checking if the file is Big Endian
#define IS_BIG_ENDIAN (!*(unsigned char *)&(uint16_t){1})

//Method for swapping little to big endian
#define SWAP_UINT64(x) \
        ( (((x) >> 56) & 0x00000000000000FF) | (((x) >> 40) & 0x000000000000FF00) | \
          (((x) >> 24) & 0x0000000000FF0000) | (((x) >>  8) & 0x00000000FF000000) | \
          (((x) <<  8) & 0x000000FF00000000) | (((x) << 24) & 0x0000FF0000000000) | \
          (((x) << 40) & 0x00FF000000000000) | (((x) << 56) & 0xFF00000000000000) )

//Creates a union for storing certain size bits
union msgblock {
    uint8_t e[64];
    uint32_t t[32];
    uint64_t s[8];
};

//Used to control the state of the program
enum status {READ, PAD0, PAD1, FINISH};

//Method stubs
uint32_t sig0(uint32_t x);
uint32_t sig1(uint32_t x);

uint32_t rotr(uint32_t a, uint32_t b);
uint32_t shr(uint32_t n, uint32_t x);

uint32_t SIG0(uint32_t x);
uint32_t SIG1(uint32_t x);

uint32_t Ch(uint32_t x, uint32_t y, uint32_t z);
uint32_t Maj(uint32_t x, uint32_t y, uint32_t z);

void sha256(FILE *f);

int nextmsgblock(FILE *f, union msgblock *M, enum status *S, uint64_t *nobits);

int BigEndianToLittle(int x);

int LittleEndianToBig(int x);

//Main method for running program
int main(int argc, char *argv[]){

    FILE* msgf;

    //Error check to see if file can be read
    if((msgf = fopen(argv[1],"r"))!=NULL){

        //Pass file into sha256 function
        sha256(msgf);

    }else{
        //Output error
        printf("Error opening the selected file!\n");
    }

    //Close file
    fclose(msgf);

    return 0;

}

//This method does the most of the 'heavy lifting', it takes in the file and hashes it using the sha256
// hash function by splitting the file into message blocks and rearranging the bits.
void sha256(FILE *msgf){

    //The current message block
    union msgblock M;

    //The number of bits read from the file
    uint64_t nobits = 0;

    //status of message block
    enum status S = READ;

    //Used as boolean for if the file is big/little endian
    int isBig = 0;
    
    //The K constants defined in the documentation
    uint32_t K[] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
        0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
        0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
        0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
        0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
        0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
        0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
        0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
        0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

    //For looping
    int t, i;

    //Message schedule
    uint32_t W[64];

    //Working variables
    uint32_t a, b, c, d, e, f, g, h;

    //Two temporary variables
    uint32_t T1, T2;

    //The hash value taken from the documentation
    uint32_t H[8] = {
        0x6a09e667,
        0xbb67ae85,
        0x3c6ef372,
        0xa54ff53a,
        0x510e527f,
        0x9b05688c,
        0x1f83d9ab,
        0x5be0cd19,
    };

    //While there is a next message block
    while(nextmsgblock(msgf, &M, &S, &nobits)){

        //Loop through the message block and add the current message block to our message schedule
        for(t = 0; t < 16; t++){
            //If the file is big endian
            if(IS_BIG_ENDIAN){
                //Calculation taken from documentation
                W[t] = M.t[t];
                //Set is big endian to true
                isBig = 1;
            }else{
                //Else convert from little endian to big endian
                W[t] = BigEndianToLittle(M.t[t]) ;
            }
        }

        //Loop taken from documentation, along with calculation
        for(t = 16; t < 64; t++){
            //Calculation taken from documentation
            W[t] = sig1(W[t - 2]) + W[t - 7] + sig0(W[t - 15]) + W[t - 16];
        }
        
        //Initialize variables
        a = H[0];
        b = H[1];
        c = H[2];
        d = H[3];
        e = H[4];
        f = H[5];
        g = H[6];
        h = H[7];

        //Loop taken from documentation, along with calculation
        for(t = 0; t < 64; t++){
            //Creating variables by storing them temporarily by using functions taken from the documentation (SIG1, SIG0, Ch, Maj)
            T1 = h + SIG1(e) + Ch(e, f, g) + K[t] + W[t];
            T2 = SIG0(a) + Maj(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
        }

        //Step 4, taken from video demonstration
        H[0] = a + H[0];
        H[1] = b + H[1];
        H[2] = c + H[2];
        H[3] = d + H[3];
        H[4] = e + H[4];
        H[5] = f + H[5];
        H[6] = g + H[6];
        H[7] = h + H[7];

    }

    //If file is big endian
    if(isBig == 1){

        printf("Big Endian:\t ");

        //Output hash of the file in big endian converted to little endian
        for(int j = 0; j < 8; j++){
            printf("%08x ",BigEndianToLittle(H[j]));
        }
    }else{
        //Output hash of file as it is (little endian)
        printf("Little Endian:\t %08x %08x %08x %08x %08x %08x %08x %08x\n", H[0], H[1], H[2], H[3], H[4], H[5], H[6], H[7]);
    }
}

//Methods rotr, shr, sig0, sig1, SIG0, SIG1, Ch, Maj adapted from documentation and video demonstrations
uint32_t rotr(uint32_t a, uint32_t b){
    return (((a) >> (b)) | ((a) << (32-(b))));
}

uint32_t shr(uint32_t n, uint32_t x){
    return (x >> n);
}

uint32_t sig0(uint32_t x){
    return (rotr(x, 7) ^ rotr(x, 18) ^ ((x) >> 3));
}

uint32_t sig1(uint32_t x){
    return (rotr(x,17) ^ rotr(x,19) ^ ((x) >> 10));
}

uint32_t SIG0(uint32_t x){
    return  (rotr(x,2) ^ rotr(x,13) ^ rotr(x,22));
}

uint32_t SIG1(uint32_t x){
    return (rotr(x,6) ^ rotr(x,11) ^ rotr(x,25));

}

uint32_t Ch(uint32_t x, uint32_t y, uint32_t z){
    return (((x) & (y)) ^ (~(x) & (z)));
}

uint32_t Maj(uint32_t x, uint32_t y, uint32_t z){
    return (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)));
}

//This method is used to handle to opening and reading of the file / message block
int nextmsgblock(FILE *msgf, union msgblock *M, enum status *S, uint64_t *nobits){

    //No of bytes left/yet to read
    uint64_t nobytes;
    int i;

     // If the padding is finished and processing all the message blocks is complete, exit the program
    if(*S == FINISH){
        return 0;
    }

    //PAD0 and PAD1 checks if we need another block for padding
    if(*S == PAD0 || *S == PAD1){
        for(i = 0; i < 56; i++){
            M->e[i] = 0x00;
        }

        //Set the last 64 bits to an integer (needs to be big endian)
        M->s[7] = SWAP_UINT64(*nobits);

        //Set state to finish
        *S = FINISH;

        //Set first bit of message block to '1'
        if (*S == PAD1){
            M->e[0] = 0x80;
        }

        //Keep while loop in the sha256 function looping for one more iteration
        return 1;
    }
    
    //Read bytes instead of characters and read until the end of the file
    nobytes = fread(M->e, 1, 64, msgf);

    //Keep track of the number of bytes that have been read
    *nobits = *nobits + (nobytes * 8);

    //If there is enough space for the padding
    if(nobytes < 56){

        //Add the one bit
        M->e[nobytes] = 0x80;

        //Add no bits until the last 64 bits
        while(nobytes < 56){
            nobytes = nobytes + 1;

            //Add multiple '0' so there is 64 bits left at the end
            M->e[nobytes] = 0x00;
        }

        //Store the length of the file in bits as an unsigned 64 bit integer, needs to be big endian
        M->s[7] = SWAP_UINT64(*nobits);

        //Set state of the program to 'Finish'
        *S = FINISH;

        //Check if there is enought room for padding in the current message block
    } else if (nobytes < 64){

        //Set state of program
        *S = PAD0;

        //Add the one bit
        M->e[nobytes] = 0x80;

        //Pad the rest of the message block with multiple '0'
        while(nobytes < 64){
            nobytes = nobytes + 1;
            M->e[nobytes] = 0x00;
        }
    
    //If at the end of the file, create new message block full of padding
    } else if (feof(msgf)){
        *S = PAD1;
    }

    return 1;
}


//Method used to convert big endian to little endian by shifting bits
int BigEndianToLittle(int x)
{
	return (((x>>24) & 0x000000ff) | ((x>>8) & 0x0000ff00) | ((x<<8) & 0x00ff0000) | ((x<<24) & 0xff000000));
}