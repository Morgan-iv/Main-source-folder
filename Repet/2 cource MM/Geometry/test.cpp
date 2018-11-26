#include <vector>
#include "geom.hpp"
#include "geom.hpp"
#include <iostream>


using namespace std;

int main()
{
    int n, m;
    cin >> n;
    std::vector<Point> vec;
    std::vector<Polygon> polys;
    for (int i = 0; i < n; ++i)
    {
        cin >> m;
        for (int j = 0; j < m; ++j)
        {
            double a, b;
            cin >> a >> b;
            vec.push_back(Point(a, b));
        }    
        polys.push_back(Polygon(Component(vec)));
        vec.clear();
    }

    Polygon pol = polys[0] || polys[1] && polys[2];

    cout << pol.CheckPoint(Point(4.5, 7)) << endl;
    return 0;
}