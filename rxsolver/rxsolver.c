#include <stdio.h>
#include <malloc.h>
#include "rxsolver.h"

int  n, m;
float *c, z;
char **x;
int  **A;
int  *b;

int dbg=0;

void rxsolver_debug(int d) {
    dbg = d;
}

bool rxsolver_objective(char *expr, int _n, int _m) {

    if (dbg > 0) {
        fprintf(stderr, "rxsolver: : %s\n", expr);
    }

    n = _n;
    m = _m;

    c = malloc(n * sizeof(float *));
    x = malloc(n * sizeof(char *));

    A = malloc(m * sizeof(int *));
    for (int i=0; i<m; i++) {
        A[i] = malloc(n * sizeof(int *));
    }
    b = malloc(m * sizeof(int *));

    if (!split_objective(expr, c, x)) {
        return false;
    }

    if (dbg > 0) {
        for (int i=0; i<n; i++) {
            fprintf(stderr, "    c[%d]x[%d] = %d%s\n", i+1, i+1, (int) c[i], x[i]);
        }
    }
}

bool rxsolver_subject_to(int i, char *expr) {

    if (dbg > 0) {
        fprintf(stderr, "rxsolver: subject_to: %s\n", expr);
    }

    if (!split_constraint(i, expr, A[i], b, x)) {
        return false;
    }
    
    if (dbg > 0) {
        for (int j=0; j<n; j++) {
            fprintf(stderr, "    A[%d][%d] = %d%s\n", i+1, j+1, A[i][j], x[j]);
        }
    }

    if (dbg > 0) {
        fprintf(stderr, "    b[%d] = %d\n", i+1, b[i]);
    }
}

float rxsolver_solve(rxsolver_methods method) {

    if (dbg > 0) {
        fprintf(stderr, "rxsolver: solve: %s\n", METHOD_NAMES[method]);
    }

    switch (method) {

        case RXSOLVER_SIMPLEX:
            z = simplex(n, m, c, x, A, b);
            break;
    }

    return z;
}

void rxsolver_show_result() {
    fprintf(stderr, "z=%.2f\n", z);
    for (int i=0; i<n; i++) {
        fprintf(stderr, "%.2f %s ", c[i], x[i]);
    }
    fprintf(stderr, "\n");
}

void rxsolver_free() {
    for (int i=0; i<m; i++) {
        free(A[i]);
    }
    free(c);
    free(x);
    free(A);
    free(b);
}
