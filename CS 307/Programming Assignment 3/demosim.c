#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <semaphore.h>

int s_inside = 0;
int a_inside = 0;
int s_free = 0;
int a_free = 0;
int s_demo = 0;
int a_demo = 0;
int g_found = 0;
int found_waiting = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

sem_t student_enter;
sem_t student_leave;
sem_t group_ready_s;
sem_t group_ready_a;
sem_t demo_end_s;
sem_t demo_end_a;
sem_t found_barrier;

void g_found_wait() {
    while (g_found == 1) {
        found_waiting++;
        pthread_mutex_unlock(&mutex);
        sem_wait(&found_barrier);
        pthread_mutex_lock(&mutex);
    }
}

void initial_session_condition() {
    if (s_free >= 2 && a_free >= 1) {
        g_found = 1;
        s_free -= 2;
        a_free--;
        sem_post(&group_ready_a);
        sem_post(&group_ready_s);
        sem_post(&group_ready_s);
    }
    else {
        if (found_waiting > 0) {
            sem_post(&found_barrier);
            found_waiting--;
        }
    }
}

void end_session_condition() {
    if (s_demo >= 2 && a_demo >= 1) {
        s_demo -= 2;
        a_demo--; 
        g_found = 0;
        if (found_waiting > 0) {
            sem_post(&found_barrier);
            found_waiting--;
        }
        sem_post(&demo_end_a);
        sem_post(&demo_end_s);
        sem_post(&demo_end_s);
    }
}

void* assistant(void* args) {
    pthread_mutex_lock(&mutex);
    g_found_wait();
    printf("Thread ID : %ld, Role : Assistant, I entered the classroom.\n", pthread_self());
    sem_post(&student_enter);
    sem_post(&student_enter);
    sem_post(&student_enter);
    a_free++;
    a_inside++;
    initial_session_condition();
    pthread_mutex_unlock(&mutex);

    sem_wait(&group_ready_a);

    pthread_mutex_lock(&mutex);
    printf("Thread ID : %ld, Role : Assistant, I am now participating.\n", pthread_self());
    a_demo++;
    end_session_condition();
    pthread_mutex_unlock(&mutex);

    sem_wait(&demo_end_a);

    pthread_mutex_lock(&mutex);
    printf("Thread ID : %ld, Role : Assistant, demo is over.\n", pthread_self());
    pthread_mutex_unlock(&mutex);

    sem_wait(&student_leave);
    sem_wait(&student_leave);

    pthread_mutex_lock(&mutex);
    printf("Thread ID : %ld, Role : Assistant, I left the classroom.\n", pthread_self());
    a_inside--;
    pthread_mutex_unlock(&mutex);
}

void* student(void* args) {
    pthread_mutex_lock(&mutex);
    printf("Thread ID : %ld, Role : Student, I want to enter the classroom.\n", pthread_self());
    pthread_mutex_unlock(&mutex);

    sem_wait(&student_enter);

    pthread_mutex_lock(&mutex);
    g_found_wait();
    printf("Thread ID : %ld, Role : Student, I entered the classroom.\n", pthread_self());
    s_inside++;
    s_free++;
    initial_session_condition();
    pthread_mutex_unlock(&mutex);

    sem_wait(&group_ready_s);

    pthread_mutex_lock(&mutex);
    printf("Thread ID : %ld, Role : Student, I am now participating.\n", pthread_self());
    s_demo++;
    end_session_condition();
    pthread_mutex_unlock(&mutex);

    sem_wait(&demo_end_s);

    pthread_mutex_lock(&mutex);
    printf("Thread ID : %ld, Role : Student, I left the classroom.\n", pthread_self());
    s_inside--;
    sem_post(&student_leave);
    pthread_mutex_unlock(&mutex);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: ./demosim <assistant_count> <student_count>\n");
        return 1;
    }

    int a_count = atoi(argv[1]);
    int s_count = atoi(argv[2]);
    int total = a_count + s_count;

    if (a_count <= 0 || s_count != a_count * 2) {
        
    }
    else {
        pthread_t threads[a_count + s_count];

        sem_init(&student_enter, 0, 0);
        sem_init(&student_leave, 0, 0);
        sem_init(&group_ready_s, 0, 0);
        sem_init(&group_ready_a, 0, 0);
        sem_init(&demo_end_s, 0, 0);
        sem_init(&demo_end_a, 0, 0);
        sem_init(&found_barrier, 0, 0);

        printf("My program complies with all the conditions.\n");

        for (int i = 0; i < total; i++) {
            if (i < s_count) {
                pthread_create(&threads[i], NULL, student, NULL);
            }
            else {
                pthread_create(&threads[i], NULL, assistant, NULL);
            }
        }

        for (int i = 0; i < total; i++) {
            pthread_join(threads[i], NULL);
        }

        sem_destroy(&student_enter);
        sem_destroy(&student_leave);
        sem_destroy(&group_ready_s);
        sem_destroy(&group_ready_a);
        sem_destroy(&demo_end_s);
        sem_destroy(&demo_end_a);
        sem_destroy(&found_barrier);
    }

    printf("The main terminates\n");

    return 0;
}
