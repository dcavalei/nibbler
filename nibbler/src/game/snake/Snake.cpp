#include "Snake.h"

#include <random>
#include <sstream>

#include "literals.h"

namespace game::snake {
    namespace {
        void init_map(Matrix<Entity> &m) {
            const size_t lt_edge = 0;
            const size_t rt_edge = m.height - 1;
            const size_t top_edge = 0;
            const size_t bot_edge = m.width - 1;

            for (size_t x = 0; x < m.width; ++x) {
                for (size_t y = 0; y < m.height; ++y) {
                    if (y == lt_edge || y == rt_edge || x == top_edge || x == bot_edge) {
                        m(x, y) = snake::Entity::Wall;
                    } else {
                        m(x, y) = snake::Entity::Background;
                    }
                }
            }
        }

        template <typename T>
        T random(T range_from, T range_to) {
            std::random_device rand_dev;
            std::mt19937 generator(rand_dev());
            std::uniform_int_distribution<T> distr(range_from, range_to);
            return distr(generator);
        }

        Entity &find_empty_entity(Matrix<Entity> &m) {
            std::vector<Entity *> bg_entities;
            bg_entities.reserve(m.height * m.height);

            for (auto &e : m) {
                if (e == Entity::Background) {
                    bg_entities.push_back(&e);
                }
            }

            if (bg_entities.empty()) {
                throw exception::SnakeNoEmptySpaceException();
            }
            return *bg_entities[random<size_t>(0, bg_entities.size())];
        }

        void spawn_food(Matrix<Entity> &m) { find_empty_entity(m) = Entity::Food; }

    }  // namespace

    Player::Player(Matrix<Entity> &matrix) : _m(matrix) { _body.reserve(literals::PLAYER_INITIAL_SIZE - 1); }

    void Player::spawn() {
        if (_m.width < literals::BOARD_MINIMUM_WIDTH || _m.height < literals::BOARD_MINIMUM_HEIGHT) {
            throw exception::SnakeSmallMatrixException();
        }

        auto x = _m.width / 2;
        auto y = _m.height / 2;

        _head_pos = std::make_pair(x, y);
        _m(x, y) = Entity::Head;

        for (size_t i = 1; i < literals::PLAYER_INITIAL_SIZE; i++) {
            _m(x, y - i) = Entity::Body;
            _body.push_back(&_m(x, y - i));
        }
    }
    void Player::move(Orientation o) {
        decltype(_head_pos) next_pos;

        const auto [curr_x, curr_y] = _head_pos;
        switch (o) {
            case Orientation::NORTH: next_pos = std::make_pair(curr_x, curr_y - 1); break;
            case Orientation::SOUTH: next_pos = std::make_pair(curr_x, curr_y + 1); break;
            case Orientation::EAST: next_pos = std::make_pair(curr_x + 1, curr_y); break;
            case Orientation::WEST: next_pos = std::make_pair(curr_x - 1, curr_y); break;
        }

        const auto [next_x, next_y] = next_pos;
        auto move_head = [&, this] {
            _m(curr_x, curr_y) = Entity::Body;
            _m(next_x, next_y) = Entity::Head;
            _body.insert(_body.begin(), &_m(curr_x, curr_y));
            _head_pos = next_pos;
        };

        auto pop_tail = [&, this] {
            *_body.back() = Entity::Background;
            _body.pop_back();
        };

        switch (_m(next_x, next_y)) {
            case Entity::Background:
                pop_tail();
                move_head();
                break;
            case Entity::Food:
                move_head();
                spawn_food(_m);
                break;
            case Entity::Wall: [[fallthrough]];
            case Entity::Head: [[fallthrough]];
            case Entity::Body:
                pop_tail();
                move_head();
                throw exception::PlayerHitException();
        }
    }

    void Snake::_debug() const {
        std::stringstream ss;
        for (size_t y = 0; y < _matrix.height; ++y) {
            for (size_t x = 0; x < _matrix.width; ++x) {
                Entity item = _matrix(x, y);
                switch (item) {
                    case Entity::Background: ss << "  "; break;
                    case Entity::Wall: ss << "▓ "; break;
                    case Entity::Head: ss << "@ "; break;
                    case Entity::Body: ss << "* "; break;
                    case Entity::Food: ss << ". "; break;
                }
            }
            ss << '\n';
        }
        std::cout << ss.str();
    }

    Snake::Snake(size_t x, size_t y) : _matrix(x, y, Entity::Background), _player(_matrix) {
        if (x < literals::BOARD_MINIMUM_WIDTH || y < literals::BOARD_MINIMUM_HEIGHT) {
            throw exception::SnakeSmallMatrixException();
        }

        init_map(_matrix);
        _player.spawn();
        spawn_food(_matrix);

        _debug();
    }

}  // namespace game::snake
