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
		RemoveEverything();
	}

#pragma region Updates
	void CoroFW::Update()
	{
		UpdateLoop(m_CoroVec);
	}

	void CoroFW::FixedUpdate()
	{
		UpdateLoop(m_FixedCoroVec);
	}

	void CoroFW::EndOfFrameUpdate()
	{
		UpdateLoop(m_EndCoroVec);
	}

	void CoroFW::DeleteUpdate()
	{
		std::reverse(m_DeleteVec.begin(), m_DeleteVec.end());

		for (auto& coro : m_DeleteVec)
		{
			RemoveCoroutine(coro);
		}

		m_DeleteVec.clear();
	}
#pragma endregion



#pragma region NormalUpdateFuncs
	void CoroFW::AddCoroutine(const Handle &coro)
	{
		AddSingleCoroutine(m_CoroVec, coro);
	}

	void CoroFW::RemoveCoroutine(Handle& coro)
	{
		RemoveSingleCoroutine(m_CoroVec, coro);
	}

	void CoroFW::RemoveCoroutine(size_t coro)
	{
		RemoveSingleCoroutine(m_CoroVec, coro);
	}

	void CoroFW::RemoveAll()
	{
		RemoveCoroutineVector(m_CoroVec);
	}
#pragma endregion



#pragma region FixedUpdateFuncs
	void CoroFW::AddFixedCoroutine(const Handle &coro)
	{
		AddSingleCoroutine(m_FixedCoroVec, coro);
	}

	void CoroFW::RemoveFixedCoroutine(Handle& coro)
	{
		RemoveSingleCoroutine(m_FixedCoroVec, coro);
	}

	void CoroFW::RemoveFixedCoroutine(size_t coro)
	{
		RemoveSingleCoroutine(m_FixedCoroVec, coro);
	}

	void CoroFW::RemoveAllFixed()
	{
		RemoveCoroutineVector(m_FixedCoroVec);
	}
#pragma endregion



#pragma region EndOfFrameUpdateFuncs
	void CoroFW::AddEndCoroutine(const Handle &coro)
	{
		AddSingleCoroutine(m_EndCoroVec, coro);
	}

	void CoroFW::RemoveEndCoroutine(Handle& coro)
	{
		RemoveSingleCoroutine(m_EndCoroVec, coro);
	}

	void CoroFW::RemoveEndCoroutine(size_t coro)
	{
		RemoveSingleCoroutine(m_EndCoroVec, coro);
	}

	void CoroFW::RemoveAllEnd()
	{
		RemoveCoroutineVector(m_EndCoroVec);
	}
#pragma endregion



	void CoroFW::RemoveEverything()
	{
		RemoveAll();
		RemoveAllFixed();
		RemoveAllEnd();
	}

	bool CoroFW::AreCoroutinesRunning() const
	{
		return !m_CoroVec.empty();
	}



#pragma region Helpers
	inline void CoroFW::UpdateLoop(std::vector<Handle>& vec)
	{
#ifdef _DEBUG
		if (!m_DeleteVec.empty())
		{
			printf("CoroFW::UpdateLoop > All coroutines are done, but no DeleteUpdate was called last frame.\n\
		> Calling DeleteUpdate\n\
		> NOTE: This will not happen automatically in Release,\n\
		add DeleteUpdate to avoid memory leaks in Release\n\n");
			DeleteUpdate();
		}
#endif

		for (size_t i = 0; i < vec.size(); ++i)
		{
			if (!vec[i].Resume())
				m_DeleteVec.push_back(i);
		}
	}

	inline void CoroFW::AddSingleCoroutine(std::vector<Handle>& vec, const Handle &coro)
	{
#ifdef _DEBUG
		if (!coro.IsValid())
			throw std::exception("CoroFW::AddSingleCoroutine > Attempting to add Destroyed coroutine.");
#endif
		vec.push_back(coro);
	}

	inline void CoroFW::RemoveSingleCoroutine(std::vector<Handle>& vec, size_t coro)
	{
		vec[coro].Destroy();
		vec[coro] = vec.back();
		vec.pop_back();
	}

	inline void CoroFW::RemoveSingleCoroutine(std::vector<Handle>& vec, Handle& coro)
	{
#ifdef _DEBUG
		if (std::find(vec.begin(), vec.end(), coro) == m_CoroVec.end())
			throw std::exception("RemoveCoroutine > Coroutine to remove not found.");
#endif
		coro.Destroy();
		vec.erase(std::remove(vec.begin(), vec.end(), coro), vec.end());
	}

	inline void CoroFW::RemoveCoroutineVector(std::vector<Handle>& vec)
	{
		for (auto& coro : vec)
			coro.Destroy();
		vec.clear();
	}
#pragma endregion
}