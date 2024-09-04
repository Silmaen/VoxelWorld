/**
 * @file Tracker.cpp
 * @author Silmaen
 * @date 17/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include <iostream>
#include <stack>

#include "Tracker.h"


#define OWL_DEALLOC_EXCEPT noexcept
namespace {

bool s_antiLoop = false;

class TrackerState {
public:
	/// Destructor.
	~TrackerState() = default;

	TrackerState(const TrackerState&) = delete;
	TrackerState(TrackerState&&) = delete;
	auto operator=(const TrackerState&) -> TrackerState& = delete;
	auto operator=(TrackerState&&) -> TrackerState& = delete;

	static auto get() -> TrackerState& {
		static TrackerState instance;
		return instance;
	}

	void pushTrack(const bool iState) { m_doTrack.push(iState); }
	void popTrack() {
		if (m_doTrack.size() > 1)
			m_doTrack.pop();
	}
	[[nodiscard]] auto canTrack() const -> bool {
		if (m_doTrack.empty())
			return true;
		return m_doTrack.top();
	}

private:
	/// states of tracking
	std::stack<bool> m_doTrack;
	TrackerState() { m_doTrack.push(true); }
};

}// namespace

#if !defined(__cpp_sized_deallocation) || __cpp_sized_deallocation == 0
void operator delete(void* iMemory, size_t iSize) OWL_DEALLOC_EXCEPT;
#endif

// NOLINTBEGIN(*-no-malloc,cppcoreguidelines-owning-memory)
auto operator new(const size_t iSize) -> void* {
	void* mem = malloc(iSize);
	owl::debug::TrackerAPI::allocate(mem, iSize);
	return mem;
}

void operator delete(void* iMemory, const size_t iSize) OWL_DEALLOC_EXCEPT {
	owl::debug::TrackerAPI::deallocate(iMemory, iSize);
	free(iMemory);
}

void operator delete(void* iMemory) OWL_DEALLOC_EXCEPT {
	owl::debug::TrackerAPI::deallocate(iMemory);
	free(iMemory);
}
// NOLINTEND(*-no-malloc,cppcoreguidelines-owning-memory)

namespace owl::debug {

namespace {

class StateManager {
public:
	static void allocate() {
		s_globalAllocationState = std::make_shared<AllocationState>();
		s_currentAllocationState = std::make_shared<AllocationState>();
		s_lastAllocationState = std::make_shared<AllocationState>();
	}
	static void pushMemory(void* iLocation, const size_t iSize) {
		if (!s_globalAllocationState)
			allocate();
		s_currentAllocationState->pushMemory(iLocation, iSize);
		s_globalAllocationState->pushMemory(iLocation, iSize);
	}
	static void freeMemory(void* iLocation, const size_t iSize) {
		s_currentAllocationState->freeMemory(iLocation, iSize);
		s_globalAllocationState->freeMemory(iLocation, iSize);
	}
	static void swapCurrent() {
		s_lastAllocationState->reset();
		std::swap(s_currentAllocationState, s_lastAllocationState);
	}
	[[nodiscard]] static auto getLastAllocationState() -> const AllocationState& { return *s_lastAllocationState; }
	[[nodiscard]] static auto getGlobalAllocationState() -> const AllocationState& { return *s_globalAllocationState; }

private:
	/// Global Memory allocation state's info.
	static shared<AllocationState> s_globalAllocationState;
	/// Current Memory allocation state's info.
	static shared<AllocationState> s_currentAllocationState;
	/// Last Memory allocation state's info.
	static shared<AllocationState> s_lastAllocationState;
};
shared<AllocationState> StateManager::s_globalAllocationState;
shared<AllocationState> StateManager::s_currentAllocationState;
shared<AllocationState> StateManager::s_lastAllocationState;
}// namespace

// =========================== TrackerAPI =================================

void TrackerAPI::allocate(void* iMemoryPtr, const size_t iSize) {
	if (s_antiLoop)
		return;
	s_antiLoop = true;
	if (!TrackerState::get().canTrack()) {
		s_antiLoop = false;
		return;
	}
	StateManager::pushMemory(iMemoryPtr, iSize);
	s_antiLoop = false;
}

void TrackerAPI::deallocate(void* iMemoryPtr, const size_t iSize) {
	if (s_antiLoop)
		return;
	s_antiLoop = true;
	if (!TrackerState::get().canTrack()) {
		s_antiLoop = false;
		return;
	}
	StateManager::freeMemory(iMemoryPtr, iSize);
	s_antiLoop = false;
}

auto TrackerAPI::checkState() -> const AllocationState& {
	StateManager::swapCurrent();
	return StateManager::getLastAllocationState();
}

auto TrackerAPI::globals() -> const AllocationState& { return StateManager::getGlobalAllocationState(); }

// =========================== Allocation Info =================================

AllocationInfo::AllocationInfo(void* iLocation, const size_t iSize) : location{iLocation}, size{iSize} {
#ifdef OWL_STACKTRACE
	fullTrace = cpptrace::generate_trace();
#endif
}

auto AllocationInfo::toStr([[maybe_unused]] const bool iTracePrint, [[maybe_unused]] const bool iFullTrace) const
		-> std::string {
	std::string result = std::format("memory chunk at {} size ({})", location, size);

#ifdef OWL_STACKTRACE
	if (fullTrace.empty()) {
		result += std::format(" <empty Trace>");
	} else {
		auto last = getCallerInfo();
		result += std::format(" allocated {} ({}:{}:{})", last.symbol, last.filename, last.line.value_or(0),
							  last.column.value_or(0));
		if (iTracePrint) {
			result += std::format("\n *** {} StackTrace (most recent call first) *** \n",
								  iFullTrace ? "Full" : "Simplified");
			uint32_t frameId = 0;
			for (auto frame: fullTrace) {
				if (!iFullTrace && !frame.symbol.starts_with("owl::"))
					continue;
				result += std::format("#{} at {}:{}:{} symbol {}{} ({:#x})\n", frameId, frame.filename,
									  frame.line.value_or(0), frame.column.value_or(0),
									  frame.is_inline ? "(inline) " : "", frame.symbol, frame.raw_address);
				++frameId;
			}
		}
	}
#endif
	return result;
}

#ifdef OWL_STACKTRACE
auto AllocationInfo::getCallerInfo() const -> const cpptrace::stacktrace_frame& {
	for (const auto& frame: fullTrace) {
		if ((!frame.filename.ends_with(".cpp") && !frame.filename.ends_with(".h")) ||
			frame.filename.ends_with("Tracker.cpp") || frame.filename.ends_with("Core.h") ||
			frame.filename.contains("bits"))
			continue;
		return frame;
	}
	return fullTrace.frames.front();
}
#endif

// =========================== Allocation state =================================

AllocationState::~AllocationState() { s_antiLoop = true; }

void AllocationState::pushMemory(void* iLocation, size_t iSize) {
	allocationCalls++;
	allocatedMemory += iSize;
	memoryPeek = std::max(memoryPeek, allocatedMemory);
	if (!s_antiLoop)
		std::println(std::cerr, "anti-loop issue!");
	allocs.emplace_back(iLocation, iSize);
}

void AllocationState::freeMemory(void* iLocation, size_t iSize) {
	if (const auto chunk = std::ranges::find_if(
				allocs.begin(), allocs.end(),
				[&iLocation](const AllocationInfo& iAllocInfo) { return iAllocInfo.location == iLocation; });
		chunk != allocs.end()) {
		if (iSize == 0) {
			iSize = chunk->size;
		}
		allocs.erase(chunk);
		deallocationCalls++;
		allocatedMemory -= iSize;
	}
}

void AllocationState::reset() {
	allocs.clear();
	allocatedMemory = 0;
	allocationCalls = 0;
	deallocationCalls = 0;
	memoryPeek = 0;
}

// =========================== scopes ==============================

ScopeUntrack::ScopeUntrack() {
	s_antiLoop = true;
	TrackerState::get().pushTrack(false);
	s_antiLoop = false;
}
ScopeUntrack::~ScopeUntrack() {
	s_antiLoop = true;
	TrackerState::get().popTrack();
	s_antiLoop = false;
}
ScopeTrack::ScopeTrack() {
	s_antiLoop = true;
	TrackerState::get().pushTrack(true);
	s_antiLoop = false;
}
ScopeTrack::~ScopeTrack() {
	s_antiLoop = true;
	TrackerState::get().popTrack();
	s_antiLoop = false;
}

}// namespace owl::debug
