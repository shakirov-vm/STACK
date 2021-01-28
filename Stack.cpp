#define TYPE int
#include "Stack.h"
#undef TYPE

int main()
{
	struct Stack_int stk;
	stk = StackConstruct();

	for (int i = 5; i < 13; i++)
	{
		DUMP(stk);
		printf("\n");
		Push(&stk, i * i);
	}

	DUMP(stk);

	int a = Pop(&stk);
	printf("\n\n%d\n\n", a);
	
	DUMP(stk);

	StackDestruct(stk);

	return 0;
}
