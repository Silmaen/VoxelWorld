/**
 * @file Voxel.h
 * @author damien.lachouette
 * @date 24/11/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "Vector3.h"


namespace vw::geom {

/**
 * @brief Class Voxel
 */
class Voxel {
public:
  /// Voxel size's type
  using SizeType = uint16_t;
  /// Voxel size's type
  using CoordType = int16_t;
  /// Definition of the voxels coordinates
  using Coord = Vector3<CoordType> ;
  /**
   * @brief Default copy constructor
   */
  Voxel(const Voxel &) = default;
  /**
   * @brief Default move constructor
   */
  Voxel(Voxel &&) = default;
  /**
   * @brief Default copy assignation
   * @return this
   */
  Voxel &operator=(const Voxel &) = default;
  /**
   * @brief Default move assignation
   * @return this
   */
  Voxel &operator=(Voxel &&) = default;
  /**
   * @brief Default constructor.
   */
  Voxel() = default;
  /**
   * @brief Destructor.
   */
  virtual ~Voxel() = default; //---UNCOVER---
private:
};

} // namespace vw::geom
