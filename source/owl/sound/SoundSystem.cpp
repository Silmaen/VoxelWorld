/**
 * @file SoundSystem.cpp
 * @author Silmaen
 * @date 11/5/24
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "SoundSystem.h"

namespace owl::sound {

SoundSystem::State SoundSystem::m_internalState = State::Created;

void SoundSystem::init() {
	OWL_PROFILE_FUNCTION()

	SoundCommand::init();
	if (SoundCommand::getState() != SoundAPI::State::Ready) {
		m_internalState = State::Error;
		return;
	}


	m_internalState = State::Running;
}

void SoundSystem::shutdown() {
	reset();
	m_internalState = State::Stopped;
}

void SoundSystem::reset() {
	SoundCommand::invalidate();
	m_internalState = State::Created;
}

}// namespace owl::sound
