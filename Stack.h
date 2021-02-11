#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define CAT(x, y) x##_##y
#define TEMPLATE(x, y) CAT(x, y)

enum STACK_ERROR
{
	STACK_LCD = 1,
	STACL_RCD = 2,
	STACK_LCS = 3,
	STACK_RCS = 4,
	STACK_NPTR = 5,
	STACK_OVER = 6,
	STACK_HASH = 7
};

typedef int canary_t;

int POISON_int = 0xDEADBEEF;  
double POISON_double = 10005000;
float POISON_float = 10.87;
canary_t POISON_can = 0xDEADBEEF;

void Print_int(int num)
{
	printf("%d", num);
}

void Print_float(float num)
{
	printf("%f", num);
}

void Print_double(double num)
{
	printf("%lf", num);
}

class TEMPLATE(Stack, TYPE)
{
private:

	canary_t  left_canary;
	TYPE* data;
	int  size;
	int  capacity;
	canary_t  right_canary;

public:

	TEMPLATE(Stack, TYPE)();
	void Push(TYPE num);
	TYPE Pop();
	void OK();
	void DUMP();
	void recalloc(int elements, int Size);
	~TEMPLATE(Stack, TYPE)();
}; 

void TEMPLATE(Stack, TYPE)::OK()
{
	if (size > capacity)
	{
		printf("!! FULL !!\n");
	}
	if (left_canary != POISON_can) printf("STRUCT: !! Left dead !!\n");
	if (right_canary != POISON_can) printf("STRUCT: !! Right dead !!\n");
	if (*((canary_t*)data - 1) != POISON_can) printf("DATA: !! left dead !!\n");
	if (*(canary_t*)(data + capacity) != POISON_can) printf("DATA: !! right dead !!\n");

	//assert(*(data + capacity) == POISON_can);
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

	size = 0;
	left_canary = POISON_can;        
	right_canary = POISON_can;

	DUMP();
}

void TEMPLATE(Stack, TYPE)::Push(TYPE num)
{
	OK();
	DUMP();

	if (size >= capacity)
	{
		capacity = capacity * 2;
		recalloc(capacity, sizeof(TYPE));
	}
	*(data + size) = num;
	size++;
}

TYPE TEMPLATE(Stack, TYPE)::Pop()
{
	OK();

	size--;

	TYPE returned = *(data + size);
	*(data + size) = TEMPLATE(POISON, TYPE);

	DUMP();

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
	printf("Stack ptr - <%p>\n", this);
	printf("Data  ptr - <%p>\n", data);
	printf("Size - <%d>, Capacity - <%d>\n", size, capacity);
	printf("DATA:   Left canary - <%d>, Right canary - <%d>\n", *((canary_t*)data - 1), *(canary_t*)(data + capacity));
	printf("STRUCT: Left canary - <%d>, Right canary - <%d>\n", left_canary, right_canary);

	for (int i = 0; i < capacity; i++)
	{
		printf("%d - [", i);
		TEMPLATE(Print, TYPE) (*(data + i));
		printf("]\n");
	}
	printf("\n");

	OK();
}

void TEMPLATE(Stack, TYPE)::recalloc(int elements, int Size)
{

	data = (TYPE*)((canary_t*)data - 1); 

	data = (TYPE*)realloc(data, capacity * sizeof(TYPE) + 2 * sizeof(canary_t));

	assert(data);

	printf("left canary - %p\n", data);

	canary_t* first_canary = (canary_t*)(data);
	*(first_canary) = POISON_can;

	data = (TYPE*)((canary_t*)data + 1);
	printf("data - %p\n", data);

	canary_t* second_canary = (canary_t*)(data + capacity);
	*(second_canary) = POISON_can;

	for (int i = size; i < capacity; i++)
	{
		data[i] = 0;
	}

	DUMP();
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