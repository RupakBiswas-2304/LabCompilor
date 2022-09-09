#include <stdio.h>
#include <stdlib.h>

#define SPACE 65536

int MEMORY[SPACE];



int main(int argc, char *argv[]) {
    FILE *fp = fopen(argv[1], "rb");
    int file_size_in_bytes;
	int i = 0;
	int A, B, SP, PC = 0;

    fseek(fp, 0, SEEK_END);
    file_size_in_bytes = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (file_size_in_bytes % 4 != 0) exit(-1);

    fread(MEMORY, 4, file_size_in_bytes / 4, fp);
	while (i < file_size_in_bytes / 4) {
		printf("%d --> %08X\n",i, MEMORY[i]);
		i++;
	}

    fclose(fp);

    if (argc < 2) {
        return 0;
    }

    return 0;
}
