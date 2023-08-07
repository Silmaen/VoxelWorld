/**
 * @file EntryPoint.h
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once
#include "Application.h"
#include "Log.h"
#include "debug/Profiler.h"
#include "debug/Tracker.h"

/**
 * @brief Main entry point for the program.
 * @param argc Number of argument.
 * @param argv List of arguments.
 * @return Execution code.
 */
int main(int argc, char *argv[]) {
	owl::core::Log::init();
	{
		// Startup
		OWL_PROFILE_BEGIN_SESSION("Startup", "OwlProfile-startup.json")
		auto app = owl::core::createApplication(argc, argv);
		OWL_PROFILE_END_SESSION()
		// runtime
		OWL_PROFILE_BEGIN_SESSION("Runtime", "OwlProfile-runtime.json")
		app->run();
		OWL_PROFILE_END_SESSION()
		// Shutdown
		OWL_PROFILE_BEGIN_SESSION("Shutdown", "OwlProfile-shutdown.json")
		app.reset();
		OWL_PROFILE_END_SESSION()
	}
	{
		OWL_SCOPE_UNTRACK
		// ==================== Print Memory informations ========================
		OWL_CORE_INFO("Memory State at the end of execution:")
		auto &memoryState = owl::debug::Tracker::get().globals();
		OWL_CORE_INFO("Residual memory          : {}", memoryState.allocatedMemory)
		OWL_CORE_INFO("Memory peek              : {}", memoryState.memoryPeek)
		OWL_CORE_INFO("Total Allocation calls   : {}", memoryState.allocationCalls)
		OWL_CORE_INFO("Total Deallocation calls : {}", memoryState.deallocationCalls)
		if (memoryState.allocationCalls > memoryState.deallocationCalls) {
			OWL_CORE_INFO("Remaining memory chunks  :", memoryState.allocationCalls)
			for (const auto &alloc: memoryState.allocs) {
				OWL_CORE_INFO("* {}", alloc.toStr())
			}
		}
	}
	// Destroy the logger
	owl::core::Log::invalidate();
	return 0;
}
