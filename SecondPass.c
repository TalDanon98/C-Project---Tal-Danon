#include "Main.h"

/* SecondPass function will recieve an array of labels collected in firstPass,
   then we scan each line again and check in every command if a label is declared in current file.
   If label doesn't declared return 0, otherwise store into memory data and return 1. */
int SecondPass(int RamMemory[], int *IC, int *DC, label *labels,char *fileName)
{
	char c;
	char *firstpart,*secondpart,*thirdpart;
	int i = 0,count = 0,node = 0,problem = 0,linenumber = 1 ;
	char line[82] = {0};
	FILE *fp;
	fp = fopen(fileName, "r");
	if(fp == NULL)
		exit(1);
	
	/* Continue until reaching end of file. */
	while(c!= EOF)
	{
		/* Pass line after line. */
		while((c = fgetc(fp))!= '\n')
		{
			if(c == EOF)
				break;
			if(c==';')
				node = 10;
			if(node == 10)
			    continue;
			else if((c==' ' || c=='\t') && (node == 0 || count == 2))
				continue;
			else if((c==' ' || c=='\t') && node == 1 && count < 2)
			{
				line[i] = ' ';
				i++;
				node = 0;
				count++;
			}
			else
			{
				if(i == 83)
				{
					node = 10;
					problem = 1;
				}
				line[i] = c;
				i++;
				node = 1;		
			}
			
		}
		if(c == EOF)
			break;

		/* Initialize. */
		node = 0;
		count = 0;
		i = 0;

		if(line[0]!='\0')
		{
			firstpart = strtok(line, " ");
			secondpart = strtok(NULL, " ");
			thirdpart = strtok(NULL, " ");
			investigation2(firstpart,secondpart,thirdpart,&IC,&DC,RamMemory,labels,&problem,linenumber);
		}
			
		linenumber++;
		memset(line,0,82);
	}
	fclose(fp);

	if(problem == 1)
		return 0;
	return 1;
}

/* Function will check and devide line into 3 parts.
   While checking the type of the command then send it for thorough check.
   If any error occurred in a line it will return 1, otherwise return 0. */
void investigation2(char firstpart[],char secondpart[],char thirdpart[],int **IC, int **DC, int RamMemory[],  label *labels, int *problem, int linenumber)
{
	int len,i;
	char commands[][20] = {"mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","jsr","red","prn","rts","hlt"};
	len = strlen(firstpart);

	/* Check if it's a label. */
	if(firstpart[len-1] == ':')
	{

		/* Check for the commands only.(secondpart) */
		for(i = 0; i< 16; i++)
		{
			if(strcmp(secondpart, commands[i])==0)
			{
				checkComma(firstpart,secondpart,thirdpart,2,&IC, RamMemory, labels , 2, &problem, linenumber);
				return;
			}
		}
	}

	/* Check for the commands only.(firstpart) */
	for(i = 0; i< 16; i++)
	{
		if(strcmp(firstpart, commands[i])==0)
		{
			if(thirdpart != NULL && secondpart != NULL)
				secondpart = mergeTwoArrays(secondpart,thirdpart);
			checkComma(firstpart,secondpart,thirdpart,1,&IC, RamMemory, labels, 2, &problem, linenumber);
			return;	
		}
	}
	
	/* Check for .entry to add to our label structs. */
	if(strcmp(firstpart, ".entry")==0)
	{
		int i = 0,node = 0;
		while(((labels+i) -> lit)!=0)
		{
			if(strcmp((labels+i) -> name, secondpart)==0)
			{
				if((labels+i) -> attributes[2] == 0)
					(labels+i) -> attributes[1] = 1;
				else
				{
					printf("line %d : external and entry label\n" , linenumber);
					*problem = 1;
				}
				node = 1;
			}
			i++;
		}
		if(node == 0)
		{
			printf("line %d : error label not found\n" , linenumber);
			*problem = 1;
		}
		
		return;
	}
}

/* ToMemoryData2 function will recieve each line data and insert into memory data the label+ARE correctly in bits, return current IC. */
int ToMemoryData2(int sourceAddress, int destinationAddress, int opcode, int ogerSource,int ogerTarget, int numForTav, int secondnumForTav, char tavNameFirstOperand[], char tavNameSecondOperand[],int field,int IC, int RamMemory[], label *labels)
{
	int i,q =0 ;
	int A = 0;
	int R = 2;
	int E = 1;

	/* commands : not ,clr , inc ,dec ,jmp,bne,get,prn,jsr */  
	if(sourceAddress == (-1) && destinationAddress != (-1))
	{
		IC = IC + 1; /* skip first word*/
		
		/****************************** Check destination address. ******************************/	
		if(destinationAddress == 0 || destinationAddress == 3)
		{
			IC = IC + 1;		
		}
			
		else if(destinationAddress == 1 || destinationAddress == 2)
		{
			/*Find in array the name that contains the label/struct break when find.*/
			for(i= 0 ; (labels[i].lit)!=0; i++)
				if(strcmp(labels[i].name, tavNameSecondOperand)==0)
					break;
			
			/*Store the address of the label,then the ARE */
			RamMemory[IC] = (((labels[i].value)<<2)|RamMemory[IC]);
			
			if(labels[i].attributes[0] == 1 || labels[i].attributes[1] == 1 ||labels[i].attributes[3] == 1 )
				RamMemory[IC] = ((R)|RamMemory[IC]);
			else if(labels[i].attributes[2] == 1) 
			{
				while(labels[i].ExternPlace[q]!=0)
					q++;
				labels[i].ExternPlace[q] = IC;
				RamMemory[IC] = ((E)|RamMemory[IC]);
				
			}
			else
				RamMemory[IC] = ((A)|RamMemory[IC]);

			/* check if need to skip the field word */
			if(destinationAddress == 2)
				IC = IC + 2;
			else
				IC = IC + 1;	
		}
	}
	else if(sourceAddress == (-1) && destinationAddress == (-1))	/* Commands: rts,hlt. */
	{
		IC = IC + 1;
	}	
	else	/* Commands: mov,cmp,add,sub,lea. */
	{
		IC = IC + 1; /* skip first word */

		/****************************** Check source address. ******************************/
		if(sourceAddress == 0 || sourceAddress == 3)
		{
			/* Skip registar / immidate word */
			IC = IC + 1;
		}
		else if(sourceAddress == 1 || sourceAddress == 2)
		{
			/*Find in array the name that contains the label/struct break when find.*/
			for(i= 0 ; (labels[i].lit)!=0; i++)
				if(strcmp(labels[i].name, tavNameFirstOperand)==0)
					break;

			RamMemory[IC] = (((labels[i].value)<<2)|RamMemory[IC]);

			if(labels[i].attributes[0] == 1 || labels[i].attributes[1] == 1 ||labels[i].attributes[3] == 1)
				RamMemory[IC] = ((R)|RamMemory[IC]);
			else if(labels[i].attributes[2] == 1) 
			{
				while(labels[i].ExternPlace[q]!=0)
					q++;
				labels[i].ExternPlace[q] = IC;
				RamMemory[IC] = ((E)|RamMemory[IC]);	
			}
			else
				RamMemory[IC] = ((A)|RamMemory[IC]);

			/* check if need to skip the field word */
			if(sourceAddress == 2)
				IC = IC + 2;
			else 
				IC = IC + 1;
		}

		/****************************** Check destination address. ******************************/ 
		if(destinationAddress == 0 || (destinationAddress == 3 && sourceAddress != 3))
		{
			/* Skip one registar only / immidate word */
			IC = IC + 1;
		}
		if(destinationAddress == 1 || destinationAddress == 2)
		{
			for(i= 0 ; (labels[i].lit)!=0; i++)
				if(strcmp(labels[i].name, tavNameSecondOperand)==0)
					break;
	
			RamMemory[IC] = (((labels[i].value)<<2)|RamMemory[IC]);

			if(labels[i].attributes[0] == 1 || labels[i].attributes[1] == 1 ||labels[i].attributes[3] == 1)
				RamMemory[IC] = ((R)|RamMemory[IC]);
			else if(labels[i].attributes[2] == 1) 
			{
				while(labels[i].ExternPlace[q]!=0)
					q++;
				labels[i].ExternPlace[q] = IC;	
				RamMemory[IC] = ((E)|RamMemory[IC]);
				
			}
			else
				RamMemory[IC] = ((A)|RamMemory[IC]);

			/* check if need to skip the field word */
			if(destinationAddress == 2)
				IC = IC + 2;
			else 
				IC = IC + 1;	
		}	
	}
	return IC;
}

