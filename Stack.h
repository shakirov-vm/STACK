#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <errno.h>

#define CAT(x, y) x##_##y
#define TEMPLATE(x, y) CAT(x, y)

#ifndef STK_CNST_H    
#define STK_CNST_H   
#include "Stk_cnst.h"
#endif 

typedef int canary_t;

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
	TEMPLATE(Stack, TYPE)(const TEMPLATE(Stack, TYPE) &stk);
	TEMPLATE(Stack, TYPE)& operator= (const TEMPLATE(Stack, TYPE) & stk);
	void Push(TYPE num);
	TYPE Pop();
	void OK();
	void DUMP();
	void recalloc(int elements, int Size);
	unsigned long long CountHash(char* str);
	~TEMPLATE(Stack, TYPE)();
};
                                                                 // Конструктор по умолчанию
TEMPLATE(Stack, TYPE)::TEMPLATE(Stack, TYPE)()
{
	capacity = 1;

	data = (TYPE*)calloc(1, capacity * sizeof(TYPE) + 2 * sizeof(canary_t));

	if (data == nullptr) OK();

	canary_t* first_canary = (canary_t*)(data);
	*(first_canary) = POISON_can;

	data = (TYPE*)((canary_t*)data + 1);

	canary_t* second_canary = (canary_t*)(data + capacity);
	*(second_canary) = POISON_can;

	hash = CountHash((char*)data);
	size = 0;
	left_canary = POISON_can;
	right_canary = POISON_can;

	OK();
}
																			  // Конструктор копирования
TEMPLATE(Stack, TYPE)::TEMPLATE(Stack, TYPE)(const TEMPLATE(Stack, TYPE)& stk)
{
	data = (TYPE*)calloc(1, stk.capacity * sizeof(TYPE) + 2 * sizeof(canary_t));

	if (data == nullptr) OK();

	size = stk.size;
	capacity = stk.capacity;
	left_canary = POISON_can;
	right_canary = POISON_can;

	canary_t* first_canary = (canary_t*)(data);
	*(first_canary) = POISON_can;

	data = (TYPE*)((canary_t*)data + 1);
													
	data = (TYPE*) memcpy(data, stk.data, stk.capacity * sizeof(TYPE)); 

	canary_t* second_canary = (canary_t*)(data + capacity);
	*(second_canary) = POISON_can;

	hash = CountHash((char*)data);

	OK();

	// ?
}
																							// Оператор присваивания
TEMPLATE(Stack, TYPE)& TEMPLATE(Stack, TYPE)::operator= (const TEMPLATE(Stack, TYPE)& stk)
{
	data = (TYPE*)calloc(1, stk.capacity * sizeof(TYPE) + 2 * sizeof(canary_t));

	if (data == nullptr)
	{
		FILE* potok = fopen("log.txt", "a");

		if (errno)
		{
			char answer[100];

			sprintf(answer, "Problem file: %s\n", "log.txt");

			perror(answer);
			exit(STACK_UNOP);
		}

		fclose(potok);
	}

	size = stk.size;
	capacity = stk.capacity;
	left_canary = POISON_can;
	right_canary = POISON_can;

	canary_t* first_canary = (canary_t*)(data);
	*(first_canary) = POISON_can;

	data = (TYPE*)((canary_t*)data + 1);

	data = (TYPE*)memcpy(data, stk.data, stk.capacity * sizeof(TYPE));

	canary_t* second_canary = (canary_t*)(data + capacity);
	*(second_canary) = POISON_can;

	hash = CountHash((char*)data);

	OK();

	//?
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
	hash = CountHash((char*)data);

	DUMP();

	OK();
}

TYPE TEMPLATE(Stack, TYPE)::Pop()
{
	if (size >= 0) size--;
	else OK();

	TYPE returned = *(data + size);
	*(data + size) = TEMPLATE(POISON, TYPE);

	hash = CountHash((char*)data);

	OK();

	return returned;
}

void TEMPLATE(Stack, TYPE)::DUMP()
{
	FILE* potok = fopen("log.txt", "a");

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

	if (data == nullptr) OK();

	canary_t* first_canary = (canary_t*)(data);
	*(first_canary) = POISON_can;  // = first_canary

	data = (TYPE*)((canary_t*)data + 1);

	canary_t* second_canary = (canary_t*)(data + capacity);
	*(second_canary) = POISON_can;

	for (int i = size; i < capacity; i++)
	{
		data[i] = 0;
	}

	hash = CountHash((char*)data);

	OK();
}

void TEMPLATE(Stack, TYPE)::OK()
{
	FILE* potok = fopen("log.txt", "a");

	if (errno)
	{
		char answer[100];

		sprintf(answer, "Problem file: %s\n", "log.txt");

		perror(answer);
		exit(STACK_UNOP);
	}

	int error = 0;

	if (data == nullptr)
	{
		error = STACK_NPTR;
		printf("!! data is nullptr !!\n");
		fprintf(potok, "ERRORS Code - <%d>", STACK_NPTR);
	}
	if ((size < 0) || (capacity < 0))
	{
		error = STACK_FEWS;
		printf("!! INDEX OUT OF RANGE !!\n");
		fprintf(potok, "ERRORS Code - <%d>", STACK_FEWS);
	}
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

	assert(data);

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
	if (hash != CountHash((char*)data))
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
// Добавить инфу о самом стэке
unsigned long long TEMPLATE(Stack, TYPE)::CountHash(char* string)
{
	int hash_sum = 0;

	for (int i = 0; i < size * sizeof(TYPE); i++) 
	{
		hash_sum = hash_sum + (unsigned long long) * (string + i);
		hash_sum = (hash_sum << 1) | (hash_sum >> 31);
	}

	hash_sum = hash_sum + capacity;
	hash_sum = hash_sum * size;

	return hash_sum;
}
