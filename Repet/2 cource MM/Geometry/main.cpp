#include <vector>
#include "geom.hpp"
#include <iostream>

int main()
{
    size_t n_iters = 100, m = 4;
    std::vector<Point> vec;
    std::vector<Polygon> polys;
    
    Polygon pol;
    for (size_t i = 0; i < n_iters; ++i)
    {
        vec.clear();
        for (size_t j = 0; j < m; ++j)
        {
            double a = rand(), b = rand();
            vec.push_back(Point(a, b));
        }
    // Polygon rand_pol(Component(vec));
    polys.push_back(Polygon(Component(vec)));
    pol = (pol || polys[i]);
    
    }
    return 0;
}