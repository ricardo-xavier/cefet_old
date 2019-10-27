#ifndef RXSOLVER_H
#define RXSOLVER_H

#define bool unsigned char
#define false 0
#define true  1

typedef enum {
    RXSOLVER_SIMPLEX,
    RXSOLVER_CUSTOM
} rxsolver_methods ;

static char *METHOD_NAMES[2] = { "SIMPLEX", "CUSTOM" };

void rxsolver_debug(int d);
bool rxsolver_objective(char *expr, int n, int m);
bool rxsolver_subject_to(int i, char *expr);
float rxsolver_solve(rxsolver_methods method);
void rxsolver_show_result();
void rxsolver_free();

float simplex(int n, int m, float *c, char **x, int **A, int *b);

bool split_objective(char *expr, float *c, char **x);
bool split_constraint(int i, char *expr, int *a, int *b, char **x);

#endif
