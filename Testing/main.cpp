#include "vld.h"

#pragma warning( push,0 )
#pragma warning( disable : 26495 )
#pragma warning( disable : 26812 )
#pragma warning( disable : 26439 )
#pragma warning( disable : 26451 )
#pragma warning( disable : 26444 )
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#pragma warning( pop )

#include "CFW.h"


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

	pFW->AddCoroutine(YieldTest());

	while (pFW->AreCoroutinesRunning())
	{
		pFW->Update();
		++loopCount;	
	}

	delete pFW;

	REQUIRE(loopCount == 1);
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

	pFW->AddCoroutine(MultiYieldTest());

	while (pFW->AreCoroutinesRunning())
	{
		pFW->Update();
		++loopCount;
	}

	delete pFW;

	REQUIRE(loopCount == 3);
}

CFW::Handle ChainTest()
{
	CFW_Yield(MultiYieldTest());
}

TEST_CASE("Chaining Coroutines Check")
{
	int loopCount{ 0 };
	CFW::CoroFW* pFW = new CFW::CoroFW();

	pFW->AddCoroutine(ChainTest());

	while (pFW->AreCoroutinesRunning())
	{
		pFW->Update();
		++loopCount;
	}

	delete pFW;

	REQUIRE(loopCount == 3);
}

int m_Loops{ 0 };

bool HasNotLoopedThreeTimes()
{
	return !(m_Loops == 3);
}

CFW::Handle WaitWhileTest()
{
	CFW_WaitWhile(HasNotLoopedThreeTimes);
}

TEST_CASE("WaitWhile Test")
{
	CFW::CoroFW* pFW = new CFW::CoroFW();

	pFW->AddCoroutine(WaitWhileTest());

	while (pFW->AreCoroutinesRunning())
	{
		pFW->Update();
		++m_Loops;
	}

	delete pFW;

	REQUIRE(m_Loops == 4);
}

bool HasLoopedThreeTimes()
{
	return (m_Loops == 3);
}

CFW::Handle WaitUntilTest()
{
	CFW_WaitUntil(HasLoopedThreeTimes);
}

TEST_CASE("WaitUntil Test")
{
	m_Loops = 0;
	CFW::CoroFW* pFW = new CFW::CoroFW();

	pFW->AddCoroutine(WaitUntilTest());

	while (pFW->AreCoroutinesRunning())
	{
		pFW->Update();
		++m_Loops;
	}

	delete pFW;

	REQUIRE(m_Loops == 4);
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

	CFW::Handle res3 = WaitUntilTest();
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

	pFW->AddCoroutine(YieldTest());

	pFW->AddCoroutine(MultiYieldTest());

	pFW->AddCoroutine(WaitUntilTest());

	while (pFW->AreCoroutinesRunning())
	{
		pFW->RemoveAll();
		pFW->Update();
		++loopCount;
	}

	delete pFW;

	REQUIRE(loopCount == 1);
}

TEST_CASE("RemoveEverything test")
{
	int loopCount{ 0 };
	CFW::CoroFW* pFW = new CFW::CoroFW();

	pFW->AddCoroutine(YieldTest());

	pFW->AddFixedCoroutine(MultiYieldTest());

	pFW->AddEndCoroutine(WaitUntilTest());

	while (pFW->AreCoroutinesRunning())
	{
		pFW->Update();
		++loopCount;
		pFW->FixedUpdate();
		pFW->RemoveEverything();
		pFW->EndOfFrameUpdate();
	}

	delete pFW;

	REQUIRE(loopCount == 1);
}

TEST_CASE("Adding and removing test")
{
	int loopCount{ 0 };
	CFW::CoroFW* pFW = new CFW::CoroFW();

	pFW->AddCoroutine(YieldTest());

	pFW->AddFixedCoroutine(MultiYieldTest());

	pFW->AddEndCoroutine(WaitUntilTest());

	for (size_t i = 0; i < 5; ++i)
	{
		CFW::Handle handle = WaitUntilTest();
		pFW->AddCoroutine(handle);
		pFW->RemoveCoroutine(handle);
	}

	while (pFW->AreCoroutinesRunning())
	{
		pFW->Update();
		++loopCount;
		pFW->FixedUpdate();
		pFW->RemoveEverything();
		pFW->EndOfFrameUpdate();
	}

	delete pFW;

	REQUIRE(loopCount == 1);
}

TEST_CASE("Throw on attempting to add destroyed coroutine test")
{
	CFW::CoroFW pFW = CFW::CoroFW();

	CFW::Handle res = MultiYieldTest();
	res.Destroy();
	REQUIRE_THROWS(pFW.AddCoroutine(res));
}

CFW::Handle YieldValueTest()
{
	CFW_YieldValue(3);
}

TEST_CASE("YieldValue Check")
{
	int loopCount{ 0 };
	CFW::CoroFW* pFW = new CFW::CoroFW();

	CFW::Handle han = YieldValueTest();
	pFW->AddCoroutine(han);

	int value = han.GetValue<int>();

	while (pFW->AreCoroutinesRunning())
	{
		pFW->Update();
		++loopCount;
	}

	delete pFW;

	REQUIRE(value == 3);
	REQUIRE(loopCount == 1);
}

CFW::Handle MultiYieldValueTest()
{
	for (int i = 0; i < 3; ++i)
	{
		CFW_YieldValue(3 * i);
	}
}

TEST_CASE("Multi YieldValue Check")
{
	int loopCount{ 0 };
	CFW::CoroFW* pFW = new CFW::CoroFW();

	CFW::Handle han = MultiYieldValueTest();
	pFW->AddCoroutine(han);

	while (pFW->AreCoroutinesRunning())
	{
		int value = han.GetValue<int>();
		REQUIRE(value == 3 * loopCount);
		pFW->Update();
		++loopCount;
	}

	delete pFW;
	REQUIRE(loopCount == 3);
}