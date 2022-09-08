#include <stdio.h>

void print ( int x ) {
  printf("%d\n", x);
}

void print2(int x )
{
    printf("%d -> ", x);
}
void func ( void (*f)(int) ) {
  for ( int ctr = 0 ; ctr < 5 ; ctr++ ) {
    (*f)(ctr);
  }
}

int main() {
    func(print);
    func(print2);
    return 0;
}