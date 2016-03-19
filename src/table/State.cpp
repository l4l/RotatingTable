#include "State.h"

State::State(int32_t size) : width(size), height(size) { }

// Just copy everything
State::State(const State& st) : 
        width(st.width),
        height(st.height), 
        ballsAndHoles(st.ballsAndHoles), 
        horObstacles(st.horObstacles), 
        vertObstacles(st.vertObstacles) { }

std::unique_ptr<State> State::move(Movement m) const {
    std::unique_ptr<State> st(new State(*this));

    return st;
}

void State::addPair(dot ball, dot hole) {
    ballsAndHoles[ballsAndHoles.size()] = std::make_pair(ball, hole);
}

void State::addObstacle(dot_pair e) {
    if (e.first.first == e.second.first) {
        horObstacles.push_back(e);
        return;
    }

    if (e.first.second == e.second.second) {
        vertObstacles.push_back(e);
        return;
    }

    throw "Wrong obstacle";
}
