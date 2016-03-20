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

enum Cell {
    NONE = 0,
    HOLE = 1,
    BALL = 2,
    OBST_LEFT = 4,
    OBST_RIGHT = 8,
    OBST_UP = 16,
    OBST_DOWN = 32
};

typedef std::pair<int32_t, int32_t> dot;
typedef std::pair<dot, dot> dot_pair;
typedef int32_t** matrix;

class State {
public:
    State(int32_t size);
    State(int32_t w, int32_t h);
    ~State();
    explicit State(const State&);

    // Perform movement of the state
    std::unique_ptr<State> move(Movement) const;

    void addBall(const dot&);
    void addHole(const dot&);
    void addObstacle(const dot_pair&);
private:
    matrix field;

    // Balls position -> ID
    std::map<dot, int> balls;
    // Holes position -> ID
    std::map<dot, int> holes;

    const int32_t width, height;

    // Check the dot business on the field 
    bool check(const dot&) const;
    // Set the obstacle by given pair of dots
    void setObstacle(int32_t, int32_t, int32_t, int32_t);

    // Perform movement of one object of the state
    static void move(Movement, std::unique_ptr<State>&, const dot&);
};


#endif //TEST_STATE_H
