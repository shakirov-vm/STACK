#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <errno.h>

typedef int canary_t;

const int POISON_int = 0xDEADBEEF;
const double POISON_double = 0xDEADBEEF;  //-666.666;
const float POISON_float = 0xDEADBEEF;    //666.666;
const canary_t POISON_can = 0xDEADBEEF;

void Fprint_int(FILE* potok, int num)
{
	fprintf(potok, "%d", num);
}

void Fprint_float(FILE* potok, float num)
{
	fprintf(potok, "%f", num);
}

void Fprint_double(FILE* potok, double num)
{
	fprintf(potok, "%lf", num);
}


enum STACK_ERROR
{
	STACK_LCD = 1,
	STACK_RCD = 2,
	STACK_LCS = 3,
	STACK_RCS = 4,
	STACK_NPTR = 5,
	STACK_OVER = 6,
	STACK_HASH = 7,
	STACK_UNOP = 8,
	STACK_FEWS = 9
};
