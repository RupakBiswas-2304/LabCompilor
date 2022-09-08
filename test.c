#include <stdio.h>
#include <string.h>

int main(void)
{
   char *token, string[] = "a string, of,; ;;;,tokens\0,after null terminator";

   token = strtok(string, ", ;");
   do
   {
      printf("token: \"%s\"\n", token);
   }
   while (token = strtok(NULL, ", ;"));

}