/**
 * @file Tracker.cpp
 * @author Silmaen
 * @date 17/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"
#include <GLFW/glfw3.h>

#include "Tracker.h"

#define OWL_DEALLOC_EXCEPT noexcept

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

Tracker &Tracker::get() {
	static Tracker instance;
	return instance;
}

void Tracker::allocate(void *memoryPtr, size_t size) {
	auto chunk = std::find_if(globalAllocationState.allocs.begin(),
							  globalAllocationState.allocs.end(),
							  [&memoryPtr](const AllocationInfo &cc) { return cc.location == memoryPtr; });
	if (chunk != globalAllocationState.allocs.end()) {
		OWL_CORE_WARN("Try to allocate already reserved {}.", chunk->toStr())
		deallocate(chunk->location, chunk->size);
	}
	globalAllocationState.allocs.emplace_back(memoryPtr, size
#ifdef OWL_STACKTRACE
											  ,
											  cpptrace::generate_trace(2).front()
#endif
	);
	++currentAllocationState.allocationCalls;
	currentAllocationState.allocs.emplace_back(memoryPtr, size
#ifdef OWL_STACKTRACE
											   ,
											   cpptrace::generate_trace(2).front()
#endif
	);

	++currentAllocationState.allocationCalls;
	currentAllocationState.allocatedMemory += size;
	currentAllocationState.memoryPeek =
			std::max(currentAllocationState.memoryPeek,
					 currentAllocationState.allocatedMemory);
	++globalAllocationState.allocationCalls;
	globalAllocationState.allocatedMemory += size;
	globalAllocationState.memoryPeek = std::max(
			globalAllocationState.memoryPeek, globalAllocationState.allocatedMemory);
}

void Tracker::deallocate(void *memoryPtr, size_t size) {
	auto chunkGlobal = std::find_if(globalAllocationState.allocs.begin(),
									globalAllocationState.allocs.end(),
									[&memoryPtr](const AllocationInfo &cc) { return cc.location == memoryPtr; });
	auto chunkCurrent = std::find_if(globalAllocationState.allocs.begin(),
									 globalAllocationState.allocs.end(),
									 [&memoryPtr](const AllocationInfo &cc) { return cc.location == memoryPtr; });
	if (chunkGlobal == globalAllocationState.allocs.end()) {
		OWL_CORE_WARN("Try to deallocate un registered memory.")
	}

	cpptrace::stacktrace_frame fr = cpptrace::generate_trace(2).front();
	if (chunkCurrent != currentAllocationState.allocs.end()) {
		chunkCurrent->traceDealloc = fr;
		uint32_t trueSize = size;
		if (size == 0)
			trueSize = chunkCurrent->size;
		if (trueSize != chunkCurrent->size)
			OWL_CORE_WARN("Memory dealloc size missmatch {} {}", trueSize, chunkCurrent->toStr())
		++currentAllocationState.deallocationCalls;
		currentAllocationState.allocatedMemory =
				currentAllocationState.allocatedMemory > trueSize
						? currentAllocationState.allocatedMemory - trueSize
						: 0;
		currentAllocationState.allocs.erase(chunkCurrent);
	}
	if (chunkGlobal != globalAllocationState.allocs.end()) {
		chunkGlobal->traceDealloc = fr;
		uint32_t trueSize = size;
		if (size == 0)
			trueSize = chunkCurrent->size;
		if (trueSize != chunkCurrent->size)
			OWL_CORE_WARN("Memory dealloc size missmatch {} {}", trueSize, chunkGlobal->toStr())
		++globalAllocationState.deallocationCalls;
		globalAllocationState.allocatedMemory =
				globalAllocationState.allocatedMemory > size
						? globalAllocationState.allocatedMemory - size
						: 0;
		globalAllocationState.allocs.erase(chunkGlobal);
	}
}

const Tracker::AllocationState &Tracker::checkState() {
	lastAllocationState.allocatedMemory = 0;
	lastAllocationState.allocationCalls = 0;
	lastAllocationState.deallocationCalls = 0;
	lastAllocationState.memoryPeek = 0;
	lastAllocationState.allocs.clear();
	if (!currentAllocationState.allocs.empty()) {
		OWL_CORE_TRACE("Memory leak{} detected since last checks:", currentAllocationState.allocs.size() > 1 ? "s" : "")
		for (const auto &trace: currentAllocationState.allocs) {
			OWL_CORE_TRACE("Leak: {}.", trace.toStr())
		}
	}
	std::swap(currentAllocationState, lastAllocationState);
	return lastAllocationState;
}

const Tracker::AllocationState &Tracker::globals() const {
	return globalAllocationState;
}

std::string Tracker::AllocationInfo::toStr() const {
#ifdef OWL_STACKTRACE
	if (traceDealloc.address == 0)
		return fmt::format("memory chunk at {} size ({}) allocated {} ({}:{}:{})", location, size,
						   traceAlloc.symbol, traceAlloc.filename, traceAlloc.line, traceAlloc.col);
	else
		return fmt::format("memory chunk at {} size ({}) allocated {} ({}:{}:{}) // deallocated {} ({}:{}:{})",
						   location, size,
						   traceAlloc.symbol, traceAlloc.filename, traceAlloc.line, traceAlloc.col,
						   traceDealloc.symbol, traceDealloc.filename, traceDealloc.line, traceDealloc.col);
#else
	return fmt::format("memory chunk at {} size ({})", location, size);
#endif
}
}// namespace owl::debug
