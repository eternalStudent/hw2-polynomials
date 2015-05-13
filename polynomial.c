#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>
#include "polynomialList.c"

#define MAX_ERROR_MSG 0x1000


struct arrayList* polynomials;        

/* Utility functions */


static int compile_regex (regex_t* r, const char* regex_text){
    int status = regcomp (r, regex_text, REG_EXTENDED);
    if (status != 0) {
		char error_message[MAX_ERROR_MSG];
		regerror(status, r, error_message, MAX_ERROR_MSG);
        printf("Regex error compiling '%s': %s\n", regex_text, error_message);
        return 1;
    }
    return 0;
}

char* getSubstring(char* str, int i, int range){
	char* substring = calloc(range+1, sizeof(char));
	strncpy(substring, str+i, range);
	return substring;
}

char* removeSpaces (char* str) {	
	char* res = calloc(strlen(str)+1,sizeof(char));
	int counter = 0;
	do {
		if (*str != ' '){
			counter++;
			*(res++) = *str;
		}
		} while (*(str++));
	return res-counter;
}

/* Functions for extracting factors from a polynomial string, handling all of the different cases */

int ax_POW_b(float* coefficient, int* power, const char* text){
	regex_t r; 	
	regmatch_t matches[3];
	
	char* pattern = "\\s*([\\+-]?\\s*[[:digit:]]+\\.?[[:digit:]]*)\\s*x\\s*\\^\\s*([[:digit:]]+)\\s*";
	compile_regex(&r, pattern);
	if (regexec(&r, text, 3, matches, 0) ==0 ){
		
		/* cleaning redundant spaces*/
		char* textCopy = calloc(strlen(text)+1, sizeof(char));
		strcpy(textCopy, text);
		int start = matches[1].rm_so;
		int range = matches[1].rm_eo-start;	
		char* coeffString;
		coeffString = getSubstring(textCopy, start,range);
		char* cleanCoeffString = removeSpaces(coeffString);
		
		*coefficient = (float)atof(cleanCoeffString);
		start = matches[2].rm_so;
		*power = (int)strtol(text+start, NULL, 10);
		
		free(textCopy);
		free (coeffString);
		free (cleanCoeffString);
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
	
	char* pattern = "\\s*-\\s*x\\s*\\^\\s*([[:digit:]]+)\\s*";
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
	
	char* pattern = "\\s*([\\+-]?\\s*[[:digit:]]+\\.?[[:digit:]]*)\\s*x\\s*";
	compile_regex(&r, pattern);
	if (regexec(&r, text, 2, matches, 0) == 0) {
		
		
		/* cleaning redundant spaces*/
		char* textCopy = calloc(strlen(text)+1, sizeof(char));
		strcpy(textCopy, text);
		int start = matches[1].rm_so;
		int range = matches[1].rm_eo-start;	
		char* coeffString;
		coeffString = getSubstring(textCopy, start,range);
		char* cleanCoeffString = removeSpaces(coeffString);
		
		*coefficient = (float)atof(cleanCoeffString);
		*power = 1;
		
		free(textCopy);
		free(coeffString);
		free(cleanCoeffString);
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
	
	char* pattern = "\\s*([\\+-]?\\s*[[:digit:]]+\\.?[[:digit:]]*)\\s*";
	compile_regex(&r, pattern);
	if (regexec(&r, text, 2, matches, 0) == 0) {
		
		/* cleaning redundant spaces*/
		char* textCopy = calloc(strlen(text)+1, sizeof(char));
		strcpy(textCopy, text);
		int start = matches[1].rm_so;
		int range = matches[1].rm_eo-start;	
		char* coeffString;
		coeffString = getSubstring(textCopy, start,range);
		char* cleanCoeffString = removeSpaces(coeffString);
		
		*coefficient = (float)atof(cleanCoeffString);
		*power = 0;
		
		free(textCopy);
		free (coeffString);
		free (cleanCoeffString);
		regfree(&r);
		return 0;		
	}
	
	regfree(&r);
	return 1;
}

/* main function for extracting factors from a polynomial string */

int getFactor(float* coefficient, int* power, const char* text){
	/* ax^b case */ 
	if (ax_POW_b(coefficient, power, text) == 0)
		return 0;
	
	/* ax case */
    if (ax(coefficient, power, text) == 0)
		return 0;
	
	/* -x^b case */
    if (M_x_POW_b(coefficient, power, text) == 0)
		return 0;
	
	/* +x^b case */
    if (P_x_POW_b(coefficient, power, text) == 0)
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
	
	printf("error getting factor\n");
	return -1;
}

/* verifies a string is a polynomial string */

int isPolynomial(const char* str){
	regex_t r;
	char* pattern = "^((\\s*[\\+-]?[[:digit:]]+\\.?[[:digit:]]*\\s*)|(\\s*[\\+-]?([[:digit:]]+\\.?[[:digit:]]*)?\\s*x\\s*)|(\\s*[\\+-]?([[:digit:]]+\\.?[[:digit:]]*)?\\s*x\\s*\\^\\s*[[:digit:]]+\\s*))((\\s*[\\+-][[:digit:]]+\\.?[[:digit:]]*\\s*)|(\\s*[\\+-]([[:digit:]]+\\.?[[:digit:]]*)?\\s*x\\s*)|(\\s*[\\+-]([[:digit:]]+\\.?[[:digit:]]*)?\\s*x\\s*\\^\\s*[[:digit:]]+\\s*))*$";
	compile_regex(&r, pattern);
    regmatch_t matches[1];
	if (regexec(&r, str, 1, matches, 0) == 0){
		regfree(&r);
		return 1;
	}	
	regfree(&r);
	return 0;
}

/* main function for converting a polynomial string into a polynomial "object" other functions could work with" */

struct polynomial* stringToPolynomial (char* name, char* str){
    struct polynomial* p = polynomial_new(name);
	if (p == NULL){
		printf("allocation error\n");
		return p;
	}
	regex_t r;
	char* pattern = "(\\s*[\\+-]?\\s*[[:digit:]]+\\.?[[:digit:]]*\\s*)|(\\s*[\\+-]?\\s*([[:digit:]]+\\.?[[:digit:]]*)?\\s*x\\s*)|(\\s*[\\+-]?\\s*([[:digit:]]+\\.?[[:digit:]]*)?\\s*x\\s*\\^\\s*[[:digit:]]+\\s*)";
	compile_regex(&r, pattern);
	const char* pos = str;
    regmatch_t match[1];
	
    while (1) {
        int nomatch = regexec (&r, pos, 1, match, 0);
        if (nomatch){
			regfree(&r);
            return p;
		}	
		
        if (match[0].rm_so != -1) {   
			int start = match[0].rm_so + (pos - str);
            int range = match[0].rm_eo + (pos - str) - start;
			char* substring = getSubstring(str, start, range);
			float coefficient;
			int power;
            getFactor(&coefficient, &power, substring);
			free(substring);
			if (polynomial_addCoefficient(p, coefficient, power)){
				printf("error adding coefficient\n");
				regfree(&r);
				return p;
			}
        }
        pos += match[0].rm_eo;
    }
}

/* checks if a given string is a legal variable name */

int isValidName(char* name){
	/* checks if name is legal */
	regex_t r;
	regmatch_t matches[3];
	int exitCodeValid = 1; /*is valid*/
	char* pattern = "^[A-Za-z][A-Za-z0-9]*$";
	compile_regex(&r, pattern);
	if (regexec(&r, name, 3, matches, 0) != 0 ){
		exitCodeValid = 0;
	}
	
	/* checks if name is identical to a legal command or to "x" */
	int isCommandDer = strcmp(name, "der\0");
	int isCommandEval = strcmp (name, "eval\0");
	int isCommandQuit = strcmp (name, "quit\0");
	int isX = strcmp(name, "x\0");
	if ((isCommandDer==0) | (isCommandEval==0) | (isCommandQuit==0) | (isX==0)){
		exitCodeValid = 0;
	}
	regfree(&r);
	return exitCodeValid;
}


/* various functions for handling polynomials */


int definePolynomial(char* str){
	regex_t r;
	regmatch_t matches[3];
	int exitcode = 1; /*did not match*/
	
	char* pattern = "\\s*(\\w*)\\s*=\\s*(.*)\\s*";
	compile_regex(&r, pattern);
	if (regexec(&r, str, 3, matches, 0) == 0 ){
		int start = matches[2].rm_so;
		int range = matches[2].rm_eo-start;
		char* polynomialString = getSubstring(str, start, range);
		char * cleanPolynomialString = removeSpaces(polynomialString);
		
		start = matches[1].rm_so;
		range = matches[1].rm_eo - start;
		char* name = getSubstring(str, start, range);
		while(1){
			if (!isPolynomial(cleanPolynomialString)){
				exitcode = 1;
				break;
			}
			if (!isValidName(name)){
				printf("illegal variable name\n");
				exitcode = 0; /*compiled successfully*/
				break;
			}
			struct polynomial* p = stringToPolynomial(name, cleanPolynomialString);
			if (p == NULL){
				printf("allocation error\n");
				exitcode = -3;
				break;
			}
			int i;
			if (polynomialList_getByName(polynomials, name, &i) != NULL){
				polynomialList_update(polynomials, p, i);
				printf("updated %s\n", name);
				exitcode = 0; /*compiled successfully*/
				break;
			}
			else if (polynomialList_add(polynomials, p)){
				printf("allocation error\n");
				exitcode = -3;
				break;
			}
			else
				printf("created %s\n", name);
				exitcode = 0; /*compiled successfully*/
				break;
		}
		free(name);
		free(polynomialString);		
		free(cleanPolynomialString);
	}
	regfree(&r);
	return exitcode;
}

int printPolynomial(char* name){
	if (!isValidName(name)){
		printf("illegal variable name\n");
		return -1;
	}
	struct polynomial* p = polynomialList_getByName(polynomials, name, NULL);
	if (p == NULL){
		printf("unknown polynomial %s\n", name);
		return -1;
	}	
	polynomial_print(p);
	return 0;
}

int summation(char* str){
	regex_t r;
	regmatch_t matches[3];
	int exitcode = 1; /*did not match*/
	
	char* pattern = "\\s*(\\w*)\\s*\\+\\s*(\\w*)\\s*";
	compile_regex(&r, pattern);
	if (regexec(&r, str, 3, matches, 0) == 0 ){
		char* name1;
		char* name2;
		while (1){
			int start = matches[1].rm_so;
			int range = matches[1].rm_eo-start;
			name1 = getSubstring(str, start, range);
			
			start = matches[2].rm_so;
			range = matches[2].rm_eo;
			name2 = getSubstring(str, start, range);
			
			struct polynomial* p1 = polynomialList_getByName(polynomials, name1, NULL);
			if (p1 == NULL){
				printf("unknown polynomial %s\n", name1);
				exitcode = 0; /*compiled successfully*/
				break;
			}

			struct polynomial* p2 = polynomialList_getByName(polynomials, name2, NULL);
			if (p2 == NULL){
				printf("unknown polynomial %s\n", name2);
				exitcode = 0; /*compiled successfully*/
				break;
			}
			struct polynomial* sum = polynomial_sum(p1, p2);
			
			if (sum == NULL){
				printf("allocation error\n");
				exitcode = -3;
				break;
			}
		
			polynomial_print(sum);
			polynomial_free(sum);
			exitcode = 0; /*compiled successfully*/
			break;
		}	
		free(name1);
		free(name2);		
	}

	regfree(&r);
	
	return exitcode;
}



int compoundSum(char* str){
	regex_t r;
	regmatch_t matches[4];
	int exitcode = 1; /*did not match*/
	
	char* pattern = "^(\\w*)[[:space:]]*=[[:space:]]*(\\w*)[[:space:]]*\\+[[:space:]]*(\\w*)$";
	compile_regex(&r, pattern);
	if (regexec(&r, str, 4, matches, 0) == 0 ){
		char* firstSummedPolyName;
		char* secondSummedPolyName;
		char* destinationName;
		int i;
		while(1) {
			int start = matches[1].rm_so;
			int range = matches[1].rm_eo-start;
			destinationName = getSubstring(str, start, range);
			
			start = matches[2].rm_so;
			range = matches[2].rm_eo-start;
			firstSummedPolyName = getSubstring(str, start, range);
			
			start = matches[3].rm_so;
			range = matches[3].rm_eo-start;
			secondSummedPolyName = getSubstring(str, start, range);
			
			if (!isValidName(destinationName)){
				printf("illegal variable name\n");
				exitcode = 0; /*compiled successfully*/	
				break;
			}

			struct polynomial* firstSummedPoly = polynomialList_getByName(polynomials, firstSummedPolyName, NULL);
			if (firstSummedPoly == NULL){
				printf("unknown polynomial %s\n", firstSummedPolyName);
				exitcode = 0; /*compiled successfully*/	
				break;
			}
					
			struct polynomial* secondSummedPoly = polynomialList_getByName(polynomials, secondSummedPolyName, NULL);
			if (secondSummedPoly == NULL){
				printf("unknown polynomial %s\n", secondSummedPolyName);
				exitcode = 0; /*compiled successfully*/	
				break;
			}
			
			struct polynomial* polySum = polynomial_sum(firstSummedPoly,secondSummedPoly);
			if (polySum == NULL){
				printf("allocation error\n");
				exitcode = -3;
				break;
			}
					
			strcpy(polySum->name, destinationName);
			
			if (polynomialList_getByName(polynomials, destinationName, &i) != NULL){
				polynomialList_update(polynomials, polySum, i);
				printf("updated %s\n", destinationName);
				exitcode = 0; /*compiled successfully*/	
				break;
			}
			else if (polynomialList_add(polynomials, polySum)){
				printf("allocation error\n");
				exitcode = -3;
				break;
				}
			else{
				printf("created %s\n", destinationName);
				exitcode = 0; /*compiled successfully*/	
				break;
				}		
		}
		
		free(firstSummedPolyName);
		free(secondSummedPolyName);
		free(destinationName);

	}
	
	regfree(&r);	
	return exitcode;
}


int subtraction(char* str){
	regex_t r;
	regmatch_t matches[3];
	int exitcode = 1; /*did not match*/
	
	char* pattern = "\\s*(\\w*)\\s*\\-\\s*(\\w*)\\s*";
	compile_regex(&r, pattern);
	if (regexec(&r, str, 3, matches, 0) == 0 ){
		char* name1;
		char* name2;
		while (1){
			int start = matches[1].rm_so;
			int range = matches[1].rm_eo-start;
			name1 = getSubstring(str, start, range);
			
			start = matches[2].rm_so;
			range = matches[2].rm_eo;
			name2 = getSubstring(str, start, range);
			
			struct polynomial* p1 = polynomialList_getByName(polynomials, name1, NULL);
			if (p1 == NULL){
				printf("unknown polynomial %s\n", name1);
				exitcode = 0; /*compiled successfully*/
				break;
			}
			
			struct polynomial* p2 = polynomialList_getByName(polynomials, name2, NULL);
			if (p2 == NULL){
				printf("unknown polynomial %s\n", name2);
				exitcode = 0; /*compiled successfully*/
				break;
			}
						
			struct polynomial* difference = polynomial_subtract(p1, p2);
			
			if (difference == NULL){
				printf("allocation error\n");
				exitcode = -3;
				break;
			}
					
			polynomial_print(difference);
			polynomial_free(difference);
			exitcode = 0; /*compiled successfully*/
			break;
		}	
	
		free(name1);
		free(name2);
	}
	
	regfree(&r);
	
	return exitcode;
}


int compoundSubtraction(char* str){
	regex_t r;
	regmatch_t matches[4];
	int exitcode = 1; /*did not match*/
	
	char* pattern = "^(\\w*)[[:space:]]*=[[:space:]]*(\\w*)[[:space:]]*-[[:space:]]*(\\w*)$";
	compile_regex(&r, pattern);
	if (regexec(&r, str, 4, matches, 0) == 0 ){
		char* firstSubtractedPolyName;
		char* secondSubtractedPolyName;
		char* destinationName;
		int i;
		while(1) {
			int start = matches[1].rm_so;
			int range = matches[1].rm_eo-start;
			destinationName = getSubstring(str, start, range);
			
			start = matches[2].rm_so;
			range = matches[2].rm_eo-start;
			firstSubtractedPolyName = getSubstring(str, start, range);
			
			start = matches[3].rm_so;
			range = matches[3].rm_eo-start;
			secondSubtractedPolyName = getSubstring(str, start, range);
			
			
			if (!isValidName(destinationName)){
				printf("illegal variable name\n");
				exitcode = 0; /*compiled successfully*/	
				break;
			}
			
			struct polynomial* firstSubtractedPoly = polynomialList_getByName(polynomials, firstSubtractedPolyName, NULL);
			if (firstSubtractedPoly == NULL){
				printf("unknown polynomial %s\n", firstSubtractedPolyName);
				exitcode = 0; /*compiled successfully*/	
				break;
			}
			
		
			struct polynomial* secondSubtractedPoly = polynomialList_getByName(polynomials, secondSubtractedPolyName, NULL);
			if (secondSubtractedPoly == NULL){
				printf("unknown polynomial %s\n", secondSubtractedPolyName);
				exitcode = 0; /*compiled successfully*/	
				break;
			}
			
			struct polynomial* polyDifference = polynomial_subtract(firstSubtractedPoly,secondSubtractedPoly);
			strcpy(polyDifference->name, destinationName);

			
			if (polynomialList_getByName(polynomials, destinationName, &i) != NULL){
				polynomialList_update(polynomials, polyDifference, i);
				printf("updated %s\n", destinationName);
				exitcode = 0; /*compiled successfully*/	
				break;
			}
			else if (polynomialList_add(polynomials, polyDifference)){
				printf("allocation error\n");
				exitcode = -3;
				break;
				}
			else{
				printf("created %s\n", destinationName);
				exitcode = 0; /*compiled successfully*/	
				break;
				}		
		}
		
		free(firstSubtractedPolyName);
		free(secondSubtractedPolyName);
		free(destinationName);
	}

	regfree(&r);
	
	return exitcode;
}


int multiplication(char* str){
	regex_t r;
	regmatch_t matches[3];
	int exitcode = 1; /*did not match*/
	
	char* pattern = "\\s*(\\w*)\\s*\\*\\s*(\\w*)\\s*";
	compile_regex(&r, pattern);
	if (regexec(&r, str, 3, matches, 0) == 0 ){
		char* name1;
		char* name2;
		while (1){
			int start = matches[1].rm_so;
			int range = matches[1].rm_eo-start;
			name1 = getSubstring(str, start, range);
			
			start = matches[2].rm_so;
			range = matches[2].rm_eo;
			name2 = getSubstring(str, start, range);
			
			struct polynomial* p1 = polynomialList_getByName(polynomials, name1, NULL);
			if (p1 == NULL){
				printf("unknown polynomial %s\n", name1);
				exitcode = 0; /*compiled successfully*/
				break;
			}

			struct polynomial* p2 = polynomialList_getByName(polynomials, name2, NULL);
			if (p2 == NULL){
				printf("unknown polynomial %s\n", name2);
				exitcode = 0; /*compiled successfully*/
				break;
			}
			struct polynomial* product = polynomial_multiply(p1, p2);
			
			if (product == NULL){
				printf("allocation error\n");
				exitcode = -3;
				break;
			}
					
			polynomial_print(product);
			polynomial_free(product);
			exitcode = 0; /*compiled successfully*/
			break;
		}	
		
		free(name1);
		free(name2);
	}
	regfree(&r);
	return exitcode;
}

int compoundMultiplication(char* str){
	regex_t r;
	regmatch_t matches[4];
	int exitcode = 1; /*did not match*/
	
	char* pattern = "^(\\w*)[[:space:]]*=[[:space:]]*(\\w*)[[:space:]]*\\*[[:space:]]*(\\w*)$";
	compile_regex(&r, pattern);
	if (regexec(&r, str, 4, matches, 0) == 0 ){
		char* firstMultipliedPolyName;
		char* secondMultipliedPolyName;
		char* destinationName;
		int i;
		while(1) {
			int start = matches[1].rm_so;
			int range = matches[1].rm_eo-start;
			destinationName = getSubstring(str, start, range);
			
			
			start = matches[2].rm_so;
			range = matches[2].rm_eo-start;
			firstMultipliedPolyName = getSubstring(str, start, range);
			
			start = matches[3].rm_so;
			range = matches[3].rm_eo-start;
			secondMultipliedPolyName = getSubstring(str, start, range);
			
			if (!isValidName(destinationName)){
				printf("illegal variable name\n");
				exitcode = 0; /*compiled successfully*/	
				break;
			}
			
			struct polynomial* firstMultipliedPoly = polynomialList_getByName(polynomials, firstMultipliedPolyName, NULL);
			if (firstMultipliedPoly == NULL){
				printf("unknown polynomial %s\n", firstMultipliedPolyName);
				exitcode = 0; /*compiled successfully*/	
				break;
			}
						
			struct polynomial* secondMultipliedPoly = polynomialList_getByName(polynomials, secondMultipliedPolyName, NULL);
			if (secondMultipliedPoly == NULL){
				printf("unknown polynomial %s\n", secondMultipliedPolyName);
				exitcode = 0; /*compiled successfully*/	
				break;
			}
			
			struct polynomial* polyProduct = polynomial_multiply(firstMultipliedPoly,secondMultipliedPoly);		
			strcpy(polyProduct->name, destinationName);
			
			
			if (polynomialList_getByName(polynomials, destinationName, &i) != NULL){
				polynomialList_update(polynomials, polyProduct, i);
				printf("updated %s\n", destinationName);
				exitcode = 0; /*compiled successfully*/	
				break;
			}
			else if (polynomialList_add(polynomials, polyProduct)){
				printf("allocation error\n");
				exitcode = -3;
				break;
				}
			else{
				printf("created %s\n", destinationName);
				exitcode = 0; /*compiled successfully*/	
				break;
				}		
		}
		
		free(firstMultipliedPolyName);
		free(secondMultipliedPolyName);
		free(destinationName);
	}
	regfree(&r);
	return exitcode;
}


int derivation(char* str) {
	regex_t r; 	
	regmatch_t matches[2];
	int exitcode = 1; /*did not match*/
	char* pattern = "^der[[:space:]]+(\\w*)$";
	compile_regex(&r, pattern);
	if (regexec(&r, str, 2, matches, 0) == 0){
		char* name;
		while(1){	
			int start = matches[1].rm_so;
			int range = matches[1].rm_eo-start;
			name = getSubstring(str, start, range);
			
			struct polynomial* polyToDerive = polynomialList_getByName(polynomials, name, NULL);
			if (polyToDerive == NULL){
				printf ("unknown polynomial %s\n",name);
				exitcode = 0;
				break;
			}
			
			struct polynomial* derivative = polynomial_derive(polyToDerive);
			
			if (derivative == NULL){
				printf("allocation error\n");
				exitcode = -3;
				break;
			}
			
			polynomial_print(derivative);
			polynomial_free(derivative);
			exitcode = 0;
			break;
			}			
		free(name);
	}
	
	regfree(&r);
	return exitcode;
}


int compoundDerivation (char* str){
	regex_t r; 	
	regmatch_t matches[3];
	int exitcode = 1; /*did not match*/
	char* pattern = "^(\\w*)[[:space:]]*=[[:space:]]*der[[:space:]]*(\\w*)$";
	compile_regex(&r, pattern);
	if (regexec(&r, str, 3, matches, 0) == 0){
		int i;
		char* destinationName;
		char* sourceName;
		while(1) {
		
			int start = matches[1].rm_so;
			int range = matches[1].rm_eo-start;
			destinationName = getSubstring(str, start, range);
			
			start = matches[2].rm_so;
			range = matches[2].rm_eo-start;
			sourceName = getSubstring(str, start, range);
			
			if (!isValidName(destinationName)){
				printf("illegal variable name\n");
				exitcode = 0; /*compiled successfully*/					
				break;
			}
			
			struct polynomial* source = polynomialList_getByName(polynomials, sourceName, NULL);
			
			if (source == NULL){
				printf ("unknown polynomial %s\n",sourceName);
				exitcode = 0; /*compiled successfully*/					
				break;
			}
			
			struct polynomial* derivative = polynomial_derive(source);
			if (derivative == NULL){
				printf ("allocation error\n");
				exitcode = -3;
				break;
			}
			
			strcpy(derivative->name, destinationName);
			
			if (polynomialList_getByName(polynomials, destinationName, &i) != NULL){
				polynomialList_update(polynomials, derivative, i);
				printf("updated %s\n", destinationName);
				exitcode = 0; /*compiled successfully*/	
				break;
			}
			else if (polynomialList_add(polynomials, derivative)){
				printf("allocation error\n");
				exitcode = -3;	/* memory allocation error */			
				break;
			}
			else{
				printf("created %s\n", destinationName);
				exitcode = 0; /*compiled successfully*/	
				break;
			}	
		}			
		free(destinationName);
		free(sourceName);
	}
	
	regfree(&r);

	return exitcode;
}


int evaluation(char* str) {
	regex_t r; 	
	regmatch_t matches[3];
	int exitcode = 1; /*did not match*/
	char* pattern = "^eval[[:space:]]+(\\w*)[[:space:]]+([-]?[0-9]+\\.?[0-9]*)$";
	compile_regex(&r, pattern);
	if (regexec(&r, str, 3, matches, 0) == 0){
		char* name;
		float number;
		while(1){	
			int start = matches[1].rm_so;
			int range = matches[1].rm_eo-start;
			name = getSubstring(str, start, range);
			
			start = matches[2].rm_so;
			range = matches[2].rm_eo-start;
			
			char* numberAsString = getSubstring(str, start, range);
			number = atof(numberAsString);
			free (numberAsString);
			
			struct polynomial* polyToEvaluate = polynomialList_getByName(polynomials, name, NULL);
			if (polyToEvaluate == NULL){
				printf ("unknown polynomial %s\n",name);
				break;
			}
			
			float evaluation = polynomial_evaluate(polyToEvaluate, number);	
			printf("%.2f\n", evaluation);
			break;
		}

		free(name);
		exitcode = 0;			
	}
		
	regfree(&r);
	
	return exitcode;
}

/* main control function */

int executeCommand(char* command){
	int error;
	int isCompound = 0;
	command = strtok(command, "\n");
	
	
	error = definePolynomial(command);
		if (error != 1) {
			return error;
		}
		

	error = compoundDerivation(command);
	if (error == 0) {
		isCompound = 1;
	}
	if (error != 1) {
		return error;
	}
	
	error = compoundSum(command);
	if (error == 0) {
		isCompound = 1;
	}
	if (error != 1) {
		return error;
	}
	
	error = compoundSubtraction(command);
	if (error == 0) {
		isCompound = 1;
	}
	if (error != 1) {
		return error;
	}
	error = compoundMultiplication(command);
	if (error == 0) {
		isCompound = 1;
	}
	if (error != 1) {
		return error;
	}
	
	
	if(!isCompound) {

		error = derivation(command);
		if (error != 1) {
			return error;
		}
		error = evaluation(command);
		if (error != 1) {
			return error;
		}
		error = summation(command);
		if (error != 1) {
			return error;
		}
		error = subtraction(command);
		if (error != 1) {
			return error;
		}
		error = multiplication(command);
		if (error != 1) {
			return error;
		}
		error = printPolynomial(command);
		if (error != 1) {	
			return error;
		}
			
	}
	return -2;
}

int main(){
	polynomials  = polynomialList_new();
	char command[256];
	printf("Welcome to polynomials!\n");
	while (1){
		int error;
		printf("> ");
		if (fgets(command, 256, stdin) == NULL){
			printf("error in command\n");
			continue;
		}
		
		if (command[0] == '\n'){
			continue;
		}
		if (strcmp(command, "quit\n") == 0){
			break;
		}	
		error = executeCommand(command);
		if (error == -2)	
			printf("error reading command\n");
		
		if(error == -3) { /* memory allocation error */
			break;
		}
	}
	polynomialList_free(polynomials);
	return 0;
}