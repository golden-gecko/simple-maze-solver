#pragma once

#include <list>

class Node
{
public:
    int x;
    int y;

    int distance_from_start;
    int distance_to_end;

    int type;

    Node* parent;
    std::list<Node*> children;

    Node(int x = 0, int y = 0);
};
