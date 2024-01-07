/**
 * @file WPointer.h
 * @author Silmaen
 * @date 29/12/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once

namespace owl::input::video::windows {

/**
 * @brief Class WPointer
 *
 * Simple class to handle windows object pointers like a unique_ptr
 */
template<class PTR>
class WPointer {
public:
	/**
	 * @brief Constructor.
	 */
	WPointer() = default;

	/**
	 * @brief Constructor with a copy.
	 * @param other Raw pointer to affect.
	 * @param own If the WPoint should take the ownership of the memory.
	 */
	WPointer(PTR *other, bool own = false) : owner{own}, object{other} {}

	/**
	 * @brief Destructor.
	 */
	virtual ~WPointer() {
		if (owner) release();
	}

	WPointer(const WPointer &) = default;
	WPointer(WPointer &&) = default;
	WPointer &operator=(const WPointer &) = default;
	WPointer &operator=(WPointer &&) = default;

	/**
	 * @brief Overload of the -> operator for using thi object as if it is the pointer.
	 * @return The backend pointer.
	 */
	PTR *operator->() { return object; }

	/**
	 * @brief Explicit type casting to bool.
	 * @return True if pointer is non-void.
	 */
	operator bool() const { return object != nullptr; }

	/**
	 * @brief Direct access to the pointer.
	 * @return The raw pointer.
	 */
	PTR *get() { return object; }
	/**
	 * @brief Direct access to the address of the pointer.
	 * @return The address of pointer.
	 */
	PTR **addr() { return (&object); }

	/**
	 * @brief Direct access to the address of the pointer with type change.
	 * @tparam ALIAS The new type.
	 * @return The raw pointer with the requested type.
	 */
	template<class ALIAS>
	ALIAS *getAs() { return reinterpret_cast<ALIAS *>(object); }

	/**
	 * @brief Direct access to the address of the pointer with type change.
	 * @tparam ALIAS The new type.
	 * @return The address of pointer with the requested type.
	 */
	template<class ALIAS>
	ALIAS **addrAs() { return reinterpret_cast<ALIAS **>(&object); }

	/**
	 * @brief Request memory release.
	 */
	void release() {
		if (object) object->Release();
		object = nullptr;
	}

	/**
	 * @brief This object takes the memory ownership
	 */
	void takeOwnership() { owner = true; }


	/**
	 * @brief This object takes the memory ownership
	 * @param other The other object to take ownership from.
	 */
	void takeOwnershipFrom(WPointer &other) {
		object = other.object;
		if (other.isMemoryOwned()) {
			other.leaveOwnership();
			takeOwnership();
		} else {
			leaveOwnership();
		}
	}

	/**
	 * @brief This object leave the memory ownership
	 */
	void leaveOwnership() { owner = false; }
	/**
	 * @brief Check if the object has memory ownership.
	 * @return True if this object has memory ownership.
	 */
	[[nodiscard]] bool isMemoryOwned() const { return owner; }
	/**
	 * @brief Defines the ownership of this object on backend object.
	 * @param own If the object should be owned.
	 */
	void setOwner(bool own) { owner = own; }

private:
	/// Ownership of this objet on the memory behind the pointer.
	bool owner = true;
	/// Pointer to the object.
	PTR *object = nullptr;
};

}// namespace owl::input::video::windows
