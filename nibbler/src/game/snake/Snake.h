#pragma once

#include <iostream>
#include <vector>

#include "game/snake/exception.h"
#include "plugin/IPlugin.h"
#include "spdlog/spdlog.h"

namespace game::snake {
    using interface::ARGB;
    static_assert(sizeof(ARGB) == 4, "ARGB must have 4 bytes");

    constexpr ARGB BLACK = 0x00000000;
    constexpr ARGB WHITE = 0x00FFFFFF;
    constexpr ARGB RED = 0x00FF0000;
    constexpr ARGB YELLOW = 0x00FCBA03;
    constexpr ARGB GREEN = 0x000000FF;

    enum class Orientation { NORTH, SOUTH, EAST, WEST };
    enum class Entity : ARGB {
        Background = BLACK,
        Wall = WHITE,
        Head = RED,
        Body = YELLOW,
        Food = GREEN,
    };

    template <class T>
    class Matrix {
       public:
        Matrix(size_t x, size_t y, T value = T()) : width(x), height(y), _buffer(x * y, value) {
            SPDLOG_DEBUG("width {}, height {}, size {}", width, height, _buffer.size());
        }

        T &operator()(size_t x, size_t y) { return _buffer[x + (y * width)]; }
        T const &operator()(size_t x, size_t y) const { return _buffer[x + (y * width)]; }

        std::vector<T>::iterator begin() { return _buffer.begin(); }
        //        std::vector<T>::const_iterator cbegin() { return _buffer.cbegin(); }
        std::vector<T>::iterator end() { return _buffer.end(); }
        //        std::vector<T>::const_iterator cend() { return _buffer.cend(); }

       public:
        const size_t width;
        const size_t height;

       private:
        std::vector<T> _buffer;
    };

    class Player {
       public:
        explicit Player(Matrix<Entity> &matrix);
        void spawn();
        void move(Orientation o);

       private:
        std::vector<Entity *> _body{};
        std::pair<size_t, size_t> _head_pos;
        Matrix<Entity> &_m;
    };

    class Snake {
       public:
        Snake(size_t x, size_t y);

       private:
       public:
        void _debug() const;

        Matrix<Entity> _matrix;
        Player _player;
    };
}  // namespace game::snake
