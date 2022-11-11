#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_WASHING_MACHINES 1000

sem_t curr_sem;
sem_t washing_machines[MAX_WASHING_MACHINES];
int total_time_wasted = 0;
int global_time = 0;

typedef struct
{
    int id; // student id --> number
    int T;  // T --> Time of arrival of student
    int W;  // W --> Time taken to wash clothes
    int P;  // P --> patience of student in seconds
} time_args;

// function to change the color of the text in the terminal to red
void red()
{
    printf("\033[1;31m");
}

// function to change the color of the text in the terminal to white
void white()
{
    printf("\033[1;37m");
}

// function to change the color of the text in the terminal to green
void green()
{
    printf("\033[1;32m");
}

// function to change the color of the text in the terminal to yellow
void yellow()
{
    printf("\033[1;33m");
}

// function to change the color of the text in the terminal to normal
void reset()
{
    printf("\033[0m");
}

// create a thread function
void *current_situation(void *arguments)
{
    time_args *args = (time_args *)arguments;
    white();
    printf("%d: Student %d arrives\n", args->T, args->id);
    reset();

    // if student has to wait for more than P seconds, then student leaves using sem_timedwait
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += args->P;

    // wait
    if (sem_timedwait(&curr_sem, &ts) == -1)
    {
        red();
        printf("%d: Student %d leaves\n", args->T, args->id);
        reset();
        return NULL;
    }

    // if student has to wait for less than P seconds, then student waits using sem_wait
    else
    {
        green();
        printf("%d: Student %d waits\n", args->T, args->id);
        reset();
    }

    // critical section
    sleep(args->W);

    // signal
    yellow();
    printf("%d: Student %d finishes\n", args->T, args->id);
    reset();
    sem_post(&curr_sem);
    return NULL;


    // if (global_time - args->T > args->P)
    // {
    //     red();
    //     printf("%d: Student %d leaves\n", global_time, args->id);
    //     reset();
    //     return NULL;
    // }

    // // wait
    // sem_wait(&curr_sem);
    // green();
    // printf("%d: Student %d starts washing\n", args->T, args->id);
    // reset();

    // // critical section
    // sleep(args->W);
    // // global_time += args->W;

    // // signal
    // yellow();
    // printf("%d: Student %d leaves after washing\n", global_time, args->id);
    // reset();
    // sem_post(&curr_sem);

    // return NULL;
}

int main()
{
    // semaphores --> Washing machines
    // threads --> students

    // N --> numeber of students
    // M --> number of washing machines
    int N, M;
    scanf("%d %d", &N, &M);

    // T --> Time of arrival of student
    // W --> Time taken to wash clothes
    // P --> patience of student in seconds
    time_args stu[N];

    for (int i = 0; i < N; i++)
    {
        stu[i].id = i + 1;
        scanf("%d %d %d", &stu[i].T, &stu[i].W, &stu[i].P);
    }

    // sort the students according to their arrival time T
    for (int i = 0; i < N; i++)
    {
        for (int j = i + 1; j < N; j++)
        {
            // if T of student i is equal to T of student j the one with lower id will be before
            if (stu[i].T == stu[j].T)
            {
                if (stu[i].id > stu[j].id)
                {
                    time_args temp = stu[i];
                    stu[i] = stu[j];
                    stu[j] = temp;
                }
            }
            if (stu[i].T > stu[j].T)
            {
                time_args temp = stu[i];
                stu[i] = stu[j];
                stu[j] = temp;
            }
        }
    }

    // print for debugging and testing
    for (int i = 0; i < N; i++)
    {
        printf("(%d) --> %d %d %d\n", stu[i].id, stu[i].T, stu[i].W, stu[i].P);
    }

    // array of N threads
    pthread_t students[N];

    // semaphore for each washing machine
    sem_init(&curr_sem, 0, M);
    for (int i = 0; i < N; i++)
    {
        while (global_time < stu[i].T)
        {
            // printf("starts sleeping\n");
            global_time++;
            sleep(1);
        }
        pthread_create(&students[i], NULL, current_situation, (void *)&stu[i]);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(students[i], NULL);
    }
    sem_destroy(&curr_sem);

    return 0;
}