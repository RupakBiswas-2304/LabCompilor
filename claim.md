# TITLE: 
## AUTHOR: RupakBiswas-2304
Declaration of Authorship
This txt file, claims.txt, is part of the assignment of CS322 at the 
department of Computer Science and Engg, IIT Patna . 

Files:
==========
*All the files i submitted, asm.c and emu.c and claims.txt, have my name and student id at the start, along with a declaration of authorship.

The evidence for Assembler including the output files from the X (eg five)  test examples has also been uploaded. They are test1(3 output files), test2(3 output files), test3
(1 output file), memcpy(3 output files) and triangle(3 output files). All the test files have been tested and work properly. As for the evidence for the emulator is uploaded
with two output files, memcpy.txt and triangle.txt.

*Both of my c programs did not show any errors and warnings when i compiled it with
gcc -std=c89 -pedantic and gcc -std=c89 -pedantic -W -Wall -Wpointer-arith -Wwrite-strings -Wstrict-prototypes standard  or equilvalent ....or gcc ... or g++

*Two programs are compilable with gcc asm.c -o asm and gcc emu.c -o emu.


=============================
Program format and structure:
=============================
1. The assembler:

    * with two data structures to store respectively labels and mnemonic table with expected operands.
    * uses a single routine for both passes.
    * detects label erros.
    * consistent and sensible formatting with sensible program structure. 
    * sensible variable, function & type names with explanatory comments.
		* advanced listing file.
		* assembles test programs.
	  * can implements and demonstrates the instruction SET.

2. The emulator ( if you are submitting) :

		* with three data structures to store respectively memory, mnemonic table with expected operands
		  and a linked list structure word.
    * loads object file, 
    * by giving the instructions like "-befor/-after" , the program can produce memory dump before/after execution.
    * by giving the instruction like "-trace", the program prints the result of program executed.
    * detects errant programs.


=============================
Testing:
=============================

1. The assembler:

I have tested  X/ five examples shown as bellow. The errors only occurred in the file test2.asm and 
the program immediately stopped on the second pass. 

The others were tested without any errors detected and produced three output files, listing file, 
error log file and machine readable object file, for each example. 

After comparing the listing files and object file of memcpy.asm , memcpy.l, triangle.l memcpy.o 
with the implemented result given on the assignment, both of them show no difference 
from those provided( if there is different  that is fine too) 
 

1)
#Input: asm test1.asm
#Output: 
a. test1.l 
b. test1.log (without any error messages)
c. test1.o

2)
#Input:asm test2.asm
#Output: 
a. test2.l 
b. test2.log (without error messages shown as below but also in log file)
c. test2.o

error messages:
ERROR: A duplicate label was found on line 4
ERROR: There is a non-numerical value on line 6
ERROR: A numerical value was expected on line 7
ERROR: A numerical value was not expected on line 8
ERROR: Only one numerical value was expected on line 9
ERROR: A bogus label name was found on line 10
ERROR: An unknown mnemonic was found on line 11
ERROR: An unknown mnemonic was found on line 12
ERROR: A non existant label was found on line 5

3)
#Input:asm test3.asm
#Output: 
a. test3.l 
b. test3.log (without any error messages)
c. test3.o


4)
#Input:asm memcpy.asm
#Output: 
a. memcpy.l 
b. memcpy.log (without any error messages)
c. memcpy.o

5)
#Input:asm triangle.asm
#Output: 
a. triangle.l 
b. triangle.log (without any error messages)
c. triangle.o



2. The emulator:

I have tested three examples shown as bellow. The errors only occurred in the file test3.o and 
the program immediately stopped with an error message on the screen. 

The others were tested without any errors detected and produced excution data both on the screen and 
output text file for each example according to the usage instruction. 

After running the bin file given, the trace data are the same as 
the implemented result from my own program.

1)
#Input:emu -trace memcpy.asm
#Output: memcpy.txt


2)
#Input: emu -trace triangle.asm
#Output: triangle.txt


3)
#Input: emu -trace test3.asm
#Output: Fail to complete. 
with error messages shown on the screen

PC=00000000, SP=08049596, A=008172D0, B=0804958C Fatal error occurred! No instruction can be found!
