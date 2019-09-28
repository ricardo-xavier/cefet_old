#include <stdio.h>
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

bool load_bin(char *filename, bin_t *bin) {

	FILE *f;
	char buf[257];
	int n, b;
	box_t box;

	if ((f = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "ERR fopen [%s] #%d\n", filename, errno);
		return false;
	}

	fgets(buf, 257, f);
	get_dimensions(buf, &bin->w, &bin->h, &bin->l);

	fgets(buf, 257, f);
	n = atoi(buf);
	bin->boxes = NULL;
	box.x = box.y = box.z = -1;

	for (b=0; b<n; b++) {
		fgets(buf, 257, f);
		get_dimensions(buf, &box.w, &box.h, &box.l);
		bin->boxes = list2_insert_last(bin->boxes, &box, sizeof(box_t));
	}

	fclose(f);
	return true;
}
/*
55 23 145
7
35 16 47
35 16 47
16 16 102
44 5 44
50 15 11
50 15 11
50 15 11
*/

