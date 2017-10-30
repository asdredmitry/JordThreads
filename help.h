#ifndef __HELP_H_INCLUDED__
#define __HELP_H_INCLUDED__

double f(int i, int j);

void readMatrix(int n, double *a, double *b, int mode, FILE *input);

void printMatrix(int n, double *a, double *b);

void printVector(int n, double *x);

double normNev(int n, double *a, double *b, double *x);

double accuracy(int n, double *x);

#endif /* __HELP_H_INCLUDED__ */
