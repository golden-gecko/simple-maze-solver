#include <fstream>

#include "Maze.hpp"

Maze::Maze(const std::string& file)
{
    load(file);
}

int Maze::get(int x, int y) const
{
    return maze[x][y];
}

int Maze::get_size_x() const
{
    return size_x;
}

int Maze::get_size_y() const
{
    return size_y;
}

Node Maze::get_start() const
{
    return start;
}

Node Maze::get_end() const
{
    return end;
}

bool Maze::is_accessible(int x, int y) const
{
    return get(x, y) == Type::Empty;
}

bool Maze::is_valid(int x, int y) const
{
    if (x < 0)
    {
        return false;
    }

    if (x > size_x - 1)
    {
        return false;
    }

    if (y < 0)
    {
        return false;
    }

    if (y > size_y - 1)
    {
        return false;
    }

    return true;
}
void Maze::load(const std::string& file_name)
{
    std::ifstream file(file_name, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Failed to open file: " + file_name);
    }

    // Read entire file into a string without precomputing size
    std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    if (data.empty())
    {
        throw std::runtime_error("Empty or unreadable file: " + file_name);
    }

    // Normalize CRLF -> LF
    data.erase(std::remove(data.begin(), data.end(), '\r'), data.end());

    size_x = 0;
    size_y = 0;
    const std::size_t file_size = data.size();

    for (std::size_t i = 0; i < file_size; ++i)
    {
        if (data[i] == '\n')
        {
            if (size_y == 0)
            {
                size_x = static_cast<int>(i);
            }

            ++size_y;
        }
    }

    maze.assign(size_x, std::vector<int>(size_y));

    std::size_t index = 0;
    for (int y = 0; y < size_y; ++y)
    {
        for (int x = 0; x < size_x; ++x)
        {
            if (index >= file_size)
            {
                throw std::runtime_error("Unexpected end of file while parsing: " + file_name);
            }
            maze[x][y] = data[index++];
        }

        // Skip newline separator if present
        if (index < file_size && data[index] == '\n') ++index;
    }

    for (int y = 0; y < size_y; ++y)
    {
        for (int x = 0; x < size_x; ++x)
        {
            switch (maze[x][y])
            {
                case ' ':
                    maze[x][y] = Type::Empty;
                    break;

                case '#':
                    maze[x][y] = Type::Wall;
                    break;

                case 'S':
                    maze[x][y] = Type::Empty;
                    start.x = x;
                    start.y = y;
                    break;

                case 'E':
                    maze[x][y] = Type::Empty;
                    end.x = x;
                    end.y = y;
                    break;
                default:
                    break;
            }
        }
    }
}
