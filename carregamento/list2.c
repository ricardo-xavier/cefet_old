#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "list2.h"

char position=0;

list2_t *list2_insert(list2_t *list, void *data, int datasz) {

	node_t *node;

	node = (node_t *) malloc(sizeof(node_t));
	node->data = malloc(datasz);
	memcpy(node->data, data, datasz);
	node->datasz = datasz;
	node->next = NULL;
	node->prev = NULL;

	if (list == NULL) {
		list = (list2_t *) malloc(sizeof(list2_t));
		list->nodes = node;
		list->first = node;
		list->last = node;
		return list;
	}

	switch (position) {

		case 'l':
			list->last->next = node;
			node->prev = list->last;
			list->last = node;
			break;

	}
		
	return list;
}

list2_t *list2_insert_last(list2_t *list, void *data, int datasz) {

	position = 'l';
	list2_t * ret = list2_insert(list, data, datasz);
	position = 0;
	return ret;

}
