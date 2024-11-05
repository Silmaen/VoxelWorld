/**
 * @file SoundData.cpp
 * @author Silmaen
 * @date 11/5/24
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "SoundData.h"

#include "SoundSystem.h"
#include "null/SoundData.h"
#include "openal/SoundData.h"

namespace owl::sound {

SoundData::~SoundData() = default;

auto SoundData::create() -> shared<SoundData> {
	switch (SoundCommand::getApi()) {
		case SoundAPI::Type::Null:
			return mkShared<null::SoundData>();
		case SoundAPI::Type::OpenAL:
			return mkShared<openal::SoundData>();
	}
	OWL_CORE_ERROR("Unknown Sound API Type!")
	return nullptr;
}

}// namespace owl::sound
