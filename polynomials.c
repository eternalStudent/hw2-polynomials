#include "polynomials.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


struct polynomial* polynomial_new(char* name){
	struct polynomial* p;
	p = (struct polynomial*)calloc(1, sizeof(struct polynomial));
	if (!p)
		return NULL;
	p->coefficients = arrayList_new(&free);
	p->name = calloc(1, sizeof(char*));
	if (!(p->name))
		return NULL;
	if (name != NULL)
		strcpy(p->name, name);
	return p;
}

int polynomial_addCoefficient(struct polynomial *p, float coefficient, int power){
	float* pointer = (float*)calloc(1, sizeof(float));
	if (!pointer)
		return 1;
	*pointer = coefficient;
	return arrayList_set(p->coefficients, (void*)pointer, power);
}

int polynomial_rank(struct polynomial *p){
	return arrayList_length(p->coefficients)-1;
}

float polynomial_getCoefficient(struct polynomial *p, int power){
	void* pointer = arrayList_get(p->coefficients, power);
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
			if ((coefficient != 1 && coefficient != -1) || power == 0)	
				printf("%.2f", coefficient);
			if (coefficient == -1)
				printf("-");
			if (power > 0)
				printf("x");
			if (power > 1)
				printf("^%d", power);
		}
	}
	printf("\n");
}

void polynomial_free(void* data){
	struct polynomial* p = (struct polynomial*)data;
	arrayList_free(p->coefficients);
	free(p->name);
	free(p);
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
	subtraction = polynomial_new(NULL);
	int rank = max(polynomial_rank(p1), polynomial_rank(p2));
	if (p1==p2) {  /* handling subtraction of polynomial from itself*/
		rank = -1;
	}
	for (int power=0; power<=rank; power++){
		float coefficient = polynomial_getCoefficient(p1, power)-polynomial_getCoefficient(p2, power);
		polynomial_addCoefficient(subtraction, coefficient, power);
	}
	return subtraction;
}

struct polynomial* polynomial_derive(struct polynomial* p){
	struct polynomial* derivative;
	derivative = polynomial_new(NULL);
	int rank = polynomial_rank(p);
	for (int power=1; power<=rank; power++){
		float coefficient = power*polynomial_getCoefficient(p, power);
		polynomial_addCoefficient(derivative, coefficient, power-1);
	}
	return derivative;
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

struct polynomial* polynomial_multiplyByOneFactor(struct polynomial* p, float factorCoefficient, int factorPower){
	struct polynomial* product;
	product = polynomial_new(NULL);
	int rank = polynomial_rank(p);
	for (int power=0; power<=rank; power++){
		float coefficient = polynomial_getCoefficient(p, power);
		polynomial_addCoefficient(product, coefficient*factorCoefficient, power+factorPower);
	}
	return product;
}

struct polynomial* polynomial_multiply(struct polynomial* p1, struct polynomial* p2){
	struct polynomial* sumOfProducts;
	sumOfProducts = polynomial_new(NULL);
	int rank = polynomial_rank(p1);
	for (int power=0; power<=rank; power++){
		float coefficient = polynomial_getCoefficient(p1, power);
		struct polynomial* product = polynomial_multiplyByOneFactor(p2, coefficient, power);
		struct polynomial* temp = polynomial_sum(sumOfProducts, product);
		polynomial_free(product);
		polynomial_free(sumOfProducts);
		sumOfProducts = temp;
	}
	return sumOfProducts;
}