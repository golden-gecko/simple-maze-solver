#pragma once

#include "Maze.hpp"
#include "Search.hpp"

class AStar
{
public:
    enum class SearchState { Searching, Found, NotFound };

    bool a_star(Maze& maze, Search& search) const;
    SearchState a_star_step(Maze& maze, Search& search) const;

private:
    int get_distance(const Node& a, const Node& b) const;
    const std::vector<Node>& get_indices() const;
};
