// George Tang, G01068335
// CS 262, Lab Section 201
// Project 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global constants ................................................................
#define fileNameSize 25
#define MAX_CHAIN_BYTES 100
const unsigned char byA = 0x00, byC = 0x40, byG = 0x80, byT = 0xC0;

typedef struct _Chain
{
    size_t SeqLen;  // Number of letters in sequence
    unsigned char Sequence[MAX_CHAIN_BYTES];
} Chain;

// Prototypes ......................................................................
// Basics
void printMenu();
char getUserChoice(int firstTime);
// 1. Read a DNA sequence from a file
Chain readDNA(int* firstTime, char DNAseq[]);
// 2. Save current sequence to a file
Chain writeDNA(Chain chain, char DNAseq[]);
// 3. Print the sequence
// 4. Splice and insert a sub-sequence
void insertSeq(char DNAseq[]);
// 5. Remove a sub-sequence
void removeSeq(char DNAseq[]);
// 6. Replace a sub-sequence with another
void replaceSeq(char DNAseq[]);

int main()  
{  	
	// boolean to make sure option 1 is chosen
	int firstTime = 1;
	int choice = 0, quit = 0;
	Chain DNAchain, verifyChain;
	// the actual DNA sequence
	char DNAseq[MAX_CHAIN_BYTES*4+1];

	do
	{
		printMenu();
		choice = getUserChoice(firstTime);
		firstTime = 0;
		// Swtich cases to perform different function calls based on the user choice
		switch (choice)
		{
			case 1: // Read a DNA sequence from a file
				DNAchain = readDNA(&firstTime, DNAseq);
				break;
			case 2: // Save current sequence to a file
				// If the file does not open, then do not overwrite current DNA chain
				verifyChain = writeDNA(DNAchain, DNAseq);
				if(verifyChain.SeqLen != 0)
					DNAchain = verifyChain;
				break;
			case 3: // Print the sequence
				printf("The DNA sequence is: %s\n", DNAseq);
				break;
			case 4: // Splice and insert a sub-sequence
				insertSeq(DNAseq);
				break;
			case 5: // Remove a sub-sequence
				removeSeq(DNAseq);
				break;
			case 6: // Replace a sub-sequence with another
				replaceSeq(DNAseq);
				break;
			case 7: // Exit the program
				printf("You are now exiting ... \n");
				quit = 1;
		}
	} while(quit == 0);
}

// Function that prints the menu choices for the user to choose from
void printMenu()
{
	printf("+-------+--------------------------------------------------------+\n"
		   "| Input | Menu Choices                                           |\n"
		   "+-------+--------------------------------------------------------+\n"
		   "| 1     | Read a DNA sequence from a file                        |\n"
		   "+-------+--------------------------------------------------------+\n"
		   "| 2     | Save the current sequence to a file                    |\n"
		   "+-------+--------------------------------------------------------+\n"
		   "| 3     | Print the current sequence                             |\n" 
		   "+-------+--------------------------------------------------------+\n"
		   "| 4     | Splice and insert a sub-sequence                       |\n"
		   "+-------+--------------------------------------------------------+\n"
		   "| 5     | Remove a sub-sequence                                  |\n"
		   "+-------+--------------------------------------------------------+\n"
		   "| 6     | Replace a sub-squence with another sub-sequence        |\n"
		   "+-------+--------------------------------------------------------+\n"
		   "| 7     | Exit the program                                       |\n"
		   "+-------+--------------------------------------------------------+\n");
}

// Function that takes in the user input for the menu choice they want and validates that
// the input is an appropriate menu choice
char getUserChoice(int firstTime)
{
	char userChoice[5];
	int u = 0;
	do
	{
		printf("Please enter your menu choice: ");
		fgets(userChoice, 5, stdin);
		sscanf(userChoice, "%d", &u);
		printf("\n");
		if(u < 8)
		{
			// if it is the first time, the user must select option 1
			if(firstTime == 1 && u != 1)
				printf("Must start with menu choice 1 first!\n");
			else
				break;
		}
	} while( 1 );
	return u;
}

// Function that takes in a user given binary file, stores the data as a chain, and convert
// the given binary data into the DNA sequence
Chain readDNA(int *firstTime, char DNAseq[])
{
	FILE *DNAfile = NULL;
	char inputFileName[fileNameSize];
	Chain temp;
	unsigned char tempSeq;
	memset(DNAseq, 0, MAX_CHAIN_BYTES*4*sizeof(char));

	printf("Please enter the name of the file containing a DNA sequence: ");
	fgets(inputFileName, fileNameSize, stdin);
	inputFileName[strlen(inputFileName) - 1] = '\0';

	DNAfile = fopen(inputFileName,"rb");
	if(DNAfile == NULL){
		*firstTime = 1;
		printf("The binary file does not exist!\n");
	}
	else 
	{	
		// read in the entire Chain struct
		fread(&temp, sizeof(Chain), 1, DNAfile);
		int count = 0;
		int limit = temp.SeqLen;
		// going through each byte in Sequence, accounting even a cutoff byte
		for(int i = 0; i < (temp.SeqLen/4 + temp.SeqLen%2); i++)
		{
			// copy the byte in the array into a temp unsigned char
			memcpy(&tempSeq, &temp.Sequence[i], sizeof(unsigned char));
			// go through the byte four times and if there is a cut off early, then dont
			while(count < 4 && limit > 0)
			{				
				// extract the first two bits from the temp unsigned char					
				unsigned char letter = tempSeq & 0xC0;	
				// depending on the pattern, append the appropriate nucleotide base to the DNA sequence			
				if(letter == 0xC0)
					strcat(DNAseq, "T");
				if(letter == 0x80)
					strcat(DNAseq, "G");
				if(letter == 0x40)
					strcat(DNAseq, "C");
				if(letter == 0x00)
					strcat(DNAseq, "A");
				// left shift to get the next 2 bits for extraction
				tempSeq <<= 2;
				// one base extracted out of four
				count ++;
				// one base extracted out of the total (temp.SeqLen)
				limit --;
			}
			count = 0;
		}
		DNAseq[temp.SeqLen] = '\0';
		fclose(DNAfile);
	}
	return temp;
}

// Function that takes in a user given binary file, converts the current DNA sequence to the chain,
// and write the chain to the binary file
Chain writeDNA(Chain chain, char DNAseq[])
{
	FILE *DNAfile = NULL;
	char outputFileName[fileNameSize];
	Chain temp;
	unsigned char insertSeq = 0x00;
	int shift = 0, index = 0;

	temp.SeqLen = strlen(DNAseq);
	printf("Please enter the name of the file to write the DNA sequence to: ");
	fgets(outputFileName, fileNameSize, stdin);
	outputFileName[strlen(outputFileName) - 1] = '\0';

	DNAfile = fopen(outputFileName,"wb");
	if(DNAfile == NULL){
		temp.SeqLen = 0;
		printf("The binary file does not exist!\n");
	} 
	else 
	{
		// go through each base in the DNA sequence6
		for(int i = 0; i < strlen(DNAseq); i++)
		{				
			// set tempSeq to 11111111 to extract the first two bits the base represents
			unsigned char tempSeq = 0xFF;							
			if(DNAseq[i] == 'T')
				tempSeq &= 0xC0;
			if(DNAseq[i] == 'G')
				tempSeq &= 0x80;
			if(DNAseq[i] == 'C')
				tempSeq &= 0x40;
			if(DNAseq[i] == 'A')
				tempSeq &= 0x00;
			// shift it based on whether its the first (0) Second(2) Third(4) or Fourth(6)
			tempSeq >>= shift;
			// then transfer the bits to unsigned char to be written in
			insertSeq |= tempSeq;
			shift += 2;
			// if four chars are used up or the last char has been reached
			if(shift == 8 || i == strlen(DNAseq)-1)
			{
				// copy the designated unsigned char into the chain Sequence
				memcpy(&temp.Sequence[index], &insertSeq, sizeof(unsigned char));
				// reset everything and increment the index of the chain Sequence for the next byte
				insertSeq = 0x00;
				shift = 0;
				index ++;
			}
		}
		// write in the entire Chain struct
		fwrite(&temp, sizeof(Chain), 1, DNAfile);
		fclose(DNAfile);
	}
	return temp;
}

// Function that takes in the sub-sequence to insert and the section of the DNA sequence to insert the sub
// after. Will return error message if the latter does not exist
void insertSeq(char DNAseq[])
{
	int i = 0, subSeqLen, insertLen, count = 0;
	char subSeq[MAX_CHAIN_BYTES*4+1], insertPoint[MAX_CHAIN_BYTES*4+1], temp[MAX_CHAIN_BYTES*4+1];

	printf("Please enter the sub-sequence to insert into the DNA sequence: ");
	fgets(subSeq, MAX_CHAIN_BYTES, stdin);
	subSeq[strlen(subSeq) - 1] = '\0';
	subSeqLen = strlen(subSeq);

	printf("Please enter sequence after which to insert the sub-sequence: ");
	fgets(insertPoint, MAX_CHAIN_BYTES, stdin);
	insertPoint[strlen(insertPoint) - 1] = '\0';
	insertLen = strlen(insertPoint);

	// check to see if there is the section of the DNA sequence the user wants to insert after exists
	if(strstr(DNAseq, insertPoint) == NULL)
		printf("The entered sub-sequence does not exist in the DNA sequence!\n");
	else
	{
		// run through the DNA sequence
		while(i < strlen(DNAseq))
		{
			// if the current base is equal to the beginning base of the interested section
			if(insertPoint[0] == DNAseq[i])
			{
				// make sure the comparison about to happen will not go over DNA sequence 
				if(i < strlen(DNAseq) - insertLen + 1)
				{
					// check that the portion of the DNA sequence matches that of the section
					for(int j = 0; j < insertLen; j++)
					{
						if(insertPoint[j] == DNAseq[i + j])
							count++;
						// if they match ...
						if(count == insertLen)
						{
							// copy everything up to and including the interested section into a temp cstring
							strncpy(temp, DNAseq, i + insertLen);
							temp[i + insertLen] = '\0';
							// append the sub-sequence
							strcat(temp, subSeq);
							// then append the rest of the original DNA sequence
							strcat(temp, DNAseq + i + insertLen);	
							// make the new cstring the DNA sequence								
							strcpy(DNAseq, temp);
							// put the iterating i back on track
							i += subSeqLen + insertLen;
							// if the number of bases is more than 400, then cut off the excess
							if(strlen(DNAseq) > 4*MAX_CHAIN_BYTES)
								DNAseq[4*MAX_CHAIN_BYTES] = '\0';
						}
					}
					count = 0;
				}
			}
			i++;
		}
		DNAseq[i] = '\0';
	}
}

// Function that takes in the sub-sequence to remove from the DNA sequence. Will return error message 
// if the latter does not exist
void removeSeq(char DNAseq[])
{
	int i = 0, subSeqLen, count = 0;
	char subSeq[MAX_CHAIN_BYTES*4+1], temp[MAX_CHAIN_BYTES*4+1];

	printf("Please enter the sub-sequence to delete from the DNA sequence: ");
	fgets(subSeq, MAX_CHAIN_BYTES, stdin);
	subSeq[strlen(subSeq) - 1] = '\0';
	subSeqLen = strlen(subSeq);
	
	// check to see if the sub-sequence the user wants to remove exists
	if(strstr(DNAseq, subSeq) == NULL)
		printf("The entered sub-sequence does not exist in the DNA sequence!\n");
	else
	{
		// run through the DNA sequence
		while(i < strlen(DNAseq))
		{
			// if the current base is equal to the beginning base of the sub-sequence
			if(subSeq[0] == DNAseq[i])
			{
				// make sure the comparison about to happen will not go over DNA sequence 
				if(i < strlen(DNAseq) - subSeqLen + 1)
				{
					// check that the portion of the DNA sequence matches that of the sub-sequence
					for(int j = 0; j < subSeqLen; j++)
					{
						if(subSeq[j] == DNAseq[i + j])
							count++;
						// if they match ...
						if(count == subSeqLen)
						{
							// copy everything up to the sub-sequence into a temp cstring
							// if its not the beginning of the DNA sequence
							if(i != 0)
							{
								strncpy(temp, DNAseq, i);
							}
							temp[i] = '\0';
							printf("1: %s\n", temp);
							// append everything else afterwards, skipping the sub-sequence
							strcat(temp, DNAseq + i + subSeqLen);		
							printf("2: %s\n", temp);							
							// make the new cstring the DNA sequence								
							strcpy(DNAseq, temp);
							printf("3: %s\n", DNAseq);
							// put the iterating i back on track
							i--;
						}
					}
					count = 0;
				}
			}
			i++;
		}
		DNAseq[i] = '\0';
	}
}

// Function that takes in the sub-sequence to remove and the section of the DNA sequence to replace it with. 
// Will return error message if the former does not exist
void replaceSeq(char DNAseq[])
{
	int i = 0, subSeqLen, deleteLen, count = 0;
	char subSeq[MAX_CHAIN_BYTES*4+1], deletePoint[MAX_CHAIN_BYTES*4+1], temp[MAX_CHAIN_BYTES*4+1];

	printf("Please enter the sub-sequence to remove from the DNA sequence: ");
	fgets(deletePoint, MAX_CHAIN_BYTES, stdin);
	deletePoint[strlen(deletePoint) - 1] = '\0';
	deleteLen = strlen(deletePoint);

	printf("Please enter sequence to replace the removed the sub-sequence: ");
	fgets(subSeq, MAX_CHAIN_BYTES, stdin);
	subSeq[strlen(subSeq) - 1] = '\0';
	subSeqLen = strlen(subSeq);

	// check to see if there is the section of the DNA sequence the user wants to replace/remove
	if(strstr(DNAseq, deletePoint) == NULL)
		printf("The entered sub-sequence does not exist in the DNA sequence!\n");
	else
	{
		// run through the DNA sequence
		while(i < strlen(DNAseq))
		{
		// if the current base is equal to the beginning base of the interested removal section
		if(deletePoint[0] == DNAseq[i])
			{
			// make sure the comparison about to happen will not go over DNA sequence 
			if(i < strlen(DNAseq) - deleteLen + 1)
				{
				// check that the portion of the DNA sequence matches that of the section
				for(int j = 0; j < deleteLen; j++)
					{
						if(deletePoint[j] == DNAseq[i + j])
							count++;
						// if they match ...
						if(count == deleteLen)
						{
							// copy everything up to the interested removal section into a temp cstring
							// if its not the beginning of the DNA sequence
							if(i != 0)
							{
								strncpy(temp, DNAseq, i);
							}
							temp[i] = '\0';
							// append the sub-sequence
							strcat(temp, subSeq);
							// append everything else afterwards, skipping the sub-sequence
							strcat(temp, DNAseq + i + deleteLen);								
							// make the new cstring the DNA sequence								
							strcpy(DNAseq, temp);
							// put the iterating i back on track
							i += subSeqLen - 1;
							// if the number of bases is more than 400, then cut off the excess
							if(strlen(DNAseq) > 4*MAX_CHAIN_BYTES)
								DNAseq[4*MAX_CHAIN_BYTES] = '\0';						
						}
					}
					count = 0;
				}
			}
			i++;
		}
		DNAseq[i] = '\0';
	}
}