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
		regfree(&r);
		return 1;
	regfree(&r);
	return 0;
}

char* getSubstring(char* str, int i, int range){
	char* substring = calloc(1, sizeof(char*));
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
	if (isCommandDer==0 | isCommandEval==0 | isCommandQuit==0 | isX==0){
		exitCodeValid = 0;
	}
	regfree(&r);
	return exitCodeValid;
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
			int i;
			if (polynomialList_getByName(polynomials, name, &i) != NULL){
				polynomialList_update(polynomials, p, i);
				printf("updated %s\n", name);
			}
			else if (polynomialList_add(polynomials, p)){
				printf("allocation error\n");
				exitcode = -1;
				break;
			}
			else
				printf("created %s\n", name);
			break;
		}
		regfree(&r);
		free(name);
		free(polynomialString);		
		exitcode = 0; /*compiled successfully*/
	}
	
	else {
		regfree(&r);
	}
	return exitcode;
}

int printPolynomial(char* name){
	if (!isValidName(name))
		return 1;
	int i;
	struct polynomial* p = polynomialList_getByName(polynomials, name, &i);
	if (p == NULL){
		printf("unknown polynomial %s\n", name);
		return 1;
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
		int i;
		while (1){
			int start = matches[1].rm_so;
			int range = matches[1].rm_eo-start;
			name1 = getSubstring(str, start, range);
			struct polynomial* p1 = polynomialList_getByName(polynomials, name1, &i);
			if (p1 == NULL){
				printf("unknown polynomial %s\n", name1);
				exitcode = -1;
				break;
			}
			start = matches[2].rm_so;
			range = matches[2].rm_eo;
			name2 = getSubstring(str, start, range);
			struct polynomial* p2 = polynomialList_getByName(polynomials, name2, &i);
			if (p2 == NULL){
				printf("unknown polynomial %s\n", name2);
				exitcode = -1;
				break;
			}
			struct polynomial* sum = polynomial_sum(p1, p2);
			polynomial_print(sum);
			polynomial_free(sum);
			exitcode = 0;
			break;
		}	
		regfree(&r);
		free(name1);
		free(name2);
		exitcode = 0; /*compiled successfully*/
	}
	else {
		regfree(&r);
	}
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
		int i;
		while (1){
			int start = matches[1].rm_so;
			int range = matches[1].rm_eo-start;
			name1 = getSubstring(str, start, range);
			struct polynomial* p1 = polynomialList_getByName(polynomials, name1, &i);
			if (p1 == NULL){
				printf("unknown polynomial %s\n", name1);
				exitcode = -1;
				break;
			}
			start = matches[2].rm_so;
			range = matches[2].rm_eo;
			name2 = getSubstring(str, start, range);
			struct polynomial* p2 = polynomialList_getByName(polynomials, name2, &i);
			if (p2 == NULL){
				printf("unknown polynomial %s\n", name2);
				exitcode = -1;
				break;
			}
			struct polynomial* difference = polynomial_subtract(p1, p2);
			polynomial_print(difference);
			polynomial_free(difference);
			exitcode = 0;
			break;
		}	
		
		regfree(&r);
		free(name1);
		free(name2);
		exitcode = 0; /*compiled successfully*/
	}
	
	else {
		regfree(&r);
	}
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
		int i;
		while (1){
			int start = matches[1].rm_so;
			int range = matches[1].rm_eo-start;
			name1 = getSubstring(str, start, range);
			struct polynomial* p1 = polynomialList_getByName(polynomials, name1, &i);
			if (p1 == NULL){
				printf("unknown polynomial %s\n", name1);
				exitcode = -1;
				break;
			}
			start = matches[2].rm_so;
			range = matches[2].rm_eo;
			name2 = getSubstring(str, start, range);
			struct polynomial* p2 = polynomialList_getByName(polynomials, name2, &i);
			if (p2 == NULL){
				printf("unknown polynomial %s\n", name2);
				exitcode = -1;
				break;
			}
			struct polynomial* product = polynomial_multiply(p1, p2);
			polynomial_print(product);
			polynomial_free(product);
			exitcode = 0;
			break;
		}	
		
		regfree(&r);
		free(name1);
		free(name2);
		exitcode = 0; /*compiled successfully*/
	}
	
	else {
		regfree(&r);
	}
	return exitcode;
}

int derivation(char* str) {
	regex_t r; 	
	regmatch_t matches[2];
	int exitcode = 1; /*did not match*/
	char* pattern = "^der[[:space:]]*([A-Za-z][A-Za-z0-9]*)$";
	compile_regex(&r, pattern);
	if (regexec(&r, str, 2, matches, 0) == 0){
		int i;
		char* name;
		while(1){	
			int start = matches[1].rm_so;
			int range = matches[1].rm_eo-start;
			char* name = getSubstring(str, start, range);
			struct polynomial* polyToDerive = polynomialList_getByName(polynomials, name, &i);
			if (polyToDerive == NULL){
				printf ("unknown polynomial %s\n",name);
				exitcode = -1;
				break;
			}
			
			struct polynomial* derivative = polynomial_derive(polyToDerive);	
			polynomial_print(derivative);
			polynomial_free(derivative);
			regfree(&r);
			free(name);
			exitcode = 0;
			break;
			}				
		}
	else {
		regfree(&r);
	}
	return exitcode;
}


int evaluation(char* str) {
	regex_t r; 	
	regmatch_t matches[3];
	int exitcode = 1; /*did not match*/
	char* pattern = "^eval[[:space:]]+(\\w*)[[:space:]]+([-]?[0-9]+\\.?[0-9]*)$";
	compile_regex(&r, pattern);
	if (regexec(&r, str, 3, matches, 0) == 0){
		int i;
		char* name;
		float number;
		while(1){	
			int start = matches[1].rm_so;
			int range = matches[1].rm_eo-start;
			char* name = getSubstring(str, start, range);
			
			start = matches[2].rm_so;
			range = matches[2].rm_eo-start;
			
			char* numberAsString = getSubstring(str, start, range);
			number = atof(numberAsString);
			
			struct polynomial* polyToEvaluate = polynomialList_getByName(polynomials, name, &i);
			if (polyToEvaluate == NULL){
				printf ("unknown polynomial %s\n",name);
				exitcode = -1;
				break;
			}
			
			float evaluation = polynomial_evaluate(polyToEvaluate, number);	
			printf("%.2f\n", evaluation);
			regfree(&r);
			free (numberAsString);
			free(name);
			exitcode = 0;
			break;
			}				
		}
		
	else {
		regfree(&r);
	}
	return exitcode;
}



int executeCommand(char* command){
	int error;
	command = strtok(command, "\n");
	
	error = definePolynomial(command);
	if (error != 1)
		return error;
	
	error = derivation(command);
	if (error != 1)
		return error;
	
	error = evaluation(command);
	if (error != 1)
		return error;
	
	error = summation(command);
	if (error != 1)
		return error;
	
	error = subtraction(command);
	if (error != 1)
		return error;
	
	error = multiplication(command);
	if (error != 1)
		return error;
	
	error = printPolynomial(command);
	if (error != 1)
		return error;
	
	printf("unknown command\n");
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
	}
	polynomialList_free(polynomials);
	return 0;
}