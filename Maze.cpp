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
    std::ifstream file(file_name, std::ios::binary | std::ios::ate);
    auto file_size = file.tellg();
    file.seekg(0, std::ifstream::beg);
    std::vector<char> data(file_size);
    file.read(data.data(), file_size);
    file.close();

    size_x = 0;
    size_y = 0;

    for (auto i = 0; i < file_size; i++)
    {
        if (data[i] == '\n')
        {
            if (size_y == 0)
            {
                size_x = i;
            }

            size_y += 1;
        }
    }

    maze.resize(size_x, std::vector<int>(size_y));

    auto index = 0;

    for (auto y = 0; y < size_y; y++)
    {
        for (auto x = 0; x < size_x; x++)
        {
            maze[x][y] = data[index++];
        }

        index += 1;
    }

    for (auto y = 0; y < size_y; y++)
    {
        for (auto x = 0; x < size_x; x++)
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
            }
        }
    }
}
