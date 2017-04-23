#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void gamma_generator()
{
	srand(time(NULL));
	while (1) {
		char c = rand() % 128;
		putchar(c);
	}
}

void gamma_reader()
{
	printf("000000011000001");
}

void file_reader(char *filename)
{
	
}

void main(int argc, char *argv[])
{
	int file_pipes[2];
	char buff[100];
	
	if (pipe(file_pipes) != 0)
		exit(EXIT_FAILURE);
	
	switch (fork())
	{
		case -1:
			exit(EXIT_FAILURE);
		case 0:
			close(1);
			dup(file_pipes[1]);
			close(file_pipes[0]);
			close(file_pipes[1]);
			if (strcmp(argv[1], "encrypt") == 0)
				gamma_generator();
			else if (strcmp(argv[1], "decrypt") == 0)
				gamma_reader();
		default:
			close(file_pipes[1]);
			memset(buff, '\0', 100);
			read(file_pipes[0], buff, 15);
			printf("--- read string = %s \n", buff);
			switch(fork())
			{
				case 0:
					exit(EXIT_FAILURE);
				case 1:
					file_reader(argv[2]);
				default:
					break;
			}
	}
}
















