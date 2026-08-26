#pragma once

#include <string>

#include "Maze.hpp"
#include "Search.hpp"

struct GLFWwindow;

class Renderer
{
public:
    Renderer(int width, int height, const std::string& title);
    ~Renderer();

    bool is_open() const;
    void poll_events() const;
    void render(const Maze& maze, const Search& search) const;

private:
    GLFWwindow* window = nullptr;
};
