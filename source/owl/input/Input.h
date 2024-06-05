/**
 * @file Input.h
 * @author Silmaen
 * @date 06/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "KeyCodes.h"
#include "MouseCode.h"
#include "core/Core.h"
#include "window/Window.h"

namespace owl::input {
/**
 * @brief Class Input
 */
class OWL_API Input {
public:
    Input(const Input &) = delete;
    Input(Input &&) = delete;
    Input &operator=(const Input &) = delete;
    Input &operator=(Input &&) = delete;

    /**
     * @brief Default constructor.
     */
    Input() = default;

    /**
     * @brief Destructor.
     */
    virtual ~Input();

    /**
     * @brief Initialize the input;
     * @param[in] iType the type of input
     */
    static void init(const window::Type &iType = window::Type::GLFW);

    /**
     * @brief Destroy the inout instance.
     */
    static void invalidate();

    /**
     * @brief Keyboard pressed check.
     * @param[in] iKeycode The Key to check.
     * @return True if pressed.
     */
    static bool isKeyPressed(KeyCode iKeycode);

    /**
     * @brief Mouse button pressed check.
     * @param[in] iMouseCode Mouse button to check.
     * @return True if pressed.
     */
    static bool isMouseButtonPressed(MouseCode iMouseCode);

    /**
     * @brief Get mouse X position.
     * @return Mouse X Position.
     */
    static float getMouseX();

    /**
     * @brief Get mouse Y position.
     * @return Mouse Y Position.
     */
    static float getMouseY();

    /**
     * @brief Get mouse position.
     * @return Mouse Position.
     */
    static glm::vec2 getMousePos();

    /**
     * @brief Simulate key toggle.
     * @param[in] iKeycode the key to press/release.
     */
    static void injectKey(KeyCode iKeycode);

    /**
     * @brief Simulate mouse key toggle.
     * @param[in] iMouseCode the key to press/release.
     */
    static void injectMouseButton(MouseCode iMouseCode);

    /**
     * @brief Simulate mouse movement.
     * @param[in] iMousePos The new mouse pos.
     */
    static void injectMousePos(const glm::vec2 &iMousePos);

    /**
     * @brief Clear the previously defined injections.
     */
    static void resetInjection();

private:
    /// The Input type.
    static window::Type ms_type;
    /// The input instance.
    static uniq<Input> mus_instance;

    /**
     * @brief Keyboard pressed check, private implementation.
     * @param[in] iKeycode The Key to check.
     * @return True if pressed.
     */
    virtual bool isKeyPressed_impl(KeyCode iKeycode) = 0;
    /**
     * @brief Mouse button pressed check, private implementation.
     * @param[in] iMouseCode Mouse button to check.
     * @return True if pressed.
     */
    virtual bool isMouseButtonPressed_impl(MouseCode iMouseCode) = 0;
    /**
     * @brief Get mouse position, private implementation.
     * @return Mouse Position.
     */
    virtual glm::vec2 getMousePos_impl() = 0;
    /**
     * @brief Simulate key toggle, private implementation.
     * @param[in] iKeycode the key to press/release.
     */
    virtual void injectKey_impl(KeyCode iKeycode) = 0;

    /**
     * @brief Simulate mouse key toggle, private implementation.
     * @param[in] iMouseCode the key to press/release.
     */
    virtual void injectMouseButton_impl(MouseCode iMouseCode) = 0;
    /**
     * @brief Simulate mouse movement, private implementation.
     * @param[in] iMousePos The new mouse pos.
     */
    virtual void injectMousePos_impl(const glm::vec2 &iMousePos) = 0;

    /**
     * @brief Clear the previously defined injections.
     */
    virtual void resetInjection_impl() = 0;
};
}// namespace owl::input
