#ifndef LIST2_H
#define LIST2_H

#include "list2.h"

typedef struct node_s {
	struct node_s *next;
	struct node_s *prev;
	void *data;
	int datasz;
} node_t;

typedef struct list2_s {
	int size;
	node_t *first;
	node_t *last;
	node_t *nodes;
} list2_t;	

list2_t *list2_insert(list2_t *list, void *data, int datasz);
list2_t *list2_insert_last(list2_t *list, void *data, int datasz);

#endif // LIST2_H
