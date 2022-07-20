/*
 * Used for converting files into arrays (memory).
 * Usage: ./a.exe file > transfer.txt
 * 
 * Credit: https://gist.github.com/albertz/1551304
 * Slightly modified
 */

#include <stdio.h>
#include <assert.h>

int main(int argc, char** argv) {
    assert(argc == 2);
    char* fn = argv[1];
    FILE* f = fopen(fn, "rb");
    printf("const unsigned char a[] = {\n");
    unsigned long n = 0;
    while(!feof(f)) {
        unsigned char c;
        if(fread(&c, 1, 1, f) == 0) break;
        printf("0x%.2X, ", (int)c);
        ++n;
        if(n % 10 == 0) printf("\n");
    }
    fclose(f);
    printf("};\n");
    printf("const unsigned int a_len = %i;", n);
}