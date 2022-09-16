/*****************************************************************************
TITLE: Claims																																
AUTHOR:  Rupak Biswas
ROLL NO: 2001CS57
EMAIL ID: rupak_2001cs57@iitp.ac.in
Declaration of Authorship
This txt file, claims.txt, is part of the assignment of CS322 at the 
department of Computer Science and Engg, IIT Patna . 
*****************************************************************************/


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
int not_halted = 1;
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
    printf("%d", value);
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
    not_halted = 0;
}
void SET(int value) { MEMORY[PC] = value; }

void print_memory(int start, int end){
    int i, j;
    printf("+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+\n");
    printf("|  MEMORY  |     0    |     1    |     2    |     3    |     4    |     5    |     6    |     7    |     8    |     9    |\n");
    printf("+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+\n");
    for(i = start; i <= end;){
        printf("| %.8X |", i);
        for (j = 0; j < 10; j++){
            printf(" %.8X |", MEMORY[i]);
            i++;
        }
        printf("\n");
        printf("+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+\n");
    }
}

void print_manual() {
    printf("USAGE: <file> [-t] [-d [sec]] [-p] ... \n");
    printf(" -t: trace execution\n");
    printf(" -d [sec]: delay execution\n");
    printf(" -p: persistant cli output\n");
    printf(" -membefore [start] [end]: print memory before execution\n");
    printf(" -memafter [start] [end]: print memory after execution\n");
    printf(" -memtrace [start] [end]: print memory after each instruction\n");
    printf(" -instructions : Prints the instruction Set");
}

int main(int argc, char *argv[]) {
    FILE *fp = fopen(argv[1], "rb");
    FILE *isetfp;
    bool persistant = false;
    int file_size_in_bytes;
    int opcode, operand;
    double delay = 0;
    void (*f[19])(int value) = {ldc, adc, ldl,  stl, ldnl, stnl, add,
                                sub, shl, shr,  adj, a2sp, sp2a, call,
                                ret, brz, brlz, br,  HALT};
    bool trace = false;
    bool memb = false, memt = false, step = false;
    int startb = 0, endb = 100;
    bool mema = false;
    int starta = 0, enda = 100;
    int startt = 0, endt = 100;
    int inputs = 0;
    char x;

    if (fp == NULL){
        print_manual();
        return 0;
    }
    if (argc < 2) {
        print_manual();
        return 0;
    }
    inputs = argc - 2;
    while (inputs){
        if (strcmp(argv[inputs +1], "-t") == 0) trace = true;
        else if (strcmp(argv[inputs +1], "-d") == 0) {
            if (!argv[inputs+2]) { print_manual(); return 0; }
            delay = atof(argv[inputs +2]);
        }
        else if (strcmp(argv[inputs +1], "-p") == 0) persistant = true;
        else if (strcmp(argv[inputs +1], "-membefore") == 0) {
            memb = true;
            startb = atoi(argv[inputs +2]);
            endb = atoi(argv[inputs +3]);
        }
        else if (strcmp(argv[inputs +1], "-memafter") == 0) {
            mema = true;
            starta = atoi(argv[inputs +2]);
            enda = atoi(argv[inputs +3]);
        }
        else if (strcmp(argv[inputs +1], "-memtrace") == 0) {
            memt = true;
            startt = atoi(argv[inputs +2]);
            endt = atoi(argv[inputs +3]);
        }
        else if (strcmp(argv[inputs +1], "-step") == 0) {
            step = true;
        }
        else if (strcmp(argv[inputs +1],"-instructions")==0){
            isetfp = fopen("instructionset.txt", "r");
            while((x=fgetc(isetfp))!=EOF) {
                printf("%c",x);
            }
            printf("\n");
            fclose(isetfp);
        }
        inputs--;
    }

    fseek(fp, 0, SEEK_END);
    file_size_in_bytes = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (file_size_in_bytes % 4 != 0) exit(-1);

    fread(MEMORY, 4, file_size_in_bytes / 4, fp);

    if (memb) print_memory(startb, endb);
    while (PC < file_size_in_bytes / 4 && not_halted) {
        opcode = MEMORY[PC] % (1 << 8);
        if (0 <= opcode && opcode < 19){
            operand = ((int)MEMORY[PC] >> 8);
            PC++;
            (*f[opcode])(operand);
        }
        else{
            operand = MEMORY[PC];
            PC++;
        } 
        if (trace) {
            if (delay) sleep(delay);
            if( !persistant) system(CLEAR);
            print_register();
            if (memt) print_memory(startt, endt);
        }
        if (step){
            printf("Press enter to continue...");
            getchar();
        }
    }
    print_register();
    if (mema) print_memory(starta, enda);
    fclose(fp);
    return 0;
}
