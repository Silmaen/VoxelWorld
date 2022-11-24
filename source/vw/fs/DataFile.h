/**
 * @file DataFile.h
 * @author damien.lachouette
 * @date 24/11/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

namespace vw::fs {

/**
 * @brief Class DataFile
 */
class DataFile {
public:
  /**
   * @brief Default copy constructor
   */
  DataFile(const DataFile &) = default;
  /**
   * @brief Default move constructor
   */
  DataFile(DataFile &&) = default;
  /**
   * @brief Default copy assignation
   * @return this
   */
  DataFile &operator=(const DataFile &) = default;
  /**
   * @brief Default move assignation
   * @return this
   */
  DataFile &operator=(DataFile &&) = default;
  /**
   * @brief Default constructor.
   */
  DataFile() = default;
  /**
   * @brief Destructor.
   */
  virtual ~DataFile() = default; //---UNCOVER---
private:
};

} // namespace vw::fs
