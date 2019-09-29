#include <stdio.h>
#include "logbins.h"

void new_box(box_t *box, int x, int y, int z, int w, int h, int d) {

	box->x = x;
	box->y = y;
	box->z = z;
	box->w = w;
	box->h = h;
	box->d = d;
	box->v = w * h * d;

}

void show_box(char *msg, box_t box) {

	fprintf(stderr, "%s %d,%d,%d %dx%dx%d %d\n", msg, box.x, box.y, box.z, 
		box.w, box.h, box.d, box.v);

}

void get_orientation(int orientation, box_t box, int *w, int *h, int *d) {

	int d1, d2, d3;
	if ((box.w >= box.h) && (box.w >= box.d)) {
		d1 = box.w;
		if (box.h >= box.d) {
			d2 = box.h;
			d3 = box.d;
		} else {
			d2 = box.d;
			d3 = box.h;
		}
	} else if ((box.h >= box.w) && (box.h >= box.d)) {
		d1 = box.h;
		if (box.w >= box.d) {
			d2 = box.w;
			d3 = box.d;
		} else {
			d2 = box.d;
			d3 = box.w;
		}
	} else {
		d1 = box.d;
		if (box.w >= box.h) {
			d2 = box.w;
			d3 = box.h;
		} else {
			d2 = box.h;
			d3 = box.w;
		}
	}

	// orientation 0 - h w d
	//             1 - w h d
	//             2 - h d w
	//             3 - w d h
	//             4 - d h w
	//             5 - d w h
	switch (orientation) {
		case 0:
			*h = d1; *w = d2; *d = d3;
			break;
		case 1:
			*w = d1; *h = d2; *d = d3;
			break;
		case 2:
			*h = d1; *d = d2; *w = d3;
			break;
		case 3:
			*w = d1; *d = d2; *h = d3;
			break;
		case 4:
			*d = d1; *h = d2; *w = d3;
			break;
		case 5:
			*d = d1; *w = d2; *h = d3;
			break;
	}
}
