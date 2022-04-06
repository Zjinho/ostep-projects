#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE (1024)

int main(int argc, char * argv[]) {
    char buffer[BUFFER_SIZE];
    
    // passed no command-line arguments
    if (argc == 1) {
        printf("wgrep: searchterm [file ...]\n");
        exit(1);
    }

    // a search term, but no file, read from stdin
    if (argc == 2) {
        while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
            if (strstr(buffer, argv[1]) != NULL) {
                printf("%s", buffer);
            }
        }
    }

    if (argc > 2) {
        for (int i = 2; i < argc; i++) {
            FILE* fp = fopen(argv[i], "r");
            if (fp == NULL) {
                printf("wgrep: cannot open file\n");
                exit(1);
            }

            while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
                if (strstr(buffer, argv[1]) != NULL) {
                    printf("%s", buffer);
                }
            }
            fclose(fp);
        }
    }
    return 0;
}