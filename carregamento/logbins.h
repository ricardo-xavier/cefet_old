#ifndef LOGBINS_H
#define LOGBINS_H

#include "list2.h"

typedef struct box_s {

	int x, y, z;
	int w, h, d;
	int v;

} box_t;

typedef struct bin_s {

	int w, h, d;
	int v;
	list2_t *boxes;
	list2_t *logbins;

} bin_t;
	
list2_t *load_instance(char *filename, bin_t *bin);

void new_box(box_t *box, int x, int y, int z, int w, int h, int d);
void show_box(char *msg, box_t box);
void get_orientation(int orientation, box_t box, int *w, int *h, int *d);

#endif // LOGBINS_H
