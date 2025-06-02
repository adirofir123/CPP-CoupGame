// adirofir123@gmail.com

#pragma once
#include <stdexcept>
#include <string>

/**
 * Thrown when a player attempts an illegal action
 * (e.g. not enough coins, targeting a non‐existent player, etc.).
 */
struct IllegalAction : std::runtime_error {
  explicit IllegalAction(const std::string& msg)
    : std::runtime_error("IllegalAction: " + msg) {}
};
