/**
 * @file Profiler.cpp
 * @author Silmaen
 * @date 07/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Profiler.h"

namespace owl::debug {

	Instrumentor &Instrumentor::get() {
		static Instrumentor instance;
		return instance;
	}

}// namespace owl::debug
