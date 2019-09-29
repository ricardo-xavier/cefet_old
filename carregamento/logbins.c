//TODO verificar colisoes
#include <stdio.h>
#include "logbins.h"

void add_logbin(bin_t *bin, box_t logbin) {

	node_t *ptr;

	if (bin->logbins->size == 0) {
		list2_insert(bin->logbins, bin->logbins->last, false, &logbin, sizeof(box_t));
		return;
	}

	for (ptr=bin->logbins->first; ptr!=NULL; ptr=ptr->next) {

		box_t *box = ptr->data;
		if ((box->z > logbin.z)
				|| ((box->z == logbin.z) && (box->y > logbin.y))
				|| ((box->z == logbin.z) && (box->y == logbin.y) && (box->x > logbin.x))) {
			list2_insert(bin->logbins, ptr, true, &logbin, sizeof(box_t));
			return;
		}
	}

	list2_insert(bin->logbins, bin->logbins->last, false, &logbin, sizeof(box_t));
}

bool put(bin_t *bin, box_t *box) {

	show_box("put box", *box);

	// procura um bin logico que caiba a caixa
	node_t *ptr;
	for (ptr=bin->logbins->first; ptr!=NULL; ptr=ptr->next) {

		box_t *logbin = ptr->data;
		show_box("logbin", *logbin);
		if (box->v > logbin->v) {
			continue;
		}

		// escolhe a orientacao
		int o, w, h, d;
		for (o=0; o<6; o++) {
			get_orientation(o, *box, &w, &h, &d);

			if ((w <= logbin->w) && (h <= logbin->h) && (d <= logbin->d)) {
				break;
			}
		}
		if (o == 6) {
			continue;
		}

		fprintf(stderr, "orientation=%d %dx%dx%d\n", o, w, h, d);

		box->x = logbin->x;
		box->y = logbin->y;
		box->z = logbin->z;
		box->w = w;
		box->h = h;
		box->d = d;
		list2_insert(bin->boxes, bin->boxes->last, false, box, sizeof(box_t));
		
		list2_delete(bin->logbins, ptr);
	
		box_t newlogbin;
		new_box(&newlogbin, box->x + w, box->y, box->z, bin->w - (box->x + w), bin->h - box->y, bin->d - box->z);
		add_logbin(bin, newlogbin);
		new_box(&newlogbin, box->x, box->y + h, box->z, bin->w - box->x, bin->h - (box->y + h), bin->d - box->z);
		add_logbin(bin, newlogbin);
		new_box(&newlogbin, box->x, box->y, box->z + d, bin->w - box->x, bin->h - box->y, bin->d - (box->z + d));
		add_logbin(bin, newlogbin);
		return true;

	}

	return false;
}

int main(int argc, char *argv[]) {

	bin_t bin;
	list2_t *boxes;

	boxes = load_instance(argv[1], &bin);

	bin.boxes = list2_new();
	box_t logbin;
	new_box(&logbin, 0, 0, 0, bin.w, bin.h, bin.d);
	bin.logbins = list2_new();
	list2_insert(bin.logbins, NULL, false, &logbin, sizeof(box_t));

	node_t *ptr;
	for (ptr=boxes->first; ptr!=NULL; ptr=ptr->next) {
		box_t *box = ptr->data;
		put(&bin, box);
	}

	printf("%dx%dx%d\n", bin.w, bin.h, bin.d);
	for (ptr=bin.boxes->first; ptr!=NULL; ptr=ptr->next) {
		box_t *box = ptr->data;
		printf("%d,%d,%d %dx%dx%d\n", box->x, box->y, box->z, box->w, box->h, box->d);
	}
	return 0;

}
