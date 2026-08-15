#include "AStar.hpp"
#include "Search.hpp"

AStar::SearchState AStar::a_star_step(Maze& maze, Search& search) const
{
    if (search.opened.empty())
    {
        return SearchState::NotFound;
    }

    if (search.is_end(*search.opened.front()))
    {
        return SearchState::Found;
    }

    auto node = search.opened.front().get();

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
                        next_node.distance_from_start = node->distance_from_start + 1;
                        next_node.distance_to_end = get_distance(search.get_end(), next_node);
                        next_node.parent = node;

                        search.add_opened(next_node);
                    }
                }
            }
        }
    }

    return SearchState::Searching;
}

bool AStar::a_star(Maze& maze, Search& search) const
{
    search.add_opened(search.get_start());

    SearchState state;

    do
    {
        state = a_star_step(maze, search);
    }
    while (state == SearchState::Searching);

    return state == SearchState::Found;
}

int AStar::get_distance(const Node& a, const Node& b) const
{
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

const std::vector<Node>& AStar::get_indices() const
{
    static const std::vector<Node> indices =
    {
        Node(-1, -1),
        Node(-1,  0),
        Node(-1,  1),
        Node(0, -1),
        Node(0,  1),
        Node(1, -1),
        Node(1,  0),
        Node(1,  1)
    };

    return indices;
}
