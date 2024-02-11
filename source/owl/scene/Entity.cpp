/**
 * @file Entity.cpp
 * @author Silmaen
 * @date 22/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Entity.h"

namespace owl::scene {

Entity::~Entity() = default;

Entity::Entity(entt::entity handle, Scene *scene_) : entityHandle{handle}, scene{scene_} {
}

}// namespace owl::scene
