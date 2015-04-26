#include <stdio.h>
#include "polynomial.c"

int main(){
	struct polynomial* p1 = polynomial_new("p1");
	polynomial_addCoefficient(p1, 4.0, 0);
	polynomial_addCoefficient(p1, 5.0, 3);
	polynomial_addCoefficient(p1, 1.2, 4);
	printf("p1(x) := ");
	polynomial_print(p1);
	
	struct polynomial* p2 = polynomial_new("p2");
	polynomial_addCoefficient(p2, 1.0, 2);
	polynomial_addCoefficient(p2, 2.0, 9);
	printf("p2(x) := ");
	polynomial_print(p2);
	
	struct polynomial* sum = polynomial_sum(p1, p2);
	printf("(p1+p2)(x) = ");
	polynomial_print(sum);
	
	struct polynomial* subtraction = polynomial_subtract(p1, p2);
	printf("(p1-p2)(x) = ");
	polynomial_print(subtraction);
	
	struct polynomial* derivative = polynomial_derive(p1);
	printf("p1'(x) = ");
	polynomial_print(derivative);
	
	float valueAt1 = polynomial_evaluate(p1, 1.0f);
	printf("p1(1) = %f\n", valueAt1);
	
	polynomial_free(p1);
	polynomial_free(p2);
	polynomial_free(sum);
	polynomial_free(subtraction);
	polynomial_free(derirative);
	
	return 0;
}