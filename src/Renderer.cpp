#include <GLFW/glfw3.h>

#include "Renderer.hpp"

Renderer::Renderer(int width, int height, const std::string& title)
{
    glfwInit();
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(window);
}

Renderer::~Renderer()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Renderer::is_open() const
{
    return !glfwWindowShouldClose(window);
}

void Renderer::poll_events() const
{
    glfwPollEvents();
}

void Renderer::render(const Maze& maze, const Search& search) const
{
    const int size_x = maze.get_size_x();
    const int size_y = maze.get_size_y();

    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, size_x, size_y, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for (int y = 0; y < size_y; y++)
    {
        for (int x = 0; x < size_x; x++)
        {
            const Node node(x, y);

            if (search.is_start(node))
                glColor3f(0.0f, 0.8f, 0.0f);
            else if (search.is_end(node))
                glColor3f(0.8f, 0.0f, 0.0f);
            else if (search.is_path(node))
                glColor3f(1.0f, 0.85f, 0.0f);
            else if (search.is_opened(node))
                glColor3f(0.4f, 0.65f, 1.0f);
            else if (search.is_closed(node))
                glColor3f(0.2f, 0.35f, 0.65f);
            else if (maze.get(x, y) == Maze::Type::Wall)
                glColor3f(0.15f, 0.15f, 0.15f);
            else
                glColor3f(0.9f, 0.9f, 0.9f);

            glBegin(GL_QUADS);
            glVertex2f(static_cast<float>(x),     static_cast<float>(y));
            glVertex2f(static_cast<float>(x + 1), static_cast<float>(y));
            glVertex2f(static_cast<float>(x + 1), static_cast<float>(y + 1));
            glVertex2f(static_cast<float>(x),     static_cast<float>(y + 1));
            glEnd();
        }
    }

    glfwSwapBuffers(window);
}
