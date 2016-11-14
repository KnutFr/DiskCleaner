#include "autoclean.h"

void myUsage()
{
	printf("\n\n\n\n Usage AutoClean.exe -[i|d] -[u] [-p path]\n");
	printf("Parameters: \n\n -i: install services \n -d: uninstall services \n -p add path to the registry\n -u: delete path from the registry \n\n\n");
	exit(EXIT_FAILURE);
}

void logThis(char *Msg)
{
	FILE *file_ptr;

	file_ptr = fopen("log.txt", "a+");
	if (!file_ptr)
		printf("Error while opening file");
	else
	{
		fprintf(file_ptr,Msg);
		fprintf(file_ptr,"\n");
	}
}

char *strcat_with_alloc(char *s1, char *s2)
{
	int len1;
	int len2;
	char *dest;

	if (!s1 || !s2)
		return NULL;
	len1 = strlen(s1);
	len2 = strlen(s2);
	dest = (char *)malloc(len1 + len2 + 1);
	if (!dest)
		return NULL;
	strcpy(dest, s1);
	strcat(dest, s2);
	return dest;
}

void parseArgs(char **argsArray, int argsNb, int* path, s_args* args)
{
	int i, j;

	for (i = 1; i < argsNb; i++) {
		if (argsArray[i][0] == '-') {
			if (argsArray[i] == "--help")
				myUsage();
			for (j = 1; argsArray[i][j] != '\0'; j++) {
				if (argsArray[i][j] == 'p')
					(*args).p = 1;
				else if (argsArray[i][j] == 'i')
					(*args).i = 1;
				else if (argsArray[i][j] == 'd')
					(*args).d = 1;
				else if (argsArray[i][j] == 'u')
					(*args).u = 1;
				else {
					printf("\n\nUndefined argument %s\n\n", argsArray[i]);
					myUsage();
				}
			}
		}
		else if (*path == 0)
			*path = i;
	}
	if (((*args).p == 1 && *path == 0) || ((*args).d == 1 && (*args).i == 1)) {
		printf("\n\nIncorrect argument(s)\n\n");
		myUsage();
	}
}