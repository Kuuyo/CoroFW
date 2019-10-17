#include "pch.h"
#include "CoroFW.h"

#include <algorithm>

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
		//m_CoroVec.erase(std::remove_if(m_CoroVec.begin(), m_CoroVec.end(), [](Result& c)
		//	{ 
		//		bool isMarkedForDelete = !c.Resume();
		//
		//		if (isMarkedForDelete)
		//		{
		//			c.Destroy();
		//		}
		//
		//		return isMarkedForDelete;
		//	}), m_CoroVec.end());

		for (auto &coro : m_CoroVec)
		{
			if (!coro.Resume())
				m_DeleteVec.push_back(&coro);
		}
		
		for (auto& coro : m_DeleteVec)
		{			
			RemoveCoroutine(*coro);
		}
		
		m_DeleteVec.clear();
	}

	void CoroFW::AddCoroutine(const Result &coro)
	{
		coro.Resume();
		m_CoroVec.push_back(coro);
	}

	void CoroFW::RemoveCoroutine(Result &coro)
	{
		coro.Destroy();
		m_CoroVec.erase(std::remove(m_CoroVec.begin(), m_CoroVec.end(), coro), m_CoroVec.end());
	}

	void CoroFW::RemoveAll()
	{
		m_CoroVec.clear();
	}

	bool CoroFW::AreCoroutinesRunning()
	{
		return !m_CoroVec.empty();
	}
}