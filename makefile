Main: Main.o FirstPass.o SecondPass.o Operand.o PreAssembler.o
	gcc -g -ansi Main.o FirstPass.o SecondPass.o Operand.o PreAssembler.o -o Main -lm

Main.o: Main.c Main.h
	gcc -c -Wall -ansi -pedantic Main.c -o Main.o -lm

FirstPass.o: FirstPass.c Main.h 
	gcc -c -Wall -ansi -pedantic FirstPass.c -o FirstPass.o

SecondPass.o: SecondPass.c Main.h 
	gcc -c -Wall -ansi -pedantic SecondPass.c -o SecondPass.o

Operand.o: Operand.c Main.h 
	gcc -c -Wall -ansi -pedantic Operand.c -o Operand.o

PreAssembler.o: PreAssembler.c Main.h 
	gcc -c -Wall -ansi -pedantic PreAssembler.c -o PreAssembler.o


