#include <iostream>

#include "AStar.hpp"
#include "Search.hpp"

int main()
{
    const std::string files[] =
    {
        "Maze.txt",
        "Maze_Closed.txt",
        "Maze_Empty.txt",
        "Maze_Room.txt",
        "Maze_Wall.txt"
    };

    Maze maze(files[4]);
    Search search(maze.get_start(), maze.get_end());
    AStar astar;

    if (astar.a_star(maze, search))
    {
        astar.a_star_path(maze, search);
    }

    std::cin.ignore();

    return 0;
}
