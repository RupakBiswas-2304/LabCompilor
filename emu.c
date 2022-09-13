#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SPACE 65536
#define UNUSED(x) (void)(x)
#define bool int
#define true 1
#define false 0
#define PRINT_LABEL \
    printf("\n|    Reg A   |    Reg B   |     PC     |     SP     |\n")
#define PRINT_LINE \
    printf("+------------+------------+------------+------------+")
#define ENDL printf("\n")
#ifdef _WIN32
#include <windows.h>
#define CLEAR "cls"
#else
#include <unistd.h>
#define CLEAR "clear"
#endif

#define RED "\x1b[0;31m"
#define GREEN "\x1b[0;32m"
#define YELLOW "\x1b[0;33m"
#define BLUE "\x1b[0;34m"
#define PURPLE "\x1b[0;35m"
#define NORMAL "\x1b[0;0m"

unsigned int MEMORY[SPACE];
int A, B, SP = 0, PC = 0;
const int SEG_LEN = 12;
void fprint(int value) {
    int length = log10(value == 0 ? 1 : value) + 1;
    int right_space = 0;
    int left_space = 0;
    int i = 0;

    if (value < 0) length = log10(abs(value)) + 2;

    right_space = (SEG_LEN - length) / 2;
    left_space = SEG_LEN - length - right_space;
    for (i = 0; i < left_space; i++) {
        printf(" ");
    }
    printf(GREEN "%d" NORMAL, value);
    for (i = 0; i < right_space; i++) {
        printf(" ");
    }
}

void print_register() {
    PRINT_LINE;
    PRINT_LABEL;
    PRINT_LINE;
    printf("\n|");
    fprint(A);
    printf("|");
    fprint(B);
    printf("|");
    fprint(PC);
    printf("|");
    fprint(SP);
    printf("|");
    ENDL;
    PRINT_LINE;
    ENDL;
}

void ldc(int value) {
    B = A;
    A = value;
}

void adc(int value) { A = A + value; }

void ldl(int offset) {
    B = A;
    A = MEMORY[SP + offset];
}

void stl(int offset) {
    MEMORY[SP + offset] = A;
    A = B;
}

void ldnl(int offset) { A = MEMORY[A + offset]; }

void stnl(int offset) { MEMORY[A + offset] = B; }

void add(int value) {
    UNUSED(value);
    A = A + B;
}

void sub(int value) {
    UNUSED(value);
    A = B - A;
}

void shl(int value) {
    UNUSED(value);
    A = B << A;
}

void shr(int value) {
    UNUSED(value);
    A = B >> A;
}

void adj(int value) { SP = SP + value; }

void a2sp(int value) {
    UNUSED(value);
    SP = A;
    A = B;
}

void sp2a(int value) {
    UNUSED(value);
    B = A;
    A = SP;
}

void call(int offset) {
    B = A;
    A = PC;
    PC += offset;
}

void ret(int value) {
    UNUSED(value);
    PC = A;
    A = B;
}

void brz(int offset) {
    if (A == 0) {
        PC += offset;
    }
}

void brlz(int offset) {
    if (A < 0) {
        PC += offset;
    }
}

void br(int offset) { PC += offset; }

void HALT(int value) {
    UNUSED(value);
    print_register();
    exit(0);
}

void SET(int value) { MEMORY[PC] = value; }



int main(int argc, char *argv[]) {
    FILE *fp = fopen(argv[1], "rb");
    int file_size_in_bytes;
    int opcode, operand;
    int delay = 0;
    void (*f[19])(int value) = {ldc, adc, ldl,  stl, ldnl, stnl, add,
                                sub, shl, shr,  adj, a2sp, sp2a, call,
                                ret, brz, brlz, br,  HALT};
    bool trace = false;
    if (argc < 2) {
        printf("USAGE: %s <file> [-t]\n", argv[0]);
        printf(" -t: trace execution\n");
        printf(" -d [sec]: delay execution\n");
        return 0;
    }
    if (argv[2]) {
        if (strcmp(argv[2], "-t") == 0) {
            trace = true;
        }
    }
    if (argv[3]) {
        if (strcmp(argv[3], "-d") == 0) {
            if (!argv[4]) {
                printf("USAGE: %s <file> [-t] [-d [sec]]\n", argv[0]);
                printf(" -t: trace execution\n");
                printf(" -d [sec]: delay execution\n");
                return 0;
            }
            delay = atoi(argv[4]);
            #ifdef _WIN32
            delay = delay*1000;
            #endif
        }
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
            sleep(delay);
            system(CLEAR);
            print_register();
        }
    }
    print_register();
    fclose(fp);
    return 0;
}
