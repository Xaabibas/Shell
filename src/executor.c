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


static void daemon_execute(char **argv, int fd_in, int fd_out)
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
		if (fd_in) {
			dup2(fd_in, 0);
			close(fd_in);
		}
		if (fd_out) {
			dup2(fd_out, 1);
			close(fd_out);
		}
		execvp(argv[0], argv);
		perror(argv[0]);
		exit(1);
	}
	if (fd_in) {
		close(fd_in);
	}
	if (fd_out) {
		close(fd_out);
	}
	printf("[%d] ", pid);
       	while (*argv) {
		printf("%s ", *argv++);
	}
	printf(" - started\n");
}

static void default_execute(char **argv, int fd_in, int fd_out)
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
		if (fd_in != -1) {
 			if (dup2(fd_in, STDIN_FILENO) == -1) {
        			perror("dup2");
        			exit(1);
    			}
		close(fd_in);
		}
		if (fd_out != -1) {
 			if (dup2(fd_out, STDIN_FILENO) == -1) {
        			perror("dup2");
        			exit(1);
    			}
		close(fd_out);
		}
		execvp(argv[0], argv);
		perror(argv[0]);
		exit(1);
	}
	if (fd_in) {
		close(fd_in);
	}
	if (fd_out) {
		close(fd_out);
	}
	do {
		p = wait(&status);
	} while (p != pid);
}

void execute(int size, token *tokens)
{
	char **argv;
	char *file;
	int type;
	int i;
	int fd_in, fd_out;

	for (i = 0; i < size; i++) {
		type = TEXT;
		argv = tokens[i].str;
		fd_in = -1;
		fd_out = -1;

		while (type != DAEMON && ++i < size) {
			type = tokens[i].type;

			switch (type) {
				case DAEMON:
					break;
				case OUT:
					file= tokens[++i].str[0];
					fd_out = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
					if (fd_out == -1) {
						perror(file);
						return;
					}
					break;
				case IN:
					file = tokens[++i].str[0];
					fd_in = open(file, O_RDONLY, 0666);
					if (fd_in == -1) {
						perror(file);
						return;
					}
					break;
				case APPEND:
					file = tokens[++i].str[0];
					fd_out = open(file, O_CREAT | O_WRONLY | O_APPEND, 0666);
					if (fd_out == -1) {
						perror(file);
						return;
					}
					break;
				default:
					printf("Not implemented yet\n");
			}
		}

		if (type == DAEMON) {
			daemon_execute(argv, fd_in, fd_out);
		} else {
			default_execute(argv, fd_in, fd_out);
		}
	}
}
