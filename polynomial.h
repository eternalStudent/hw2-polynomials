#include "arrayList.c"

struct polynomial{
	struct arrayList* coefficients;
	char* name;
};

struct polynomial* polynomial_new(char* name, int size);

int polynomial_addCoefficient(struct polynomial *p, float coefficient, int pow);

float polynomial_getCoefficient(struct polynomial *p, int i);

int polynomial_rank(struct polynomial *p);

void polynomial_print(struct polynomial *p);

void polynomial_free(void* data);

struct polynomial* polynomial_sum(struct polynomial* p1, struct polynomial* p2);

struct polynomial* polynomial_subtract(struct polynomial* p1, struct polynomial* p2);

struct polynomial* polynomial_derive(struct polynomial* p);

float polynomial_evaluate(struct polynomial* p, float x);

struct polynomial* polynomial_multiplyByOneFactor(struct polynomial* p, float factorCoefficient, int factorPower);

struct polynomial* polynomial_multiply(struct polynomial* p1, struct polynomial* p2);

