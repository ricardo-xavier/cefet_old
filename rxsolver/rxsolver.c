#include <stdio.h>
#include <malloc.h>
#include "rxsolver.h"

int  n, m;
float *c;
char **x;
int  **A;
int  *b;

int dbg=0;

void debug(int d) {
    dbg = d;
}

void maximize(char *expr, int _n, int _m) {

    if (dbg > 0) {
        fprintf(stderr, "rxsolver: maximize: %s\n", expr);
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

    split_objective(expr, c, x);

    if (dbg > 0) {
        for (int i=0; i<n; i++) {
            fprintf(stderr, "%d %d%s\n", i, (int) c[i], x[i]);
        }
    }
}

void subject_to(int i, char *expr, char *op, int _b) {

    if (dbg > 0) {
        fprintf(stderr, "rxsolver: subject_to: %s\n", expr);
    }

    split_constraint(expr, A[i], n, x);
    
    if (dbg > 0) {
        for (int j=0; j<n; j++) {
            fprintf(stderr, "%d%s ", A[i][j], x[j]);
        }
    }

    b[i] = _b;
    if (dbg > 0) {
        fprintf(stderr, " %s %d\n", op, b[i]);
    }
}

float solve(rxsolver_methods method) {

    float z=0;

    if (dbg > 0) {
        fprintf(stderr, "rxsolver: solve: %d\n", method);
    }

    switch (method) {

        case SIMPLEX:
            z = simplex(n, m, c, x, A, b);
            break;
    }

    if (dbg > 0) {
        fprintf(stderr, "z=%.2f\n", z);
        for (int i=0; i<n; i++) {
            fprintf(stderr, "%.2f %s ", c[i], x[i]);
        }
        fprintf(stderr, "\n");
    }
}
