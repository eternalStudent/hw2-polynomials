#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>
#include "polynomialList.c"

#define MAX_ERROR_MSG 0x1000


struct arrayList* polynomials;        

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
	
	char* pattern = "\\s*([\\+-]?[[:digit:]]+\\.?[[:digit:]]*)\\s*x\\s*";
	compile_regex(&r, pattern);
	if (regexec(&r, text, 2, matches, 0) == 0) {
		int start = matches[1].rm_so;
		*coefficient = (float)atof(text+start);
		*power = 1;
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
	
	printf("baHHH\n");
	return -1;
}

int isPolynomial(const char* str){
	regex_t r;
	char* pattern = "(\\s*[\\+-]?[[:digit:]]+\\.?[[:digit:]]*\\s*)?\\s*(\\s*[\\+-]?([[:digit:]]+\\.?[[:digit:]]*)?\\s*x\\s*)?\\s*(\\s*[\\+-]?([[:digit:]]+\\.?[[:digit:]]*)?\\s*x\\s*\\^\\s*[[:digit:]]+\\s*)*";
	compile_regex(&r, pattern);
    regmatch_t matches[1];
	if (regexec(&r, str, 1, matches, 0) == 0)
		return 1;
	return 0;
}

char* getSubstring(char* str, int i, int range){
	char* substring = calloc(1, sizeof(char));
	strncpy(substring, str+i, range);
	return substring;
}

struct polynomial* stringToPolynomial (char* name, char* str){
    struct polynomial* p = polynomial_new(name);
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

int isValidName(char* name){
	return 1;
}

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
		start = matches[1].rm_so;
		range = matches[1].rm_eo - start;
		char* name = getSubstring(str, start, range);
		while(1){
			if (!isPolynomial(polynomialString)){
				break;
			}
			if (!isValidName(name)){
				printf("illegal variable name\n");
				exitcode = -1;
				break;
			}
			struct polynomial* p = stringToPolynomial(name, polynomialString);
			if (p == NULL){
				printf("allocation error\n");
				exitcode = -1;
				break;
			}
			polynomialList_add(polynomials, p);
			polynomial_print(p);
			break;
		}
		regfree(&r);
		free(name);
		free(polynomialString);		
		exitcode = 0; /*compiled successfully*/
	}
	return exitcode;
}

int printPolynomial(char* name){
	struct polynomial* p = polynomialList_getByName(polynomials, name);
	if (p == NULL)
		return 1;
	polynomial_print(p);
	return 0;
}

int executeCommand(char* command){
	int error;
	command = strtok(command, "\n");
	
	error = definePolynomial(command);
	if (error != 1)
		return error;
	
	error = printPolynomial(command);
	if (error != 1)
		return error;
	
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
		if (strcmp(command, "quit\n") == 0){
			break;
		}	
		error = executeCommand(command);
		if (error == -1)	
			printf("error reading command\n");
		if (error == -2)
			printf("unknown command\n");
	}
	
	struct polynomial* p = (struct polynomial*)polynomials->array[0];
	polynomialList_free(polynomials);
	return 0;
}