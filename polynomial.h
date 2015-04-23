#include "arrayList.c"

struct polynomial{
	struct arrayList coefficients;
	char* name;
};

struct polynomial* polynomial_new(char* name);

void polynomial_addCoefficient(struct polynomial *p, float coefficient, int pow);

int polynomial_rank(struct polynomial *p);

void polynomial_print(struct polynomial *p);

void polynomial_free(struct polynomial *p);

