#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SPACE 65536
#define UNUSED(x) (void)(x)
#define bool int
#define true 1
#define false 0

unsigned int MEMORY[SPACE];
int A, B, SP = 0, PC = 0;

void ldc(int value){
    B = A;
    A = value;
}

void adc(int value){
    A = A + value;
}

void ldl(int offset){
    B = A;
    A = MEMORY[SP + offset];
}

void stl(int offset){
    MEMORY[SP + offset] = A;
    A = B;
}

void ldnl(int offset){
    A = MEMORY[A + offset];
}

void stnl(int offset){
    MEMORY[A + offset] = B;
}

void add(int value){
    UNUSED(value);
    A = A + B;
}

void sub(int value){
    UNUSED(value);
    A = B - A;
}

void shl(int value){
    UNUSED(value);
    A = B << A;
}

void shr(int value){
    UNUSED(value);
    A = B >> A;
}

void adj(int value){
    SP = SP + value;
}

void a2sp(int value){
    UNUSED(value);
    SP = A;
    A = B;
}

void sp2a(int value){
    UNUSED(value);
    B = A;
    A = SP;
}

void call(int offset){
    B = A;
    A = PC;
    PC += offset;
}

void ret(int value){
    UNUSED(value);
    PC = A;
    A = B;
}

void brz(int offset){
    if(A == 0){
        PC += offset;
    }
}

void brlz(int offset){
    if(A < 0){
        PC += offset;
    }
}

void br(int offset){
    PC += offset;
}

void HALT(int value){
    UNUSED(value);
    printf("A = %d, B = %d, SP = %d, PC = %d\n", A, B, SP, PC);
    exit(0);
}

void SET(int value){
    MEMORY[PC] = value;
}


int main(int argc, char *argv[]) {
    FILE *fp = fopen(argv[1], "rb");
    int file_size_in_bytes;
    int opcode, operand;
    void (*f[19])(int value) = {ldc, adc, ldl, stl, ldnl, stnl, add, sub, shl, shr, adj, a2sp, sp2a, call, ret, brz, brlz, br, HALT};
    bool trace= false;
    if (argc < 2) {
        printf("USAGE: %s <file> [-t]\n", argv[0]);
        printf(" -t: trace execution\n");
        return 0;
    }
    if (strcmp(argv[2], "-t") == 0) {
        trace = true;
    }
    fseek(fp, 0, SEEK_END);
    file_size_in_bytes = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (file_size_in_bytes % 4 != 0) exit(-1);

    fread(MEMORY, 4, file_size_in_bytes / 4, fp);
	while (PC < file_size_in_bytes / 4) {
        opcode = MEMORY[PC] % (1 << 8);
        operand = ((int)MEMORY[PC] >> 8);
        PC++;
        (*f[opcode])(operand);
        if (trace) {
            printf("A = %d, B = %d, SP = %d, PC = %d\n", A, B, SP, PC);
        }
	}

    printf("A = %d, B = %d, SP = %d, PC = %d\n", A, B, SP, PC);

    fclose(fp);

    return 0;
}
