#include <chrono>
#include <thread>

#include "AStar.hpp"
#include "Maze.hpp"
#include "Renderer.hpp"
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

    constexpr int cell_size = 20;
    Renderer renderer(maze.get_size_x() * cell_size, maze.get_size_y() * cell_size, "Maze Solver");

    search.add_opened(search.get_start());

    enum class Phase { Searching, TracingPath, Done };
    Phase phase = Phase::Searching;
    Node* path_node = nullptr;

    while (renderer.is_open())
    {
        renderer.poll_events();

        if (phase == Phase::Searching)
        {
            const auto state = astar.a_star_step(maze, search);

            if (state == AStar::SearchState::Found)
            {
                path_node = search.opened.front().get();
                phase = Phase::TracingPath;
            }
            else if (state == AStar::SearchState::NotFound)
            {
                phase = Phase::Done;
            }
        }
        else if (phase == Phase::TracingPath)
        {
            if (search.is_start(*path_node) == false)
            {
                search.add_path(path_node);
                path_node = path_node->parent;
            }
            else
            {
                phase = Phase::Done;
            }
        }

        renderer.render(maze, search);

        if (phase != Phase::Done)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    return 0;
}
