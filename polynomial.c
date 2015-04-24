#include "polynomial.h"
#include <stdio.h>
#include <stdlib.h>

struct polynomial* polynomial_new(char* name){
	struct polynomial *p;
	p = (struct polynomial*)calloc(1, sizeof(struct polynomial));
	p->coefficients = *arrayList_new(&nothing);
	p->name = name;
}

void nothing(void* v){}

void polynomial_addCoefficient(struct polynomial *p, float coefficient, int power){
	void* pointer = &coefficient;
	arrayList_set(&(p->coefficients), power, pointer);
}

int polynomial_rank(struct polynomial *p){
	return arrayList_length(&(p->coefficients))-1;
}

float polynomial_getCoefficient(struct polynomial *p, int power){
	void* pointer = arrayList_get(&(p->coefficients), power);
	return (pointer == 0)? 0: *((float *)pointer);
}

void polynomial_print(struct polynomial *p){
	int rank = polynomial_rank(p);
	int first_non_zero = 0;
	for (int power = 0; power <= rank; power++){
		float coefficient = polynomial_getCoefficient(p, power);
		if (coefficient != 0){
			if (first_non_zero && coefficient > 0)
				printf("+");
			first_non_zero = 1;
			printf("%.2fx^%d", coefficient, power);
		}
	}
	printf("\n");
}

void polynomial_free(struct polynomial *p){
	arrayList_free(&(p->coefficients));
	/*free(p);*/
}