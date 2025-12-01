#include "Node.hpp"

Node::Node(int x, int y) :
    x(x), y(y), type(0), distance_from_start(0), distance_to_end(0), parent(nullptr)
{
}
