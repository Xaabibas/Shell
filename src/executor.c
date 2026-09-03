#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "cd.h"
#include "types.h"
#include "tokenizer.h"
#include "vector_token.h"


static void default_execute(char **argv)
{
	int status;
	int pid, p;
	
	if (0 == strcmp(argv[0], "cd")) {
		change_dir(argv);
		return;
	}

	pid = fork();
	if (pid == -1) {
		perror("fork");
		exit(1);
	}
	if (pid == 0) {
		execvp(argv[0], argv);
		perror(argv[0]);
		exit(1);
	}
	do {
		p = wait(&status);
	} while (p != pid);
}
 
static void deamon_execute(char **argv)
{
	int pid;

	pid = fork();
	if (pid == -1) {
		perror("fork");
		exit(1);
	}
	if (pid == 0) {
		if (0 == strcmp(argv[0], "cd")) {
			change_dir(argv);
			exit(0);
		}
		execvp(argv[0], argv);
		perror(argv[0]);
		exit(1);
	}
	printf("[%d] ", pid);
       	while (*argv) {
		printf("%s ", *argv++);
	}
	printf(" - started\n");
}

static void out_execute(char **argv, char *file)
{
	int status;
	int pid, p;
	int fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	if (fd == -1) {
		perror(file);
		return;
	}
	pid = fork();
	if (pid == -1) {
		perror("fork");
		exit(1);
	}
	if (pid == 0) {
		dup2(fd, 1);
		close(fd);
		execvp(argv[0], argv);
		perror(argv[0]);
		exit(1);
	}
	close(fd);
	do {
		p = wait(&status);
	} while (p != pid);
}

static void std_change_execute(char **argv, char *file, int std, int flags)
{
	int status;
	int pid, p;
	int fd = open(file, flags, 0666);
	if (fd == -1) {
		perror(file);
		return;
	}
	pid = fork();
	if (pid == -1) {
		perror("fork");
		exit(1);
	}
	if (pid == 0) {
		dup2(fd, std);
		close(fd);
		execvp(argv[0], argv);
		perror(argv[0]);
		exit(1);
	}
	close(fd);
	do {
		p = wait(&status);
	} while (p != pid);
}

void execute(token *tokens, int size)
{
	char **argv;
	char *file;
	int type;
	int i;

	for (i = 0; i < size; i++) {
		type = TEXT;
		argv = tokens[i].str;
		if (++i != size) {
			type = tokens[i].type;
		}
		switch (type) {
			case TEXT:
				default_execute(argv);
				break;
			case DEAMON:
				deamon_execute(argv);
				break;
			case OUT:
				file = tokens[++i].str[0];
				std_change_execute(argv, file, 1, O_CREAT | O_WRONLY | O_TRUNC);
				break;
			case IN:
				file = tokens[++i].str[0];
				std_change_execute(argv, file, 0, O_RDONLY);
				break;
			case APPEND:
				file = tokens[++i].str[0];
				std_change_execute(argv, file, 1, O_CREAT | O_WRONLY | O_APPEND);
				break;
			default:
				printf("Not implemented yet\n");
		}
	}
}
