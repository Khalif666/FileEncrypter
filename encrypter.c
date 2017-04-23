#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void main()
{
	FILE *output;
	char buffer[100];
	memset(buffer, '\0', 100);
	output = popen("cat ./encrypter.c", "r");
	printf("------ entry of file ------\n");
	while (fgets(buffer, 100, output) != NULL) {
		printf("> %s", buffer);
	}
	printf("------ end of file -------\n");
}
