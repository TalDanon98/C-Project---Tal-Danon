#ifndef Main_h

/******************** Libraries ********************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

/******************** Defines ********************/
#define Main_h
#define WhatBit(RamMemory,k)    (RamMemory[((k)/10)] & (1 << ((k)%10))) /* Let us know what the specific bit is. */
#define MAX_MEM_CAPACITY 256
#define MAX_LINE_LEN 81


/******************** Structors ********************/

/* label will save each label with the right attributes. */
typedef struct 
{
	char name[31];
	int value;
	int lit;
	int ExternPlace[31];
	int attributes[4];		/* Each place is like a switch of type : code,entry,extern,data */
}label;

/* Macro will save macro's names and thier data for the PreAssembler. */
typedef struct Macro 
{
	char macroName[MAX_LINE_LEN];
	char *macroData;
}Macro;

/******************** Declarations ********************/

/* Functions for Main. */
char convert_Base(char n[]);
void AddArrays( int RamMemory[] , int SymbolMemory[] ,int IC ,int DC);
void FinalToFile(int RamMemory [MAX_MEM_CAPACITY], int n, int f, char* path,label labels[]);
char convert_Base(char n[]);
void convert_int_base(int n, char *symbol);


/* Functions for PreAssembler. */
void PreAssembler(char* path, int argc);
char *clear_spaces(char *str);

/* Functions for FirstPass. */
int StartPass(int RamMemory[], int SymbolMemory[],int *IC, int *DC, label *labels,char* path);
int investigation(char firstpart[],char secondpart[],char thirdpart[],int **IC, int **DC, int RamMemory[], int SymbolMemory[], label *labels, int *problem, int linenumber);
void checkComma(char firstpart[],char secondpart[],char thirdpart[], int labelOrNot,int ***IC, int RamMemory[],label *labels,int firstOrSecondPass, int **problem , int linenumber);
int tosymbolstring(char thirdpart[], int SymbolMemory[] , int DC, int **problem, int linenumber);
int tosymboldata(char thirdpart[], int SymbolMemory[] , int DC , int **problem, int linenumber);
int tosymbolstruct(char thirdpart[], int SymbolMemory[] , int DC , int **problem, int linenumber);
int addlabel(label new ,label labels[], int attribute);
int ToMemoryData(int sourceAddress, int destinationAddress, int opcode, int ogerSource,int ogerTarget, int numForTav, int secondnumForTav, char tavNameFirstOperand[], char tavNameSecondOperand[],int field,int IC, int RamMemory[]);
char *mergeTwoArrays(char *a, char *b);

/* Functions for SecondPass. */
int SecondPass(int RamMemory[],int *IC, int *DC, label *labels,char* path);
void investigation2(char firstpart[],char secondpart[],char thirdpart[],int **IC, int **DC, int RamMemory[], label *labels, int *problem, int linenumber);
int ToMemoryData2(int sourceAddress, int destinationAddress, int opcode,  int ogerSource,int ogerTarget, int numForTav, int secondnumForTav, char tavNameFirstOperand[], char tavNameSecondOperand[],int field,int IC, int RamMemory[], label *labels);

/* Functions for Operand. */
void checkOperand(char command[], char firstoperand[], char secondoperand[], int**** IC, int RamMemory[], label *labels, int firstOrSecondPass,int linenumber,int ***problem);
int isItValidNum(char operand[],int linenumber,int ****problem,int *skip);
int isItRegister(char operand[]);
int isItAccess(char operand[], int linenumber, int *****problem,int **skip);
void isItDirect(char operand[], char tavName[],int linenumber,int *****problem,int **skip);
void labelAndField(char operand[], char tavName[], int **field, int linenumber, int *****problem , int **skip);
void isItValidLabel(char tavName[],int linenumber, int *****problem,int **skip,int firstOrSecondPass,label *labels);
void directOrAccess(char operand[], char tavName[], int *address, int *field, int linenumber, int ****problem,int *skip, int firstOrSecondPass,label *labels);

#endif
