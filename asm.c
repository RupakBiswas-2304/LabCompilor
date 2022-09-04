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


int main(int argc, char *argv[]) {
	char c;
	FILE* fp = fopen(argv[1], "r"), *lfile;
	char* filename = argv[1];
	int comment_flag = 0, char_in_line = 0, label_flag = 0, space_flag = 0, line_start_flag = 0;
	/*
	comment_flag --> helps to ignore comments, 
	char_in_line --> helps to ignore empty lines,
	label_flag --> helps to not incrementing the code address
	space_flag --> helps to ignore multiple spaces
	line_start_flag --> helps to ignore spaces at the start of the line
	*/
	int line_counter = 0x0000;
	filename = right_trim(filename, 3);
	strcat(filename, "l");
	lfile = fopen(filename, "w+");

	if (fp == NULL) {
		printf("Error opening file");
	}


	printf("creating file %s\n", filename);
	fprintf(lfile,"%04X ", line_counter);
	while ((c = fgetc(fp)) != EOF) {
		if (c == '\n'){
			if (char_in_line != 0){
				fprintf(lfile, "%c", c);
				line_start_flag = 1;
				space_flag = 0;
				if (label_flag == 0){
					line_counter ++;
				}
				printf("%d", label_flag);
				fprintf(lfile,"%04X ", line_counter);
			}
			comment_flag = 0;
			char_in_line = 0;
		}
		else if (comment_flag == 1) {	
		}
		else if (c == ';'){
			comment_flag = 1;
		}
		else {
			char_in_line++;
			if (c =='\t'){} /* ignoring tabs*/
			else if (c == ' ') {
				if (line_start_flag || space_flag>1){}
				else {
					fprintf(lfile, "%c", c);
					space_flag++;
				}
			}
			else{
				fprintf(lfile, "%c", c);
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


	fclose(fp);
	fclose(lfile);
	(void)argc;
	return 0;
}
