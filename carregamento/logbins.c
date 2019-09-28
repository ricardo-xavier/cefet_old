#include <stdio.h>
#include "logbins.h"

void new_box(box_t *box, int x, int y, int z, int w, int h, int l) {
	box->x = x;
	box->y = y;
	box->z = z;
	box->w = w;
	box->h = h;
	box->l = l;
}

int main(int argc, char *argv[]) {

	list2_t *boxes = NULL;
	box_t box;
	bin_t bin;

	load_bin(argv[1], &bin);

	new_box(&box, 0, 0, 0, 35, 16, 47);
	boxes = list2_insert(NULL, &box, sizeof(box_t));
	new_box(&box, 0, 0, 47, 35, 16, 47);
	boxes = list2_insert_last(boxes, &box, sizeof(box_t));
	new_box(&box, 35, 0, 0, 16, 16, 102);
	boxes = list2_insert_last(boxes, &box, sizeof(box_t));
	new_box(&box, 0, 16, 0, 44, 5, 44);
	boxes = list2_insert_last(boxes, &box, sizeof(box_t));
	new_box(&box, 0, 0, 102, 50, 15, 11);
	boxes = list2_insert_last(boxes, &box, sizeof(box_t));
	new_box(&box, 0, 0, 113, 50, 15, 11);
	boxes = list2_insert_last(boxes, &box, sizeof(box_t));
	new_box(&box, 0, 0, 124, 50, 15, 11);
	boxes = list2_insert_last(boxes, &box, sizeof(box_t));

	fprintf(stderr, "%dx%dx%d\n", 55, 23, 145);

	node_t *ptr;
	for (ptr=bin.boxes->first; ptr!=NULL; ptr=ptr->next) {
		box_t *b = (box_t *) ptr->data;
		fprintf(stderr, "%d,%d,%d %dx%dx%d\n", b->x, b->y, b->z, b->w, b->h, b->l);
	}	

	return 0;
}
