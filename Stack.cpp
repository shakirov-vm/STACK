#if 0
#define TYPE int
#include "Stack.h"
#undef TYPE
#endif//                                                         @@@@@@

#if 0
#define TYPE float
#include "Stack.h"
#undef TYPE
#endif

#if 1
#define TYPE double
#include "Stack.h"
#undef TYPE
#endif

int main()
{
	#if 0
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
	#endif
	      ///                                                      @@@@@@@@@

	#if 1
	Stack_double stek;

	stek.Push(18);

	stek.Push(1818);
	stek.Push(181818);

	stek.DUMP();

	double a = stek.Pop();
	double b = stek.Pop();

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
