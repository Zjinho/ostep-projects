#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_SIZE (3)
#define VALUE_SIZE (10)

typedef struct node {
    int key;
    char value[10];
    struct node* next;
}node_t;

typedef struct list {
    node_t* head;
    node_t* tail;
}list_t;

static void parse_line(const char *line, char * key, char * value);
static void parse_arg(const char* arg, char* op, char* key, char* value);
static node_t* create_list();
static void print_list(node_t* head, FILE *stream);
static void insert_node(list_t* data_list, int key, const char *value);
static void free_list (node_t *head);
static node_t* search_key (node_t* head, int key);
static int delete_node (list_t* date_list, int key);
static void create_pair(char* key, char *value, char* pair);

int main(int argc, char* argv[]) {

    // no arguments
    if (argc == 1) {
        exit(0);
    }

    // create linked list
    list_t data_list = {
        .head = NULL, .tail = NULL
    };
    // read from database.txt
    FILE* fp = fopen("database.txt", "r");
    char* buffer = NULL;
    size_t len = 0;
    data_list.head = create_list();
    data_list.tail = data_list.head;
    while (getline(&buffer, &len, fp) != -1) {
        char key_char[KEY_SIZE] = {'\0'};
        int key = 0;
        char value[VALUE_SIZE] = {'\0'};
        parse_line(buffer, key_char, value);
        key = atoi(key_char);
        insert_node(&data_list, key, value);
    }
    fclose(fp);
    free(buffer);

    char* argument = argv[1];
    char op = '\0';
    char key_char[KEY_SIZE] = {'\0'};
    int key = 0;
    char value[VALUE_SIZE] = {'\0'};
    char pair[KEY_SIZE + VALUE_SIZE] = {'\0'};
    parse_arg(argument, &op, key_char, value);
    key = atoi(key_char);

    if (op == 'p') {
        insert_node(&data_list, key, value);
        create_pair(key_char, value, pair);

        // write back the data
        FILE* fp = fopen("database.txt", "w");
        print_list(data_list.head, fp);
        fclose(fp);
    }
    else if (op == 'g') {
        node_t* prev = search_key(data_list.head, key);
        if (prev == NULL) {
            printf("%d not found\n", key);
        }
        else {
            printf("%d,%s\n", key, prev->next->value);
        }
    }
    else if (op == 'd') {
        delete_node(&data_list, key);

        // write back the data
        FILE* fp = fopen("database.txt", "w");
        print_list(data_list.head, fp);
        fclose(fp);
    }
    else if (op == 'c') {
        // write back the data
        FILE* fp = fopen("database.txt", "w");
        fclose(fp);
    }
    else if (op == 'a') {
        print_list(data_list.head, stdout);
    }
    else {
        printf("Bad command\n");
    }

    free_list(data_list.head);
    return 0;
}

static void parse_arg(const char* arg, char* op, char* key, char* value) {
    int arg_len = strlen(arg);
    int state = 0; // state 0--op, 1--key, 2--value
    int key_index = 0;
    int value_index = 0;
    for (int i = 0; i < arg_len; i++) {
        if (arg[i] == ',' || arg[i] == '\n') {
            state ++;
            continue;
        }
        if (state == 0) {
            *op = arg[i];
        }
        else if (state == 1 ) {
            key[key_index] = arg[i];
            key_index++;
        }
        else if(state == 2) {
            value[value_index] = arg[i];
            value_index++;
        }
    }
}

static void parse_line(const char *line, char * key, char * value) {
    int line_len = strlen(line);
    int state = 0; // state 0--key, 1--value
    int key_index = 0;
    int value_index = 0;
    for (int i = 0; i < line_len; i++) {
        if (line[i] == ',' || line[i] == '\n') {
            state ++;
            continue;
        }
        
        if (state == 0 ) {
            key[key_index] = line[i];
            key_index++;
        }
        else if(state == 1) {
            value[value_index] = line[i];
            value_index++;
        }
    }
    key[key_index] = '\0';
    value[value_index] = '\0';
}    

static node_t* create_list() {
    node_t* head = malloc(sizeof(node_t));
    if (head == NULL) {
        perror("malloc failed\n");
        exit(1);
    }
    head->key = 0;
    head->next = NULL;
    return head;
}

static void print_list(node_t* head, FILE *stream) {
    node_t* ptr = head->next;
    while(ptr != NULL) {
        fprintf(stream,"%d,%s\n", ptr->key, ptr->value);
        ptr = ptr->next;
    }
}

static void insert_node(list_t* data_list, int key, const char *value) {

    node_t* key_prev_node = search_key(data_list->head, key);
    if (key_prev_node != NULL) {
        strcpy(key_prev_node->next->value, value);
    }
    else{
        node_t* new_node = malloc(sizeof(node_t));
        if (new_node == NULL) {
            perror("malloc failed\n");
            exit(1);
        }
        new_node->key = key;
        strcpy(new_node->value, value);
        new_node->next = NULL;

        data_list->tail->next = new_node;    
        data_list->tail = new_node;
    }
}

static void free_list (node_t *head) {
    node_t* ptr = head->next;
    node_t* temp;
    while (ptr != NULL) {
        temp = ptr->next;
        free (ptr);
        ptr = temp;
    }
    free(head);
}

static node_t* search_key (node_t* head, int key) {
    node_t* prev = head;
    node_t* ptr = prev->next;
    while (ptr!= NULL) {
        if (ptr->key == key) {
            break;
        }
        prev = ptr;
        ptr = ptr->next;
    }    
    if (prev->next == NULL) {
        return NULL;
    }
    return prev;
}

static int delete_node (list_t *date_list, int key) { 
    node_t* prev = search_key(date_list->head, key);
    if(prev != NULL) {
        node_t* del_node = prev->next;
        if (del_node->next != NULL) {
            prev->next = del_node->next;
        } else{
            date_list->tail = prev;
            prev->next = NULL;
        }
        free(del_node);
        return 1;
    }
    return 0;
}

static void create_pair(char* key, char *value, char* pair) {
    int i = 0;
    for ( int j = 0; key[j] != '\0'; j++) {
        pair[i++] = key[j];
    }
    pair[i++] = ',';
    for ( int j = 0; value[j] != '\0'; j++) {
        pair[i++] = value[j];
    }
}