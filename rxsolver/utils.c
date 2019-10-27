#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "rxsolver.h"

char **argv;
int argc;

void split(char *expr) {

    char *p, *q;;
    int a;

    argc = 1;
    for (p=expr; *p; p++) {
        if (*p == ' ') {
            while (*p == ' ') p++;
            p--;
            argc++;
        }
    }

    argv = malloc(argc * sizeof(char *));
    a = 0;
    for (p=expr;;) {
        if ((q = strchr(p, ' ')) == NULL) {
            q = p + strlen(p);
        }
        argv[a] = malloc(q-p+1);
        memcpy(argv[a], p, q-p);
        argv[a][q-p] = 0;
        if (!*q) {
            break;
        }
        while (*q && (*q == ' ')) q++;
        a++;
        p = q;
    }
}

bool split_objective(char *expr, float *c, char **x) {

    char s, *p, aux[9];
    int i;

    split(expr);

    if (!strcmp(argv[0], "min") && !strcmp(argv[0], "max")) {
        fprintf(stderr, "expected min|max\n");
        return false;
    }

    s = '+';
    i = 0;
    for (int a=1; a<argc; a++) {

        if (!strcmp(argv[a], "+") || !strcmp(argv[a], "-")) {
            s = argv[a][0];
            continue;
        }

        for (p=argv[a]; *p; p++) {
            if (!isdigit(*p) && (*p != '.')) {
                if (p == argv[a]) {
                    c[i] = 1.0f;
                    x[i] = strdup(argv[a]);
                } else {
                    memcpy(aux, argv[a], p-argv[a]);
                    aux[p-argv[a]] = 0;
                    c[i] = atof(aux);
                    x[i] = strdup(p);
                }
                if (s == '-') {
                    c[i] *= -1;
                }
                i++;
            }
        }
    }

    for (int a=0; a<argc; a++) {
        free(argv[a]);
    }
    free(argv);
    return true;
}

bool split_constraint(int i, char *expr, int *_a, int *b, char **x) {

    char s, *p, aux[9], *_x;
    int _c;

    split(expr);

    s = '+';
    for (int a=0; a<argc; a++) {

        if (!strcmp(argv[a], "+") || !strcmp(argv[a], "-")) {
            s = argv[a][0];
            continue;
        }

        if ((argv[a][0] == '>') || (argv[a][0] == '<') || (argv[a][0] == '=')) {
            b[i] = atoi(argv[++a]);
            break;
        }

        for (p=argv[a]; *p; p++) {
            if (!isdigit(*p) && (*p != '.')) {
                if (p == argv[a]) {
                    _c = 1;
                    _x = argv[a];
                } else {
                    memcpy(aux, argv[a], p-argv[a]);
                    aux[p-argv[a]] = 0;
                    _c = atoi(aux);
                    _x = p;
                }
                if (s == '-') {
                    _c *= -1;
                }

                for (int j=0; ; j++) {
                    if (!strcmp(x[j], _x)) {
                        _a[j] = _c;
                        break;
                    }
                }
            }
        }
    }

    for (int a=0; a<argc; a++) {
        free(argv[a]);
    }
    free(argv);
    return true;
}
