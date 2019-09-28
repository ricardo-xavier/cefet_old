#ifndef LOGBINS_H
#define LOGBINS_H

#define bool char
#define false 0
#define true  1

#include "list2.h"

typedef struct box_s {

	int x, y, z;
	int w, h, l;

} box_t;

typedef struct bin_s {

	int w, h, l;
	list2_t *boxes;

} bin_t;
	

bool load_bin(char *filename, bin_t *bin);

#endif // LOGBINS_H
