/*
|--------------------------------------|
|                                      |
|--------------------------------------|
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NEWLINE printf("\n")
#define Bool int
#define true 1
#define false 0
#define UNUSED(x) (void)(x)

typedef struct label {
    char *key;
    int address;
    struct label *next;
} label;

label *search(label *L, const char *key) {
    while (L != NULL) {
        if (strcmp(L->key, key) == 0) {
            return L;
        }
        L = L->next;
    }
    return NULL;
}

void insert(char *key, int addrs, label **head) {
    label *new_label = malloc(sizeof(label));
    new_label->address = addrs;
    new_label->key = malloc(strlen(key) + 1);
    strcpy(new_label->key, key);
    new_label->next = *head;
    *head = new_label;
}

char *right_trim(char *str, int n) {
    /* trim the right side of the string upto n characters */
    char *back = str + strlen(str);
    while (n--) {
        back--;
        *back = '\0';
    }

    *back = '\0';
    return str;
}

Bool has_char(char *str) {
    /* check if the string has ':' */
    char p[100];
    int i = 0;
    strcpy(p, str);
    while (p[i] != '\0') {
        if (p[i] == ':') {
            return true;
        }
        i++;
    }
    return false;
}

void append_line(char *str, int counter, FILE *fp, label **head) {
    /* write a line to the file */
    char *token;
    int value;
    char line[100];
    int instruction_code = 0x00000000;
    char instructions[23][7] = {"data", "ldc",    "adc",  "adj",  "SET", "ldl",
                                "stl",  "ldnl",   "stnl", "call", "brz", "brlz",
                                "br",   "add",    "sub",  "shl",  "shr", "a2sp",
                                "sp2a", "return", "HALT"};
    int instruction_opcode[23] = {-1,   0x00, 0x01, 0x0a, -1,   0x02, 0x03,
                                  0x04, 0x05, 0x0d, 0x0f, 0x10, 0x11, 0x06,
                                  0x07, 0x08, 0x09, 0x0b, 0x0c, 0x0e, 0x12};
    int i;
    int token_len = 0;
    int label_flag = 0;
    UNUSED(head);
    fprintf(fp, "%04X ", counter); /*writing address in .l file*/
    strcpy(line, str);
    token = strtok(line, " "); /* get the first token */
    do {
        token_len = strlen(token);
        label_flag = 0;
        for (i = 0; i < token_len; i++) {
            if (token[i] == ':') {
                label_flag = 1;
            }
        }
    } while (label_flag && (token = strtok(NULL, " ")) != NULL);

    if (token) {
        for (i = 0; i < 23; i++) {
            if (strncmp(token, instructions[i], strlen(instructions[i])) == 0) {
                instruction_code += instruction_opcode[i];
                if (i == 0) {
                    value = atoi(strtok(NULL, " "));
                    instruction_code = value;
                    break;
                } else if (i > 0 && i < 13) {
                    value = atoi(strtok(NULL, " "));
                    value *= 0x00000100;
                    instruction_code += value;
                    break;
                }
            }
        }
        fprintf(fp, "%08X ", instruction_code);
    } else if (label_flag) {
        fprintf(fp, "         ");
    }

    fprintf(fp, "%s\n", str);
}

void extract_label(char *str, int counter, FILE *fp, label **head) {
    char *token;
    char line[100];
    UNUSED(fp);
    strcpy(line, str);
    if (has_char(line)) {
        token = strtok(line, ":");
        insert(token, counter, head);
    }
    while (has_char(line)) {
        token = strtok(NULL, ":");
        insert(token, counter, head);
    }
}

void read_file( FILE *fp, FILE *lfile,
               void (*f)(char *str, int counter, FILE *fp, label **label_list),
               label **head) {
    int line_counter = 0x0000;
    int char_in_line = 0;    /* char_in_line --> helps to ignore empty lines */
    int line_start_flag = 1; /* line_start_flag --> helps to ignore spaces at
                                the start of the line */
    int comment_flag = 0;    /* comment_flag --> helps to ignore comments, */
    int space_flag = 0; /* space_flag --> helps to ignore multiple spaces */
    int label_flag =
        0; /* label_flag --> helps to not incrementing the code address */
    char temp_line[100] = "";
    char c;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            if (char_in_line != 0) {
                (*f)(temp_line, line_counter, lfile, head);
                temp_line[0] = '\0';
                line_start_flag = 1;
                space_flag = 0;
                if (label_flag == 0) {
                    line_counter++;
                }
            }
            comment_flag = 0;
            char_in_line = 0;
        } else if (comment_flag == 1) {
        } /* ignore comments */
        else if (c == ';') {
            comment_flag = 1;
        } else {
            char_in_line++;
            if (c == '\t') {
                c = ' ';
            } /* treating tabs as space */
            if (c == ' ') {
                if (line_start_flag || space_flag > 1) {
                    char_in_line--;
                } else {
                    strncat(temp_line, &c, 1);
                    space_flag++;
                }
            } else {
                strncat(temp_line, &c, 1);
                line_start_flag = 0;
                space_flag = 0;
            }

            if (c == ':') {
                label_flag = 1;
            } else if (c != ' ' && c != '\t' && label_flag == 1) {
                label_flag = 0;
                space_flag = 0;
            }
        }
    }
    if (c == EOF) {
        if (char_in_line != 0) {
            /* printf("%s\n", temp_line); */
            (*f)(temp_line, line_counter, lfile, head);
            temp_line[0] = '\0';
            line_start_flag = 1;
            space_flag = 0;
            if (label_flag == 0) {
                line_counter++;
            }
        }
        comment_flag = 0;
        char_in_line = 0;
    }
    UNUSED(head);
}

int main(int argc, char *argv[]) {
    FILE *fp = fopen(argv[1], "r"), *lfile; /* main file pointer */
    char *filename = argv[1];
    label *L = ((void *)0);
    if (argc < 2) {
        return 0;
    }
    filename = right_trim(filename, 3);
    strcat(filename, "l");
    lfile = fopen(filename, "w+");

    if (fp == NULL) {
        printf("Error opening file");
        return -1;
    }
    read_file(fp, lfile, extract_label, &L);
    fseek(fp, 0, SEEK_SET);
    read_file(fp, lfile, append_line, &L);
    label *iter = L;
    while (iter != NULL) {
        printf("%s --> %04X\n", iter->key, iter->address);
        iter = iter->next;
    }
    fclose(fp);
    fclose(lfile);
    return 0;
}
