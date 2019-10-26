#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "rxsolver.h"

void split_objective(char *expr, float *c, char **x) {

    int i;
    char *p, *q, aux[257];

    for (i=0, p=expr; *p; i++, p++) {

        for (q=p; isdigit(*q) || (*q == '+') || (*q == '-'); q++);
        if (q == p) {
            c[i] = 1;
        } else {
            memcpy(aux, p, q-p);
            aux[q-p] = 0;
            c[i] = atof(aux);
        }

        p = q;
        for (; *q && (*q != ' '); q++);
        x[i] = malloc(q-p+1);
        memcpy(x[i], p, q-p);
        x[i][q-p] = 0;

        if (!*q) {
            break;
        }

        p = q;
    }
}

void split_constraint(char *expr, int *a, int n, char **x) {

    int i, j, c;
    char *p, *q, aux[257];

    for (i=0, p=expr; *p; i++, p++) {

        for (q=p; isdigit(*q) || (*q == '+') || (*q == '-'); q++);
        if (q == p) {
            c = 1;
        } else {
            memcpy(aux, p, q-p);
            aux[q-p] = 0;
            c = atoi(aux);
        }

        p = q;
        for (; *q && (*q != ' '); q++);

        for (j=0; j<n; j++) {
            if (!memcmp(x[j], p, q-p)) {
                a[j] = c;
                break;
            }
        }

        if (!*q) {
            break;
        }

        p = q;
    }
}
