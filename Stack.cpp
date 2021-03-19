
#ifndef STK_CNST_H    
#define STK_CNST_H  
#include "Stk_cnst.h"
#endif 

#if 1
#define TYPE int
#include "Stack.h"
#undef TYPE
#endif                                                        

#if 1
#define TYPE float
#include "Stack.h"
#undef TYPE
#endif

#if 1
#define TYPE double
#include "Stack.h"
#undef TYPE
#endif

void ErroRio(Stack_double stk)
{
	printf("You there\n");
}


int main()
{
#if 1
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
	
#if 1
	Stack_double stek;

	stek.Push(18);

	stek.Push(1818);
	stek.Push(181818);

	stek.DUMP();

	double c = stek.Pop();
	double b = stek.Pop();

	printf("c = %lf, b = %lf\n\n", c, b);

	stek.DUMP();

	stek.Push(137);
	stek.Push(2.71826);
	stek.Push(3.1416);
	stek.Push(123456);
	stek.Push(7.218);
	stek.Push(55);
	stek.Push(37);
	stek.Push(985);
	stek.Push(12.485);
	stek.Push(2487);
	stek.Push(3528);
	stek.Push(0.21651);
	stek.Push(0);


	stek.DUMP();

	Stack_double CKATS;

	CKATS = stek;

	CKATS.Push(7);

	ErroRio(stek);

#endif

	return 0;
}

