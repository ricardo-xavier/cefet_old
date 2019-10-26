#ifndef RXSOLVER_H
#define RXSOLVER_H

#define bool unsigned char
#define false 0
#define true  1

typedef enum rxsolver_enum {
    SIMPLEX
} rxsolver_methods ;

void debug(int d);
void maximize(char *expr, int n, int m);
void subject_to(int i, char *expr, char *op, int b);
float solve(rxsolver_methods method);

float simplex(int n, int m, float *c, char **x, int **A, int *b);

void split_objective(char *expr, float *c, char **x);
void split_constraint(char *expr, int *a, int n, char **x);

#endif
