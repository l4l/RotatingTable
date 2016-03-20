#include "State.h"

State::State(int32_t size) : State(size, size) { }

State::State(int32_t w, int32_t h) : width(w), height(h) {
    field = new int32_t*[width];
    for (int i = 0; i < width; ++i) {
        field[i] = new int32_t[height];
    }
}

State::~State() {
    for (int i = 0; i < width; ++i) {
        delete[] (field[i]);
    }
    delete[] field;
}

// Just copy everything
State::State(const State& st) : State(st.width, st.height) {
    balls = st.balls; 
    holes = st.holes;
}

std::unique_ptr<State> State::move(Movement m) const {
    std::unique_ptr<State> st(new State(*this));
    std::vector<dot_pair> transfers;

    for (auto &k: st->balls) {
        State::move(m, st, k.first);
    }

    return st;
}

void State::addBall(const dot& ball) {
    check(ball);
    field[ball.first][ball.second] |= BALL;

    balls[ball] = balls.size();
}

void State::addHole(const dot& hole) {
    check(hole);
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

void State::check(const dot& d) const {
    auto f = field[d.first][d.second];
    if (BALL == f || HOLE == f)
        throw "Tried to replace object";
}

void State::setObstacle(int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    bool isVert = (x0 == x1);
    if (!isVert) isVert = (y0 == y1);

    if (!isVert) throw "Wrong obstacle";
    
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

constexpr bool is_horizontal(Movement m) {
    switch (m) {
        case NORTH:
        case SOUTH:
            return false;
        case EAST:
        case WEST:
            return true;
    }
}

constexpr int32_t get_dx(Movement m) {
    if (!is_horizontal(m)) return 0;
    return EAST == m ? 1 : -1;
}

constexpr int32_t get_dy(Movement m) {
    if (!is_horizontal(m)) return 0;
    return NORTH == m ? 1 : -1;
}

void State::move(Movement m, std::unique_ptr<State> &st, const dot& d) {
    const int32_t dx = get_dx(m);
    const int32_t dy = get_dy(m);

    dot final = d;


}