
#define TYPE int
#include "Stack.h"
#undef TYPE
/*
#define TYPE float
#include "Stack.h"
#undef TYPE
*/
int main()
{
	Stack_int stk;

	for (int i = 5; i < 25; i++)
	{
		stk.DUMP();
		printf("\n");
		stk.Push(i * i);
	}

	stk.DUMP();

	int a = stk.Pop();
	printf("\n\n%d\n\n", a);
	
	stk.DUMP();

	//StackDestruct(stk);
	/*
	Stack_float stek;
	printf("A\n");
	stek.Push(18);
	printf("B\n");
	stek.Push(1818);
	stek.Push(181818);

	stek.DUMP();

	int a = stek.Pop();
	int b = stek.Pop();

	printf("a = %d, b = %d\n\n", a, b);

	stek.DUMP();

	stek.Push(137);
	stek.Push(2.71826);
	stek.Push(3.1416);
	stek.Push(123456);

	stek.DUMP();
	*/

	return 0;
}
