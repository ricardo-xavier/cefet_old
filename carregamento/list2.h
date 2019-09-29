#ifndef LIST2_H
#define LIST2_H

#include "list2.h"

#ifndef bool
#define bool char
#define false 0
#define true  1
#endif

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

list2_t *list2_new();
node_t *new_node(void *data, int datasz);
node_t *list2_insert(list2_t *list, node_t *ptr, bool before, void *data, int datasz);
node_t *list2_delete(list2_t *list, node_t *node);
void list2_clear(list2_t *list);

#endif // LIST2_H
