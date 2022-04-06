#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * argv[]) {
    if (argc == 1) {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }

    char* buffer = NULL;
    size_t len = 0;
    char last_char = '\0';
    int last_char_count = 0;

    for (int i = 1; i < argc; i++) {

        FILE* fp = fopen(argv[i], "r");
        if (fp == NULL) {
            printf("wzip: cannot open file\n");
            exit(1);
        }
        
        while (getline(&buffer, &len, fp) != -1) {
            int buff_len = strlen(buffer);
            int j = 0;

            int count_last = 0;
            while (buffer[j] == last_char && j < buff_len) {
                count_last = 1;
                last_char_count++;
                j++;     
            }

            if ((buffer[0] != last_char && last_char != '\0')||
                (count_last && j < buff_len)) {
                fwrite(&last_char_count, 4, 1, stdout);
                fwrite(&last_char, 1, 1, stdout);
                last_char_count = 0;  
            }
            
            last_char = buffer[buff_len - 1];

            while (buffer[j] != last_char && j < buff_len) {
                int count = 0;
                char c = buffer[j];
                while (buffer[j] == c) {
                    count++;
                    j++;
                }
                fwrite(&count, 4, 1, stdout);
                fwrite(&c, 1, 1, stdout);
            }

            while (buffer[j] == last_char && j < buff_len) {
                last_char_count++;
                j++;
            }
        }
        fclose(fp);
    }
    fwrite(&last_char_count, 4, 1, stdout);
    fwrite(&last_char, 1, 1, stdout);
    free(buffer);
    return 0;
}