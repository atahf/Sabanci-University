#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

#define thr_size 100
#define buff_size 5000

pthread_mutex_t count_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t buff_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cout_lock = PTHREAD_MUTEX_INITIALIZER;

int thread_count = 0;

void* console_output(void* args) {
	char buff[buff_size];

	pthread_mutex_lock(&buff_lock);
	for (int i = 0; i < buff_size; i++) {
		buff[i] = '\0';
	}
	pthread_mutex_unlock(&buff_lock);

	int fd = *(int*)args;
	while (read(fd, buff, buff_size) == 0);

	pthread_mutex_lock(&buff_lock);
	printf("---- %ld\n", pthread_self());
	printf("%s", buff);
	printf("---- %ld\n", pthread_self());
	fflush(stdout);
	pthread_mutex_unlock(&buff_lock);

	return NULL;
}

int main(int argc, char* argv[]) {
	pthread_t threads[thr_size];
	int fd_list[thr_size][2];

	FILE* file = fopen("commands.txt", "r");
	if (file == NULL) {
		exit(1);
	}

	char line[256];

	while (fgets(line, sizeof(line), file)) {
		char cmd[10] = "";
		char in[50] = "";
		char opt[3] = "";
		char rd[2] = "";
		char rd_file[256] = "";
		char bg_j[2] = "n";

		// beginning of parsing
		char* itr = line;
		int var_idx = 0;
		int need2switch = 0;
		int w_count = 1;

		while (*itr != '\0' && *itr != '\n' && *itr != '&') {
			if (need2switch == 1) {
				if (var_idx == 0 || var_idx == 1 || var_idx == 2) {
					if (*itr == '-') {
						var_idx = 2;
					}
					else if (*itr == '>' || *itr == '<') {
						var_idx = 3;
					}
					else {
						var_idx = 1;
					}
				}
				else if (var_idx == 3) {
					var_idx++;
				}
				need2switch = 0;
			}
			if (!strcmp(cmd, "wait")) {
				break;
			}
			if (*itr != ' ') {
				switch (var_idx) {
				case 0:
					strncat(cmd, itr, 1);
					break;
				case 1:
					/* to handle special characters of(") and (-), current character
					   is one of those, does not add to input string.*/
					if (*itr != '"' && *itr != '-') {
						strncat(in, itr, 1);
					}
					break;
				case 2:
					strncat(opt, itr, 1);
					break;
				case 3:
					strncat(rd, itr, 1);
					break;
				case 4:
					strncat(rd_file, itr, 1);
					break;
				}
			}
			else {
				need2switch = 1;
				w_count++;
			}
			itr++;
		}
		if (*itr == '&') {
			bg_j[0] = 'y';
		}
		if (strlen(rd) == 0) {
			rd[0] = '-';
		}

		int idx = 0;
		char* args[4];
		args[idx++] = cmd;
		if (strlen(in) > 0) {
			args[idx++] = in;
		}
		if (strlen(opt) > 0) {
			args[idx++] = opt;
		}
		args[idx] = NULL;
		// end of parsing

		FILE* parse_file = fopen("parse.txt", "a");
		if (parse_file == NULL) {
			exit(1);
		}

		fprintf(parse_file, "----------\n");
		fprintf(parse_file, "Command: %s\n", cmd);
		fprintf(parse_file, "Inputs: %s\n", in);
		fprintf(parse_file, "Options: %s\n", opt);
		fprintf(parse_file, "Redirection: %s\n", rd);
		fprintf(parse_file, "Background Job: %s\n", bg_j);
		fprintf(parse_file, "----------\n");

		fclose(parse_file);

		if (!strcmp(cmd, "wait")) {
			fflush(stdout);

			int waiting = wait(NULL);
			while (waiting > 0) {
				waiting = wait(NULL);
			}

			/* If there is any background jobs, to make sure they also
			   are done, program have to wait for all threads to join. */
			pthread_mutex_lock(&count_lock);
			for (int i = 0; i < thread_count; i++) {
				pthread_join(threads[i], NULL);
			}
			thread_count = 0;
			memset(fd_list, 0, sizeof(fd_list));
			pthread_mutex_unlock(&count_lock);
		}
		else {
			if (rd[0] != '>') {
				pipe(fd_list[thread_count]);
			}

			int command = fork();
			if (command < 0) {
				exit(1);
			}
			else if (command == 0) {
				if (rd[0] == '<') {
					close(STDIN_FILENO);

					int fileNo = open(rd_file, O_RDONLY);

					dup2(fd_list[thread_count][1], STDOUT_FILENO);
				}
				else if (rd[0] == '>') {
					close(STDOUT_FILENO);

					open(rd_file, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
				}
				else {
					dup2(fd_list[thread_count][1], STDOUT_FILENO);

					fflush(stdout);
				}

				execvp(args[0], args);
			}
			else {
				if (bg_j == "n") {
					waitpid(command, NULL, 0);
				}

				if (rd[0] != '>') {
					pthread_create(&threads[thread_count], NULL, console_output, &fd_list[thread_count][0]);


					pthread_mutex_lock(&count_lock);
					thread_count++;

					/* If the thread list is fully filled, then program wait for all threads to join and
					   will reset fd_list, if this is not done, next iteration thread_count will be out
					   of range of threads list and this will cause a crash in program.*/
					if (thread_count == thr_size) {
						for (int i = 0; i < thread_count; i++) {
							pthread_join(threads[i], NULL);
						}
						thread_count = 0;
						memset(fd_list, 0, sizeof(fd_list));
					}

					pthread_mutex_unlock(&count_lock);
				}
			}
		}
	}

	int waiting = wait(NULL);
	while (waiting > 0) {
		waiting = wait(NULL);
	}

	/* If there is any background jobs, to make sure they also
	   are done, program have to wait for all threads to join. */
	for (int i = 0; i < thread_count; i++) {
		pthread_join(threads[i], NULL);
	}

	fclose(file);

	return 0;
}
