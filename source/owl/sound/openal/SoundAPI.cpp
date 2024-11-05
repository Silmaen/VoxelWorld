/**
 * @file SoundAPI.cpp
 * @author Silmaen
 * @date 11/5/24
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "SoundAPI.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <debug/Profiler.h>

namespace owl::sound::openal {


namespace internal {
struct OpenAlAPI {
	ALCdevice* device = nullptr;
	ALCcontext* context = nullptr;
	void reset() {
		device = nullptr;
		context = nullptr;
	}
};
}// namespace internal

namespace {
internal::OpenAlAPI g_Device;
}// namespace

void SoundAPI::init() {
	OWL_PROFILE_FUNCTION()

	// Device (TODO: Devices management)
	if (g_Device.device != nullptr) {
		OWL_CORE_WARN("SoundAPI(OpenAL): Device already initialized")
	} else {
		g_Device.device = alcOpenDevice(nullptr);
		if (g_Device.device == nullptr) {
			OWL_CORE_ERROR("SoundAPI(OpenAL): Device could not be opened")
			setState(State::Error);
			return;
		}
	}

	// Context
	if (g_Device.context != nullptr) {
		OWL_CORE_WARN("SoundAPI(OpenAL): Context already initialized")
	} else {
		g_Device.context = alcCreateContext(g_Device.device, nullptr);
		if (g_Device.context == nullptr) {
			OWL_CORE_ERROR("SoundAPI(OpenAL): Context could not be created")
			setState(State::Error);
			return;
		}
		if (alcMakeContextCurrent(g_Device.context) == 0) {
			OWL_CORE_ERROR("SoundAPI(OpenAL): Failed to make context current")
			setState(State::Error);
			return;
		}
	}

	// Listener (very basic for the moment)
	{
		const ALfloat listenerOri[] = {0, 0, 1, 0, 1, 0};
		alListener3f(AL_POSITION, 0, 0, 0);
		alListener3f(AL_VELOCITY, 0, 0, 0);
		alListenerfv(AL_ORIENTATION, listenerOri);
	}
	setState(State::Ready);
}

SoundAPI::~SoundAPI() {
	OWL_PROFILE_FUNCTION()

	if (g_Device.context != nullptr) {
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(g_Device.context);
	}
	if (g_Device.device != nullptr) {
		alcCloseDevice(g_Device.device);
	}
	g_Device.reset();
	setState(State::Created);
}

void SoundAPI::playSound([[maybe_unused]] const shared<SoundData>& iData) {}

}// namespace owl::sound::openal
