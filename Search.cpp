#include <algorithm>

#include "Search.hpp"

Search::Search(const Node& start, const Node& end) : start(start), end(end)
{
}

Search::~Search()
{
    for (const auto node : opened)
    {
        delete node;
    }

    for (const auto node : closed)
    {
        delete node;
    }
}

Node Search::get_start() const
{
    return start;
}

Node Search::get_end() const
{
    return end;
}

void Search::set_current_as_visited()
{
    closed.emplace_back(opened.front());
    opened.pop_front();
}

bool Search::is_opened(const Node& node) const
{
    auto result = std::find_if(opened.begin(), opened.end(), [node](Node* i) {
        return node.x == i->x && node.y == i->y;
    });

    return result != opened.end();
}

bool Search::is_closed(const Node& node) const
{
    auto result = std::find_if(closed.begin(), closed.end(), [node](Node* i) {
        return node.x == i->x && node.y == i->y;
    });

    return result != closed.end();
}

bool Search::is_path(const Node& node) const
{
    auto result = std::find_if(path.begin(), path.end(), [node](Node* i) {
        return node.x == i->x && node.y == i->y;
    });

    return result != path.end();
}

bool Search::is_start(const Node& node) const
{
    return start.x == node.x && start.y == node.y;
}

bool Search::is_end(const Node& node) const
{
    return end.x == node.x && end.y == node.y;
}

void Search::add_opened(const Node& node)
{
    opened.push_back(new Node(node));

    sort();
}

void Search::add_path(Node* node)
{
    path.push_back(node);
}

void Search::sort()
{
    std::sort(opened.begin(), opened.end(), [](Node* a, Node* b) {
        return a->distance_to_end < b->distance_to_end;
    });
}
