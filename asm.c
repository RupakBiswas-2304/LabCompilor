/*
|--------------------------------------|
|                                      |
|--------------------------------------|
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *right_trim(char *str, int n)
{
	/* trim the right side of the string upto n characters */
	char * back = str + strlen(str);
	while(n--){
		back--;
		*back = '\0';
	}
	*back = '\0';
	return str;
}

void append_line(char *str, FILE *fp)
{
	/* write a line to the file */
	char *token;
	int value;
	char line[100];
	int instruction_code = 0x00000000;
	char instructions[23][6] = {
		"data",
		"ldc","adc","adj","SET",
		"ldl","stl","ldnl","stnl","call","brz","brlz","br",
		"add","sub","shl","shr","a2sp","sp2a","return","HALT"
	};
	int instruction_opcode[23] = {
		NULL,
		0x00,0x01,0x0a,NULL,
		0x02,0x03,0x04,0x05,0x0d,0x0f,0x10,0x11,
		0x06,0x07,0x08,0x09,0x0b,0x0c,0x0e ,0x12
	};
	int i;

	strcpy(line, str);
	token = strtok(str, " "); /* get the first token */
	/*if (strncmp(token,"ldc",3)==0){
		instruction_code += 0x00000000;
		value = atoi(strtok(NULL, " "));
		value *= 0x00000100;
		instruction_code += value;
	}*/
	for (i = 0; i < 23;i++){
		if (strncmp(token,instructions[i],strlen(instructions[i]))==0){
			instruction_code += instruction_opcode[i];
			if (i>0 && i < 13){
				value = atoi(strtok(NULL, " "));
				value *= 0x00000100;
				instruction_code += value;
			}
		}
	}	


	fprintf(fp, "%08X ", instruction_code);
	fprintf(fp, "%s", line);
}

int main(int argc, char *argv[]) {
	char c;
	FILE* fp = fopen(argv[1], "r"), *lfile;
	char* filename = argv[1];
	int comment_flag = 0, char_in_line = 0, label_flag = 0, space_flag = 0, line_start_flag = 0;
	char temp_line[100]= "";

	/*
	comment_flag --> helps to ignore comments, 
	char_in_line --> helps to ignore empty lines,
	label_flag --> helps to not incrementing the code address
	space_flag --> helps to ignore multiple spaces
	line_start_flag --> helps to ignore spaces at the start of the line
	*/
	int line_counter = 0x0000;
	if (argc < 2){
		return 0;
	}
	filename = right_trim(filename, 3);
	strcat(filename, "l");
	lfile = fopen(filename, "w+");

	if (fp == NULL) {
		printf("Error opening file");
		return -1;
	}


	printf("creating file %s\n", filename);
	fprintf(lfile,"%04X ", line_counter);
	while ((c = fgetc(fp)) != EOF) {
		if (c == '\n'){
			if (char_in_line != 0){
				append_line(temp_line, lfile);
				temp_line[0] = '\0';
				fprintf(lfile, "%c", c); /* writing new line charecter in .l file*/
				line_start_flag = 1;
				space_flag = 0;
				if (label_flag == 0){
					line_counter ++;
				}
				fprintf(lfile,"%04X ", line_counter); /*writing address in .l file*/
			}
			comment_flag = 0;
			char_in_line = 0;
		}
		else if (comment_flag == 1) {} /* ignore comments */
		else if (c == ';'){
			comment_flag = 1;
		}
		else {
			char_in_line++;
			if (c =='\t'){} /* ignoring tabs*/
			else if (c == ' ') {
				if (line_start_flag || space_flag>1){}
				else {
					/* fprintf(lfile, "%c", c);  writing a single space in .l file */
					strncat(temp_line, &c, 1);
					space_flag++;
				}
			}
			else{
				/* fprintf(lfile, "%c", c);  writing charecter in .l file */
				strncat(temp_line, &c, 1);
				line_start_flag = 0;
			}


			if (c == ':'){
				label_flag = 1;
			}
			else if (c != ' ' && c != '\t' && label_flag == 1){
				label_flag = 0;
				space_flag = 0;
			}
		}
	}
	fprintf(lfile, "%s", temp_line);


	fclose(fp);
	fclose(lfile);
	return 0;
}
