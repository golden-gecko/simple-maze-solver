#include <algorithm>
#include <cmath>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <vector>

class Node
{
public:
    int x;
    int y;

    int distance_from_start;
    int distance_to_end;

    int type;

    Node* parent;
    std::list<Node*> children;

    Node(int x = 0, int y = 0) :
        x(x), y(y), distance_from_start(0), distance_to_end(0), parent(nullptr)
    {
    }
};

class Search
{
public:
    std::deque<Node*> opened;
    std::deque<Node*> closed;
    std::deque<Node*> path;

    Search(const Node& start, const Node& end) : start(start), end(end)
    {
    }

    ~Search()
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

    Node get_start() const
    {
        return start;
    }

    Node get_end() const
    {
        return end;
    }

    void set_current_as_visited()
    {
        closed.emplace_back(opened.front());
        opened.pop_front();
    }

    bool is_opened(const Node& node) const
    {
        auto result = std::find_if(opened.begin(), opened.end(), [node](Node* i) {
            return node.x == i->x && node.y == i->y;
        });

        return result != opened.end();
    }

    bool is_closed(const Node& node) const
    {
        auto result = std::find_if(closed.begin(), closed.end(), [node](Node* i) {
            return node.x == i->x && node.y == i->y;
        });

        return result != closed.end();
    }

    bool is_path(const Node& node) const
    {
        auto result = std::find_if(path.begin(), path.end(), [node](Node* i) {
            return node.x == i->x && node.y == i->y;
        });

        return result != path.end();
    }

    bool is_start(const Node& node) const
    {
        return start.x == node.x && start.y == node.y;
    }

    bool is_end(const Node& node) const
    {
        return end.x == node.x && end.y == node.y;
    }

    void add_opened(const Node& node)
    {
        opened.push_back(new Node(node));

        sort();
    }

    void add_path(Node* node)
    {
        path.push_back(node);
    }
private:
    const Node start;
    const Node end;

    void sort()
    {
        std::sort(opened.begin(), opened.end(), [](Node* a, Node* b) {
            return a->distance_to_end < b->distance_to_end;
        });
    }
};

class Maze
{
public:
    enum Type
    {
        Empty = -1,
        Wall = -2,
        Start = -3,
        End = -4,
        Way = -5,
        Visited = -6
    };

    Maze(const std::string file)
    {
        load(file);
    }

    int get(int x, int y) const
    {
        return maze[x][y];
    }

    Node get_start() const
    {
        return start;
    }

    Node get_end() const
    {
        return end;
    }

    bool is_accessible(int x, int y) const
    {
        return get(x, y) == Type::Empty;
    }

    bool is_accessible(int x_1, int y_1, int x_2, int y_2) const
    {
        return false;
    }

    bool is_valid(int x, int y)
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

    void clear_way()
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

    void load(const std::string& fileName)
    {
        std::ifstream file(fileName, std::ios::binary | std::ios::ate);
        auto fileSize = file.tellg();
        file.seekg(0, file.beg);
        std::vector<char> data(fileSize);
        file.read(data.data(), fileSize);
        file.close();

        size_x = 0;
        size_y = 0;

        for (auto i = 0; i < fileSize; i++)
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

    void print(const Search& search)
    {
        system("cls");

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

            std::cout << std::endl;
        }
    }
private:
    int size_x;
    int size_y;

    std::vector<std::vector<int>> maze;

    Node start;
    Node end;
};

int get_distance(const Node& a, const Node& b)
{
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

std::vector<Node> get_indices()
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

bool a_star_path(Maze& maze, Search& search)
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

bool a_star(Maze& maze, Search& search)
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
    std::string files[] =
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
