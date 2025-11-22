#include <iostream>
#include <fstream>
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

std::vector<int> maze;

int sizeX = 0;
int sizeY = 0;

int startPos = 0;
int endPos = 0;

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

    maze.resize(sizeX * sizeY);

    auto index = 0;

    for (auto y = 0; y < sizeY; y++)
    {
        for (auto x = 0; x < sizeX; x++)
        {
            maze[y * sizeX + x] = data[index++];
        }

        index += 1;
    }

    for (auto i = 0; i < sizeX * sizeY; i++)
    {
        switch (maze[i])
        {
            case '0':
                maze[i] = Type::Empty;
                break;

            case '1':
                maze[i] = Type::Wall;
                break;

            case 'S':
                maze[i] = Type::Start;
                startPos = i;
                break;

            case 'K':
                maze[i] = Type::End;
                endPos = i;
                break;
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
            switch (maze[y * sizeX + x])
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

int getLeft(int n) 
{
    if (n % sizeX != 0)
    {
        return maze[n - 1];
    }

    return Type::Wall;
}

int getRight(int n)
{
    if (n % sizeX != sizeX - 1)
    {
        return maze[n + 1];
    }

    return Type::Wall;
}

int getUpper(int n)
{
    if (n >= sizeX)
    {
        return maze[n - sizeX];
    }

    return Type::Wall;
}

int getLower(int n)
{
    if (n < sizeX * (sizeY - 1))
    {
        return maze[n + sizeX];
    }

    return Type::Wall;
}

int set(const std::list<short>& pos, int level)
{
    showMaze();

    std::list<short> posNext;

    for (auto i = pos.begin(); i != pos.end(); i++)
    {
        if (getLeft(*i) == Type::End || getRight(*i) == Type::End || getUpper(*i) == Type::End || getLower(*i) == Type::End)
        {
            while (posNext.size())
            {
                posNext.pop_back();
            }

            return level;
        }

        if (getLeft(*i) == Type::Empty)
        {
            posNext.push_back(*i - 1);
            maze[*i - 1] = level;
        }

        if (getRight(*i) == Type::Empty)
        {
            posNext.push_back(*i + 1);
            maze[*i + 1] = level;
        }

        if (getUpper(*i) == Type::Empty)
        {
            posNext.push_back(*i - sizeX);
            maze[*i - sizeX] = level;
        }

        if (getLower(*i) == Type::Empty)
        {
            posNext.push_back(*i + sizeX);
            maze[*i + sizeX] = level;
        }
    }

    if (posNext.size())
    {
        return set(posNext, level + 1);
    }

    return 0;
}

void findWay(int pos, int level)
{
    showMaze();

    level -= 1;

    if (getLeft(pos) == level)
    {
        maze[pos - 1] = Type::Way;

        if (level > 1)
        {
            findWay(pos - 1, level);

            return;
        }
    }
    else if (getRight(pos) == level)
    {
        maze[pos + 1] = Type::Way;

        if (level > 1)
        {
            findWay(pos + 1, level);

            return;
        }
    }
    else if (getUpper(pos) == level)
    {
        maze[pos - sizeX] = Type::Way;

        if (level > 1)
        {
            findWay(pos - sizeX, level);

            return;
        }
    }
    else if (getLower(pos) == level)
    {
        maze[pos + sizeX] = Type::Way;

        if (level > 1)
        {
            findWay(pos + sizeX, level);

            return;
        }
    }
}

void solveMaze()
{
    showMaze();

    std::list<short> pos;

    pos.push_back(startPos);

    auto length = set(pos, 1);

    findWay(endPos, length);
    showMaze();

    std::cout << "Size: " << sizeX << "x" << sizeY << std::endl;
    std::cout << "Start: " << startPos / sizeX + 1 << ":" << startPos % sizeX + 1 << std::endl;
    std::cout << "End: " << endPos / sizeX + 1 << ":" << endPos % sizeX + 1 << std::endl;
    std::cout << "Length: " << length << std::endl;

    std::cin.get();
}

int main()
{
    char c;

    do 
    {
        std::cout << "1 - Brute force" << std::endl;
        std::cout << "q - Quit" << std::endl;

        c = std::cin.get();

        switch (c)
        {
            case '1':
            {
                loadMaze("maze.txt");
                solveMaze();
                break;
            }
        }
    }
    while (c != 'q');

    return 0;
}
