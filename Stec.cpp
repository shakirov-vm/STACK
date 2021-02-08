/*
#define TYPE int
#include "Stec.h"
#undef TYPE
*/
#define TYPE double
#include "Stec.h"
#undef TYPE

#define TYPE double

int main()
{
#if 1
	Stack_double stk;

	for (int i = 5; i < 25; i++)
	{
		stk.DUMP();
		printf("\n");
		stk.Push(i * i);
	}

	stk.DUMP();

	TYPE c = stk.Pop();
//	printf("\n\n%d\n\n", c);
	printf("\n\n%lf\n\n", c);

	stk.DUMP();
#endif
	
#if 1
	Stack_double stek;

	stek.Push(18);
	stek.Push(1818);
	stek.Push(181818);
	stek.DUMP();

	TYPE a = stek.Pop();
	TYPE b = stek.Pop();

	//printf("a = %d, b = %d\n\n", a, b);
	printf("a = %lf, b = %lf\n\n", a, b);

	stek.DUMP();
	stek.Push(137);
	stek.Push(2.71826);
	stek.Push(3.1416);
	stek.Push(123456);
	stek.DUMP();
#endif

	return 0;
}