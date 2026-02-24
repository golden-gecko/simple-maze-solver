#pragma once

#include <list>

class Node
{
public:
    int x = 0;
    int y = 0;

    int distance_from_start = 0;
    int distance_to_end = 0;

    int type = 0;

    Node* parent = nullptr;
    std::list<Node*> children;

    Node(int x = 0, int y = 0);
};
