/**
 * @file main.cpp
 * @author Silmaen
 * @date 24/11/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owl.h"

class Sandbox: public owl::core::Application{
public:
};

std::shared_ptr<owl::core::Application> owl::core::createApplication(){
  return std::make_shared<Sandbox>();
}
