/**
 * @file Tracker.cpp
 * @author Silmaen
 * @date 17/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "Tracker.h"

// NOLINTBEGIN(misc-no-recursion)

#define OWL_DEALLOC_EXCEPT noexcept
namespace {
bool s_doTrack = true;
}// namespace
#ifdef OWL_STACKTRACE
static bool s_doTrace = false;
#endif

#if !defined(__cpp_sized_deallocation) || __cpp_sized_deallocation == 0
void operator delete(void *iMemory, size_t size) OWL_DEALLOC_EXCEPT;
#endif
// NOLINTBEGIN(*-no-malloc,cppcoreguidelines-owning-memory)
void *operator new(size_t iSize) {
	void *mem = malloc(iSize);
	owl::debug::Tracker::get().allocate(mem, iSize);
	return mem;
}

void operator delete(void *iMemory, size_t iSize) OWL_DEALLOC_EXCEPT {
	owl::debug::Tracker::get().deallocate(iMemory, iSize);
	free(iMemory);
}

void operator delete(void *iMemory) OWL_DEALLOC_EXCEPT {
	owl::debug::Tracker::get().deallocate(iMemory);
	free(iMemory);
}
// NOLINTEND(*-no-malloc,cppcoreguidelines-owning-memory)

namespace owl::debug {

Tracker::Tracker() { s_doTrack = true; }

Tracker::~Tracker() { s_doTrack = false; }

Tracker &Tracker::get() {
	static Tracker instance;
	return instance;
}

void Tracker::allocate(void *iMemoryPtr, const size_t iSize) {
	if (!s_doTrack)
		return;
	m_currentAllocationState.pushMemory(iMemoryPtr, iSize);
	m_globalAllocationState.pushMemory(iMemoryPtr, iSize);
}

void Tracker::deallocate(void *iMemoryPtr, const size_t iSize) {
	if (!s_doTrack)
		return;
	m_currentAllocationState.freeMemory(iMemoryPtr, iSize);
	m_globalAllocationState.freeMemory(iMemoryPtr, iSize);
}

const Tracker::AllocationState &Tracker::checkState() {
	s_doTrack = false;
	m_lastAllocationState.reset();
	std::swap(m_currentAllocationState, m_lastAllocationState);
	s_doTrack = true;
	return m_lastAllocationState;
}

const Tracker::AllocationState &Tracker::globals() const { return m_globalAllocationState; }

std::string Tracker::AllocationInfo::toStr() const {
#ifdef OWL_STACKTRACE
	return fmt::format("memory chunk at {} size ({}) allocated {} ({}:{}:{})", location, size, traceAlloc.symbol,
					   traceAlloc.filename, traceAlloc.line, traceAlloc.col);
#else
	return fmt::format("memory chunk at {} size ({})", location, size);
#endif
}

void Tracker::AllocationState::pushMemory(void *iLocation, size_t iSize) {
	s_doTrack = false;
	allocationCalls++;
	allocatedMemory += iSize;
	memoryPeek = std::max(memoryPeek, allocatedMemory);
	allocs.emplace_back(iLocation, iSize);
#ifdef OWL_STACKTRACE
	if (s_doTrace) {
		auto trace = cpptrace::generate_trace(3);

		if (auto chunk = std::find_if(trace.begin(), trace.end(),
									  [](const cpptrace::stacktrace_frame itrace) {
										  return itrace.symbol.starts_with("owl::") || itrace.symbol.contains("main");
									  });
			chunk != trace.end())
			allocs.back().traceAlloc = *chunk;
	}
#endif
	s_doTrack = true;
}

void Tracker::AllocationState::freeMemory(void *iLocation, size_t iSize) {
	s_doTrack = false;
	if (const auto chunk = std::ranges::find_if(
				allocs.begin(), allocs.end(),
				[&iLocation](const AllocationInfo &iAllocInfo) { return iAllocInfo.location == iLocation; });
		chunk != allocs.end()) {
		if (iSize == 0) {
			iSize = chunk->size;
		}
		allocs.erase(chunk);
		deallocationCalls++;
		allocatedMemory -= iSize;
	}
	s_doTrack = true;
}

void Tracker::AllocationState::reset() {
	allocs.clear();
	allocatedMemory = 0;
	allocationCalls = 0;
	deallocationCalls = 0;
	memoryPeek = 0;
}

#ifdef OWL_STACKTRACE
ScopeTrace::ScopeTrace() { s_doTrace = true; }
ScopeTrace::~ScopeTrace() { s_doTrace = false; }
#endif

ScopeUntrack::ScopeUntrack() { s_doTrack = false; }

ScopeUntrack::~ScopeUntrack() { s_doTrack = true; }

}// namespace owl::debug


// NOLINTEND(misc-no-recursion)
