#include "Main.h"

/* Main function we will read each file at a time, 
   check for errors and then create the output files if no error occurred.
   Main uses all of the functions in different varieties
   which will be explain in thier time in order to let the project work. */
int main(int argc, char* argv[]) 
{
	char fileName[80];
	int RamMemory [MAX_MEM_CAPACITY] = {0};
	int SymbolMemory [MAX_MEM_CAPACITY] = {0};
	int IC = 100, DC = 0,IC2 = 100, j, len;
	label *labels;

   	/* Saves the labels through the program. */
	labels = (label *) malloc(100*sizeof(label));
	if(labels == NULL)
		exit(1);

    	/* Check if the is a missing file. */
    	if (argc == 1)
    	{
	   	printf("Missing file.\n");
	   	exit(0);
   	}
    	/* Check each file user has entered. */
	for(j = 1;j < argc; j++)
	{
		strcpy(fileName, argv[j]);
		len = strlen(fileName);
		fileName[len] = '\0';

		/* Remove all macros. */
		PreAssembler(fileName, argc);
		/* Proceed if no error on StartPass. */					   				
		if(StartPass(RamMemory,SymbolMemory,&IC,&DC,labels,fileName))
		{   			
			/* Proceed if no error on SecondPass. */				
			if(SecondPass(RamMemory,&IC2,&DC,labels,fileName)) 		
            		{
				AddArrays( RamMemory , SymbolMemory , IC , DC); 	/* Merge SymbolMemory and RamMemory arrays. */
				FinalToFile(RamMemory,(IC-100),DC, fileName,labels);	/* Create output files.(ob,ent,ext) */
			}
		}
        	/* Initialize, for the next file. */
		IC = 100;
		DC = 0;
		IC2 = 100;
		memset(labels, 0, sizeof(*labels));
		memset(RamMemory, 0, MAX_MEM_CAPACITY);
		memset(SymbolMemory, 0, MAX_MEM_CAPACITY);
		memset(fileName, 0, 80);
	}
    free(labels);
    return 0;
}

/* The function convert array of chars into correct base - 32. */
char convert_Base(char n[])
{
   	int sum = 0 , i;
    	char ch[] = {'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f',
				'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'};
	for(i = 0; i < strlen(n); i++)
	{
		if(n[strlen(n)-i-1] == '1')
			sum += pow(2,i);		
	}
    	return ch[sum];
}

/* The function convert int into correct base -32, store it in pointer. */
void convert_int_base(int n, char *symbol)
{
    	int left_Five_Bits = 0;
    	int right_Five_Bits = 0;
    	char ch[] = {'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f',
				'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'};
	memset(symbol,'\0',2);

	/* Separate 10 bits into: 5 left bits, 5 right bits. */
    	right_Five_Bits = (n & 31);
    	left_Five_Bits = (n & (31 << 5));
    	left_Five_Bits >>= 5;

    	if(left_Five_Bits == 0)
		symbol[0] = ch[right_Five_Bits];	
	else
	{
		symbol[0] = ch[left_Five_Bits];
    		symbol[1] = ch[right_Five_Bits];
	}
    
}

/* Function will merge all the elements from SymbolMemory into RamMemory for the final product. */
void AddArrays( int RamMemory[] , int SymbolMemory[] ,int IC ,int DC)
{
	int i;
	for(i = 0; i<DC; i++)
	{
		RamMemory[IC] = SymbolMemory[i];
		IC++;
	}
}

/* Function will create the files: .ob, .ent, .ext from final RamMemory. */
void FinalToFile(int RamMemory [MAX_MEM_CAPACITY], int IC, int DC, char *fileName,label *labels)
{
	int i,j,k,len;
	int q = 0,p = 0,size;
	char res[2];
	FILE *fp;
	len = strlen(fileName);
	
   	/****************************** Create .ob file  ******************************/
	fileName[len-1] = 'b';
	fileName[len-2] = 'o';
	fp = fopen (fileName, "w");
	if(fp == NULL)
		exit(1);
    
 	 
    	/* First line of file contain instruction-counter and then data-counter (as base 32). */
	convert_int_base(IC,res);
	fprintf(fp,"     %s ",res);
	convert_int_base(DC,res);
	fprintf(fp,"%s\n",res);  

    	/* Each line represents memory address, then the data of the word (as base 32). */
	for(i = 100; i<(100+IC+DC); i++)
	{
 
		char* symbols = (char*)malloc(sizeof(char) * 2);
		if(symbols == NULL)
			exit(1);
		for(j = 2; j>0; j--)
		{
			char* number = (char*)malloc(sizeof(char) * 5);
			if(number == NULL)
				exit(1);
			for(k = 0; k<5; k++)
			{
				if(WhatBit(RamMemory,(i*10)+(j*5)-k-1))		
					number[k] = '1'; 
				else
					number[k] = '0';
				
			}
			number[k] = '\0';			
			symbols[j-1] = convert_Base(number);
			free(number);
		}
		/*Print: line number and RamMemory in 32 base. */
		convert_int_base(i,res);	
		fprintf(fp,"%s\t  %c%c\n",res,symbols[1],symbols[0]);

		free(symbols);
	}
	fclose(fp);
	
    	/****************************** Create .ent file  ******************************/
	fileName[len-1] = 'n';
	fileName[len-2] = 'e';
	strcat(fileName,"t");  
	fp = fopen (fileName, "w");
	if(fp == NULL)
		exit(1);


   	/* Each line represents symbol name as entry, then his data (as base 32). */
	while(((labels+q) -> lit)!=0)
    	{
		if((labels+q) -> attributes[1] == 1)
        	{
			convert_int_base(labels[q].value,res);
			fprintf(fp,"%s		 %s\n",labels[q].name,res);	
		}
		q++;
	}
	if (NULL != fp) 
	{
    		fseek (fp, 0, SEEK_END);
    		size = ftell(fp);

    		if (0 == size) 
        		remove(fileName);
	}
	q = 0;
	fclose(fp);

    	/****************************** Create .ext file  ******************************/
	fileName[len] = 't';
	fileName[len-1] = 'x';
	fileName[len-2] = 'e';
	fp = fopen (fileName, "w");
	if(fp == NULL)
		exit(1);

    	/* Each line represents name of symbol as external, then his caller address - may be more than one (as base 32). */
	while(((labels+q) -> lit)!=0)
	{
		while((labels+q) -> ExternPlace[p] != 0)
		{
			convert_int_base(labels[q].ExternPlace[p],res);
			fprintf(fp,"%s	%s\n" ,labels[q].name,res);
			p++;
		}
		p = 0;
		q++;
	}
	if (NULL != fp) 
	{
    		fseek (fp, 0, SEEK_END);
    		size = ftell(fp);

    		if (0 == size)
        		remove(fileName);
	}
	fclose(fp);
	
}

