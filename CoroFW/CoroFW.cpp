#include "pch.h"
#include "CoroFW.h"

namespace CFW
{
	CoroFW::CoroFW()
	{
	}

	CoroFW::~CoroFW()
	{
	}

	void CoroFW::Update()
	{
		for (auto &coro : m_CoroVec)
		{
			coro.Resume();
		}
	}

	void CoroFW::AddCoroutine(const Result &coro)
	{
		coro.Resume();
		m_CoroVec.push_back(coro);
	}

	void CoroFW::RemoveCoroutine()
	{

	}

	void CoroFW::RemoveAll()
	{
		m_CoroVec.clear();
	}
}