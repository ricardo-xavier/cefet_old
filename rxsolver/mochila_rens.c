#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rxsolver.h"

int main(int argc, char *argv[]) {

    if ((argc > 2) && !strcmp(argv[1], "-d")) {
        debug(atoi(argv[2]));
    }

    maximize("3x1 4x2 6x3", 3, 4);
    subject_to(0, "x1 3x2 5x3", "<=", 7);
    subject_to(1, "x1", "<=", 1);
    subject_to(2, "x2", "<=", 1);
    subject_to(3, "x3", "<=", 1);
    solve(SIMPLEX);

	return 0;
}
