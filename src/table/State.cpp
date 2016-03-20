#include <memory.h>

#include "State.h"

constexpr bool is_horizontal(Movement m) {
    switch (m) {
        case NORTH:
        case SOUTH:
            return false;
        case EAST:
        case WEST:
        default:
            return true;
    }
}

constexpr int32_t get_dx(Movement m) {
    if (!is_horizontal(m)) return 0;
    return EAST == m ? 1 : -1;
}

constexpr int32_t get_dy(Movement m) {
    if (is_horizontal(m)) return 0;
    return NORTH == m ? -1 : 1;
}

inline void move_dot(dot &d, int32_t dx, int32_t dy) {
    d.first += dx;
    d.second += dy;
}

State::State(int32_t size) : State(size, size) { }

State::State(int32_t w, int32_t h) : width(w), height(h) {
    field = new int32_t*[width];
    for (int i = 0; i < width; ++i) {
        field[i] = new int32_t[height];
        memset(field[i], NONE, height * sizeof(int32_t));
    }
}

State::~State() {
    for (int i = 0; i < width; ++i) {
        delete[] (field[i]);
    }
    delete[] field;
}

State::State(const State& st) : State(st.width, st.height) {
    balls = st.balls; 
    holes = st.holes;
    for (int i = 0; i < width; ++i) {
        memcpy(field[i], st.field[i], height * sizeof(int32_t));
    }
}

std::unique_ptr<State> State::move(Movement m) const {
    std::unique_ptr<State> st(new State(*this));

    for (auto &k: st->balls) {
        State::move(m, st, k.first);
    }

    return st;
}

void State::addBall(const dot& ball) {
    if (!check(ball)) throw "Tried to replace object by ball";

    field[ball.first][ball.second] |= BALL;
    balls[ball] = balls.size();
}

void State::addHole(const dot& hole) {
    if (!check(hole)) throw "Tried to replace object by hole";

    field[hole.first][hole.second] |= HOLE;
    holes[hole] = holes.size();
}

void State::addObstacle(const dot_pair& e) {
    auto x0 = e.first.first;
    auto y0 = e.first.second;
    auto x1 = e.second.first;
    auto y1 = e.second.second;

    setObstacle(x0, y0, x1, y1);
}

bool State::check(const dot& d) const {
    auto f = field[d.first][d.second];
    return BALL != f && HOLE != f;
}

void State::setObstacle(int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    bool isVert = (x0 == x1);
    // In case that given pair of dots are not near
    if (!isVert && y0 == y1) throw "Wrong obstacle";

    auto t1 = isVert ? OBST_DOWN : OBST_LEFT;
    auto t2 = isVert ? OBST_UP : OBST_RIGHT;

    bool upper = isVert ? (y0 > y1) : (x0 > x1);

    if (upper) {
        field[x0][y0] |= t2;
        field[x1][y1] |= t1;
    } else {
        field[x0][y0] |= t1;
        field[x1][y1] |= t2;
    }
}

void State::moveBall(const dot &old, const dot &n) {
    auto id = balls.at(old);
    balls.erase(old);
    balls.insert(std::make_pair(n, id));

    field[old.first][old.second] &= static_cast<unsigned>(-1) ^ BALL;
    field[n.first][n.second] |= BALL;
}

bool State::destroyPair(const dot &ball, const dot &hole) {
    if (balls[ball] != holes[hole]) return false;

    balls.erase(ball);
    holes.erase(hole);

    // checking the field?
    field[ball.first][ball.second] &= static_cast<unsigned>(-1) ^ BALL;
    field[hole.first][hole.second] &= static_cast<unsigned>(-1) ^ HOLE;

    return true;
}

bool State::atEdge(const dot &d, bool isVert) const {
    int32_t coord = isVert ? d.first : d.second;
    int32_t max = isVert ? width : height;

    return 0 == coord || coord == (max - 1);
}

bool State::isOut(const dot &d) const {
    return d.first < 0 || d.second < 0 || d.first >= width || d.second >= height;
}

void State::move(Movement m, std::unique_ptr<State> &st, const dot d) {
    const int32_t dx = get_dx(m);
    const int32_t dy = get_dy(m);

    dot final = d;

    // we don't need to check initial position
    do {
        move_dot(final, dx, dy);
        if (st->isOut(final)) return;

        const auto cell = st->field[final.first][final.second];
        
        if (BALL & cell) {
            move_dot(final, -dx, -dy);
            break;
        }

        if (HOLE & cell) {
            if (!st->destroyPair(d, final)) {
                st = std::unique_ptr<State>();
            }

            return;
        }

        // TODO: check for obstacles
    } while (!st->atEdge(final, is_horizontal(m)));

    st->moveBall(d, final);
}
