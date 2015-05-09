#include "polynomials.c"


struct arrayList* polynomialList_new();

int polynomialList_add(struct arrayList* polynomials, struct polynomial* p);

struct polynomial* polynomialList_getByName(struct arrayList* polynomials, char* name, int* i);

int polynomialList_update(struct arrayList* polynomials, struct polynomial* p, int i);

void polynomialList_free(struct arrayList* polynomials);