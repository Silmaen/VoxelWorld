/**
 * @file ResourceManager.h
 * @author argawaen
 * @date 03/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <filesystem>

namespace owl::core {

/// alias to path
using path = std::filesystem::path;

/**
 * @brief Class ResourceManager
 */
class ResourceManager {
public:
  ResourceManager(const ResourceManager &) = delete;
  ResourceManager(ResourceManager &&) = delete;
  ResourceManager &operator=(const ResourceManager &) = delete;
  ResourceManager &operator=(ResourceManager &&) = delete;
  /**
   * @brief Destructor.
   */
  virtual ~ResourceManager() = default;

  /**
   * @brief Get singleton instance of the manager
   * @return The Manager
   */
  static ResourceManager& get(){
    static ResourceManager manager;
    return manager;
  }

  /**
   * @brief Try to fin resource Folder based on a exec path
   * @param execPath Path to the executable
   */
  void findResource(const path& execPath = path{});

  /**
   * @brief Get the path to the shaders
   * @return Path to the shaders
   */
  [[nodiscard]] path getShaderPath()const {
    return ResourcePath / "shaders";
  }
private:
  /**
   * @brief Default constructor.
   */
  ResourceManager() = default;
  /**
   * @brief Base path to the resource
   */
  std::filesystem::path ResourcePath;
};

} // namespace owl::core
