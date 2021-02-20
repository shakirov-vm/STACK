#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <errno.h>

#define CAT(x, y) x##_##y
#define TEMPLATE(x, y) CAT(x, y)

enum STACK_ERROR
{
	STACK_LCD  = 1,
	STACK_RCD  = 2,
	STACK_LCS  = 3,
	STACK_RCS  = 4,
	STACK_NPTR = 5,
	STACK_OVER = 6,
	STACK_HASH = 7,
	STACK_UNOP = 8
};

typedef int canary_t;

int POISON_int = 0xDEADBEEF;  
double POISON_double = 100500;
float POISON_float = 10.87;
canary_t POISON_can = 0xDEADBEEF;

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

class TEMPLATE(Stack, TYPE)
{
private:

	canary_t  left_canary;
	TYPE* data;
	int  size;
	int  capacity;
	unsigned long long hash;
	canary_t  right_canary;

public:

	TEMPLATE(Stack, TYPE)();
	void Push(TYPE num);
	TYPE Pop();
	void OK();
	void DUMP();
	void recalloc(int elements, int Size);
	unsigned long long Hash_Compute();
	~TEMPLATE(Stack, TYPE)();
}; 

void TEMPLATE(Stack, TYPE)::OK()
{
	FILE* potok;
	potok = fopen("log.txt", "a");

	if (errno)
	{
		char answer[100];

		sprintf(answer, "Problem file: %s\n", "log.txt");

		perror(answer);
		exit(STACK_UNOP);
	}

	int error = 0;

	if (size > capacity)
	{
		error = STACK_OVER;
		printf("!! FULL !!\n");
		fprintf(potok, "ERRORS Code - <%d>", STACK_OVER);
	}
	if (left_canary != POISON_can)
	{
		error = STACK_LCS;
		printf("STRUCT: !! Left dead !!\n");
		fprintf(potok, "ERRORS Code - <%d>", STACK_LCS);
	}
	if (right_canary != POISON_can)
	{
		error = STACK_RCS;
		printf("STRUCT: !! Right dead !!\n");
		fprintf(potok, "ERRORS Code - <%d>", STACK_RCS);
	}
	if (*((canary_t*)data - 1) != POISON_can)
	{
		error = STACK_LCD;
		printf("DATA: !! left dead !!\n");
		fprintf(potok, "ERRORS Code - <%d>", STACK_LCD);
	}
	if (*(canary_t*)(data + capacity) != POISON_can)
	{
		error = STACK_RCD;
		printf("DATA: !! right dead !!\n");
		fprintf(potok, "ERRORS Code - <%d>", STACK_RCD);
	}
	if (hash != Hash_Compute())
	{
		error = STACK_HASH;
		printf("Hash isn't true");
		fprintf(potok, "ERRORS Code - <%d>", STACK_HASH);
	}
	if (error != 0)
	{
		DUMP();
	}

	fclose(potok);
}


TEMPLATE (Stack, TYPE)::TEMPLATE(Stack, TYPE)()
{	
	capacity = 1;

	data = (TYPE*)calloc(1, capacity * sizeof(TYPE) + 2 * sizeof(canary_t));

	assert(data);

	canary_t* first_canary = (canary_t*)(data);
	*(first_canary) = POISON_can;

	data = (TYPE*)((canary_t*)data + 1);

	canary_t* second_canary = (canary_t*)(data + capacity);
	*(second_canary) = POISON_can;

	hash = Hash_Compute();
	size = 0;
	left_canary = POISON_can;        
	right_canary = POISON_can;
}

void TEMPLATE(Stack, TYPE)::Push(TYPE num)
{
	OK();

	if (size >= capacity)
	{
		capacity = capacity * 2;
		recalloc(capacity, sizeof(TYPE));
	}
	*(data + size) = num;
	size++;
	hash = Hash_Compute();
}

TYPE TEMPLATE(Stack, TYPE)::Pop()
{
	OK();

	size--;

	TYPE returned = *(data + size);
	*(data + size) = TEMPLATE(POISON, TYPE);

	hash = Hash_Compute();

	return returned;
}

TEMPLATE(Stack, TYPE)::~TEMPLATE(Stack, TYPE)()
{
	data = (TYPE*)((canary_t*)data - 1);

	free(data);          

	size = POISON_int;
	capacity = POISON_int;
	left_canary = POISON_can;
	right_canary = POISON_can;
}

void TEMPLATE(Stack, TYPE)::DUMP()
{
	FILE* potok;
	potok = fopen("log.txt", "a");

	if (errno)
	{
		char answer[100];

		sprintf(answer, "Problem file: %s\n", "log.txt");

		perror(answer);
		exit(STACK_UNOP);
	}

	fprintf(potok, "Stack ptr - <%p>\n", this);
	fprintf(potok, "Data  ptr - <%p>\n", data);
	fprintf(potok, "Size - <%d>, Capacity - <%d>\n", size, capacity);
	fprintf(potok, "DATA:   Left canary - <%d>, Right canary - <%d>\n", *((canary_t*)data - 1), *(canary_t*)(data + capacity));
	fprintf(potok, "STRUCT: Left canary - <%d>, Right canary - <%d>\n", left_canary, right_canary);
	fprintf(potok, "Hash - <%llu>\n", hash);

	for (int i = 0; i < capacity; i++)
	{
		fprintf(potok, "%d - [", i);
		TEMPLATE(Fprint, TYPE) (potok, *(data + i));
		fprintf(potok, "]\n");
	}
	fprintf(potok, "\n");

	fclose(potok);
}

void TEMPLATE(Stack, TYPE)::recalloc(int elements, int Size)
{

	data = (TYPE*)((canary_t*)data - 1); 

	data = (TYPE*)realloc(data, capacity * sizeof(TYPE) + 2 * sizeof(canary_t));

	assert(data);

	canary_t* first_canary = (canary_t*)(data);
	*(first_canary) = POISON_can;

	data = (TYPE*)((canary_t*)data + 1);

	canary_t* second_canary = (canary_t*)(data + capacity);
	*(second_canary) = POISON_can;

	for (int i = size; i < capacity; i++)
	{
		data[i] = 0;
	}

	hash = Hash_Compute();

	OK();
}


unsigned long long TEMPLATE(Stack, TYPE)::Hash_Compute()  //              ??????
{
	hash = size ^ capacity;
	unsigned long long hash_dop = 0;

	for (int i = 1; i < capacity - 1; i++)
	{
		hash_dop = (int)(1000 * data[i]) & (int)(1000 * data[i - 1]) | (int)(1000 * data[i + 1]);
		hash = hash ^ hash_dop;
	}
	return hash;
}

/*
	printf(">>> %p >>> %p >>> %p\n", (canary_t*)data - 1, data, data + capacity);


	data = (TYPE*)calloc(1, capacity * sizeof(TYPE) + 2 * sizeof(canary_t));

	assert(data);

	printf("left canary - %p\n", data);

	canary_t* first_canary = (canary_t*)(data);
	*(first_canary) = POISON_can;

	data = (TYPE*)((canary_t*)data + 1);
	printf("data - %p\n", data);

	canary_t* second_canary = (canary_t*)(data + capacity);
	*(second_canary) = POISON_can;

	printf("right canary - %p\n", second_canary);
*/