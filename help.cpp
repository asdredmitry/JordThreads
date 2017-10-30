#include <sys/resource.h>
#include <sys/time.h>
#include <stdio.h>
#include <math.h>

#include "help.h"

#define MAX_OUTPUT_SIZE 5
double f(int i, int j)
{
    return 1.0/(i + j + 1.0);
}
void readMatrix(int n, double *a, double *b, int mode, FILE *input)
{
	int i, j;
	double tmp;

	if (mode == 1)
	{
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
				fscanf(input, "%lf", &a[i * n + j]);
			fscanf(input, "%lf", &b[i]);
		}
		fclose(input);
	}
	else
	{
		for (i = 0; i < n; i++)
		{
			tmp = 0.0;
			for (j = 0; j < n; j++)
			{
				a[i * n + j] = f(i, j);
				if (!(j % 2)) tmp += a[i * n + j];
			}
			b[i] = tmp;
		}
	}
}

void printMatrix(int n, double *a, double *b)
{
	int i, j;
	int m = (n > MAX_OUTPUT_SIZE) ? MAX_OUTPUT_SIZE : n;

	for (i = 0; i < m; i++)
	{
		printf("| ");
		for (j = 0; j < m; j++)
			printf("%10.3g ", a[i * n +j]);
		printf(" |   %10.3g\n", b[i]);
	}
}

void printVector(int n, double *x)
{
	int i;
	int m = (n > MAX_OUTPUT_SIZE) ? MAX_OUTPUT_SIZE : n;

	for (i = 0; i < m; i++)
		printf("%10.3g ", x[i]);
}

double normNev(int n, double *a, double *b, double *x)
{
	int i, j;
	double tmp;
	double rezult;

	rezult = 0.0;
	for (i = 0; i < n; i++)
	{
		tmp = 0.0;
		for (j = 0; j < n; j++)
			tmp += a[i * n + j] * x[j];
		tmp -= b[i];
		rezult += tmp * tmp;
	}
	return sqrt(rezult);
}

double accuracy(int n, double *x)
{
	int i;
	double tmp;
	double rezult;

	rezult = 0.0;
	for (i = 0; i < n; i++)
	{
		tmp = x[i];
		if (!(i%2)) tmp -= 1.0;
		rezult += tmp * tmp;
	}
	return sqrt(rezult);
}

