#include "Main.h"

/* StartPass function will recieve a file and check for errors in each line.
   Each line we will send to investigation after deviding line into 3 parts, for thorough check.
   If any error occurred in a line it will return 0, otherwise return 1. */
int StartPass(int RamMemory[], int SymbolMemory[],int *IC, int *DC, label *labels,char* path)
{
	FILE *fp;
	enum status{nothing ,data ,structor,string}type = nothing;
	char *firstpart,*secondpart,*thirdpart;
	int c, i = 0, count = 0, node = 0, linenumber = 1, problem = 0, skip = 0;
	int dataPatt = 0, structPatt = 0, stringPatt = 0;
	char line[82] = {0};	
	char am[] = ".am";
	strcat(path, am);

	fp = fopen(path, "r");
	if(fp == NULL)
		exit(1);
	
	/* Continue until reaching end of file. */
	while(c != EOF)
	{
		/* Pass line after line. */
		while((c = fgetc(fp))!= '\n')
		{
			if(c == EOF)
				break;
			if(c < 32)
				continue;
			if(i==0 && c==';')
				skip = 1;
			if(c == ';' && i != 0)
			{
				printf("Line %d problem is: comment is not in the right place.\n", linenumber);
				problem = 1;
				skip = 1;
			}

			/* Check if line is out of amount of characters. */
			if(i == 82)
			{
				printf("Line %d problem is: too much characters in a line\n", linenumber);
				problem = 1;
				node = 10;
				skip = 1;
				break;
			}

			/* Check if current line is a data type then get inside if statement. */
			if(type == data)
			{
				if((dataPatt == 0 || dataPatt == 1) && (c==' ' || c=='\t'))
					dataPatt = 1;
				else if((dataPatt == 0 || dataPatt == 1) && (c=='-' || c=='+'))
					dataPatt = 2;
				else if((dataPatt == 0 || dataPatt == 1 || dataPatt == 2 || dataPatt == 3) && (isdigit(c)))
					dataPatt = 3;
				else if((dataPatt == 3 || dataPatt == 4) && (c==' ' || c=='\t'))
					dataPatt = 4;
				else if((dataPatt == 3 || dataPatt == 4) && c==',' )
					dataPatt = 0;
				else
				{
					if(skip != 1)
						printf("Line %d problem is: problem with numbers of data\n", linenumber);
					problem = 1;	
					skip = 1;
				}
			}

			/* Check if current line is a struct type then get inside if statement. */
			if((type == structor) && structPatt < 5)
			{
				if((structPatt == 0 || structPatt == 1) && (c==' ' || c=='\t'))
					structPatt = 1;
				else if((structPatt == 0 || structPatt == 1) && (c=='-' || c=='+'))
					structPatt = 2;
				else if((structPatt == 0 || structPatt == 1 || structPatt == 2 || structPatt == 3) && (isdigit(c)))
					structPatt = 3;
				else if((structPatt == 3 || structPatt == 4) && (c==' ' || c=='\t'))
					structPatt = 4;
				else if((structPatt == 3 || structPatt == 4) && c==',' )
					structPatt = 5;
				else
				{
					if(skip != 1)
						printf("Line %d problem is: problem in the field of the number in struct.\n", linenumber);
					problem = 1;	
					skip = 1;
				}
			}
			else if(type == structor)
			{	
				if((structPatt == 5 || structPatt == 6) && (c==' ' || c=='\t'))
					structPatt = 6;
				else if((structPatt == 5 || structPatt == 6)&& (c=='"'))
					structPatt = 7;
				else if((structPatt == 7 || structPatt == 8) && (((c >= 32) && (c <127) && (c != 34)) || (c==' ' || c=='\t')))
					structPatt = 8;
				else if((structPatt == 7 || structPatt == 8) && c == '"')
					structPatt = 9;
				else if((structPatt == 9) && (c==' ' || c=='\t'))
					structPatt = 9;
				else if(skip != 1 && structPatt != 9)
				{
					printf("Line %d problem is: problem in the field of the string in struct.\n", linenumber);
					problem = 1;	
					skip = 1;
				}
			}

			if(type == string)
			{
				if((stringPatt == 0 || stringPatt == 1) && (c==' ' || c=='\t'))
					stringPatt = 1;
				else if((stringPatt == 0 || stringPatt == 1)&& (c=='"'))
					stringPatt = 2;
				else if((stringPatt == 2 || stringPatt == 3) && ((c >= 32 && c <127) || (c==' ' || c=='\t')))
					stringPatt = 3;
				else if((stringPatt == 2 || stringPatt == 3) && c == '"')
					stringPatt = 4;
				else if((stringPatt == 4) && (c==' ' || c=='\t'))
					stringPatt = 4;
				else
				{
					if(skip != 1)
						printf("Line %d problem is: problem with string.\n", linenumber);
					problem = 1;	
					skip = 1;
				}
			} 	
	
			/* Skip extra space. */
			if((c==' ' || c=='\t') && ((node == 0 || count == 2) && stringPatt != 3 && structPatt != 8))
				continue;
			else if((c==' ' || c=='\t') && (node == 1) && (count < 2) && (stringPatt != 3) && (structPatt != 8))
			{
				/* Check if apperance types: .struct/.string/.data */
				line[i] = ' ';
				if(i > 6)
				{
					if(line[i-1]=='g' && line[i-2]=='n' && line[i-3]=='i' && line[i-4]=='r' && line[i-5]=='t' && line[i-6]=='s' && line[i-7]=='.')
						type = string;
					else if(line[i-1]=='t' && line[i-2]=='c' && line[i-3]=='u' && line[i-4]=='r' && line[i-5]=='t' && line[i-6]=='s' && line[i-7]=='.')
						type = structor ;
				}
				if(i > 4)
				{
					if(line[i-1]=='a' && line[i-2]=='t' && line[i-3]=='a' && line[i-4]=='d' && line[i-5]=='.')
						type = data ;
				}
				i++;
				node = 0;
				count++;
			}
			else
			{
				if(i == 82)
				{
					if(skip != 1)
						printf("Line %d problem is: too much characters in a line\n", linenumber);
					problem = 1;
					node = 10;
					skip = 1;
					break;
					
				}
				line[i] = c;
				i++;
				node = 1;		
			}
			
		}
		
		/* Skip and move to the next line. */
		if(i == 82)
			while(((c = fgetc(fp)) != '\0') && (c != '\n') && (c >= 32 && c <127)); 

		if(c == EOF)
			break;

		if(type == data && ((dataPatt != 0) && (dataPatt != 3) && (dataPatt != 4)))
		{
			printf("Line %d problem is: spacing problem in the numbers\n", linenumber);
			problem = 1;
			skip = 1;
		}

		/* Initialize. */
		node = 0;
		count = 0;
		i = 0;
		dataPatt = 0;
		structPatt = 0;	
		stringPatt = 0;

		if(*IC+*DC > MAX_MEM_CAPACITY)
		{
			printf("Line %d problem is: too much characters in a line\n", linenumber);
			problem = 1;
			break;	
		}
		
		if(skip == 1)
		{
			memset(line,0,82);
			linenumber++;
			skip = 0;
			type = 0;
			continue;
		}
	
		if(line[0]!='\0')
		{
			if((type == string) || (type == structor))
			{
				firstpart = strtok(line, " ");
				secondpart = strtok(NULL, " ");
				thirdpart = strtok(NULL, "");
			}
			else
			{
				firstpart = strtok(line, " ");
				secondpart = strtok(NULL, " ");
				thirdpart = strtok(NULL, " ");
			}
				
			
			
			if(strcmp(firstpart, ".entry")==0)
			{
				if(secondpart == NULL && thirdpart == NULL)
				{
					printf("Line %d problem is: Missing label in entry command\n", linenumber);
					problem = 1;
				}
				memset(line,0,82);
				linenumber++;
				type = 0;
				continue;
				
			}

			if(investigation(firstpart,secondpart,thirdpart,&IC,&DC,RamMemory,SymbolMemory,labels,&problem,linenumber))
				problem = 1;
		}
		
		type = 0;
		memset(line,0,82);
		linenumber++;	
	}
	fclose(fp);

	/*The label is data.*/
    	for (i = 0; ((labels + i) -> lit)!=0; i++)
	{
        	if((labels + i) -> attributes[3] == 1)
           		(labels + i) -> value = (labels + i) -> value + *IC;
	}

	if(problem == 1)
	{
		printf("end of problems file : %s\n\n" , path);
		return 0;
	}
	return 1;
}

/* Function will check and devide line into 3 parts.
   While checking the type: data/struct/string/command then send it for thorough check.
   If any error occurred in a line it will return 1, otherwise return 0. */
int investigation(char firstpart[],char secondpart[],char thirdpart[],int **IC, int **DC, int RamMemory[], int SymbolMemory[], label *labels,int *problem, int linenumber)
{
	int i,j,k;
	char commands[][20] = {"mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","get","prn","jsr","rts","hlt"};
	char names[][20] = {"r0","r1","r2","r3","r4","r5","r6","r7",".string",".data", ".struct", ".entry",".extern"};
	int len = strlen(firstpart);
	
    	/* Check if it's a label - if YES check if it's valid. */
	if(firstpart[len-1] == ':')
   	{
		if(len>31)
        	{
			printf("Line %d problem is: the label is too long\n", linenumber);
			return 1;
		}
		firstpart[len-1] = '\0';      /* Replace ':' to EOF. */
		
       	 	/* Check if label is equal to command. */
		for(j = 0; j < 16; j++)
       		{
			if(strcmp(firstpart, commands[j])==0)
           		{
				printf("Line %d problem is: label name is illegal - using command name.\n", linenumber);
				return 1;
			}
		}
		
       		/* Check if label is equal to saved word. */
		for(k = 0; k < 13; k++)
        	{
			if(strcmp(firstpart, names[k])==0)
            		{
				printf("Line %d problem is: label name is illegal - using saved word.\n", linenumber);
				return 1;
			}
		}

		/* Check if label name is legal.*/
		if(isalpha(firstpart[0]))
		{
			for(j = 1 ; j < len - 1; j++)
			{
				if(!(isalnum(firstpart[j])))
				{
					printf("Line %d problem is: label name is illegal - illegal symbol in label.\n", linenumber);
					return 1;
				}				
			}
		}
		else
		{
			printf("Line %d problem is: label name is illegal - using illegal symbol.\n", linenumber);
			return 1;
		}

		/* Check if empty label declaration.*/
		if(secondpart == NULL && thirdpart == NULL)
		{
			printf("Line %d problem is: illegal label - missing arguments.\n", linenumber);
			return 1;
		}
        	
       		/* Check if the command after the label is ".data" or ".string" or ".struct". */
		if(strcmp(secondpart, ".data")==0 || strcmp(secondpart, ".string")==0 || strcmp(secondpart, ".struct")==0)
        	{
			label new;
			strcpy(new.name, firstpart);
			new.value = **DC;	
			new.lit = 1;

			if(addlabel(new, labels , 3))
            		{
				printf("Line %d problem is: the label is defined twice.\n", linenumber);
				return 1;
			}
			
			if((**IC+**DC+(strlen(secondpart))) < MAX_MEM_CAPACITY+1)	
			{
				if(thirdpart != NULL)
				{		
					if(strcmp(secondpart, ".data")==0)
						**DC = tosymboldata(thirdpart, SymbolMemory , **DC, &problem, linenumber);
					else if(strcmp(secondpart, ".string")==0)
						**DC = tosymbolstring(thirdpart, SymbolMemory , **DC, &problem, linenumber);
					else	/* .struct */
						**DC = tosymbolstruct(thirdpart, SymbolMemory , **DC, &problem, linenumber);
				}
				else
				{
					printf("Line %d problem is: Missing Argument in \'%s\'.\n", linenumber,secondpart);
					*problem = 1;
					return 1;
				}
			}
			else
           		{
			    printf("Line %d problem is: out of memory range.\n", linenumber);
			    return 1;
			}
			return 0;
		}		

        	/* Check if the command after the label is a command. */
		for(i = 0; i< 16; i++)
        	{
			if(strcmp(secondpart, commands[i])==0)   
            		{
				/* Define new label. */
				label new;
				strcpy(new.name, firstpart);
				new.value = **IC;
				new.lit = 1;

				if(addlabel(new, labels , 0))
                		{
					printf("Line %d problem is: the label is defined twice\n", linenumber);
					return 1;
				}

				checkComma(firstpart,secondpart,thirdpart,2,&IC, RamMemory, labels, 1, &problem, linenumber);
				return 0;
			}
		}
	}

	/* Check if the command is ".data" / ".string" / ".struct" without label. */
	if(strcmp(firstpart, ".data")==0 || strcmp(firstpart, ".string")==0 || strcmp(firstpart, ".struct")==0)
    	{
		if(secondpart == NULL && thirdpart == NULL)
		{
			printf("Line %d problem is: Missing Argument in \'%s\'.\n", linenumber,firstpart);
			*problem = 1;
			return 1;
		}

		if(thirdpart != NULL)
			secondpart = mergeTwoArrays(secondpart,thirdpart);

		if((**IC+**DC+(strlen(secondpart))) < MAX_MEM_CAPACITY+1)
		{
			if(strcmp(firstpart, ".data")==0 )
				**DC = tosymboldata(secondpart, SymbolMemory , **DC, &problem, linenumber);
			else if(strcmp(firstpart, ".string")==0 )	
				**DC = tosymbolstring(secondpart, SymbolMemory , **DC, &problem, linenumber);
			else	/* .struct */
				**DC = tosymbolstruct(secondpart, SymbolMemory , **DC, &problem, linenumber);
		}
		else
        	{
			printf("Line %d problem is: out of memory range.\n", linenumber);
			return 1;
		}
		return 0;
	}

    	/* Check if the command is ".extern". */
	if((strcmp(firstpart, ".extern")==0) && (secondpart == NULL) && (thirdpart == NULL))
	{
		printf("Line %d problem is: Missing label in extern command\n", linenumber);
		return 1;
	}
	else if(strcmp(firstpart, ".extern")==0)
    	{
		label new;
		strcpy(new.name, secondpart);
		new.value = 0;	
		new.lit = 1;

		if(addlabel(new, labels , 2))
        	{
			printf("Line %d problem is: the label is defined twice.\n", linenumber);
			return 1;
		}
		return 0;
	}
	
    	/* Check if the command is a formal command. */
	for(i = 0; i< 16; i++)
    	{
		if(strcmp(firstpart, commands[i])==0)
        	{
			if(thirdpart != NULL && secondpart != NULL)
			{
				  if(thirdpart[0] != ',' && secondpart[strlen(secondpart)-1] != ',')
  				  {
					printf("Line %d problem is: Missing comma in arguments\n", linenumber);
					*problem = 1;
  				  }
				  secondpart = mergeTwoArrays(secondpart,thirdpart);
			}

			checkComma(firstpart,secondpart,thirdpart,1,&IC, RamMemory, labels, 1, &problem, linenumber);
			return 0;	
		} 
	}
	
	/* If none of the options above worked we return error. */
	printf("Line %d problem is: unknown command\n", linenumber);
	return 1;
}

/* Function will check comma in the given line. At the end it
   will send the parts into checkOperand function for final check. */
void checkComma(char firstpart[],char secondpart[],char thirdpart[], int labelOrNot,int ***IC, int RamMemory[],label *labels,int firstOrSecondPass, int **problem , int linenumber)
{
	char *firstoperand = (char*)malloc(sizeof(char));
	char *secondoperand = (char*)malloc(sizeof(char));		

	char a;
	int word = 0; 
	int len = strlen(secondpart);

	/* Represent indexes. */
	int i = 0,j= 0, k = 0;
	
	firstoperand[0] = '\0';
	secondoperand[0] = '\0';

	/* Check if formal command. */
	if(labelOrNot == 1)
	{ 
		if(secondpart != NULL)
		{
			/* Seperate second part into two operands. */
			while((a = secondpart[k])!= '\0')
			{
				/* Check if comma is in wrong place. */
				if(a == ',' && (word == 0 || word == 2 || word == 3 || a == secondpart[len-1]))
				{
					printf("Line %d problem is: comma in the wrong place\n", linenumber);
					**problem = 1;
					break;
				}

				/* Switch to reading second operand. */
				if(a == ',' && word == 1)
				{
					word = 2;
					k++;
				}

				/* Reading and save first operand. */
				if(a != ',' && (word == 0 || word == 1))
				{
					firstoperand[i] = a;
					i++;
					k++;
					word = 1;
					firstoperand = (char *) realloc(firstoperand, (sizeof(char)*i));
				}

				/* Reading and save second operand. */
				if(a != ',' && (word == 2 || word == 3))
				{
					secondoperand[j] = a;
					j++;
					k++;
					word = 3;
					secondoperand = (char *) realloc(secondoperand, (sizeof(char)*j));
				}						
			}
		}
		
		if(secondoperand[0] != '\0')
		{
			/* There is two operands to send to checkOperand. */	
			firstoperand[i] = '\0';
			secondoperand[j] = '\0';
			checkOperand(firstpart,firstoperand,secondoperand,&IC, RamMemory,labels, firstOrSecondPass, linenumber, &problem);
		}
		else if (firstoperand[0] != '\0' )
		{
			/* There is one operand to send to checkOperand. */	
			firstoperand[i] = '\0';
			secondoperand = NULL;
			checkOperand(firstpart,secondoperand,firstoperand,&IC, RamMemory,labels, firstOrSecondPass, linenumber, &problem);
		}
		else
		{
			/* There is no operands to send to checkOperand. */	
			firstoperand = NULL;
			secondoperand = NULL;
			checkOperand(firstpart,secondoperand,firstoperand,&IC, RamMemory,labels, firstOrSecondPass, linenumber, &problem);
		}
	}	
	else if(labelOrNot == 2)	/* Check if command after label. */
	{ 
		if(thirdpart != NULL)
		{
			while((a = thirdpart[k])!= '\0')
			{
				/* Check if comma is in wrong place. */
				if(a == ',' && (word == 0 || word == 2 || word == 3))
				{
					printf("Line %d problem is: comma in the wrong place\n", linenumber);
					**problem = 1;
					break;
				}

				/* Switch to reading second operand. */
				if(a == ',' && word == 1)
				{
					word = 2;
					k++;
				}
				
				/* Reading and save first operand. */
				if(a != ',' && (word == 0 || word == 1))
				{
					firstoperand[i] = a;
					i++;
					k++;
					word = 1;
					firstoperand = (char *) realloc(firstoperand, (sizeof(char)*i));
				}

				/* Reading and save second operand. */
				if(a != ',' && (word == 2 || word == 3))
				{
					secondoperand[j] = a;
					j++;
					k++;
					word = 3;
					secondoperand = (char *) realloc(secondoperand, (sizeof(char)*j));
				}
			}
		}
		/* Check if there is no second operand after the comma. */
		if(word == 2)
		{
			printf("Line %d problem is: no second operand\n", linenumber);
			**problem = 1;
		}

		if( secondoperand[0] != '\0')
		{
			/* There is two operands to send to checkOperand. */
			firstoperand[i] = '\0';
			secondoperand[j] = '\0';
			checkOperand(secondpart,firstoperand,secondoperand,&IC, RamMemory,labels, firstOrSecondPass, linenumber, &problem);
		}
		else if (firstoperand[0] != '\0' )
		{
			/* There is one operand to send to checkOperand. */	
			firstoperand[i] = '\0';
			secondoperand = NULL;
			checkOperand(secondpart,secondoperand,firstoperand,&IC, RamMemory,labels, firstOrSecondPass, linenumber, &problem);
		}
		else
		{
			/* There is no operands to send to checkOperand. */	
			firstoperand = NULL;
			secondoperand = NULL;
			checkOperand(secondpart,secondoperand,firstoperand,&IC, RamMemory,labels, firstOrSecondPass, linenumber, &problem);
		}
		
	}
		
	/* Free allocated memory. */
	free(firstoperand);
	free(secondoperand);
}

/* tosymbolstring function will take data from thirdpart and send it to SymbolMemory, return current DC. */
int tosymbolstring(char thirdpart[], int SymbolMemory[] ,int DC, int **problem, int linenumber)
{
	int i ;  /* Skip first character: " .*/
	int len = strlen(thirdpart);

	if((len != 1) && (thirdpart[0] == '"') && (thirdpart[len -1] == '"'))
    	{
		thirdpart[len -1] = '\0';  /* Replace last character from: " ,to: '\0'. */

		for (i = 1; thirdpart[i]!= '\0'; i++, DC++)
			SymbolMemory[DC] = (int)thirdpart[i];
		
		SymbolMemory[DC] = 0;   /* Last word is zero. */
		DC++;
	}
	else
    	{
		printf("Line %d problem is: Unvalid .string statment.\n", linenumber);
		**problem = 1;
	}
	return DC;
}

/* tosymboldata function will take data from thirdpart and send it to SymbolMemory, return current DC. */
int tosymboldata(char thirdpart[], int SymbolMemory[] , int DC , int **problem, int linenumber)
{
	int i = 0,j = 0, p = 0,warning = 0, skip = 0;
	char num[5];
	int numbers[38] = {0};

	while(thirdpart[i]!= '\0')
    	{
		while(thirdpart[i]!= ',' && thirdpart[i]!= '\0')
        	{
			warning = 0;
			if(isdigit(thirdpart[i]) || thirdpart[i] == '-' || thirdpart[i] == '+')
            		{
				num[j] = thirdpart[i];
				j++;
			}
			i++;
		}
		numbers[p] = atoi(num);

		/* If there is ',' in the end and no numbers after.*/
        	if(thirdpart[i] == ',' )
			warning = 1; 
		
        	/* Max number in 10 digits is 1023. */
		if(numbers[p]>1023 && skip != 1)
        	{
			printf("Line %d problem is: problem in '.data' - number too large\n", linenumber);
			**problem = 1;
			skip = 1;
		}
		p++;
		i++;
		j=0;
		memset(num,0,4);
	}
	if(warning == 1 && skip != 1)
	{
		printf("Line %d problem is: problem in '.data' - comma in wrong place\n", linenumber);
		**problem = 1;
	}

	for (i = 0; i < (p); i++, DC++)
		SymbolMemory[DC] = numbers[i];

	return DC;
}

/* tosymbolstruct function will take data from thirdpart and send it to SymbolMemory, return current DC. */
int tosymbolstruct(char thirdpart[], int SymbolMemory[] , int DC , int **problem, int linenumber)
{
	int i ;
	int len = strlen(thirdpart);
	int number;
	char num[5];
	int count = 0;
	
	/* Here we save the digit in struct field. */
	for (i = 0; (thirdpart[i]!= '\0')&&(thirdpart[i]!= ','); i++)
	{
		if(isdigit(thirdpart[i]) || thirdpart[i] == '-' || thirdpart[i] == '+')
        	{
			num[count] = thirdpart[i];
			count++;
		}
	}
	number = atoi(num);
	
	/* Max number in 10 digits is 1023. */
	if(number>1023)
    	{
		printf("Line %d problem is: number too large\n", linenumber);
		**problem = 1;
	}
	SymbolMemory[DC] = number;
	DC++;
	i++;

	/* Trim left side */
	while(thirdpart[i] != '\0' && (thirdpart[i] == ' ' || thirdpart[i] == '\t'))
		i++;

	/* Here we save the string in struct field. */
	if( (i != (len-1)) && (thirdpart[i] == '"') && (thirdpart[len-1] == '"'))
    	{
		/* Replace last character from: " ,to: '\0'. */
		thirdpart[len-1] = '\0'; 
		i++;
		
		for (; thirdpart[i]!= '\0'; i++, DC++)
			SymbolMemory[DC] = (int)thirdpart[i];

		SymbolMemory[DC] = 0;   /* Last word is zero. */
		DC++;
	}
    	else
    	{
		printf("Line %d problem is: Unvalid struct.\n", linenumber);
		**problem = 1;
	}
	memset(num,0,5);
	return DC;
}

/* addlabel function will add new label into the dynamic labels array
   If label already exists in array return 1, otherwise add label and return 0. */
int addlabel(label new,label *labels, int attribute)
{
	int i;
	
	for (i=0; ((labels + i) -> lit)!=0; i++)
		if(strcmp((labels + i) -> name, new.name)==0)
			return 1;

	strcpy(((labels + i) -> name), new.name);
	(labels + i) -> value = new.value;		
	(labels + i) -> lit = 1;
	(labels + i) -> attributes[attribute] = 1;

	labels = (label *) malloc((i+3)*sizeof(label)); /* Make array bigger. */
	if(labels == NULL)
		exit(1);
	
	return 0;
}

/* ToMemoryData function will recieve each line data and insert into memory data the command correctly in bits, return current IC. */
int ToMemoryData(int sourceAddress, int destinationAddress, int opcode, int ogerSource,int ogerTarget, int numForTav, int secondnumForTav, char tavNameFirstOperand[], char tavNameSecondOperand[],int field,int IC, int RamMemory[])
{
	/* One operand - commands: not, clr, inc, dec, jmp, bne, get, prn, jsr. */
	if(sourceAddress == (-1) && destinationAddress != (-1))  
	{	
		
		/* First word - command and method. */
		RamMemory[IC] = ((destinationAddress<<2)|RamMemory[IC]);
		RamMemory[IC] = ((opcode<<6)|RamMemory[IC]);
		/* Move to new word. */
		IC = IC + 1;	
	
		/****************************** Check destination address. ******************************/
		if(destinationAddress == 0)
		{
			
			/* Command with immediate number.*/	
			RamMemory[IC] = ((secondnumForTav<<2)|RamMemory[IC]);
			IC = IC + 1;
		}
		else if(destinationAddress == 1 )										
		{			
			/* Command with a label.*/									
			IC = IC + 1;	/* Skip for label address. */
		}
		else if(destinationAddress == 2)
		{
			/* Command with a label struct.*/	
			IC = IC + 1;	/* Skip for label address. */
			RamMemory[IC] = ((field<<2)|RamMemory[IC]);
			IC = IC + 1;
		}
		else if(destinationAddress == 3)
		{
			/* Command with a destination register only.*/
			RamMemory[IC] = ((ogerTarget<<2)|RamMemory[IC]);
			IC = IC + 1;
		}	
	}
	else if(sourceAddress == (-1) && destinationAddress == (-1)) /* No operands - commands : rts, hlt. */
	{
		/* First word - command and method. */
		RamMemory[IC] = ((opcode<<6)|RamMemory[IC]);
		IC = IC + 1;
	}	
	else	/* Two operands - commands : mov, cmp, add, sub, lea. */
	{
		/* First word - command and method. */
		RamMemory[IC] = ((destinationAddress<<2)|RamMemory[IC]);
		RamMemory[IC] = ((sourceAddress<<4)|RamMemory[IC]);
		RamMemory[IC] = ((opcode<<6)|RamMemory[IC]);
		/* Move to new word. */
		IC = IC + 1;

		/****************************** Check source address. ******************************/
		if(sourceAddress == 0)
		{
			/* Command with a immediate number.*/	
			RamMemory[IC] = ((numForTav<<2)|RamMemory[IC]);
			IC = IC + 1;
		}
		else if(sourceAddress == 1)
		{
			/* Command with a label.*/
			IC = IC + 1;	/* Skip for label address. */
		}
		else if(sourceAddress == 2)
		{
			/* Command with a label struct.*/
			IC = IC + 1;	/* Skip for label address. */
			RamMemory[IC] = ((field<<2)|RamMemory[IC]);
			IC = IC + 1;
		}
		else if(sourceAddress == 3 && destinationAddress == 3)
		{
			/* Command with a source register and destination register.*/
			RamMemory[IC] = ((ogerTarget<<2)|RamMemory[IC]);
			RamMemory[IC] = ((ogerSource<<6)|RamMemory[IC]);
			IC = IC + 1;
		}
		else if(sourceAddress == 3 && destinationAddress != 3)
		{
			/* Command with a source register only.*/
			RamMemory[IC] = ((ogerSource<<6)|RamMemory[IC]);
			IC = IC + 1;
		}

		/****************************** Check destination address. ******************************/
		if(destinationAddress == 0)
		{
			/* Command with a immediate number.*/
			RamMemory[IC] = ((secondnumForTav<<2)|RamMemory[IC]);
			IC = IC + 1;
		}
		else if(destinationAddress == 1)
		{
			/* Command with a label.*/
			IC = IC + 1;	/* Skip for label address. */
		}
		else if(destinationAddress == 2)
		{
			/* Command with a label struct.*/
			IC = IC + 1;	/* Skip for label address. */
			RamMemory[IC] = ((field<<2)|RamMemory[IC]);
			IC = IC + 1;
		}
		else if(destinationAddress == 3 && sourceAddress != 3)
		{
			/* Command with a destination register only.*/
			RamMemory[IC] = ((ogerTarget<<2)|RamMemory[IC]);
			IC = IC + 1;
		}
	}
	return IC;
}

/* Function insert each charcter from b[] into a[] without getting out of range ,return result.*/
char  *mergeTwoArrays(char *a, char *b)
{
	int i = 0, j = 0;
	int len = strlen(a) + strlen(b) + 1;
	int cont = 0;
	char *result = (char *)malloc(len *sizeof(char*));

	for(i = 0; i < strlen(a); i++) 
    		*(result+i) = *(a+i);

	for(j = i; j < strlen(b)+ strlen(a); j++, cont++)
		*(result+j) = *(b + cont);
  
	*(result+len-1) = '\0';
	return result;
}


