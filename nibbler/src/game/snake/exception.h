#pragma once

#include <exception>

namespace game::snake::exception {
    struct SnakeSmallMatrixException : public std::exception {
        [[nodiscard]] const char *what() const noexcept final { return "Snake Game size must be at least 10x10"; }
    };

    struct SnakeNoEmptySpaceException : public std::exception {
        [[nodiscard]] const char *what() const noexcept final { return "Snake Game has no empty space left!"; }
    };
    struct PlayerHitException : public std::exception {
        [[nodiscard]] const char *what() const noexcept final { return "Player hit something!"; }
    };

}  // namespace game::snake::exception
