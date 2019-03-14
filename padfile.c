//Conor Raftery G00274094

//Input/output header file
#include <stdio.h>
//Fixed bit length integers
#include <stdint.h>

int main(int argc, char *argv[]){
    
    FILE* f;

    char c;

    f = fopen(argv[1], 'r');

    printf("%C\n", fread(&c, 1, 1, f));

    return 0;
}