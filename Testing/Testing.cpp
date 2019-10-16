#include "CFW.h"
#include <iostream>
#include <experimental/coroutine>

using namespace CFW;

Result PrintHelloCoroAndCount(int &c);

int main()
{
	int count{ 0 };

	CoroFW* pCFW = new CoroFW();

	Result res = PrintHelloCoroAndCount(count);
	pCFW->AddCoroutine(res);

	std::cout << "Starting loop!\n";

	while (true)
	{
		std::cout << ".";
		++count;
		std::cout << "Before: " << count << std::endl;
		pCFW->Update();
		std::cout << "After: " << count << std::endl;
	}

	delete pCFW;
	return 0;
}

Result PrintHelloCoroAndCount(int &c)
{
	std::cout << "PrintHelloStarted\n";
	for (size_t i = 0; i < 3; i++)
	{
		++c;
		std::cout << "Hello!\n";
		CFW_YieldNull();
	}
	std::cout << "PrintHelloEnded\n";
}
