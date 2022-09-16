/*****************************************************************************
TITLE: Claims																																
AUTHOR:  Rupak Biswas
ROLL NO: 2001CS57
EMAIL ID: rupak_2001cs57@iitp.ac.in
Declaration of Authorship
This txt file, claims.txt, is part of the assignment of CS322 at the 
department of Computer Science and Engg, IIT Patna . 
*****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NEWLINE printf("\n")
#define Bool int
#define true 1
#define false 0
#define UNUSED(x) (void)(x)
#define and &&
#define or ||

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

Bool isvalidNumber(char *str){
    /* check if the string is a valid number */
    char p[100];
    int i = 0;
    strcpy(p, str);
    while (p[i] != '\0') {
        if (p[i] == '0'|| p[i]== '-' || p[i] == '1' || p[i] == '2' || p[i] == '3' || p[i] == '4' || p[i] == '5' || p[i] == '6' || p[i] == '7' || p[i] == '8' || p[i] == '9') {
            i++;
        }
        else{
            return false;
        }
    }
    return true;
}

Bool isvalidHexnumber(char *str){
    /* checks if the string id a valid hex number */
    char p[100];
    int i = 2;
    strcpy(p, str);
    if (p[0] == '0' && p[1] == 'x') {
        while (p[i] != '\0') {
            if (p[i] == '0' || p[i] == '1' || p[i] == '2' || p[i] == '3' || p[i] == '4' || p[i] == '5' || p[i] == '6' || p[i] == '7' || p[i] == '8' || p[i] == '9' || p[i] == 'a' || p[i] == 'b' || p[i] == 'c' || p[i] == 'd' || p[i] == 'e' || p[i] == 'f' || p[i] == 'A' || p[i] == 'B' || p[i] == 'C' || p[i] == 'D' || p[i] == 'E' || p[i] == 'F') {
                i++;
            }
            else{
                return false;
            }
        }
        if (i<=8){
            return true;
        }
        return false;
    }
    return false;
}

Bool containsSET(char *str){
    /* checks if the string contains SET */
    char p[100];
    int i = 0;
    strcpy(p, str);
    while (p[i] != '\0') {
        if (p[i] == 'S' && p[i+1] == 'E' && p[i+2] == 'T') {
            return true;
        }
        i++;
    }
    return false;
}


/* Function for appending line ( address | istruction | code ) in list file and writing to object file */
void append_line(char *str, int counter, FILE *fp, FILE *logfile, FILE *ofile, label **head) {
    char *token;
    int found = 0;
    int value;
    char line[100];
    int instruction_code = 0x00000000;
    char instructions[21][7] = {"data", "ldc",    "adc",  "adj",  "SET", "ldl",
                                "stl",  "ldnl",   "stnl", "call", "brz", "brlz",
                                "br",   "add",    "sub",  "shl",  "shr", "a2sp",
                                "sp2a", "return", "HALT"};
    int instruction_opcode[21] = {-1,   0x00, 0x01, 0x0a, -1,   0x02, 0x03,
                                  0x04, 0x05, 0x0d, 0x0f, 0x10, 0x11, 0x06,
                                  0x07, 0x08, 0x09, 0x0b, 0x0c, 0x0e, 0x12};
    int i;
    int token_len = 0;
    int label_flag = 0;
	label *l = malloc(sizeof(label));
	
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
        for (i = 0; i < 21; i++) {
            if (strncmp(token, instructions[i], strlen(instructions[i])) == 0) {
                instruction_code += instruction_opcode[i];
                found++;
                if (i == 0) {
                    value = atoi(strtok(NULL, " "));
                    instruction_code = value;
                    break;
                }else if (i == 1 || i == 2) {
					token = strtok(NULL, " ");
                    if (token == NULL){
                        fprintf(logfile, "ERROR: Missing operand for instruction %s at address %04X\n", instructions[i], counter);
                        printf("ERROR: Missing operand for instruction %s at address %04X\n", instructions[i], counter);
                        break;
                    }
					l = search(*head, token);
					if (l != NULL) {
						value = l->address;
					}else {
                        if (!isvalidNumber(token) && !isvalidHexnumber(token)){
                            fprintf(logfile, "ERROR: Invalid number %s at address %04X\n", token, counter);
                            printf("ERROR: Invalid number %s at address %04X\n", token, counter);
                            break;
                        }
                        value = strtol(token, NULL, 16);
					}
					value *= 0x00000100;
					instruction_code += value;
					break;
				}else if (i == 4){
                    break;
                }else if (i >= 5 && i < 13 ){
					token = strtok(NULL, " ");
                    if (token == NULL){
                        fprintf(logfile, "ERROR: Missing operand for instruction %s at address %04X\n", instructions[i], counter);
                        printf("ERROR: Missing operand for instruction %s at address %04X\n", instructions[i], counter);
                        break;
                    }
					l = search(*head, token);                
                    if (l == NULL){
                        if (!isvalidNumber(token) && !isvalidHexnumber(token)){
                            fprintf(logfile, "ERROR: Invalid label or offset %s at address %04X\n", token, counter);
                            printf("ERROR: Invalid label or offset %s at address %04X\n", token, counter);
                            break;
                        }
                        value = strtol(token, NULL, 16);
                    }
                    else { value = l->address - counter - 1; }
					value *= 0x00000100;
					instruction_code += value;
					break;
				}else if (i > 1 && i < 4) {
                    token = strtok(NULL, " ");
					l = search(*head, token);                
                    if (!isvalidHexnumber(token) && !isvalidNumber(token) && l == NULL){
                        fprintf(logfile, "ERROR: Invalid number %s at address %04X\n", instructions[i], counter);
                        printf("ERROR: Invalid number %s at address %04X\n", instructions[i], counter);
                        break;
                    }

                    if ( l != NULL) {
                        value = l->address;
                    }else {
                        value = strtol(token, NULL, 16);
                        value *= 0x00000100;
                        instruction_code += value;
                    }
                    break;
                }
            }
        }
        if (i == 4){
            fprintf(fp, "         ");
        }
        else{
            fprintf(fp, "%08X ", instruction_code);
            fwrite(&instruction_code, sizeof(int), 1, ofile);
        }
        if (!found) {
            fprintf(logfile, "ERROR: Invalid mnemonic %s at address %04X\n", token, counter);
            printf("ERROR: Invalid mnemonic %s at address %04X\n", token, counter);
        }
    } else if (label_flag) {
        fprintf(fp, "         ");
    }

    fprintf(fp, "%s\n", str);
}

void extract_label(char *str, int counter, FILE *fp,FILE *logfile,FILE *ofile, label **head) {
    char *token,*token2;
    char line[100], line2[100];
    int address;
	label *l = malloc(sizeof(label));
    UNUSED(fp);
    UNUSED(ofile);
    strcpy(line, str);
    strcpy(line2, str);
    if (has_char(line)) {
        token = strtok(line, ":");
		l = search(*head, token);
		if (l == NULL) {
            if (token[0] >= '0' && token[0] <= '9') {
                fprintf(logfile, "ERROR: Invalid label %s at address %04X\n", token, counter);
                printf("ERROR: Invalid label %s at address %04X\n", token, counter);
            }
       		insert(token, counter, head);
		}
		else{
			fprintf(logfile, "ERROR: Duplicate labels found for %s.\n", token);
			printf("ERROR: Duplicate labels found for %s.\n", token);
		}
    }
    if (containsSET(line2)){
        token2 = strtok(NULL, "SET ");
        if (token2){
            address = strtol(token2, NULL, 0);
            l = search(*head, token);
            l->address = address;
        }
    }
}

void read_file( FILE *fp, FILE *lfile, FILE *logfile, FILE *ofile,
               void (*f)(char *str, int counter, FILE *fp,FILE *logfile,FILE *ofile, label **label_list),
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
                (*f)(temp_line, line_counter, lfile, logfile,ofile,head);
                if (containsSET(temp_line)) {label_flag++;}
                temp_line[0] = '\0';
                line_start_flag = 1;
                space_flag = 0;
                if (label_flag == 0  ) {
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
                strcat(temp_line, " ");
                space_flag++;
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
            (*f)(temp_line, line_counter, lfile,logfile,ofile, head);
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
    FILE *fp = fopen(argv[1], "r"), *lfile,*logfile,*ofile; /* main file pointer */
    char *filename = argv[1];
    label *L = ((void *)0);
	char *log_file = malloc(sizeof(char) * 100);
    char *out_file = malloc(sizeof(char) * 100);
    if (fp == NULL) {
        printf("Error opening file");
        return -1;
    }
    if (argc < 2) {
        return 0;
    }
    filename = right_trim(filename, 3);
	strcpy(log_file, filename);
    strcpy(out_file, filename);
	strcat(log_file, "log");
    strcat(filename, "l");
    strcat(out_file, "o");
    lfile = fopen(filename, "w+");
	logfile = fopen(log_file, "w+");
    ofile = fopen(out_file, "wb");

    read_file(fp, lfile, logfile,ofile, extract_label, &L);
    fseek(fp, 0, SEEK_SET);
    read_file(fp, lfile, logfile,ofile, append_line, &L);
    fclose(fp);
    fclose(lfile);
	fclose(logfile);
    fclose(ofile);
    return 0;
}
