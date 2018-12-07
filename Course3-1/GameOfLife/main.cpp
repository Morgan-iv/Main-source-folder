#include <iostream>
#include <chrono>
#include <thread>
#include "game.hpp"

void print_plane(cell_t * plane, size_t n, size_t m)
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            std::cout << (plane[i * m + j] ? '0' : '.');
        }
        std::cout << std::endl;
    }
}

void moveup(int count)
{
    printf("\e[%dF", count);
}

int main(int argc, char const *argv[])
{
    size_t n, m;
    std::cin >> n >> m;
    cell_t * plane1 = new cell_t[n * m];
    cell_t * plane2 = new cell_t[n * m];
    char c;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            std::cin >> c;
            plane1[i * m + j] = c != '.';
        }
    }
    print_plane(plane1, n, m);
    while(1)
    {
        moveup(n);
        print_plane(plane1, n, m);
        CalcGame(plane1, plane1 + (n - 1) * m, plane1, plane2, n, m);
        cell_t * tmp = plane1;
        plane1 = plane2;
        plane2 = tmp;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}