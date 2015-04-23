#include "arraylist.h"
#include <stdlib.h>
#include <string.h>

struct arrayList* arrayList_new(void(*free_fn)(void*)){
	struct arrayList *list;
	list = (struct arrayList*)calloc(1, sizeof(struct arrayList));
	if(!list) return NULL;
	list->size = 4;
	list->length = 0;
	list->free_fn = free_fn;
	if(!(list->array = (void**)calloc(sizeof(void*), list->size))) {
		free(list);
		return NULL;
	}
	return list;
}

void arrayList_free(struct arrayList *list){
	int i;
	for(i = 0; i < list->length; i++)
		if(list->array[i]) list->free_fn(list->array[i]);
	free(list->array);
	free(list);
}

void* arrayList_get(struct arrayList *list, int i){
	if(i >= list->length) 
		return NULL;
	return list->array[i];
}

static int arrayList_expand_internal(struct arrayList *list, int max){
	void *new_array;
	int new_size;

	if(max < list->size) 
		return 0;
	new_size = list->size << 1;
	if (new_size < max)
		new_size = max;
	new_array = realloc(list->array, new_size*sizeof(void*));
	if(!new_array) 
		return -1;
	list->array = (void**)new_array;
	(void)memset(list->array + list->size, 0, (new_size-list->size)*sizeof(void*));
	list->size = new_size;
	return 0;
}

int arrayList_set(struct arrayList *list, int i, void *data){
	if(arrayList_expand_internal(list, i+1))
		return -1;
	if(list->array[i]) 
		list->free_fn(list->array[i]);
	list->array[i] = data;
	if(list->length <= i) 
		list->length = i + 1;
	return 0;
}

int arrayList_add(struct arrayList *list, void *data){
	return arrayList_set(list, list->length, data);
}

int arrayList_length(struct arrayList *list){
	return list->length;
}