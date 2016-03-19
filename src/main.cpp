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

int main() {
    
}
