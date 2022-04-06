#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * argv[]) {
    if (argc == 1) {
        printf("wunzip: file1 [file2 ...]\n");
        exit(1);
    }

    int int_buff;
    char char_buff;
    for (int i = 1; i < argc; i++) {
        FILE* fp = fopen(argv[i], "r");

        if (fp == NULL) {
            printf("cannot open file\n");
            exit(1);
        }

        while (fread(&int_buff, 4, 1, fp) != 0) {
            fread(&char_buff, 1, 1, fp);
            for(int i = 0; i < int_buff; i++) {
                printf("%c",char_buff);
            } 
        }

        fclose(fp);
    }
    return 0;
}