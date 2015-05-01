#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>
#include "polynomial.c"

/* The following is the size of a buffer to contain any error messages
   encountered when the regular expression is compiled. */

#define MAX_ERROR_MSG 0x1000
        

/* Compile the regular expression described by "regex_text" into
   "r". */

static int compile_regex (regex_t* r, const char* regex_text){
    int status = regcomp (r, regex_text, REG_EXTENDED);
    if (status != 0) {
	char error_message[MAX_ERROR_MSG];
	regerror (status, r, error_message, MAX_ERROR_MSG);
        printf ("Regex error compiling '%s': %s\n",
                 regex_text, error_message);
        return 1;
    }
    return 0;
}

/*
  Match the string in "to_match" against the compiled regular
  expression in "r".
 */

void getFactor(float* coefficient, int* power, const char* text){
	regex_t r; 	
	char* regex_text = "([\\+-]?[[:digit:]]+\\.?[[:digit:]]*)x\\^([[:digit:]]+)";
	regmatch_t matches[3];
	
	compile_regex(&r, regex_text);
	int nomatch = regexec(&r, text, 3, matches, 0);
    	if (nomatch) {
		printf ("No matches.\n");
	}

	for (int i = 0; i < 3; i++) {
            	if (matches[i].rm_so == -1) {
                	break;
            	}
            	int start = matches[i].rm_so;
            	int finish = matches[i].rm_eo;
            	if (i == 1){
			const char* tmp = text+start;
			*coefficient = (float)atof(tmp);			
		}
		if (i == 2){
			const char* tmp = text+start;
			*power = (int)strtol(tmp, NULL, 10);
		}
        } 
	regfree (&r);
	
}

int main(){
	const char* polynomialString = "3.2x^2+5.1x^3-4x^9";
 	struct polynomial* p = polynomial_new(NULL);   	
	
		float* coefficient = calloc(1, sizeof(float));
		int* power = calloc(1, sizeof(int));
		getFactor(coefficient, power, "3.2x^2");
		printf("coeffecient: %.2f\n", *coefficient);
		printf("power: %d\n", *power);
		free(coefficient);
		free(power);
	
	polynomial_free(p);
	return 0;
}