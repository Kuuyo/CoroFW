#include "pch.h"
#include "Yield.h"

namespace CFW
{
	namespace Yields
	{
		CFW::Handle WaitWhile(const std::function<bool()>& func)
		{
			while (func())
			{
				CFW_YieldNull();
			}
		}

		CFW::Handle WaitUntil(const std::function<bool()>& func)
		{
			while (!func())
			{
				CFW_YieldNull();
			}
		}
	}
}