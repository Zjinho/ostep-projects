#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME_LENGTH (100)

typedef struct node {
    char* value;
    struct node* next;
}node_t;

static node_t* create_list();
static void insert_node(node_t* head, const char* line_buffer);
static void free_list(node_t* head);
static void print_list(node_t* head, FILE *stream);
static void parse_arg(const char* arg, char* filename);

int main(int argc, char * argv[]) {

    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    char* line_buffer = NULL;
    size_t len = 0;
    node_t* list_head = create_list();

    if (argc == 1) {
        while (getline(&line_buffer, &len, stdin) != -1) {
            insert_node(list_head, line_buffer);
        }
        print_list(list_head, stdout);
        exit(0);
    }

    FILE * input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
        exit(1);
    }

    while (getline(&line_buffer, &len, input_file) != -1) {
        insert_node(list_head, line_buffer);
    }

    if (argc == 2) {
        print_list(list_head, stdout);
    }

    if (argc == 3) {
        FILE * output_file = fopen(argv[2], "w");
        if (output_file == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
            exit(1);
        }

        // check if input is the same as output 
        char input_file_name[MAX_FILENAME_LENGTH] = {'\0'};
        char output_file_name[MAX_FILENAME_LENGTH] = {'\0'};
        parse_arg(argv[1], input_file_name);
        parse_arg(argv[2], output_file_name);
        if (strcmp(output_file_name, input_file_name) == 0) {
            fprintf(stderr, "reverse: input and output file must differ\n");
            exit(1);
        }

        print_list(list_head, output_file);

        fclose(output_file);
    }

    free_list(list_head);
    free(line_buffer);
    fclose(input_file);    
    return 0;
}

static node_t* create_list() {
    node_t* head = malloc(sizeof(node_t));
    if (head == NULL) {
        perror("malloc failed\n");
        exit(1);
    }
    head->next = NULL;
    return head;
}

static void insert_node(node_t* head, const char* line_buffer) {
    node_t * new_node = malloc(sizeof(node_t));
    if (new_node == NULL) {
        perror("malloc failed");
        exit(1);
    }

    new_node->value = malloc(sizeof(line_buffer));
    if (new_node->value == NULL) {
        perror("malloc failed");
        exit(1);
    }
    strcpy(new_node->value, line_buffer);

    new_node->next = head->next;
    head->next = new_node;
}

static void print_list(node_t* head, FILE *stream) {
    node_t* ptr = head->next;
    while(ptr != NULL) {
        fprintf(stream, "%s", ptr->value);
        ptr = ptr->next;
    }
}

static void free_list (node_t *head) {
    node_t* temp = head->next;
    while (head->next != NULL) {
        free(head->value);
        free (head);
        head = temp;
        temp = temp->next;
    }
    free(head->value);
    free(head);
}

static void parse_arg(const char* arg, char* filename) {
    int arg_len = strlen(arg);
    int filename_index = 0;
    int slash_num = 0;

    for (int i = 0; i < arg_len; i++) {
        if (arg[i] == '/') {
            slash_num++;
        }
    }

    for (int i = 0; i < arg_len; i++) {
        if (arg[i] == '/') {
            slash_num--;
        }
        if (slash_num == 0) {
            filename[filename_index] = arg[i];
            filename_index++;
        }
    }
    filename[filename_index] = '\0';
}