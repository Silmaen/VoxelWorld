/**
 * @file Vector3.h
 * @author Silmaen
 * @date 02/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "Vector2.h"

namespace vw::geom {

/**
 * @brief Class Vector3
 */
template<class DataType>
class Vector3 {
public:
    /**
     * @brief Default copy constructor
     */
    Vector3(const Vector3&) = default;
    /**
     * @brief Copy constructor with type change
     * @tparam DatatypeOther The other's type
     * @param other The vector to copy
     */
    template<class DatatypeOther>
    explicit Vector3(const Vector3<DatatypeOther>& other):X{static_cast<DataType>(other[0])},Y{static_cast<DataType>(other[1])},Z{static_cast<DataType>(other[2])}{}
    /**
     * @brief Copy constructor with type change
     * @tparam DatatypeOther The other's type
     * @param other The vector to copy
     */
    template<class DatatypeOther>
    explicit Vector3(const Vector2<DatatypeOther>& other):X{static_cast<DataType>(other[0])},Y{static_cast<DataType>(other[1])},Z{0}{}
    /**
     * @brief Default move constructor
     */
    Vector3(Vector3&&) noexcept = default;
    /**
     * @brief Default copy assignation
     * @return this
     */
    Vector3& operator=(const Vector3& ) = default;
    /**
     * @brief Copy assignation with type change
     * @tparam DatatypeOther The other's type
     * @param other The vector to copy
     * @return this
     */
    template<class DatatypeOther>
    Vector3& operator=(const Vector3<DatatypeOther>& other){
            X = static_cast<DataType>(other[0]);
            Y = static_cast<DataType>(other[1]);
            Z = static_cast<DataType>(other[2]);
            return *this;
    }
    /**
     * @brief Copy assignation with type change
     * @tparam DatatypeOther The other's type
     * @param other The vector to copy
     * @return this
     */
    template<class DatatypeOther>
    Vector3& operator=(const Vector2<DatatypeOther>& other){
        X = static_cast<DataType>(other[0]);
        Y = static_cast<DataType>(other[1]);
        Z = 0;
        return *this;
    }
    /**
     * @brief Default move assignation
     * @return this
     */
    Vector3& operator=(Vector3&&) noexcept = default;
    /**
     * @brief Default constructor.
     */
    Vector3()= default;
    /**
     * @brief Destructor.
     */
    virtual ~Vector3()= default;//---UNCOVER---
    /**
     * @brief Constructor with coordinates
     * @param x First coordinates
     * @param y Second coordinates
     * @param z Third coordinates
     */
    Vector3(DataType x, DataType y, DataType z=0):X(x),Y(y),Z(z) {}

    /**
     * @brief Comparison operator
     * @param other Other vector to compare
     * @return True if equal
     */
    bool operator==(const Vector3& other)const{return X==other.X && Y==other.Y && Z==other.Z;}

    /**
     * @brief Comparison operator
     * @param other Other vector to compare
     * @return True if not equal
     */
    bool operator!=(const Vector3& other)const{return X!=other.X || Y!=other.Y|| Z!=other.Z;}
    /**
     * @brief Data access
     * @param idx component index
     * @return data
     */
    DataType& operator[](uint8_t idx){return idx == 0 ? X : idx == 1 ? Y : Z;}
    /**
     * @brief Data access
     * @param idx component index
     * @return data
     */
    const DataType& operator[](uint8_t idx)const{return idx == 0 ? X : idx == 1 ? Y : Z;}

    /**
     * @brief Addition operator
     * @param other Other vector to add
     * @return This actualized vector
     */
    Vector3& operator+=(const Vector3& other){X+=other.X;Y+=other.Y;Z+=other.Z;return *this;}

    /**
     * @brief Sub operator
     * @param other Other vector to sub
     * @return This actualized vector
     */
    Vector3& operator-=(const Vector3& other){X-=other.X;Y-=other.Y;Z-=other.Z;return *this;}

    /**
     * @brief product operator
     * @param other Scalar to multiply
     * @return This actualized vector
     */
    Vector3& operator*=(const DataType& other){X*=other;Y*=other;Z*=other;return *this;}

    /**
     * @brief Division operator
     * @param other Scalar to divide
     * @return This actualized vector
     */
    Vector3& operator/=(const DataType& other){X/=other;Y/=other;Z/=other;return *this;}

    /**
     * @brief Addition operator
     * @param other Other vector to add
     * @return Result vector
     */
    Vector3 operator+(const Vector3& other)const{Vector3 res{*this};res+=other;return res;}
    /**
     * @brief Sub operator
     * @param other Other vector to sub
     * @return Result vector
     */
    Vector3 operator-(const Vector3& other)const{Vector3 res{*this};res-=other;return res;}
    /**
     * @brief Product operator
     * @param other Scalar to multiply
     * @return Result vector
     */
    Vector3 operator*(const DataType& other)const{Vector3 res{*this};res*=other;return res;}
    /**
     * @brief Division operator
     * @param other Scalar to divide
     * @return Result vector
     */
    Vector3 operator/(const DataType& other)const{Vector3 res{*this};res/=other;return res;}

    /**
     * @brief Dot product
     * @param other Other vector to multiply
     * @return The dot product
     */
    DataType dot(const Vector3& other)const {return X*other.X+Y*other.Y+Z*other.Z;}
    /**
     * @brief Get the square length
     * @return Square length
     */
    DataType lengthSQ()const{return X*X+Y*Y+Z*Z;}
    /**
     * @brief Get the length
     * @return Length
     */
    DataType length()const{return static_cast<DataType>(std::sqrt(lengthSQ()));}

    /**
     * @brief Project this vector in 2D
     * @return Projected 2D vector
     */
    [[nodiscard]]Vector2<DataType> toVector2()const{return {X,Y};}
private:
    /// First coordinate
    DataType X{};
    /// Second coordinate
    DataType Y{};
    /// Third coordinate
    DataType Z{};
};

/**
 * @brief Serialize this objet to json
 * @param jso The json output
 * @param vect The vector to serialize
 */
template<class DataType>
inline void to_json(nlohmann::json& jso, const Vector3<DataType>& vect){
    jso = nlohmann::json{vect[0],vect[1],vect[2]};
}
/**
 * @brief Deserialize this object from json
 * @param jso Json source
 * @param vect Destination vector
 */
template<class DataType>
inline void from_json(const nlohmann::json& jso, Vector3<DataType>& vect){
    vect[0] = jso.at(0);
    vect[1] = jso.at(1);
    vect[2] = jso.at(2);
}

/// Shortcut for a vector of float
using Vect3f=Vector3<double>;
/// Shortcut for a vector of int
using Vect3i=Vector3<int32_t>;
/// Shortcut for a vector of unsigned char
using Vect3uis=Vector3<uint8_t>;

}// namespace rc::math
