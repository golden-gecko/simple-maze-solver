#pragma once

#include "Node.hpp"
#include "Maze.hpp"

class AStar
{
public:
    bool a_star_path(Maze& maze, Search& search) const;
    bool a_star(Maze& maze, Search& search) const;

private:
    int get_distance(const Node& a, const Node& b) const;
    const std::vector<Node>& get_indices() const;
};
