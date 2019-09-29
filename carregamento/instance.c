#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "logbins.h"

void get_dimensions(char *buf, int *w, int *h, int *l) {

	char *pw, *ph, *pl;

	pw = buf;
	ph = strchr(pw, ' '); *ph++ = 0;
	pl = strchr(ph, ' '); *pl++ = 0;

	*w = atoi(pw);
	*h = atoi(ph);
	*l = atoi(pl);
}

list2_t *load_instance(char *filename, bin_t *bin) {

	FILE *f;
	char buf[257];
	int n, b;
	box_t box;
	list2_t *boxes=NULL;

	if ((f = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "ERR fopen [%s] #%d\n", filename, errno);
		return NULL;
	}

	fgets(buf, 257, f);
	get_dimensions(buf, &bin->w, &bin->h, &bin->d);
	bin->v = bin->w * bin->h * bin->d;

	fgets(buf, 257, f);
	n = atoi(buf);
	box.x = box.y = box.z = -1;

	boxes = list2_new();
	for (b=0; b<n; b++) {
		fgets(buf, 257, f);
		get_dimensions(buf, &box.w, &box.h, &box.d);
		box.v = box.w * box.h * box.d;
		list2_insert(boxes, boxes->last, false, &box, sizeof(box_t));
	}
	//TODO ordenar em ordem decrescente de maior dimensao

	fclose(f);
	return boxes;
}
