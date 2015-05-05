#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>
#include "polynomial.c"

#define MAX_ERROR_MSG 0x1000
        

static int compile_regex (regex_t* r, const char* regex_text){
    int status = regcomp (r, regex_text, REG_EXTENDED);
    if (status != 0) {
		char error_message[MAX_ERROR_MSG];
		regerror(status, r, error_message, MAX_ERROR_MSG);
        printf("Regex error compiling '%s': %s\n",
                 regex_text, error_message);
        return 1;
    }
    return 0;
}

int ax_POW_b(float* coefficient, int* power, const char* text){
	regex_t r; 	
	regmatch_t matches[3];
	
	char* pattern = "\\s*([\\+-]?[[:digit:]]+\\.?[[:digit:]]*)\\s*x\\s*\\^\\s*([[:digit:]]+)\\s*";
	compile_regex(&r, pattern);
	if (regexec(&r, text, 3, matches, 0) ==0 ){
		int start = matches[1].rm_so;
		*coefficient = (float)atof(text+start);
		start = matches[2].rm_so;
		*power = (int)strtol(text+start, NULL, 10);
		regfree(&r);
		return 0;
	}
	
	regfree(&r);
	return 1;
}

int P_x_POW_b(float* coefficient, int* power, const char* text){
	regex_t r; 	
	regmatch_t matches[2];
	
	char* pattern = "\\s*\\+?x\\s*\\^\\s*([[:digit:]]+)\\s*";
	compile_regex(&r, pattern);
	if (regexec(&r, text, 2, matches, 0) == 0) {
		int start = matches[1].rm_so;
		*coefficient = 1;
		*power = (int)strtol(text+start, NULL, 10);
		regfree(&r);
		return 0;
	}
	
	regfree(&r);
	return 1;
}

int M_x_POW_b(float* coefficient, int* power, const char* text){
	regex_t r; 	
	regmatch_t matches[2];
	
	char* pattern = "\\s*-x\\s*\\^\\s*([[:digit:]]+)\\s*";
	compile_regex(&r, pattern);
	if (regexec(&r, text, 2, matches, 0) == 0) {
		int start = matches[1].rm_so;
		*coefficient = -1;
		*power = (int)strtol(text+start, NULL, 10);
		regfree(&r);
		return 0;
	}
	
	regfree(&r);
	return 1;
}

int ax(float* coefficient, int* power, const char* text){
	regex_t r; 	
	regmatch_t matches[2];
	
	char* pattern = "\\s*-x\\s*\\^\\s*([[:digit:]]+)\\s*";
	compile_regex(&r, pattern);
	if (regexec(&r, text, 2, matches, 0) == 0) {
		int start = matches[1].rm_so;
		*coefficient = -1;
		*power = (int)strtol(text+start, NULL, 10);
		regfree(&r);
		return 0;
	}
	
	regfree(&r);
	return 1;
}

int P_x(float* coefficient, int* power, const char* text){
	regex_t r; 	
	regmatch_t matches[1];
	
	char* pattern = "\\s*\\+?\\s*x\\s*";
	compile_regex(&r, pattern);
	if (regexec(&r, text, 1, matches, 0) == 0) {
		*coefficient = 1;
		*power = 1;
		regfree(&r);
		return 0;
	}
	
	regfree(&r);
	return 1;
}

int M_x(float* coefficient, int* power, const char* text){
	regex_t r; 	
	regmatch_t matches[1];
	
	char* pattern = "\\s*-\\s*x\\s*";
	compile_regex(&r, pattern);
	if (regexec(&r, text, 1, matches, 0) == 0) {
		*coefficient = -1;
		*power = 1;
		regfree(&r);
		return 0;
	}
	
	regfree(&r);
	return 1;
}

int ax_POW_0(float* coefficient, int* power, const char* text){
	regex_t r; 	
	regmatch_t matches[2];
	
	char* pattern = "\\s*([\\+-]?[[:digit:]]+\\.?[[:digit:]]*)\\s*";
	compile_regex(&r, pattern);
	if (regexec(&r, text, 2, matches, 0) == 0) {
		int start = matches[1].rm_so;
		*coefficient = (float)atof(text+start);
		*power = 0;
		regfree(&r);
		return 0;
	}
	
	regfree(&r);
	return 1;
}

int getFactor(float* coefficient, int* power, const char* text){
	/* ax^b case */ 
	if (ax_POW_b(coefficient, power, text) == 0)
		return 0;
	
	/* -x^b case */
    if (M_x_POW_b(coefficient, power, text) == 0)
		return 0;
	
	/* +x^b case */
    if (P_x_POW_b(coefficient, power, text) == 0)
		return 0;
	
	/* ax case */
    if (ax(coefficient, power, text) == 0)
		return 0;
	
	/* -x case */
    if (M_x(coefficient, power, text) == 0)
		return 0;
	
	/* +x case */
    if (P_x(coefficient, power, text) == 0)
		return 0;
	
	/* a case */
    if (ax_POW_0(coefficient, power, text) == 0)
		return 0;
	
	printf("baHHH\n");
	return -1;
}

struct polynomial* stringToPolynomial (const char* str){
    struct polynomial* p = polynomial_new("p1");
	if (p == NULL){
		printf("allocation error\n");
		return p;
	}
	regex_t r;
	char* pattern = "(\\s*[\\+-]?[[:digit:]]+\\.?[[:digit:]]*\\s*)|(\\s*[\\+-]?([[:digit:]]+\\.?[[:digit:]]*)?\\s*x\\s*)|(\\s*[\\+-]?([[:digit:]]+\\.?[[:digit:]]*)?\\s*x\\s*\\^\\s*[[:digit:]]+\\s*)";
	compile_regex(&r, pattern);
	const char* pos = str;
    regmatch_t match[1];
		
    while (1) {
        int nomatch = regexec (&r, pos, 1, match, 0);
        if (nomatch){
			regfree(&r);
            return p;
		}	
		
        int start;
        int finish;
        if (match[0].rm_so != -1) {   
            start = match[0].rm_so + (pos - str);
            finish = match[0].rm_eo + (pos - str);
			char* substring = calloc(1, sizeof(char));
			strncpy(substring, str+start, finish-start);
			
			float coefficient;
			int power;
			getFactor(&coefficient, &power, substring);
			if (polynomial_addCoefficient(p, coefficient, power)){
				printf("error adding coefficient\n");
				return p;
			}	
			
			free(substring);
        }
        pos += match[0].rm_eo;
    }
	regfree(&r);
}

int main(){
	char polynomialString[256];
	printf("enter a polynomial string: ");
	scanf("%s", polynomialString);
	struct polynomial* p = stringToPolynomial(polynomialString);
	if (p == NULL){
		printf("program exited with errors\n");
		return -1;
	}
	polynomial_print(p);
	polynomial_free(p);
	return 0;
}