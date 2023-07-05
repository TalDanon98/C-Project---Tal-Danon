#include "Main.h"

/* PreAssembler function recieve original file and argc.
orgFileName - represent the name of the original file name.
argc - represent the numer of arguments in the command line.
The function goes through the original file and make new file with the ending ".am".
The new file (".am") contains the original file after removing macros and switch into thier data.
Function will also print an error messeges if there is any.
*/
void PreAssembler(char *orgFileName, int argc)
{
    FILE *beforeMacrosPointer;     	 /* Pointer to current file with macros. */
    FILE *afterMacrosPointer;   	/* Pointer to new file without macros. */

    char commands[21][8] = {"mov","cmp","add","sub","lea","not","clr","inc","dec","jmp","bne","get","prn","jsr","rts","hlt",
			    ".data",".string",".struct",".entry",".extern"};

    char ch;
    int i = 0, j = 0, macroIndx = 0, macroCounter = 0;
    int flag = 0, rol = 0, ptr = 0;
    int count = 0;

    Macro* macros = (Macro*)malloc(sizeof(Macro));
	
    char currLine[MAX_LINE_LEN];
    char copyLine[MAX_LINE_LEN];
    char fileNameOrg[MAX_LINE_LEN];
    char fileNameEnd[MAX_LINE_LEN];
    char nameForMacro[MAX_LINE_LEN];

    char as[] = ".as";
    char am[] = ".am";

    
    /* Initialize arrays to null. */
    memset(fileNameOrg, '\0', MAX_LINE_LEN);
    memset(fileNameEnd, '\0', MAX_LINE_LEN);
    memset(currLine, '\0', MAX_LINE_LEN);
    memset(copyLine, '\0', MAX_LINE_LEN);
    memset(nameForMacro, '\0', MAX_LINE_LEN);

    /* Set name of new files as the same with different adding at end. */
    strcpy(fileNameOrg, orgFileName);
    strcpy(fileNameEnd, orgFileName);
    strcat(fileNameOrg, as);
    strcat(fileNameEnd, am);
	
    /* Check if a file is exist. */
    if (access(fileNameOrg, F_OK) != 0)
    {
	    printf("%s is not exist.\n", fileNameOrg);
	    exit(0);
    }
	
    beforeMacrosPointer = fopen(fileNameOrg, "r");
    afterMacrosPointer = fopen(fileNameEnd, "w+");

    if (!macros)
    {
	    printf("Could not make a pointer.\n");
	    exit(0);
    }

    /* Check if we unable to find such file name. */
    if (!beforeMacrosPointer || !afterMacrosPointer)
    {
	    perror("No such file.\n");
	    exit(0);
    }
   	
    /* Check each line in original file, to identify the macros. */
    while (fgets(currLine, MAX_LINE_LEN, beforeMacrosPointer) != NULL)
    {
	    ptr = 0;

            /* First occurrence of substring "macro" only. */
	    if (((strstr(currLine, "macro ")) != NULL) && ((strstr(currLine, "endmacro")) == NULL))
	    {

		for (i = 0; i < strlen(currLine); i++)
		{
			
			ch = currLine[i];

			if (ch == (int)"\n")
			{
				break;
			}
			
			switch (ch)
			{
			case 'm':
				if (flag == 0)
				{
					flag = 1;
				}
				break;
			case 'a':
				if (flag == 1)
				{
					flag = 2;
				}
				break;
			case 'c':
				if (flag == 2)
				{
					flag = 3;
				}
				break;
			case 'r':
				if (flag == 3)
				{
					flag = 4;
				}
				break;
			case 'o':
				if (flag == 4)
				{
					flag = 5;
				}
				break;
			case ' ':
				if (flag == 5)
				{
					flag = 6;
				}
				break;

			}

                        /* Check if we have reached a macro. */
			if (flag == 6)
			{
				flag = 7;

                                /* Allocate more memory for the macros. */
				if (macroCounter != 0)
				{
					macros = (Macro*)realloc(macros, (macroCounter + 1) * sizeof(Macro));

					if (!macros)
					{
						printf("Could not make a pointer.\n");
						exit(0);
					}
				}
				continue;
			}

                	/* Save macro name into macroName field. */
			if (flag == 7)
			{
				if (ch == '\n')
					continue;
				(*(macros + macroCounter)).macroName[ptr++] = ch;
				count = ptr;
			}

		}
                /* Allocate memory for macro data. */
		if (flag == 7)
		{
			(*(macros + macroCounter)).macroData = (char*)malloc(sizeof(currLine));
			if (!(*(macros + macroCounter)).macroData)
			{
				printf("Could not make a pointer.\n");
				exit(0);
			}
			rol = 1;
			continue;
		}
	}

	(*(macros + macroCounter)).macroName[count] = '\0';

        /* Check if we have reached end of macro. */
	if (((strstr(currLine, "endmacro")) != NULL) && (flag == 7))
	{
		macroCounter++;
		flag = 0;
		j = 0;
		rol = 0;
		
		for ( i = 0; i < strlen(currLine); i++)
		{
			ch = currLine[i];

			if (ch == (int)"\n")
			{
				flag = 0;
				break;
			}
			
			switch (ch) 
			{
			case 'e':
				if (flag == 0)
				{
					flag = 1;
				}
				break;
			case 'n':
				if (flag == 1)
				{
					flag = 2;
				}
				break;
			case 'd':
				if (flag == 2)
				{
					flag = 3;
				}
				break;
			case 'm':
				if (flag == 3)
				{
					flag = 4;
				}
				break;
			case 'a':
				if (flag == 4)
				{
					flag = 5;
				}
				break;
			case 'c':
				if (flag == 5)
				{
					flag = 6;
				}
				break;
			case 'r':
				if (flag == 6)
				{
					flag = 7;
				}
				break;
			case 'o':
				if (flag == 7)
				{
					flag = 8;
				}
				break;
			case ' ':
				if (flag == 8)
				{
					flag = 9;
				}
				break;
			default:
				continue;
			}
			if (flag == 8 || flag == 9)
			{
				flag = 0;
			}
		}
	}
	
	if (flag == 7)
	{
        	/* Allocate more memory for the macro data. */
		if (rol != 1)
		{
			(*(macros + macroCounter)).macroData = (char*)realloc((*(macros + macroCounter)).macroData, sizeof(currLine) + sizeof((*(macros + macroCounter)).macroData));
		}
		rol++;

                /* Save macro data into macroData. */
		for (i = 0; i < strlen(currLine); i++)
		{
			(*(macros + macroCounter)).macroData[j++] = currLine[i];
		}

		if (!(*(macros + macroCounter)).macroData)
		{
			printf("Could not make a pointer.\n");
			exit(0);
		}
	}
    }

    /* Clear spaces from the macro name in macros. */
    for (i = 0; i < macroCounter; i++)
    {
	clear_spaces(macros[i].macroName);
    }

    /* Check if macros names is valid. */
    for (i = 0; i < macroCounter; i++)
    {
	for (ptr = 0; ptr < 20; ptr++)
	{
		if (strstr(commands[ptr], macros[i].macroName) != NULL)
		{
			printf("%s is not a valid name for a macro.\n", macros[i].macroName);
			exit(0);
		}
	}
		
    }
	
    /* Initial to start. */
    fseek(beforeMacrosPointer, 0, SEEK_SET);
    flag = 0;
    i = 0;

    /* Insert each line of origin file into new file, and switch the macros names into thier data. */
    while (fgets(currLine, MAX_LINE_LEN, beforeMacrosPointer) != NULL)
    {
	strcpy(copyLine, currLine);
	copyLine[strlen(copyLine)] = '\0';

	/*check for a specific situation when the line is only ENTER*/
	if(strlen(copyLine) == 1 && copyLine[0] == '\n')
	{
		fputs(copyLine, afterMacrosPointer);
		continue;
	}

        /* First occurrence of substring "macro" only. */
	if (((strstr(copyLine, "macro ")) != NULL)&& ((strstr(copyLine, "endmacro")) == NULL))
	{
		flag = 1;	
	}
		
	if (flag != 1)
	{
		/* Check if we arrived to macro name. */
		for (i = 0; i < macroCounter; i++)
		{	
			memset(nameForMacro, '\0', MAX_LINE_LEN);
			strcpy(nameForMacro, ((*(macros + i)).macroName));
				
			if (strstr(clear_spaces(currLine), nameForMacro) != NULL)
			{
				macroIndx = i;
				flag = 2;
			}

		}	
	}

        /* Check if a row is an end of defining a macro. */
	if ((strstr(copyLine, "endmacro") != NULL ) && (flag == 1))
	{
		flag = 0;
		continue;
	}

        /* Copy a row into the new file as it. */
	if (flag != 1 && flag != 2)
	{

		fputs(copyLine, afterMacrosPointer);
	}

        /* Copy the data of a macro to the new file. */
	if (flag == 2 && flag != 1)
	{		
		fputs(((macros + macroIndx)->macroData), afterMacrosPointer);
		flag = 0;
	}
		
    }
    /* Set and free allocated memory. */
    for (i = 0; i < macroCounter; i++)
    {
	memset(macros[i].macroName, '\0', MAX_LINE_LEN);
	memset(macros[i].macroData, '\0', MAX_LINE_LEN);
	free(((macros + i)->macroData));
	((macros + i)->macroData) = NULL;
    }

    /* Set and free allocated memory. */
    fclose(beforeMacrosPointer);
    free(macros);
    fclose(afterMacrosPointer);

}

/* clear_spaces function remove all spaces, tabs from the string. */
char *clear_spaces(char *str)
{
    int i = 0, j = 0;
    while (i <= strlen(str) - 1)
    {
	if (str[i] != ' ' && str[i] != '\n' && str[i] != '\t')
	{
		str[j++] = str[i];
	}
	i++;
    }
    for (i = j; i <= strlen(str) - 1; i++)
    {
	str[i] = '\0';
    }
    return str;
}

