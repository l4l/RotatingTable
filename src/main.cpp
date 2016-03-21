#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <queue>

#include "State.h"

constexpr const char* to_string(Movement m) {
    switch (m) {
        case NORTH: return "N";
        case EAST: return "E";
        case WEST: return "W";
        case SOUTH: return "S";
    }
    return nullptr;
}

void print(const State &st) {
    for (int i = 0; i < st.w(); i++) {
        for (int j = 0; j < st.h(); j++) {
            printf("%5d", st.getField()[j][i]);
        }
        puts("\n");
    }
}

typedef std::vector<std::string> strings;
typedef std::pair<std::string, sptr> state;
typedef std::vector<state> states;

void bfs(const State &st, states &result) {
    std::queue<state> queue;
    static const Movement m[] = {NORTH, EAST, WEST, SOUTH};
    sptr ptr(new State(st));

    state s = std::make_pair("", std::move(ptr));

    int maxDepth = 12345;

    do {
        if (!s.second->isEmpty()) break;

        for (int i = 0; i < sizeof(Movement); ++i) {
            queue.push(std::make_pair(s.first + to_string(m[i]), s.second->move(m[i])));
        }

        s = std::move(queue.front());
        queue.pop();
    } while (queue.size() && maxDepth--);


    result.push_back(std::move(s));
    const auto len = s.first.size();

    while (queue.size()) {
        auto v = std::move(queue.front());
        queue.pop();

        if (len == v.first.size()) {
            result.push_back(std::move(s));
        } else {
            break;
        }
    }
}

int main() {
    int32_t n, k, m;
    std::cin>>n>>k>>m;
    State st(n);

    for (int i = 0; i < k; ++i) {
        int32_t a, b;
        std::cin>>a>>b;
        st.addBall(std::make_pair(a, b));
    }

    for (int i = 0; i < k; ++i) {
        int32_t a, b;
        std::cin>>a>>b;
        st.addHole(std::make_pair(a, b));
    }

    for (int i = 0; i < m; ++i) {
        int32_t a, b, c, d;
        std::cin>>a>>b>>c>>d;
        st.addObstacle(std::make_pair(std::make_pair(a, b), std::make_pair(c, d)));
    }

    // print(st);

    states s;
    bfs(st, s);

    for (auto i = s.begin(); i != s.end(); ++i) {
        // print(*i->second);
        std::cout<<i->first;
    }
}
