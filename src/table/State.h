#ifndef TEST_STATE_H
#define TEST_STATE_H

#include <vector>
#include <map>
#include <memory>
#include <stdint.h>

enum Movement {
    NORTH,
    EAST,
    WEST,
    SOUTH
};

typedef std::pair<int32_t, int32_t> dot;
typedef std::pair<dot, dot> dot_pair;

class State {
public:
    State(int32_t size);
    explicit State(const State&);

    // Perform rotation of the table
    std::unique_ptr<State> move(Movement) const;

    void addPair(dot ball, dot hole);
    void addObstacle(dot_pair);
private:

    // Index -> pair of dots, where first points to the ball, second to the hole
    std::map<int, dot_pair> ballsAndHoles;
    
    std::vector<dot_pair> horObstacles;
    std::vector<dot_pair> vertObstacles;

    const int32_t width, height;
};


#endif //TEST_STATE_H
