#include <stdio.h>
#include <stdlib.h>

#define CAT(x, y) x##_##y
#define TEMPLATE(x, y) CAT(x, y)

typedef int canary_t;

int POISON_int = 0xDEADBEEF;   //????
//double POISON_double = NAN;
canary_t POISON_can = 0xDEADBEEF;

void recalloc(struct TEMPLATE(Stack, TYPE)* stk, int elements, int size);
void DUMP(struct TEMPLATE(Stack, TYPE) stk);

struct TEMPLATE(Stack, TYPE)
{
	canary_t  left_canary;
	TYPE* data;
	int  size;
	int  capacity;
	canary_t  right_canary;
}; 

void OK(TEMPLATE(Stack, TYPE) stk)
{
	if (stk.size >= stk.capacity) printf("!! FULL !!\n");
	if (stk.left_canary != POISON_can) printf("!! Left dead !!\n");
	if (stk.right_canary != POISON_can) printf("!! Right dead !!\n");
}


struct TEMPLATE(Stack, TYPE) StackConstruct()
{
	struct TEMPLATE(Stack, TYPE) stk = {};

	int capacity = 1;

	canary_t* can_change = (canary_t*)calloc(1, capacity * sizeof(TYPE) + 2 * sizeof(canary_t));
	*can_change = POISON_can;
	
	stk.data = (TYPE*)(can_change + 1);
	//                                 ???
	can_change = (canary_t*)(stk.data + capacity);
	*can_change = POISON_can;
	
	stk.size = 0;
	stk.capacity = capacity;
	stk.left_canary = POISON_can;        
	stk.right_canary = POISON_can;

	DUMP(stk);

	return stk;
}

void Push(struct TEMPLATE(Stack, TYPE)* stk, int num)
{
	OK(*stk);

	if (stk->size + 1 >= stk->capacity)
	{
		stk->capacity = stk->capacity * 2;
		//stk->data = (int*)realloc(stk->data, stk->capacity * sizeof(int));
		recalloc(stk, stk->capacity, sizeof(TYPE));
	}
	*(stk->data + stk->size) = num;
	stk->size++;
	printf(">>>%d, <<<%d\n", stk->size, stk->capacity);
}

int Pop(struct TEMPLATE(Stack, TYPE)* stk)
{
	OK(*stk);

	stk->size--;
	int returned = *(stk->data + stk->size);
	*(stk->data + stk->size) = TEMPLATE(POISON, TYPE);

	return returned;
}

void StackDestruct(struct TEMPLATE(Stack, TYPE) stk)
{
	stk.data = stk.data - sizeof(int);

	printf("\nNOT FREE!\n\n");
	free(stk.data);           //?????????????????
	printf("FREE!\n\n");

	stk.size = POISON_int;
	stk.capacity = POISON_int;
	stk.left_canary = POISON_can;
	stk.right_canary = POISON_can;
}

void DUMP(struct TEMPLATE(Stack, TYPE) stk)
{
	OK(stk);

	printf("Stack ptr - <%p>\n", &stk);
	printf("Data  ptr - <%p>\n", stk.data);
	printf("Size - <%d>, Capacity - <%d>\n", stk.size, stk.capacity);
	printf("Left canary - <%d>, Right canary - <%d>\n", *(stk.data - sizeof(TYPE)), *(stk.data + stk.capacity * sizeof(TYPE)));
	printf("Left canary - <%d>, Right canary - <%d>\n", stk.left_canary, stk.right_canary);

	for (int i = 0; i < stk.capacity; i++)
	{
		printf("%d - [%d]\n", i, *(stk.data + i));
	}
	printf("\n");
}

void recalloc(struct TEMPLATE(Stack, TYPE) * stk, int elements, int size) // ???????????????????
{
	printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!, %d, %d, %d\n\n", stk->capacity, sizeof(TYPE), sizeof(int));
	stk->data = stk->data - sizeof(int);
	stk->data = (TYPE*)realloc(stk->data, stk->capacity * sizeof(TYPE) + 2 * sizeof(int));
	printf("A\n");

	stk->left_canary = POISON_can;         // ????
	stk->right_canary = POISON_can;

	stk->data = stk->data + sizeof(int);
	/*
	stk->data[0] = stk->left_canary; //      ?????
	stk->data[sizeof(TYPE) * (1 + stk->capacity)] = stk->right_canary;
	stk->data = stk->data + sizeof(int);
	*/
	printf("B\n");

	for (int i = stk->size; i < stk->capacity; i++)
	{
		stk->data[i] = 0;
		printf("C - %d\n", i);
	}

	DUMP(*stk);
}