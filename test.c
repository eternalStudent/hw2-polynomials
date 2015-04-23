#include <stdio.h>
#include "polynomial.c"

int main(){
	struct polynomial* p;
	p = polynomial_new("p");
	polynomial_addCoefficient(p, 5.0, 3);
	polynomial_print(p);
	polynomial_free(p);
	return 0;
}