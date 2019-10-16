#include "CFW.h"
#include <iostream>
#include <experimental/coroutine>

using namespace CFW;

Result PrintHelloCoro();

int main()
{
	CoroFW* pCFW = new CoroFW();

	auto res = PrintHelloCoro();
	pCFW->AddCoroutine(res);

	std::cout << "Starting loop!\n";

	while (true)
	{
		std::cout << ".";
		pCFW->Update();
	}

	delete pCFW;
	return 0;
}

Result PrintHelloCoro()
{
	std::cout << "PrintHelloStarted";
	for (size_t i = 0; i < 3; i++)
	{
		std::cout << "Hello!\n";
		CFW_YieldNull();
	}
	std::cout << "PrintHelloEnded";
}
