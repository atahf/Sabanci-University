#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

int main() {
    printf("I'm SHELL process, with PID: %d - Main command is: man -P cat ping | grep -e -U -A 2 > output.txt\n", getpid());

	// create pipe
    int fd[2];
    pipe(fd);

	// create process to be used for GREP
    int child = fork();

    if (child < 0) {
        exit(1);
    }
    else if (child == 0) {
		// create process to be used for MAN
        int grandchild = fork();

        if (grandchild < 0) {
            exit(1);
        }
        else if (grandchild == 0) {
            printf("I’m MAN process, with PID: %d - My command is: man -P cat ping \n", getpid());

			// redirect output to Anon file
            dup2(fd[1], STDOUT_FILENO);

			// executes man -P cat ping
            execlp("man", "man", "-P", "cat", "ping", NULL);
        }
        else {
			// waits until grandchild process is done
            waitpid(grandchild, NULL, 0);

            printf("I’m GREP process, with PID: %d - My command is: grep -e -U -A 2 \n", getpid());

			// create output.txt
            int file = open("output.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

			// get input from Anon file
            dup2(fd[0], STDIN_FILENO);

			// redirect output to output.txt
            dup2(file, STDOUT_FILENO);

            close(fd[1]);

			// executes grep -e -U -A 2
            execlp("grep", "grep", "-e", "-U", "-A", "2", NULL);
        }
    }
    else {
        close(fd[0]);
        close(fd[1]);

		// waits until child process is done
        waitpid(child, NULL, 0);

        printf("I’m SHELL process, with PID: %d - execution is completed, you can find the results in output.txt\n", getpid());
    }

    return 0;
}
