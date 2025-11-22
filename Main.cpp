#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <vector>

enum Type
{
    Empty = -1,
    Wall = -2,
    Start = -3,
    End = -4,
    Way = -5
};

struct Node
{
    int x;
    int y;

    int depth;

    float distance_from_start;
    float distance_to_end;

    Node(int x = 0, int y = 0, int depth = 0) : x(x), y(y), depth(depth), distance_from_start(0.0f), distance_to_end(0.0f)
    {
    }
};

std::vector<std::vector<int>> maze;

int sizeX;
int sizeY;

Node startPos;
Node endPos;

std::deque<Node> nodes;

bool is_valid(const Node& node)
{
    if (node.x < 0)
    {
        return false;
    }

    if (node.x > sizeX - 1)
    {
        return false;
    }

    if (node.y < 0)
    {
        return false;
    }

    if (node.y > sizeY - 1)
    {
        return false;
    }

    return true;
}

void loadMaze(const std::string& fileName)
{
    std::ifstream file(fileName, std::ios::binary | std::ios::ate);
    auto fileSize = file.tellg();
    file.seekg(0, file.beg);
    std::vector<char> data(fileSize);
    file.read(data.data(), fileSize);
    file.close();

    sizeX = 0;
    sizeY = 0;

    for (auto i = 0; i < fileSize; i++)
    {
        if (data[i] == '\n')
        {
            if (sizeY == 0)
            {
                sizeX = i;
            }

            sizeY += 1;
        }
    }

    maze.resize(sizeX, std::vector<int>(sizeY));

    auto index = 0;

    for (auto y = 0; y < sizeY; y++)
    {
        for (auto x = 0; x < sizeX; x++)
        {
            maze[x][y] = data[index++];
        }

        index += 1;
    }

    for (auto y = 0; y < sizeY; y++)
    {
        for (auto x = 0; x < sizeX; x++)
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
                    maze[x][y] = Type::Start;
                    startPos = Node(x, y);
                    break;

                case 'E':
                    maze[x][y] = Type::End;
                    endPos = Node(x, y);
                    break;
            }
        }
    }
}

void showMaze()
{
    system("cls");
    
    for (auto y = 0; y < sizeY; y++)
    {
        for (auto x = 0; x < sizeX; x++)
        {
            switch (maze[x][y])
            {
                case Type::Empty:
                    std::cout << ' ';
                    break;

                case Type::Wall:
                    std::cout << static_cast<unsigned char>(219);
                    break;

                case Type::Start:
                    std::cout << 'S';
                    break;

                case Type::End:
                    std::cout << 'E';
                    break;

                case Type::Way:
                    std::cout << static_cast<unsigned char>(178);
                    break;

                default:
                    std::cout << static_cast<unsigned char>(177);
            }
        }

        std::cout << std::endl;
    }
}

bool a_star()
{
    maze[startPos.x][startPos.y] = 0;
    nodes.push_back(startPos);

    while (nodes.size() > 0)
    {
        auto node = nodes.front();

        Node indices[4] =
        {
            Node(-1,  0),
            Node( 1,  0),
            Node( 0, -1),
            Node( 0,  1)
        };

        for (auto i = 0; i < 4; ++i)
        {
            auto next_node = Node(node.x + indices[i].x, node.y + indices[i].y, node.depth + 1);

            if (maze[next_node.x][next_node.y] == Type::End)
            {
                maze[next_node.x][next_node.y] = next_node.depth;

                return true;
            }

            if (is_valid(next_node))
            {
                if (maze[next_node.x][next_node.y] == Type::Empty)
                {
                    if (maze[next_node.x][next_node.y] < 0)
                    {
                        maze[next_node.x][next_node.y] = next_node.depth;

                        nodes.push_back(next_node);
                    }
                }
            }
        }

        nodes.pop_front();
    }

    return false;
}

bool a_star_path()
{
    auto node = Node(endPos.x, endPos.y);
    auto depth = maze[node.x][node.y];

    maze[node.x][node.y] = Type::Way;

    while (node.x != startPos.x || node.y != startPos.y)
    {
        Node indices[4] =
        {
            Node(-1,  0),
            Node( 1,  0),
            Node( 0, -1),
            Node( 0,  1)
        };

        auto found = false;

        for (auto i = 0; i < 4; ++i)
        {
            auto next_node = Node(node.x + indices[i].x, node.y + indices[i].y);
            auto next_depth = maze[next_node.x][next_node.y];

            if (is_valid(next_node))
            {
                if (maze[next_node.x][next_node.y] >= 0)
                {
                    if (next_depth < depth)
                    {
                        maze[next_node.x][next_node.y] = Type::Way;

                        node = next_node;
                        depth = next_depth;

                        found = true;

                        break;
                    }
                }
            }
        }

        if (found == false)
        {
            return false;
        }
    }

    return true;
}

int main()
{
    loadMaze("Maze.txt");

    if (a_star())
    {
        a_star_path();
    }

    maze[startPos.x][startPos.y] = Type::Start;
    maze[endPos.x][endPos.y] = Type::End;

    showMaze();

    std::cin.ignore();

    return 0;
}
