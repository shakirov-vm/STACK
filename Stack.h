#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define CAT(x, y) x##_##y
#define TEMPLATE(x, y) CAT(x, y)

typedef int canary_t;

int POISON_int = 0xDEADBEEF;   //????
//double POISON_double = NAN;
//double POISON_float = 10.877765;
canary_t POISON_can = 0xDEADBEEF;
//canary_t POISON_can = 115115;

void Print_int(int num)
{
	printf("%d", num);
}

void Print_float(float num)
{
	printf("%f", num);
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
	int Pop();
	void OK();
	void DUMP();
	void recalloc(int elements, int Size);
	~TEMPLATE(Stack, TYPE)();
}; 

void TEMPLATE(Stack, TYPE)::OK()
{
	if (size > capacity) printf("!! FULL !!\n");
	if (left_canary != POISON_can) printf("STRUCT: !! Left dead !!\n");
	if (right_canary != POISON_can) printf("STRUCT: !! Right dead !!\n");
	if (*(data - 1) != POISON_can) printf("DATA: !! left dead !!\n");
	if (*(data + capacity) != POISON_can) printf("DATA: !! right dead !!\n");
	//assert(*(data + capacity) == POISON_can);
}


TEMPLATE(Stack, TYPE)::TEMPLATE(Stack, TYPE)()
{
	int _capacity = 1;

	canary_t* can_change = (canary_t*)calloc(1, _capacity * sizeof(TYPE) + 2 * sizeof(canary_t));
	*can_change = POISON_can;
	
	data = (TYPE*)(can_change + 1);

	can_change = (canary_t*)(data + _capacity);
	*can_change = POISON_can;
	
	size = 0;
	capacity = _capacity;
	left_canary = POISON_can;        
	right_canary = POISON_can;

	DUMP();
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
	printf(">>>%d, <<<%d\n", size, capacity);
}

int TEMPLATE(Stack, TYPE)::Pop()
{
	OK();

	size--;

	int returned = *(data + size);
	*(data + size) = TEMPLATE(POISON, TYPE);

	return returned;
}

TEMPLATE(Stack, TYPE)::~TEMPLATE(Stack, TYPE)()
{
	data = data - 1;

	printf("\nNOT FREE!\n\n");
	free(data);          
	printf("FREE!\n\n");

	size = POISON_int;
	capacity = POISON_int;
	left_canary = POISON_can;
	right_canary = POISON_can;
}

void TEMPLATE(Stack, TYPE)::DUMP()
{
	OK();

	printf("Stack ptr - <%p>\n", this);
	printf("Data  ptr - <%p>\n", data);
	printf("Size - <%d>, Capacity - <%d>\n", size, capacity);
	printf("DATA:   Left canary - <%d>, Right canary - <%d>\n", *(data - 1), *(data + capacity));
	printf("STRUCT: Left canary - <%d>, Right canary - <%d>\n", left_canary, right_canary);

	for (int i = 0; i < capacity; i++)
	{
		printf("%d - [", i);
		TEMPLATE(Print, TYPE) (*(data + i));
		printf("]\n");
	}
	printf("\n");
}

void TEMPLATE(Stack, TYPE)::recalloc(int elements, int Size)
{
	data = data - 1;
	//                                                         Size
	canary_t* can_change = (canary_t*)realloc(data, capacity * sizeof(TYPE) + 2 * sizeof(canary_t));
	*can_change = POISON_can;

	data = (TYPE*)(can_change + 1);

	can_change = (canary_t*)(data + capacity);
	*can_change = POISON_can;

	//stk->left_canary = POISON_can;
	//stk->right_canary = POISON_can;

	printf("!!! SIZE IS %d\n\n\n", size);

	for (int i = size; i < capacity; i++)
	{
		data[i] = 0;
		printf("C - %d\n", i);
	}

	DUMP();
}