#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "help.h"
#include "task.h"
#include <iostream>

#define INPUT_FILE "data.dat"


typedef struct
{
	int n;
	double *a;
	double *b;
	double *x;
	int *index;
	int my_rank;
	int total_threads;
} ARGS;

long int thread_time = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *Solution(void *p_arg)
{
	ARGS *arg = (ARGS*)p_arg;
	long int t1;

	SolveSystem(arg->n, arg->a, arg->b, arg->x, arg->index, arg->my_rank, arg->total_threads);

	pthread_mutex_lock(&mutex);
	thread_time += t1;
	pthread_mutex_unlock(&mutex);

	return NULL;
}

int main()
{
	int i;
	int n;
	double *a;
	double *b;
	double *x;
	int *index;
	int mode;
	FILE *input;
	int total_threads;
	pthread_t *threads;
	ARGS *args;

    printf("Enter num of threads\n");
    scanf("%d" , &total_threads) ;

    printf("Read from file [y/n]\n");
    char fromFile;
    std :: cin >> fromFile;
    if(fromFile == 'Y' || fromFile == 'y')
        mode = 1;
    else
        mode = 2;

    if(mode == 1)
    {
        if ((input = fopen(INPUT_FILE, "r")) == NULL)
        {
            printf("Can't open file \"%s\"!\n", INPUT_FILE);
            return -2;
        }
        else fscanf(input, "%d", &n);
    }
    else
    {
        printf("Enter num \n");
        scanf("%d",&n);
    }

	a = (double*)malloc(n * n * sizeof(double));
	b = (double*)malloc(n * sizeof(double));
	x = (double*)malloc(n * sizeof(double));
	index = (int*)malloc(n * sizeof(double));
	threads = (pthread_t*)malloc(total_threads * sizeof(pthread_t));
	args = (ARGS*)malloc(total_threads * sizeof(ARGS));

    readMatrix(n, a, b, mode, input);
    for (i = 0; i < n; i++)
        index[i] = i;
	printf("Matrix A:\n");
    printMatrix(n, a, b);

	for (i = 0; i < total_threads; i++)
	{
		args[i].n = n;
		args[i].a = a;
		args[i].b = b;
		args[i].x = x;
		args[i].index = index;
		args[i].my_rank = i;
		args[i].total_threads = total_threads;
	}

    timespec begin, end;
    clock_gettime(CLOCK_REALTIME,&begin);

	for (i = 0; i < total_threads; i++)
		if (pthread_create(threads + i, 0, Solution, args + i))
		{
			printf("Cannot create thread %d!\n", i);

			if (a) free(a);
			if (b) free(b);
			if (x) free(x);
			if (index) free(index);
			if (threads) free(threads);
			if (args) free(args);

			return -7;
		}

	for (i = 0; i < total_threads; i++)
		if (pthread_join(threads[i], 0))
		{
			printf("Cannot wait thread %d!\n", i);

			if (a) free(a);
			if (b) free(b);
			if (x) free(x);
			if (index) free(index);
			if (threads) free(threads);
			if (args) free(args);

			return -8;
		}

    clock_gettime(CLOCK_REALTIME,&end);

    printf("\nRezult:\n");
    printVector(n, x);

    printf(" \n time needed - %lf \n",(double)(end.tv_sec - begin.tv_sec) + (double)(end.tv_nsec - begin.tv_nsec)/1000000000);

	if (mode == 1)
	{
		input = fopen(INPUT_FILE, "r");
		fscanf(input, "%d", &n);
	}
    readMatrix(n, a, b, mode, input);

    printf("\n||Ax - b|| = %e\n", normNev(n, a, b, x));

	if (mode == 2)
        printf("accuracy = %e\n", accuracy(n, x));

	free(a);
	free(b);
	free(x);
	free(index);
	free(threads);
	free(args);

	return 0;
}
