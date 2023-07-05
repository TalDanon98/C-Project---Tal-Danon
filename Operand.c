#include "Main.h"

enum status{NO, YES};

/* The function checks that the given line can be executed.
   First we will check whether the command is correct.
   If everything is correct, we will continue to check the rest of the line based on the command.
   If there is an error in the process, it will write the cause and location (line) of the problem.
   Lastly, common language about our methods: immediate = 0, direct = 1, access = 2, register direct = 3.
*/
void checkOperand(char command[], char firstoperand[], char secondoperand[], int**** IC, int RamMemory[], label *labels, int firstOrSecondPass,int linenumber,int ***problem)
{
    	int sourceAddress = 0, destinationAddress = 0;
    	int opcode = 0;
    	int ogerSource = 0, ogerTarget = 0;
    	int firstImdNum = 0, secondImdNum = 0;
    	int field = 0;
    	int skip = 0;
    	char temp[1];
	
    	/* Save label name for the first and second operands. */
    	char firstLabelName[80];
    	char secondLabelName[80];
  
    	/* Initialize to zero. */
    	memset(firstLabelName, 0, 80);
    	memset(secondLabelName, 0, 80);
    	memset(temp, 0, 1);

    	/* First group of commands (two operands): mov, cmp, add, sub, lea. */
    	if((firstoperand != NULL) && (secondoperand != NULL))
    	{
        	if (strcmp(command, "mov") == 0 || strcmp(command, "add") == 0 || strcmp(command, "sub") == 0)
        	{
            		/* Check which command name is it, and set correct opcode. */
            		if (strcmp(command, "mov") == 0)
				opcode = 0;
            		else if (strcmp(command, "add") == 0)
				opcode = 2;
	    		else if (strcmp(command, "sub") == 0)
				opcode = 3;


            		/* Check the first operand. */
            		if (firstoperand[0] == '#')     /* Immediate method. */ 
			{
				sourceAddress = 0;
				firstImdNum = isItValidNum(firstoperand,linenumber,&problem,&skip);
					
			}
			else if (isItRegister(firstoperand) == 0)    /* Register direct method. */
			{
				sourceAddress = 3;
				temp[0] = firstoperand[1];
				ogerSource = atoi(temp);
			}
			else   /* Direct or Access method. */
			{
				directOrAccess(firstoperand,firstLabelName,&sourceAddress,&field,linenumber,&problem,&skip,firstOrSecondPass,labels);
			}

            		/* Check the second operand. */
			if(skip != 1)
			{	
				if (secondoperand[0] == '#')   /* Immediate method. */ 
				{
					printf("Line %d problem is: immediate method is invalid for %s in second operand.\n", linenumber,command);
					***problem = 1;
					skip =1;
					return;
				}
				else if (isItRegister(secondoperand) == 0)    /* Register direct method. */
				{	
					destinationAddress = 3;
					temp[0] = secondoperand[1];
					ogerTarget = atoi(temp);
				}
				else   /* Direct or Access method. */
				{
					directOrAccess		(secondoperand,secondLabelName,&destinationAddress,&field,linenumber,&problem,&skip,firstOrSecondPass,labels);
				}
			}
    		}

        	if (strcmp(command, "cmp") == 0)
		{
            		opcode = 1;

            		/* Check the first operand. */
            		if (firstoperand[0] == '#')     /* Immediate method. */ 
	    		{
				sourceAddress = 0;
				firstImdNum = isItValidNum(firstoperand,linenumber,&problem,&skip);
			}
			else if (isItRegister(firstoperand) == 0)    /* Register direct method. */
			{
				sourceAddress = 3;
				temp[0] = firstoperand[1];
				ogerSource = atoi(temp);
				
			}
			else    /* Direct or Access method. */
			{
				directOrAccess(firstoperand,firstLabelName,&sourceAddress,&field,linenumber,&problem,&skip,firstOrSecondPass,labels);
			}
			if(skip != 1)
			{
            			/* Check the second operand. */
            			if (secondoperand[0] == '#')   /* Immediate method. */ 
				{
					destinationAddress = 0;
					secondImdNum = isItValidNum(secondoperand,linenumber,&problem,&skip);
				} 
				else if (isItRegister(secondoperand) == 0)    /* Register direct method. */
				{
					destinationAddress = 3;
					temp[0] = secondoperand[1];
					ogerTarget = atoi(temp);
				}
				else   /* Direct or Access method. */
				{
					directOrAccess(secondoperand,secondLabelName,&destinationAddress,&field,linenumber,&problem,&skip,firstOrSecondPass,labels);
				}
	    		}	
        	}

        	if(strcmp(command, "lea") == 0)
        	{
           		 opcode = 6;

            		/* Check the first operand. */
            		if (firstoperand[0] == '#')     /* Immediate method. */ 
	    		{
				printf("Line %d problem is: immediate method is invalid for %s in first operand\n", linenumber,command);
				***problem = 1;
				skip =1;
				return;
	    		}
			else if (isItRegister(firstoperand) == 0)    /* Register direct method. */
			{
				printf("Line %d problem is: register direct method is invalid for %s in first operand\n", linenumber,command);
				***problem = 1;
				skip =1;
				return;
			}
			else    /* Direct or Access method. */
			{
				directOrAccess(firstoperand,firstLabelName,&sourceAddress,&field,linenumber,&problem,&skip,firstOrSecondPass,labels);
			}

            		/* Check the second operand. */
	    		if(skip != 1)
	    		{
            			if (secondoperand[0] == '#')   /* Immediate method. */ 
	    			{
                			printf("Line %d problem is:, immediate method is invalid for %s in second operand.\n", linenumber,command);
					***problem = 1;
					skip =1;
					return;
				} 
				else if (isItRegister(secondoperand) == 0)    /* Register direct method. */
				{
					destinationAddress = 3;
					temp[0] = secondoperand[1];
					ogerTarget = atoi(temp);
				}
				else   /* Direct or Access method. */
				{
					directOrAccess(secondoperand,secondLabelName,&destinationAddress,&field,linenumber,&problem,&skip,firstOrSecondPass,labels);
				}
	    		}
        	}
    	}

	
    	/* Second group of commands (one operands): not, clr, inc, dec, jmp, bne, get, prn, jsr. */
    	if((firstoperand == NULL) && (secondoperand != NULL))
    	{
        	sourceAddress = -1;

        	if (strcmp(command, "not") == 0 || strcmp(command, "clr") == 0 || strcmp(command, "inc") == 0 || 
            	strcmp(command, "dec") == 0 || strcmp(command, "jmp") == 0 || strcmp(command, "bne") == 0 || 
            	strcmp(command, "get") == 0 || strcmp(command, "jsr") == 0)
		{
            		/* Check which command name is it, and set correct opcode. */
			if (strcmp(command, "not") == 0)
				opcode = 4;
			else if (strcmp(command, "clr") == 0)
				opcode = 5;
			else if (strcmp(command, "inc") == 0)
				opcode = 7;
			else if (strcmp(command, "dec") == 0)
				opcode = 8;
			else if (strcmp(command, "jmp") == 0)
				opcode = 9;
			else if (strcmp(command, "bne") == 0)
				opcode = 10;
			else if (strcmp(command, "get") == 0)
				opcode = 11;
			else if (strcmp(command, "jsr") == 0)
				opcode = 13;

            		/* Check the second operand. */
            		if (secondoperand[0] == '#')    /* Immediate method. */
			{
				printf("Line %d problem is: immediate method is invalid for %s \n", linenumber,command);
				***problem = 1;
				skip =1;
				return;
			}
			else if (isItRegister(secondoperand) == 0)    /* Register direct method. */
			{ 
				destinationAddress = 3;
				temp[0] = secondoperand[1];
				ogerTarget = atoi(temp);
			}
            		else    /* Direct or Access method. */
            		{
                		directOrAccess(secondoperand,secondLabelName,&destinationAddress,&field,linenumber,&problem,&skip,firstOrSecondPass,labels);
            		}
        	}

        	if(strcmp(command, "prn") == 0)
        	{
            		opcode = 12;

            		/* Check the second operand. */
            		if (secondoperand[0] == '#')    /* Immediate method. */
	    		{
				destinationAddress = 0;
				secondImdNum = isItValidNum(secondoperand,linenumber,&problem,&skip);
	    		}
			else if (isItRegister(secondoperand) == 0)    /* Register direct method. */
			{
				destinationAddress = 3;
				temp[0] = secondoperand[1];
				ogerTarget = atoi(temp);
			}
            		else    /* Direct or Access method. */
            		{
                		directOrAccess(secondoperand,secondLabelName,&destinationAddress,&field,linenumber,&problem,&skip,firstOrSecondPass,labels);
            		}
        	}
    	}

    	/* Third group of commands (no operands): rts, hlt. */
    	if((firstoperand == NULL) && (secondoperand == NULL))
    	{
        	sourceAddress = -1;
		destinationAddress = -1;

		if (strcmp(command, "rts") == 0)
			opcode = 14;
		else if (strcmp(command, "hlt") == 0)
			opcode = 15;
    	}

    	/* Check if there is too many operands for the command. */
    	if ((opcode == 0) && (strcmp(command, "mov") != 0))
    	{
	    	printf("Line %d problem is: Illegal amount of operands for %s\n", linenumber, command);
	    	***problem = 1;
	    	skip = 1;
	    	return;
    	}

    	if (firstOrSecondPass == 1) 
    	{
	    	****IC = ToMemoryData(sourceAddress, destinationAddress, opcode,ogerSource, ogerTarget, firstImdNum, secondImdNum, firstLabelName, secondLabelName,field, ****IC, RamMemory);
    	}
    	else 
    	{
	    	****IC = ToMemoryData2(sourceAddress, destinationAddress, opcode, ogerSource, ogerTarget, firstImdNum, secondImdNum, firstLabelName, secondLabelName,field, ****IC, RamMemory, labels);
    	}
}

/* Function check if given string is a valid number for immediate method.
   If there is an error return 0. otherwise return the number in the method. */
int isItValidNum(char operand[],int linenumber,int ****problem,int *skip)
{
    	int i = 1, counter = 0;
    	char str[80];
    	memset(str, 0, 80);
    
    	if(operand[i] == '-')
    	{
       		str[counter] = operand[i];
		counter++;
		i++;
    	}
    	else if(operand[i] == '+')
        	i++;

    	/* Check each character if it's a digit. */
    	for (; i < strlen(operand); i++)
    	{
        	if(isdigit(operand[i]))
        	{
            		str[counter] = operand[i];
	    		counter++;
        	}
        	else
		{
			printf("Line %d problem is: %s is invalid number in immediate method.\n", linenumber,operand);
			****problem = 1;
			*skip =1;
			return NO;
		}
    	}
    	/* 255 represent the biggest number that we could make in 8 bits*/
    	if(atoi(str) > 255)
    	{
		printf("Line %d problem is: %s is invalid number in immediate method - out of range.\n", linenumber,operand);
		****problem = 1;
		*skip =1;
		return NO;
    	}
    	return atoi(str);
}

/* Function check if given string is valid register.
   If string is a register return 0. otherwise return 1. */
int isItRegister(char operand[])
{
    	int i = 0, Num = 0;
	char str[80];
	memset(str,0,80);
	if(operand[0] == 'r')
	{
		for (i = 1; i < strlen(operand); i++)
			str[i-1] = operand[i];

		Num = atoi(str);	
		if(Num > 7)
			return 1;
		return 0;
	}
	return 1;
} 

/* Function check if given string is valid access method.
   If string is valid return YES. otherwise return NO and print error messege. */
int isItAccess(char operand[], int linenumber, int *****problem,int **skip)
{
    	int i , j = 0;
    	char str[80];
    	int num = 0;
    	int afterDot = NO;

    	/*Check if there is a DOT*/
    	for(i = 0 ; i < strlen(operand) && afterDot != YES ; i++)
    	{
		if(operand[i] =='.')
			afterDot = YES;	
    	}	
    	/*Check if there is DOT '.' and  if No is direct method else access*/
    	if(afterDot == NO)
		return NO;
	
    	/* Check if valid access method*/
    	for (; i < strlen(operand); i++,j++)
	    	str[j] = operand[i];
    
    	str[j] = '\0';
    	num = atoi(str);
    	if(num != 1 && num != 2)
    	{
	   	printf("Line %d problem is: invalid number of field in access method in %s\n", linenumber, operand);
	    	*****problem = 1;
	    	**skip =1;
    	}	
    	return YES;			 
}

/* Function check if given string is a valid string for direct method. */
void isItDirect(char operand[], char labelName[],int linenumber,int *****problem,int **skip)
{
	int i;

	if(isalpha(operand[0]))
	{
		for ( i = 0; i < strlen(operand); i++)
		{
			if(isalnum(operand[i]))
				labelName[i] = operand[i];
			else
			{
				printf("Line %d problem is: invalid label in direct method in \'%s\'\n", linenumber, operand);
				*****problem = 1;
				**skip =1;

				memset(labelName,0,80);
				strcpy(labelName,operand);
           			break;
			}
		}
		
	}
	else
	{
		printf("Line %d problem is: invalid label in direct method in \'%s\'\n", linenumber, operand);
		*****problem = 1;
		**skip =1;	
	}
}

/* Function will check and sort string into two seperate parameters: label and field. 
Label is the name of the structor and field (1 or 2) contain the location number in structor.
If operand is invalid, print an error messege. */
void labelAndField(char operand[], char labelName[], int **field, int linenumber, int *****problem , int **skip)
{
    	int i;
    	char temp[1];
    
    	for (i = 0; i < strlen(operand); i++)
    	{
        	if(isalnum(operand[i]))
            		labelName[i] = operand[i];
        	else if(operand[i] == '.')
        	{
            		temp[0] = operand[i+1];
	    		**field = atoi(temp);
            		break;
        	}
        	else
        	{
            		printf("Line %d problem is: invalid label in Access method in %s\n", linenumber, operand);
			*****problem = 1;
			**skip =1;

			memset(labelName,0,80);
			strcpy(labelName,operand);
            		break;
        	}   
    	} 
}

/*Function check if given label at secondpass is already exists , if not print error messege.*/
void isItValidLabel(char labelName[],int linenumber, int *****problem,int **skip,int firstOrSecondPass , label *labels)
{
	int i;
	if(firstOrSecondPass == 2)
	{
		for(i= 0; (labels[i].lit) != 0; i++)
			if(strcmp(labelName ,labels[i].name) == 0)
				break;

		if((labels[i].lit) == 0)
		{
			printf("Line %d problem is: invalid label - Label \'%s\' not found. \n", linenumber, labelName);
			*****problem = 1;
	   		**skip =1;
		}		
	}

}

/* Function check which operand method is it: direct or access.
   If it's direct method return 0, otherwise it's access method return 1. */
void directOrAccess(char operand[], char labelName[], int *address, int *field, int linenumber, int ****problem,int *skip,int firstOrSecondPass,label *labels)
{
    	if(isItAccess(operand, linenumber, &problem,&skip)) /* Access method. */
	{
       		*address = 2;
		if(*skip != 1)
		{
			labelAndField(operand, labelName, &field, linenumber, &problem,&skip);
			isItValidLabel(labelName,linenumber,&problem,&skip,firstOrSecondPass, labels);
		}
	}
	else    /* Direct method. */
	{
		*address = 1;
		isItDirect(operand, labelName,linenumber,&problem,&skip);
		isItValidLabel(labelName,linenumber,&problem,&skip,firstOrSecondPass, labels);
	}
}

