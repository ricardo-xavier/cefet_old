#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "rxsolver.h"

float *tableau;
char **base;
float *c;
char **x;
int **A;
int *b;
int n, m;
extern int dbg;

void set(int i, int j, float v) {
    tableau[i*(n+m+1) + j] = v;
}

float get(int i, int j) {
    return tableau[i*(n+m+1) + j];
}

void show() {

    if (dbg == 0) {
        return;
    }

    fprintf(stderr, "%5s %5s ", "", "");
    for (int i=0; i<n; i++) {
        fprintf(stderr, "%-5s ", x[i]);
    }
    for (int i=0; i<m; i++) {
        fprintf(stderr, "s%-4d ", i);
    }
    fprintf(stderr, "\n");

    for (int i=0; i<m+1; i++) {
        fprintf(stderr, "%5s ", base[i]);
        for (int j=0; j<n+m+1; j++) {
            fprintf(stderr, "%5.2f ", get(i, j));
        }
        fprintf(stderr, "\n");
    }
}

void initialize() {

    // inicializa a matriz inteira com 0
    for (int i=0; i<m+1; i++) {
        for (int j=0; j<n+m+1; j++) {
            set(i, j, 0);
        }
    }

    // a primeira coluna das M primeiras linhas(uma linha por constraint) terao os valores b
    for (int i=0; i<m; i++) {
        set(i, 0, b[i]);
    }

    // a partir da segunda coluna, tera uma coluna com o coeficiente de cada variavel em cada constraint(linha)
    for (int i=0; i<m; i++) {
        for (int j=0; j<n; j++) {
            set(i, j+1, A[i][j]);
        }
    }

    // nas colunas seguintes, inicializa com a vaiavel de folga de cada constraint
    for (int i=0; i<m; i++) {
        set(i, 1+n+i, 1);
    }

    // a ultima linha, a partir da segunda coluna,  inicia com os valores dos coeficientes das variaveis na funcao objetivo
    for (int j=0; j<n; j++) {
        set(m, j+1, c[j] * -1);
    }
}

bool next() {

    int x_pivot=-1;
    int y_pivot=-1;
    float v, vx, q, q_pivot, v_pivot, f;
    
    // procura a coluna pivo (menor valor negativo na linha z)
    for (int j=1; j<n+m+1; j++) {
        v = get(m, j);
        if (v < 0) {
            if (x_pivot == -1) {
                x_pivot = j;
            } else {
                if (v < get(m, x_pivot)) {
                    x_pivot = j;
                }
            }
        }    
    }

    // interrompe se nao encontrar nenhum negativo
    if (x_pivot == -1) {
        return false;
    }

    if (dbg >= 1) {
        fprintf(stderr, "x_pivot = %d\n", x_pivot);
        if (x_pivot <= n) {
            fprintf(stderr, "enter %s\n", x[x_pivot-1]);
        } else {
            fprintf(stderr, "enter s%d\n", x_pivot-n-1);
        }
    }

    // procura a linha pivo - menor quociente positivo
    for (int i=0; i<m; i++) {
        v = get(i, 0);
        vx = get(i, x_pivot);
        q = v / vx;
        if (q > 0) {
            if (y_pivot == -1) {
                y_pivot = i;
                q_pivot = q;
            } else {
                if (q < q_pivot) {
                    y_pivot = i;
                    q_pivot = q;
                }
            }
        }
    }

    // interrompe se nao encontrar nenhum valor positivo
    if (y_pivot == -1) {
        return false;
    }

    v_pivot = get(y_pivot, x_pivot);

    if (dbg >= 1) {
        fprintf(stderr, "y_pivot = %d %.2f %.2f\n", y_pivot, q_pivot, v_pivot);
        fprintf(stderr, "leave %s\n", base[y_pivot]);
    }

    // ajusta as outras linhas para que o elemento na coluna pivo seja 0
    for (int i=0; i<m+1; i++) {
        if (i == y_pivot) {
            continue;
        }
        vx = get(i, x_pivot);
        if ((vx > -0.0001) && (vx < 0.0001)) {
            continue;
        }
        f = vx / v_pivot * -1;
        for (int j=0; j<n+m+1; j++) {
            set(i, j, get(i, j) + f * get(y_pivot, j));
        }
    }

    // ajusta a linha pivot para que o elemento na coluna pivo seja 1
    for (int i=0; i<m; i++) {
        if (i != y_pivot) {
            continue;
        }
        vx = get(i, x_pivot);
        if ((vx > 0.9999) && (vx < 1.0001)) {
            break;
        }
        f = 1 / vx;
        for (int j=0; j<n+m+1; j++) {
            set(i, j, get(i, j) * f);
        }
    }

    // troca a variavel da base
    free(base[y_pivot]);    
    if (x_pivot <= n) {
        base[y_pivot] = strdup(x[x_pivot-1]);
    } else {
        char aux[5];
        sprintf(aux, "s%d", x_pivot-n-1);
        base[y_pivot] = strdup(aux);
    }
    return true;
}

float simplex(int _n, int _m, float *_c, char **_x, int **_A, int *_b) {

    char aux[5];
    float z;

    // seta variaveis globais
    n = _n;
    m = _m;
    c = _c;
    x = _x;
    A = _A;
    b = _b;

    // aloca memoria
    tableau = malloc((m+1) * (n+m+1) * sizeof(float));
    base = malloc((m+1) * sizeof(char *));

    // monta variaveis da base
    for (int i=0; i<m; i++) {
        sprintf(aux, "s%d", i);
        base[i] = strdup(aux);
    }
    base[m] = strdup("z");

    // inicializa e mostra o tableau inicial
    if (dbg > 0) {
        fprintf(stderr, "simplex %d x %d\n", n, m);
    }
    initialize();
    show();

    // executa as iteracoes
    while (next()) {
        show();
    }

    // mostra o resultado
    show();
    z = get(m, 0);
    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            if (!strcmp(base[j], x[i])) {
                c[i] = get(j, 0);
                break;
            }
        }
    }

    // desaloca memoria
    free(tableau);
    for (int i=0; i<m+1; i++) {
        free(base[i]);
    }
    free(base);

    return z;
}
