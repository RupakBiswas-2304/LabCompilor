#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    char *str = "0efeww";
    int d = atoi(str);
    int h;
    if (atoi(str)) {
        printf("true");
    } else {
        printf("false");
    }
    h = (int)strtol(str, NULL, 16);
    printf("d->%d | s->%s | sd -> %d | h -> %d", d, str,str, h);
    return 0;
}