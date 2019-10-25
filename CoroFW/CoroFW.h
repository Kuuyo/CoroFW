#pragma once
#include <vector>
#include "Handle.h"

namespace CFW
{
	class CoroFW final
	{
	public:
		CoroFW();
		~CoroFW();

		void Update();
		void FixedUpdate();
		void EndOfFrameUpdate();
		void DeleteUpdate();

		void AddCoroutine(const Handle &coro);
		void RemoveCoroutine(Handle& coro);
		void RemoveAll();

		void AddFixedCoroutine(const Handle &coro);
		void RemoveFixedCoroutine(Handle& coro);
		void RemoveAllFixed();

		void AddEndCoroutine(const Handle &coro);
		void RemoveEndCoroutine(Handle& coro);
		void RemoveAllEnd();

		void RemoveEverything();

		bool AreCoroutinesRunning();

		CoroFW(const CoroFW& other) = delete;
		CoroFW(CoroFW&& other) noexcept = delete;
		CoroFW& operator=(const CoroFW& other) = delete;
		CoroFW& operator=(CoroFW&& other) noexcept = delete;

	private:
		std::vector<Handle> m_CoroVec;
		std::vector<Handle> m_FixedCoroVec;
		std::vector<Handle> m_EndCoroVec;
		std::vector<size_t> m_DeleteVec;

		void RemoveCoroutine(size_t coro);
		void RemoveFixedCoroutine(size_t coro);
		void RemoveEndCoroutine(size_t coro);

		inline void UpdateLoop(std::vector<Handle> &vec);

		inline void AddSingleCoroutine(std::vector<Handle>& vec, const Handle &coro);
		inline void RemoveSingleCoroutine(std::vector<Handle>& vec, size_t coro);
		inline void RemoveSingleCoroutine(std::vector<Handle>& vec, Handle& coro);
		inline void RemoveCoroutineVector(std::vector<Handle>& vec);
	};
}