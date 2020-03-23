#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()  
{  
	char subSeq[100] = "Hello I am George";
    char temp[100];
    subSeq[strlen(subSeq)] = '\0';

    strncpy(temp, subSeq, 6);
    temp[6] = '\0';
    strcat(temp, subSeq + 6 + 5);									
    strcpy(subSeq, temp);
    subSeq[strlen(subSeq)] = '\0';

    printf("%s", subSeq);
}