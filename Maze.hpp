#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Node.hpp"

class Search;

class Maze
{
public:
    enum Type
    {
        Empty = -1,
        Wall = -2,
        Start = -3,
        End = -4,
        Way = -5,
        Visited = -6
    };

    Maze(const std::string& file);

    int get(int x, int y) const;
    Node get_start() const;
    Node get_end() const;

    bool is_accessible(int x, int y) const;
    bool is_valid(int x, int y) const;

    void clear_way();
    void load(const std::string& fileName);
    void print(const Search& search) const;

private:
    int size_x = 0;
    int size_y = 0;

    std::vector<std::vector<int>> maze;

    Node start;
    Node end;
};
