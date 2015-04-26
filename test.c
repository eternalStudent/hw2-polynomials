#include <stdio.h>
#include "polynomial.c"

int main(){
	struct polynomial* p1;
	p1 = polynomial_new("p1");
	float c0 = 1;
	float c3 = 0.4f;
	float c4 = 1.3f;
	polynomial_addCoefficient(p1, &c0, 0);
	polynomial_addCoefficient(p1, &c3, 3);
	polynomial_addCoefficient(p1, &c4, 4);
	polynomial_print(p1);
	
	struct polynomial* p2;
	p2 = polynomial_new("p2");
	float d2 = 5.4f;
	float d9 = 1.0f;
	polynomial_addCoefficient(p2, &d2, 2);
	polynomial_addCoefficient(p2, &d9, 9);
	polynomial_print(p2);
	
	struct polynomial* sum;
	sum = polynomial_sum(p1, p2);
	polynomial_print(sum);
	
	return 0;
}