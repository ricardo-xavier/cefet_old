#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rxsolver.h"

int main(int argc, char *argv[]) {

    if ((argc > 2) && !strcmp(argv[1], "-d")) {
        rxsolver_debug(atoi(argv[2]));
    }

    // max cx
    // 3 variaveis
    // 4 constraints
    rxsolver_objective("max 3x1 + 4x2 + 6x3", 3, 4);

    // subject to Ax = b
    rxsolver_subject_to(0, "x1 + 3x2 + 5x3 <= 7");
    rxsolver_subject_to(1, "x1 <= 1");
    rxsolver_subject_to(2, "x2 <= 1");
    rxsolver_subject_to(3, "x3 <= 1");

    // resolve utilizando o metodo simplex
    rxsolver_solve(RXSOLVER_SIMPLEX);

    // mostra o resultado da solucao relaxada
    rxsolver_show_result();

	return 0;
}
