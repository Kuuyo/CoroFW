#include "vld.h"
#include "CFW.h"
#include <iostream>

CFW::Handle YieldTest()
{
	CFW_YieldNull();
}

CFW::Handle DoubleYieldTest()
{
	CFW_YieldNull();
	CFW_YieldNull();
}

int main()
{
	CFW::CoroFW* pCFW = new CFW::CoroFW();

	// 26906977
	for (size_t i = 0; i < 1000000; ++i)
	{
		CFW::Handle han = YieldTest();
		pCFW->AddCoroutine(han);

		// han = DoubleYieldTest();
		// pCFW->AddCoroutine(han);
	}

	while (pCFW->AreCoroutinesRunning())
	{
		pCFW->Update();
	}

	delete pCFW;
	return 0;
}