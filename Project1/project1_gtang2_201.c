// George Tang, G01068335
// CS 262, Lab Section 201
// Lab3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global constants ................................................................
#define TABULASIZE 44 
#define KEYSIZE 21
#define MSGSIZE 251
const char CHARSET[TABULASIZE] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 .,;!-'?";

// Prototypes ......................................................................
// Basics
void printMenu();
char getUserChoice();
int getPosition(const char *str, const char c);
// 1. Seeding
long getSeed();
void tableShuffle(long seed, char table[TABULASIZE][TABULASIZE]);
void printTabulaRecta(char table[TABULASIZE][TABULASIZE]);
// 2. Keyword (and getting messages)
void getWords(char words[]);
// 3. Encryption
void msgEncrypt(char plain[], char keyword[], char table[TABULASIZE][TABULASIZE]);
// 4. Decryption
void msgDecrypt(char enMsg[], char keyword[], char table[TABULASIZE][TABULASIZE]);

int main()  
{  	
	int choice = 0;
	long seed = 0;
	// booleans for checking conditions
	int yesTabula, yesKeyword, quit = 0;
	char tabula[TABULASIZE][TABULASIZE];
	// strings for the keyword, plaintext, and encrypted message	
	char keyword[KEYSIZE], plaintext[MSGSIZE], enMsg[MSGSIZE] = " ";	

	do
	{
		printMenu();
		choice = getUserChoice();
		// Swtich cases to perform different function calls based on the user choice
		switch (choice)
		{
			case 1: // Get seed and generate table
				seed = getSeed();
				tableShuffle(seed, tabula);
				printTabulaRecta(tabula);
				yesTabula = 1;
				break;
			case 2: // Get keyword
				printf("Entering the keyword ... \n");
				getWords(keyword);
				yesKeyword = 1;
				printf("The keyword you entered is: %s\n\n", keyword);
				break;
			case 3: // Get plaintext then encrypt
				if(yesTabula == 0 || yesKeyword == 0)
					break;
				msgEncrypt(plaintext, keyword, tabula);
				break;
			case 4: // Get encryption then decrypt
				if(yesTabula == 0 || yesKeyword == 0)
					break;
				msgDecrypt(enMsg, keyword, tabula);
				break;
			case 5: // Exit the program
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
		   "| 1     | Enter a random # seed to generate the Tabula Recta     |\n"
		   "+-------+--------------------------------------------------------+\n"
		   "| 2     | Enter the keyword                                      |\n"
		   "+-------+--------------------------------------------------------+\n"
		   "| 3     | Enter a message to encrypt                             |\n" 
		   "+-------+--------------------------------------------------------+\n"
		   "| 4     | Enter a message to decrypt                             |\n"
		   "+-------+--------------------------------------------------------+\n"
		   "| 5     | Exit the program                                       |\n"
		   "+-------+--------------------------------------------------------+\n");
}

// Function that takes in the user input for the menu choice they want and validates that
// the input is an appropriate menu choice
char getUserChoice()
{
	char userChoice[5];
	int u = 0;
	do
	{
		printf("Please enter your menu choice: ");
		fgets(userChoice, 5, stdin);
		sscanf(userChoice, "%d", &u);
		printf("\n");
		if ( u == 1 || u == 2 || u == 3 || u == 4 || u == 5 )
			break;
	} while( 1 );
	return u;
}

// Function that takes in the user's seed used to generate the tabula recta
long getSeed()
{
	char userNumInput[25];
	long num  = 0;
	printf("Please enter the value to seed the random number generator: ");
	fgets(userNumInput, 25, stdin);
	sscanf(userNumInput, "%ld", &num);
	printf("\n");
	return num;
}

// Function that takes in the seed and the empty tabula recta and fills it up
void tableShuffle(long seed, char table[TABULASIZE][TABULASIZE])
{	
	// Set the first row of the table equal to the elements in the constant character set
	for (int n = 0; n < 45; n++)
		table[0][n] = CHARSET[n];
	// shuffle the first row using the seed and the Fisher-Yates shuffle
	srandom(seed);
	for (int i = TABULASIZE - 1; i > 0; i--)
	{
		char temp;
		long idx = random()%(i+1);
		temp = table[0][idx];
		table[0][idx] = table[0][i];
		table[0][i] = temp;
	}
	// fill the rest of the rows in the table
	for (int j = 1; j < TABULASIZE; j++)
	{
		// this part fills from the beginning of the row using the leftovers from the first row
		for (int k = 0; k < TABULASIZE - j; k++)
		{
			table[j][k] = table[0][k + j];
		}
		// this part takes the beginning part of the first row and adds it to the end of the row
		for (int l = TABULASIZE - j, m = 0; l < TABULASIZE && m < j; l++, m++)
		{
			table[j][l] = table[0][m];
		}
	}
}

// Function that prints the tabula recta generated from the seed
void printTabulaRecta(char table[TABULASIZE][TABULASIZE])
{
	for (int i = 0; i < TABULASIZE; i++)
	{
		for (int j = 0; j < TABULASIZE; j++)
		{
			printf("%c", table[i][j]);
		}
		printf("\n");
	}
}

// Function that gets the user input for text and turns it to uppercase
// and replaces the newline character with the null terminator
void getWords(char words[])
{
	char userWords[256];
	int c = 0;
	printf("Please enter your choice of characters: ");
	fgets(userWords, 256, stdin);
	// replacing the newline character with the null terminator
	userWords[strlen(userWords)-1] = '\0';
	printf("\n");
	// turning the lowercase characters into uppercases
	while (userWords[c] != '\0') {
		if (userWords[c] >= 'a' && userWords[c] <= 'z') {
			userWords[c] = userWords[c] - 32;
		}
		c++;
	}
	strcpy(words, userWords);
}

// Function that finds the position of the first instance of a given 
// character within a given cstring.
int getPosition(const char *str, const char c)
{
	int retVal = -1;
	char *pos = strchr(str, c);
	if (pos != NULL)
	{
		retVal = pos - str;
	}
	return retVal;
}

// Function that gets the user input for the plaintext, encrypts the text
// based on the existing tabula recta, and prints the encrypted text
void msgEncrypt (char plain[], char keyword[], char table[TABULASIZE][TABULASIZE])
{
	// Get the plaintext and process it into a properly formatted cstring
	printf("Entering the message to encrypt ... \n");
	getWords(plain);
	printf("Your plaintext message is: %s\n\n", plain);

	int i, p, k = 0;
	char encryption[MSGSIZE];
	for(i = 0; i < strlen(plain); i++)  
	{
		// Get the index of the character in the plaintext
		p = getPosition(CHARSET, plain[i]);
		// Get the index of the character in the keyword and use mod to cycle through the keyword once end is reached
		k = getPosition(CHARSET, keyword[i % strlen(keyword)]);
		// Set encryption at index i to the intersect of the indexes based on the tabula recta
		encryption[i] = table[k][p];
	}
	// Set the "last" character after the encryption to the null terminator for cstring
	encryption[strlen(plain) + 1] = '\0';
	printf("Your encrypted message is: %s\n\n", encryption);
}

// Function that gets the user input for the encrypted text, decrypts the text
// based on the existing tabula recta, and prints the plaintext
void msgDecrypt (char enMsg[], char keyword[], char table[TABULASIZE][TABULASIZE])
{
	printf("Entering the message to decrypt ... \n");
	getWords(enMsg);
	printf("Your decrypted message is: %s\n\n", enMsg);

	int i, j, k = 0;
	char decryption[MSGSIZE];
	for(i = 0; i < strlen(enMsg); i++)  
	{
		// Get the index of the character in the keyword
		k = getPosition(CHARSET, keyword[i % strlen(keyword)]);
		// Cycle through the row where the keyword character resides and find the index to which the encrypted character is at
		for(j = 0; j < TABULASIZE; j++)
		{
			// Once found set decryption at index i to the corresponding plaintext character
			if(table[k][j] == enMsg[i])
				decryption[i] = CHARSET[j];
		}		
	}
	// Set the "last" character after the decryption to the null terminator for cstring
	decryption[strlen(enMsg)] = '\0';
	printf("Your decrypted message is: %s\n\n", decryption);
}