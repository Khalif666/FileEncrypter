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

void alerterror(int n)
{
	printf("error - %d\n", n);
	exit(1);
}

void main(int argc, char *argv[])
{
	int key_pipes[2];
	int file_pipes[2];
	char key;
	char data;
	char result;
	size_t n1, n2;
	FILE *key_file;
	FILE *result_file = fopen(argv[3], "w");
	
	if (strcmp(argv[1], "encrypt") == 0)
		key_file = fopen("key", "w");

	if (pipe(file_pipes) != 0)
		alerterror(1);	
	
	if (pipe(key_pipes) != 0)
		alerterror(2);

	switch (fork())
	{
		case -1:
			alerterror(3);
		case 0:
			close(1);
			dup(key_pipes[1]);
			close(key_pipes[0]);
			close(key_pipes[1]);
			if (strcmp(argv[1], "encrypt") == 0)
				gamma_generator();
			else if (strcmp(argv[1], "decrypt") == 0)
				execlp("cat", "cat", "key", 0);
		default:
			close(key_pipes[1]);
			switch(fork())
			{
				case -1:
					alerterror(4);
				case 0:
					close(1);
					dup(file_pipes[1]);
					close(file_pipes[0]);
					close(file_pipes[1]);
					execlp("cat", "cat", argv[2], 0);
				default:
					while ((n2 = read(file_pipes[0], &data, 1)) > 0) {
						n1 = read(key_pipes[0], &key, 1);
						result = data ^ key;
						printf("n2 = %d \t data = %d \t key = %d \t result = %d \n", n2, data, key, result);
						fputc(result, result_file);
						if (strcmp(argv[1], "encrypt") == 0)
							fputc(key, key_file);
					}
					if (strcmp(argv[1], "encrypt") == 0) {
						fputc('\0', key_file);
						fclose(key_file);
					}
					fclose(result_file);
			}
	}
}
