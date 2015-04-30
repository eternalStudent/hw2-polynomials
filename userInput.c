#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <regex.h>
#include "arrayList.h"

#define MAX_ERROR_MSG 0x1000

//(\+|-)\s*?\d*\s*^x
//(\+|-)?\d*

//(-|\+)\s*\d*\s*x\s*^\^
//(-|\+)\d*

//(\+|-)\s*\d*\s*x\s*\^\s*\d+\s*
//(\+|-)\d*
//\d+



static int compile_regex (regex_t * r, const char * regex_text)
{
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


static regmatch_t* match_regex (regex_t * r, const char * to_match)
{
    /* "P" is a pointer into the string which points to the end of the
       previous match. */
    const char * p = to_match;
    /* "M" contains the matches found. */
	int n_matches = 10;
	regmatch_t* mPointer = (regmatch_t*)calloc(10, sizeof(regmatch_t));
	int i = 0;
    
	while (1) {
        int nomatch = regexec (r, p, n_matches, mPointer, 0);
        if (nomatch) {
            printf ("No more matches.\n");
            return nomatch;
        }
        while(1) {
			
            int start;
            int finish;
            if (m[i].rm_so == -1) {
                break;
            }
            start = m[i].rm_so + (p - to_match);
            finish = m[i].rm_eo + (p - to_match);
            if (i == 0) {
                printf ("$& is ");
            }
            else {
                printf ("$%d is ", i);
            }
            printf ("'%.*s' (bytes %d:%d)\n", (finish - start),
                    to_match + start, start, finish);
			i++;
			if (i>n_matches) {
				n_matches*=2;
				m = realloc(m, n_matches);			
			}
        }
        p += m[0].rm_eo;
    }
    return mPointer;
	}


regmatch_t* main(int argc, char ** argv)
{
    regex_t r;
    const char * regex_text;
    const char * find_text;
    regex_text = argv[1];
    find_text = argv[2];
    printf ("Trying to find '%s' in '%s'\n", regex_text, find_text);
    compile_regex(& r, regex_text);
    regmatch_t matchesArray = match_regex(& r, find_text);
    regfree (& r);
    return matchesArray;
}





//regex for legal variable name, remember to exclude legal commands
"[A-Za-z]+[A-Za-z0-9]*"


//^[A-Za-z]+[A-Za-z0-9]*$

// regex for polynomial input


//^(\+|-)\s*?\d*\s*^x   //
//^(\+|-)?\d*

//^(-|\+)\s*\d*\s*x\s*^\^
//^(-|\+)\d*

//^(\+|-)\s*\d*\s*x\s*\^\s*\d+\s*
//(\+|-)\d*
//\d+



