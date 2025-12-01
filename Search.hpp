#pragma once

#include <deque>

#include "Node.hpp"

class Search
{
public:
    std::deque<Node*> opened;
    std::deque<Node*> closed;
    std::deque<Node*> path;

    Search(const Node& start, const Node& end);
    ~Search();

    Node get_start() const;
    Node get_end() const;

    void set_current_as_visited();

    bool is_opened(const Node& node) const;
    bool is_closed(const Node& node) const;
    bool is_path(const Node& node) const;
    bool is_start(const Node& node) const;
    bool is_end(const Node& node) const;

    void add_opened(const Node& node);
    void add_path(Node* node);

private:
    const Node start;
    const Node end;

    void sort();
};
