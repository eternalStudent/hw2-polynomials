#include "polynomial.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


struct polynomial* polynomial_new(char* name){
	struct polynomial *p;
	p = (struct polynomial*)calloc(1, sizeof(struct polynomial));
	p->coefficients = *arrayList_new(&free);
	p->name = name;
}

void polynomial_addCoefficient(struct polynomial *p, float coefficient, int power){
	float* pointer = (float*)calloc(1, sizeof(float));
	*pointer = coefficient;
	arrayList_set(&(p->coefficients), (void*)pointer, power);
}

int polynomial_rank(struct polynomial *p){
	return arrayList_length(&(p->coefficients))-1;
}

float polynomial_getCoefficient(struct polynomial *p, int power){
	void* pointer = arrayList_get(&(p->coefficients), power);
	return (pointer == 0 || pointer == NULL)? 0: *((float *)pointer);
}

void polynomial_print(struct polynomial *p){
	int rank = polynomial_rank(p);
	if (rank == -1){
		printf("0\n");
		return;
	}
	int first_non_zero = 0;
	for (int power = 0; power <= rank; power++){
		float coefficient = polynomial_getCoefficient(p, power);
		if (coefficient != 0.0){
			if (first_non_zero && coefficient > 0)
				printf("+");
			first_non_zero = 1;
			if (coefficient != 1 || power == 0)	
				printf("%.2f", coefficient);
			if (power > 0)
				printf("x");
			if (power > 1)
				printf("^%d", power);
		}
	}
	printf("\n");
}

void polynomial_free(struct polynomial* p){
	arrayList_free(&(p->coefficients));
	/*free(p);*/
}

static int max(int a, int b){
	return (a > b)? a: b;
}

struct polynomial* polynomial_sum(struct polynomial* p1, struct polynomial* p2){
	struct polynomial* sum;
	sum = polynomial_new(NULL);
	int rank = max(polynomial_rank(p1), polynomial_rank(p2));
	for (int power=0; power<=rank; power++){
		float coefficient = polynomial_getCoefficient(p1, power)+polynomial_getCoefficient(p2, power);
		polynomial_addCoefficient(sum, coefficient, power);
	}
	return sum;
}

struct polynomial* polynomial_subtract(struct polynomial* p1, struct polynomial* p2){
	struct polynomial* subtraction;
	subtraction = polynomial_new("");
	int rank = max(polynomial_rank(p1), polynomial_rank(p2));
	for (int power=0; power<=rank; power++){
		float coefficient = polynomial_getCoefficient(p1, power)-polynomial_getCoefficient(p2, power);
		polynomial_addCoefficient(subtraction, coefficient, power);
	}
	return subtraction;
}

struct polynomial* polynomial_derive(struct polynomial* p){
	struct polynomial* derirative;
	derirative = polynomial_new("");
	int rank = polynomial_rank(p);
	for (int power=1; power<=rank; power++){
		float coefficient = power*polynomial_getCoefficient(p, power);
		polynomial_addCoefficient(derirative, coefficient, power-1);
	}
	return derirative;
}

float polynomial_evaluate(struct polynomial* p, float x){
	float sum = 0;
	int rank = polynomial_rank(p);
	for (int power=0; power<=rank; power++){
		float coefficient = polynomial_getCoefficient(p, power);
		sum += coefficient*pow(x, power);
	}
	return sum;
}