#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "list2.h"

list2_t *list2_new() {

	list2_t *list = (list2_t *) malloc(sizeof(list2_t));
	list->nodes = NULL;
	list->first = NULL;
	list->last = NULL;
	list->size = 0;
	return list;

}

node_t *new_node(void *data, int datasz) {

	node_t *node = (node_t *) malloc(sizeof(node_t));
	node->data = malloc(datasz);
	memcpy(node->data, data, datasz);
	node->datasz = datasz;
	node->next = NULL;
	node->prev = NULL;
	return node;

}

node_t *list2_insert(list2_t *list, node_t *ptr, bool before, void *data, int datasz) {

	if (list == NULL) {
		return NULL;
	}

	node_t *node = new_node(data, datasz);
	list->size++;

	if (ptr == NULL) {
		list->nodes = node;
		list->first = node;
		list->last = node;
		return node;
	}

	if (before) {

		if (ptr->prev == NULL) {
			node->next = ptr;
			ptr->prev = node;
			list->first = node;
			return node;
		}

		ptr->prev->next = node;
		node->prev = ptr->prev;
		ptr->prev = node;
		node->next = ptr;
		return node;

	}

	// after
	if (ptr->next == NULL) {
		ptr->next = node;
		node->prev = ptr;
		list->last = node;
		return node;
	}		

	ptr->next->prev = node;
	node->next = ptr->next;
	ptr->next = node;
	node->prev = ptr;
	return node;
}

node_t *list2_delete(list2_t *list, node_t *node) {

	if ((list == NULL) || (node == NULL)) {
		return NULL;
	}

	free(node->data);
	list->size--;

	if (list->size == 0) {
		free(node);
		list->nodes = NULL;
		list->first = NULL;
		list->last = NULL;	
		return NULL;
	}

	if (node == list->first) {
		list->first = node->next;
		node->next->prev = NULL;
		free(node);
		return list->first;
	}

	if (node == list->last) {
		list->last = node->prev;
		node->prev->next = NULL;
		free(node);
		return list->last;	
	}

	node_t *ret = node->prev;
	node->prev->next = node->next;
	node->next->prev = node->prev;
	free(node);
	return ret;
}

void list2_clear(list2_t *list) {

	if ((list == NULL) || (list->size == 0)) {
		return;
	}

	node_t *ptr;
	for (ptr=list->first; ptr!=NULL; ptr=ptr->next) {
		free(ptr->data);
	}
	list->size = 0;
}
