/**
 * @file Tracker.cpp
 * @author Silmaen
 * @date 17/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "Tracker.h"

#define OWL_DEALLOC_EXCEPT noexcept

static bool doTrack = true;
#ifdef OWL_STACKTRACE
static bool doTrace = false;
#endif

#if !defined(__cpp_sized_deallocation) || __cpp_sized_deallocation == 0
void operator delete(void *memory, size_t size) OWL_DEALLOC_EXCEPT;
#endif

void *operator new(size_t size) {
	void *mem = malloc(size);
	owl::debug::Tracker::get().allocate(mem, size);
	return mem;
}

void operator delete(void *memory, size_t size) OWL_DEALLOC_EXCEPT {
	owl::debug::Tracker::get().deallocate(memory, size);
	free(memory);
}

void operator delete(void *memory) OWL_DEALLOC_EXCEPT {
	owl::debug::Tracker::get().deallocate(memory);
	free(memory);
}

namespace owl::debug {

Tracker::Tracker() {
	doTrack = true;
}

Tracker::~Tracker() {
	doTrack = false;
}

Tracker &Tracker::get() {
	static Tracker instance;
	return instance;
}

void Tracker::allocate(void *memoryPtr, size_t size) {
	if (!doTrack) return;
	currentAllocationState.pushMemory(memoryPtr, size);
	globalAllocationState.pushMemory(memoryPtr, size);
}

void Tracker::deallocate(void *memoryPtr, size_t size) {
	if (!doTrack) return;
	currentAllocationState.freeMemory(memoryPtr, size);
	globalAllocationState.freeMemory(memoryPtr, size);
}

const Tracker::AllocationState &Tracker::checkState() {
	doTrack = false;
	lastAllocationState.reset();
	std::swap(currentAllocationState, lastAllocationState);
	doTrack = true;
	return lastAllocationState;
}

const Tracker::AllocationState &Tracker::globals() const {
	return globalAllocationState;
}

std::string Tracker::AllocationInfo::toStr() const {
#ifdef OWL_STACKTRACE
	return fmt::format("memory chunk at {} size ({}) allocated {} ({}:{}:{})", location, size,
					   traceAlloc.symbol, traceAlloc.filename, traceAlloc.line, traceAlloc.col);
#else
	return fmt::format("memory chunk at {} size ({})", location, size);
#endif
}

void Tracker::AllocationState::pushMemory(void *memPtr, size_t size) {
	doTrack = false;
	allocationCalls++;
	allocatedMemory += size;
	memoryPeek = std::max(memoryPeek, allocatedMemory);
	allocs.emplace_back(memPtr, size);
#ifdef OWL_STACKTRACE
	if (doTrace) {
		auto trace = cpptrace::generate_trace(3);
		auto bob = std::find_if(trace.begin(), trace.end(),
								[](const cpptrace::stacktrace_frame itrace) { return itrace.symbol.starts_with("owl::") || itrace.symbol.contains("main"); });
		if (bob != trace.end())
			allocs.back().traceAlloc = *bob;
	}
#endif
	doTrack = true;
}

void Tracker::AllocationState::freeMemory(void *memPtr, size_t size) {
	doTrack = false;
	auto chunk = std::ranges::find_if(allocs.begin(), allocs.end(), [&memPtr](const AllocationInfo &cc) { return cc.location == memPtr; });
	if (chunk != allocs.end()) {
		if (size == 0) {
			size = chunk->size;
		}
		allocs.erase(chunk);
		deallocationCalls++;
		allocatedMemory -= size;
	}
	doTrack = true;
}

void Tracker::AllocationState::reset() {
	allocs.clear();
	allocatedMemory = 0;
	allocationCalls = 0;
	deallocationCalls = 0;
	memoryPeek = 0;
}

#ifdef OWL_STACKTRACE
ScopeTrace::ScopeTrace() {
	doTrace = true;
}
ScopeTrace::~ScopeTrace() {
	doTrace = false;
}
#endif

ScopeUntrack::ScopeUntrack() {
	doTrack = false;
}

ScopeUntrack::~ScopeUntrack() {
	doTrack = true;
}

}// namespace owl::debug
