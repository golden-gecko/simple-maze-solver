#pragma once

class Node
{
public:
    int x = 0;
    int y = 0;

    int distance_from_start = 0;
    int distance_to_end = 0;

    Node* parent = nullptr;

    Node(int x = 0, int y = 0);
};
