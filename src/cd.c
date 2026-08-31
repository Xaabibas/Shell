#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


static int count(char **args)
{
	int c = 0;
	while (*args++) {
		c++;
	}
	return c;
}

void change_dir(char **args)
{
	char *path;
	int c = count(args);

	if (c == 1) {
		path = getenv("HOME");
	} else if (c == 2) {
		path = args[1];
	} else {
		printf("Error: Too much arguments\n");
		return;
	}
	if (chdir(path)) {
		perror("chdir");
	}
}
