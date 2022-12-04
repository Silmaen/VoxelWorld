/**
 * @file ResourceManager.cpp
 * @author argawaen
 * @date 03/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "ResourceManager.h"

namespace owl::core {

void ResourceManager::findResource(const path &execPath) {
  path tmp{execPath};
  if (tmp.empty()){
    tmp = std::filesystem::current_path();
  }
  tmp = absolute(tmp);
  if (exists(tmp / "res")) {
    ResourcePath = tmp / "res";
    return;
  }
  if (exists(tmp.parent_path() / "res")) {
    ResourcePath = tmp.parent_path() / "res";
    return;
  }
  if (exists(tmp.parent_path().parent_path() / "res")) {
    ResourcePath = tmp.parent_path().parent_path() / "res";
    return;
  }
  ResourcePath = tmp;
}

} // namespace owl::core
