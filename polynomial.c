#include "polynomial.h"
#include <stdio.h>
#include <stdlib.h>

struct polynomial* polynomial_new(char* name){
	struct polynomial *p;
	p = (struct polynomial*)calloc(1, sizeof(struct polynomial));
	p->coefficients = *arrayList_new(&free);
	p->name = name;
}

void polynomial_addCoefficient(struct polynomial *p, float coefficient, int pow){
	arrayList_set(&(p->coefficients), pow, (void*)&coefficient);
}

int polynomial_rank(struct polynomial *p){
	return arrayList_length(&(p->coefficients))-1;
}

void polynomial_print(struct polynomial *p){
	for (int i = 0; i < p->coefficients.length; i++){
		float coefficient = *((float*)(arrayList_get(&(p->coefficients), i)));
		if (coefficient!=0){
			if (i>0 && coefficient>0)
				printf("+");
			printf("%.2fx^%d", coefficient, i);
		}	
	}
	printf("\n");
}

void polynomial_free(struct polynomial *p){
	arrayList_free(&(p->coefficients));
	free(p->name);
	free(p);
}