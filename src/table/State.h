#ifndef TEST_STATE_H
#define TEST_STATE_H

#include <vector>
#include <map>
#include <memory>
#include <stdint.h>

/*

x
^
|    N 
|    |
| W--+--E
|    |
|    S
+--------->y

*/

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

    const int32_t* const* getField() { return field; }
    int32_t w() { return width; }
    int32_t h() { return height; }
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
    // Move ball to new position
    void moveBall(const dot&, const dot&);
    // Remove pair ball&hole from hole if elements have same id
    // Return true if removed 
    bool destroyPair(const dot &ball, const dot &hole);
    // True when moved to the edge and edge not parallel to isVert
    bool atEdge(const dot&, bool isVert) const;
    // True when out of the field
    bool isOut(const dot&) const;

    // Perform movement of one object of the state
    // TODO: make movement by entire line
    static void move(Movement, std::unique_ptr<State>&, const dot);
};


#endif //TEST_STATE_H
