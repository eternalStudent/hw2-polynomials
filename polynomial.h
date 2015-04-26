#include "arrayList.c"

struct polynomial{
	struct arrayList coefficients;
	char* name;
};

struct polynomial* polynomial_new(char* name);

void nothing(void* v);

void polynomial_addCoefficient(struct polynomial *p, float* coefficient, int pow);

float polynomial_getCoefficient(struct polynomial *p, int i);

int polynomial_rank(struct polynomial *p);

void polynomial_print(struct polynomial *p);

void polynomial_free(struct polynomial *p);

struct polynomial* polynomial_sum(struct polynomial* p1, struct polynomial* p2);

