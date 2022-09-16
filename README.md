# TITLE: 4 Register Assembler and Emulator
### Author: [RUPAK BISWAS]( https://github.com/RupakBiswas-2304 )
### Date: 2021-09-16


Files:
==========
- *All the files i submitted, asm.c and emu.c and claims.txt, have my name and student id at the start, along with a declaration of authorship.

- The evidence for Assembler including the output files from the X (eg five)  test examples has also been uploaded. They are test1(3 output files), test2(3 output files), test3
(1 output file), memcpy(3 output files) and triangle(3 output files). All the test files have been tested and work properly. As for the evidence for the emulator is uploaded
with two output files, memcpy.txt and triangle.txt.

- *Both of my c programs did not show any errors and warnings when i compiled it with
gcc -std=c89 -pedantic and gcc -std=c89 -pedantic -W -Wall -Wpointer-arith -Wwrite-strings -Wstrict-prototypes standard  or equilvalent ....or gcc ... or g++

- *Two programs are compilable with gcc asm.c -o asm and gcc emu.c -o emu.



## Program format and structure:
## 
1. The assembler:

    * with two data structures to store respectively labels and mnemonic table with expected operands.
    * uses a single routine for both passes.
    * detects label erros.
    * consistent and sensible formatting with sensible program structure. 
    * sensible variable, function & type names with explanatory comments.
		* advanced listing file.
		* assembles test programs.
	  * can implements and demonstrates the instruction SET.

2. The emulator :

	*  with three data structures to store respectively memory, mnemonic table with expected operands
		  and a linked list structure word.
    * loads object file, 
    * by giving the instructions like "-befor/-after" , the program can produce memory dump before/after execution.
    * by giving the instruction like "-trace", the program prints the result of program executed.
    * detects errant programs.



## Testing:


- ## 1. The assembler:

  - I have tested  5/ five examples shown as bellow. The errors only occurred in the file test2.asm and 
the program immediately stopped on the second pass. 

  - The others were tested without any errors detected and produced three output files, listing file, 
error log file and machine readable object file, for each example. 

  - After comparing the listing files and object file of memcpy.asm , memcpy.l, triangle.l memcpy.o 
with the implemented result given on the assignment, both of them show no difference 
from those provided( if there is different  that is fine too) 
 


- 1)
```
#Input: ./asm test1.asm
#Output: 
a. test1.l 
b. test1.log (without any error messages)
c. test1.o
```
- 2)
```
#Input: ./asm test2.asm
#Output: 
a. test2.l 
b. test2.log (without error messages shown as below but also in log file)
c. test2.o

error messages:
ERROR: Duplicate labels found for label.
ERROR: Invalid label 0def at address 0005
ERROR: Invalid label or offset nonesuch at address 0000
ERROR: Invalid number 08ge at address 0001
ERROR: Missing operand for instruction ldc at address 0002
ERROR: Invalid number 5, at address 0004
ERROR: Invalid mnemonic fibble at address 0005
ERROR: Invalid mnemonic 0def at address 0006
```

- 3)
```
#Input: ./asm TEST/test3.asm (memcopy.asm)
#Output: 
a. test3.l 
b. test3.log (without any error messages)
c. test3.o
```

- 4)
```
#Input: ./asm test4.asm (triangle.asm)
#Output: 
a. memcpy.l 
b. memcpy.log (without any error messages)
c. memcpy.o
```

- 5)
```
#Input: ./asm sums.asm 
#Output: 
a. triangle.l 
b. triangle.log (without any error messages)
c. triangle.o
```


- ## 2. The emulator:

  - I have tested three examples shown as bellow. 

  - The others were tested without any errors detected and produced excution data both on the screen and 
output text file for each example according to the usage instruction. 

  - After running the bin file given, the trace data are the same as 
the implemented result from my own program.

  - Flags : 
```
input :  ./emu help
USAGE: <file> [-t] [-d [sec]] [-p] ...
 -t: trace execution
 -d [sec]: delay execution
 -p: persistant trace output
 -membefore [start] [end]: print memory before execution
 -memafter [start] [end]: print memory after execution
 -memtrace [start] [end]: print memory after each instruction
```
- 1)
```
Input :  ./emu TEST/test3.o -t -p -memafter 0 20
Output :
+------------+------------+------------+------------+
|    Reg A   |    Reg B   |     PC     |     SP     |
+------------+------------+------------+------------+
|     75     |      0     |      1     |      0     |
+------------+------------+------------+------------+
+------------+------------+------------+------------+
|    Reg A   |    Reg B   |     PC     |     SP     |
+------------+------------+------------+------------+
|     141    |      0     |      2     |      0     |
+------------+------------+------------+------------+
+------------+------------+------------+------------+
|    Reg A   |    Reg B   |     PC     |     SP     |
+------------+------------+------------+------------+
|     141    |      0     |      2     |      0     |
+------------+------------+------------+------------+
+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+
|  MEMORY  |     0    |     1    |     2    |     3    |     4    |     5    |     6    |     7    |     8    |     9    |
+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+
| 00000000 | 00004B00 | 00004201 | 00000000 | 00000000 | 00000000 | 00000000 | 00000000 | 00000000 | 00000000 | 00000000 |
+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+
| 0000000A | 00000000 | 00000000 | 00000000 | 00000000 | 00000000 | 00000000 | 00000000 | 00000000 | 00000000 | 00000000 |
+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+
| 00000014 | 00000000 | 00000000 | 00000000 | 00000000 | 00000000 | 00000000 | 00000000 | 00000000 | 00000000 | 00000000 |
+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+
```

- 2)
```
#Input: ./emu TEST/test4.o -t -p -memafter 0 100 > TEST/triangle.txt
#Output: triangle.txt
```

- 3)
```
#Input: ./emu TEST/test1.o -t 
#Output: Gone in infinite loop ( no error or warning message)

```