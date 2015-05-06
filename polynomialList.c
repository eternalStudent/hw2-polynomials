#include "polynomialList.h"


struct arrayList* polynomialList_new(){
	return arrayList_new(&polynomial_free);
}

int polynomialList_add(struct arrayList* polynomials, struct polynomial* p){
	return arrayList_add(polynomials, (void*)p);
}

struct polynomial* polynomialList_getByName(struct arrayList* polynomials, char* name){
	int length = arrayList_length(polynomials);
	for (int i=0; i<length; i++){
		struct polynomial* p = (struct polynomial*)arrayList_get(polynomials, i);
		if (strcmp(name, p->name) == 0)
			return p;
	}
	return NULL;
}

void polynomialList_free(struct arrayList* polynomials){
	arrayList_free(polynomials);
}