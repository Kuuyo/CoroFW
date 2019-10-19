#include "vld.h"
#include "CFW.h"
#include <future>

#pragma warning( push,0 )
#pragma warning( disable : 26495 )
#pragma warning( disable : 26812 )
#pragma warning( disable : 26439 )
#pragma warning( disable : 26451 )
#pragma warning( disable : 26444 )
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#pragma warning( pop )


TEST_CASE("Are Coroutines Running Check")
{
	int loopCount{ 0 };
	CFW::CoroFW* pFW = new CFW::CoroFW();

	while (pFW->AreCoroutinesRunning())
	{
		++loopCount;
	}

	delete pFW;

	REQUIRE(loopCount == 0);
}

CFW::Handle YieldTest()
{
	CFW_YieldNull();
}

TEST_CASE("YieldNull Check")
{
	int loopCount{ 0 };
	CFW::CoroFW* pFW = new CFW::CoroFW();

	CFW::Handle res = YieldTest();
	pFW->AddCoroutine(res);

	while (pFW->AreCoroutinesRunning())
	{
		pFW->Update();
		++loopCount;	
	}

	delete pFW;

	REQUIRE(loopCount == 1);
}

CFW::Handle Wait(int time)
{
	int timer{ 0 };

	while (time > timer)
	{
		++timer;
		CFW_YieldNull();
	}
}

CFW::Handle YieldWaitTest()
{
	co_await Wait(3);
}

TEST_CASE("YieldWaitTest Check")
{
	int loopCount{ 0 };
	CFW::CoroFW* pFW = new CFW::CoroFW();

	CFW::Handle res = YieldWaitTest();
	pFW->AddCoroutine(res);

	while (pFW->AreCoroutinesRunning())
	{
		pFW->Update();
		++loopCount;
	}

	delete pFW;

	REQUIRE(loopCount == 4);
}

CFW::Handle MultiYieldTest()
{
	int count{ 0 };
	
	for (size_t i = 0; i < 3; ++i)
	{
		++count;
		CFW_YieldNull();
	}

	REQUIRE(count == 3);
}

TEST_CASE("Multiple Yield Test")
{
	int loopCount{ 0 };
	CFW::CoroFW* pFW = new CFW::CoroFW();

	CFW::Handle res = MultiYieldTest();
	pFW->AddCoroutine(res);

	while (pFW->AreCoroutinesRunning())
	{
		pFW->Update();
		++loopCount;
	}

	delete pFW;

	REQUIRE(loopCount == 3);
}

TEST_CASE("Manual Remove Single Coroutine test")
{
	int loopCount{ 0 };
	CFW::CoroFW* pFW = new CFW::CoroFW();

	CFW::Handle res = MultiYieldTest();
	pFW->AddCoroutine(res);

	while (pFW->AreCoroutinesRunning())
	{
		pFW->Update();
		++loopCount;
		if (loopCount == 2)
		{
			pFW->RemoveCoroutine(res);
		}
	}

	delete pFW;

	REQUIRE(loopCount == 2);
}

TEST_CASE("Manual Remove Multiple Coroutines test")
{
	int loopCount{ 0 };
	CFW::CoroFW* pFW = new CFW::CoroFW();

	CFW::Handle res = YieldTest();
	pFW->AddCoroutine(res);

	CFW::Handle res2 = MultiYieldTest();
	pFW->AddCoroutine(res2);

	CFW::Handle res3 = MultiYieldTest();
	pFW->AddCoroutine(res3);

	while (pFW->AreCoroutinesRunning())
	{
		pFW->Update();
		++loopCount;

		if (loopCount == 1)
		{
			pFW->RemoveCoroutine(res2);
		}

		if (loopCount == 2)
		{
			pFW->RemoveCoroutine(res3);
		}
	}

	delete pFW;

	REQUIRE(loopCount == 2);
}

TEST_CASE("RemoveAll test")
{
	int loopCount{ 0 };
	CFW::CoroFW* pFW = new CFW::CoroFW();

	CFW::Handle res = YieldTest();
	pFW->AddCoroutine(res);

	CFW::Handle res2 = MultiYieldTest();
	pFW->AddCoroutine(res2);

	CFW::Handle res3 = MultiYieldTest();
	pFW->AddCoroutine(res3);

	while (pFW->AreCoroutinesRunning())
	{
		pFW->RemoveAll();
		pFW->Update();
		++loopCount;
	}

	delete pFW;

	REQUIRE(loopCount == 1);
}