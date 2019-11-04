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

		// Regular update - uses AddCoroutine
		void Update();
		// FixedUpdate - uses AddFixedCoroutine
		void FixedUpdate();
		// Update for at the end of a frame - uses AddEndCoroutine
		void EndOfFrameUpdate();
		// Cleanup update, place after all the other updates
		void DeleteUpdate();

		// Adds a coroutine to Update
		void AddCoroutine(const Handle &coro);
		// Removes a coroutine from Update
		void RemoveCoroutine(Handle& coro);
		// Removes all coroutines from Update
		// If using multiple Updates, use RemoveEverything to remove everything
		void RemoveAll();

		// Adds a coroutine to FixedUpdate
		void AddFixedCoroutine(const Handle &coro);
		// Removes a coroutine from FixedUpdate
		void RemoveFixedCoroutine(Handle& coro);
		// Removes all coroutines from FixedUpdate
		// If using multiple Updates, use RemoveEverything to remove everything
		void RemoveAllFixed();

		// Adds a coroutine to EndOfFrameUpdate
		void AddEndCoroutine(const Handle &coro);
		// Removes a coroutine from EndOfFrameUpdate
		void RemoveEndCoroutine(Handle& coro);
		// Removes all coroutines from EndOfFrameUpdate
		// If using multiple Updates, use RemoveEverything to remove everything
		void RemoveAllEnd();

		// Removes every single coroutine from the manager
		void RemoveEverything();

		// Returns true if there is any coroutine in the framework
		bool AreCoroutinesRunning() const;

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