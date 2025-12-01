#include <iostream>
#include <vector>

#include "Maze.hpp"
#include "Search.hpp"

static int get_distance(const Node& a, const Node& b)
{
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

static std::vector<Node> get_indices()
{
    static const std::vector<Node> indices =
    {
        Node(-1, -1),
        Node(-1,  0),
        Node(-1,  1),
        Node( 0, -1),
        Node( 0,  1),
        Node( 1, -1),
        Node( 1,  0),
        Node( 1,  1)
    };

    return indices;
}

static bool a_star_path(Maze& maze, Search& search)
{
    auto node = search.opened.front();

    while (search.is_start(*node) == false)
    {
        search.add_path(node);

        maze.print(search);

        node = node->parent;
    }

    return true;
}

static bool a_star(Maze& maze, Search& search)
{
    search.add_opened(search.get_start());

    while (search.opened.size())
    {
        if (search.is_end(*search.opened.front()))
        {
            return true;
        }

        auto node = search.opened.front();

        search.set_current_as_visited();

        for (const auto& i : get_indices())
        {
            auto next_node = Node(node->x + i.x, node->y + i.y);

            if (maze.is_valid(next_node.x, next_node.y))
            {
                if (search.is_closed(next_node) == false)
                {
                    if (search.is_opened(next_node) == false)
                    {
                        if (maze.is_accessible(next_node.x, next_node.y))
                        {
                            next_node.distance_from_start = get_distance(search.get_start(), next_node);
                            next_node.distance_to_end = get_distance(search.get_end(), next_node);
                            next_node.parent = node;

                            search.add_opened(next_node);
                        }
                    }
                }
            }
        }

        maze.print(search);
    }

    return false;
}

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

    if (a_star(maze, search))
    {
        a_star_path(maze, search);
    }

    std::cin.ignore();

    return 0;
}
