#include <iostream>

#include "Maze.hpp"
#include "Search.hpp"

Maze::Maze(const std::string& file)
{
    load(file);
}

int Maze::get(int x, int y) const
{
    return maze[x][y];
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

void Maze::clear_way()
{
    for (auto y = 0; y < size_y; y++)
    {
        for (auto x = 0; x < size_x; x++)
        {
            if (maze[x][y] == Type::Way)
            {
                maze[x][y] = Type::Visited;
            }
        }
    }
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

void Maze::print(const Search& search) const
{
    std::system("cls");

    for (auto y = 0; y < size_y; y++)
    {
        for (auto x = 0; x < size_x; x++)
        {
            if (search.is_start(Node(x, y)))
            {
                std::cout << 'S';
            }
            else if (search.is_end(Node(x, y)))
            {
                std::cout << 'E';
            }
            else if (search.is_path(Node(x, y)))
            {
                std::cout << static_cast<unsigned char>(178);
            }
            else if (search.is_opened(Node(x, y)))
            {
                std::cout << static_cast<unsigned char>(177);
            }
            else if (search.is_closed(Node(x, y)))
            {
                std::cout << static_cast<unsigned char>(177);
            }
            else
            {
                switch (maze[x][y])
                {
                    case Type::Empty:
                        std::cout << ' ';
                        break;

                    case Type::Wall:
                        std::cout << static_cast<unsigned char>(219);
                        break;
                }
            }
        }

        std::cout << '\n';
    }

    std::cout.flush();
}
