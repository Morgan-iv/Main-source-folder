#ifndef  GEOM_HPP
#define  GEOM_HPP

#include <vector>
#include <algorithm>
//#include <iostream>
//#include <utility>

class Point;
class Component;

typedef std::vector<Point>::iterator PointIt;
typedef std::vector<Component>::iterator ComponentIt;
typedef unsigned int uint;

const double eps = 1e-9;
//#define nullptr NULL /*if c++ < c++11*/

enum
{
    OP_NOP = 0,
    OP_AND = 1,
    OP_OR = 2
};

enum
{
    RES_OUT = 0,
    RES_IN = 1,
    RES_ON = 2
};

struct Point
{
    double x;
    double y;
    Point()
    {
        x = 0.0;
        y = 0.0;
    }
    Point(double xx, double yy)
    {
        x = xx;
        y = yy;
    }
};

inline double PRotate(Point a, Point b, Point c)
{
    return (b.x - a.x) * (c.y - b.y) -
           (b.y - a.y) * (c.x - b.x);
}

class Component
{
private:
    std::vector<Point> points;

public:
    Component();
    Component(const std::vector<Point> & pts)
    {
        points = pts;
    }
    Component(const Point * pts, size_t length)
    {
        points.resize(length);
        for (uint i = 0; i < length; ++i)
        {
            points[i] = pts[i];
        }
    }
    Component(const double * pts, size_t length)
    {
        points.resize(length);
        for (uint i = 0; i < length; ++i)
        {
            points[i].x = pts[i * 2];
            points[i].y = pts[i * 2 + 1];
        }
    }
    void AddPoint(Point p)
    {
        points.push_back(p);
    }
    void AddPoint(double x, double y)
    {
        points.push_back(Point(x, y));
    }
    void AddOnEdge(size_t pos, double ratio = 0.5)
    {
        if (points.size() < 2)
            return;
        if (ratio < 0.0)
            ratio = 0.0;
        if (ratio > 1.0)
            ratio = 1.0;
        Point p1 = points[pos % points.size()];
        Point p2 = points[(pos + 1) % points.size()];
        Point ins = Point((p1.x - p2.x) * ratio + p1.x,
                          (p1.y - p2.y) * ratio + p1.y);
        points.insert(points.begin() + (pos + 1) % points.size(), ins);
    }
    int CheckPoint(Point point) const
    {
        if (points.size() < 3)
            return 0;
        size_t num_intersections = 0;
        for (size_t i = 0; i != points.size(); i++)
        {
            Point min_point = points[i];
            Point max_point = points[(i + 1) % points.size()];
            if (min_point.y > max_point.y)
                std::swap(min_point, max_point);
            if (max_point.y < point.y || min_point.y > point.y)
                continue;
            double orient = PRotate(min_point, max_point, point);
            if (orient < eps && orient > -eps)
            {
                if (max_point.y - min_point.y < eps &&
                   (point.x > max_point.x && point.x > min_point.x ||
                    point.x < max_point.x && point.x < min_point.x))
                    continue;
                return RES_ON;
            }
            if (orient > 0 && max_point.y > point.y)
                num_intersections++;
        }
        return num_intersections % 2;
    }
    Point & operator [] (size_t pos)
    {
        return points[pos];
    }
    void ErasePoint(size_t pos)
    {
        points.erase(points.begin() + pos % points.size());
    }
    PointIt begin()
    {
        return points.begin();
    }
    PointIt end()
    {
        return points.end();
    }
    size_t size() const
    {
        return points.size();
    }
};

struct AST
{
    size_t Op;
    size_t ID;
    AST * left;
    AST * right;

    AST(size_t pos = 0)
    {
        Op = OP_NOP;
        ID = pos;
        left = nullptr;
        right = nullptr;
    }
    AST(const AST & other)
    {
        Op = other.Op;
        ID = other.ID;
        left = (other.left) ? new AST(*other.left) : nullptr;
        right = (other.right) ? new AST(*other.right) : nullptr;
    }
    /*
    AST(AST && other)
    {
        Op = other.Op;
        ID = other.ID;
        left = other.left;
        right = other.right;
        other.Op = OP_NOP;
        other.ID = 0;
        other.left = nullptr;
        other.right = nullptr;
    }
    */
    AST & operator = (const AST & other)
    {
        Op = other.Op;
        ID = other.ID;
        delete left;
        delete right;
        left = new AST(*other.left);
        right = new AST(*other.right);
        return *this;
    }
    /*
    AST & operator = (AST && other)
    {
        Op = other.Op;
        ID = other.ID;
        delete left;
        delete right;
        left = other.left;
        right = other.right;
        other.Op = OP_NOP;
        other.ID = 0;
        other.left = nullptr;
        other.right = nullptr;
        return *this;
    }
    */
    ~AST()
    {
        delete left;
        delete right;
    }

    void Incr(size_t s)
    {
        ID += s;
        if (left)
            left->Incr(s);
        if (right)
            right->Incr(s);
    }
    friend AST operator && (const AST & a, const AST & b);
    friend AST operator || (const AST & a, const AST & b);
};

AST operator && (const AST & a, const AST & b)
{
    AST res;
    res.Op = OP_AND;
    res.ID = 0;
    res.left = new AST(a);
    res.right = new AST(b);
    return res;
}

AST operator || (const AST & a, const AST & b)
{
    AST res;
    res.Op = OP_OR;
    res.ID = 0;
    res.left = new AST(a);
    res.right = new AST(b);
    return res;
}

class Polygon
{
private:
    std::vector<Component> comps;
    AST * tree;

    int RunTree(AST * t, Point point) const
    {
        if (t->Op == OP_NOP)
            return comps[t->ID].CheckPoint(point);
        int r1 = RunTree(t->left, point);
        if (t->Op == OP_AND && r1 == RES_OUT)
            return RES_OUT;
        if (t->Op == OP_OR && r1 == RES_IN)
            return RES_IN;
        int r2 = RunTree(t->right, point);
        if (t->Op == OP_AND)
        {
            if (r2 == RES_OUT)
                return RES_OUT;
            if (r1 == RES_IN || r2 == RES_IN)
                return RES_IN;
            return RES_ON;
        }
        if (t->Op == OP_OR)
        {
            if (r2 == RES_IN)
                return RES_IN;
            if (r1 == RES_ON || r2 == RES_ON)
                return RES_ON;
            return RES_OUT;
        }
    }

public:
    Polygon()
    {
        tree = nullptr;
    }
    Polygon(const Component & component)
    {
        comps.push_back(component);
        tree = new AST(0);
    }
    Polygon(const Polygon & other)
    {
        comps = other.comps;
        tree = new AST(*other.tree);
    }
    /*
    Polygon(Polygon && other)
    {
        comps.swap(other.comps);
        tree = other.tree;
        other.tree = nullptr;
    }
    */
    Polygon & operator = (const Polygon & other)
    {
        comps = other.comps;
        delete tree;
        tree = new AST(*other.tree);
        return *this;
    }
    /*
    Polygon & operator = (Polygon && other)
    {
        comps.swap(other.comps);
        delete tree;
        tree = other.tree;
        other.tree = nullptr;
        return *this;
    }
    */
    ~Polygon()
    {
        delete tree;
    }

    int CheckPoint (Point point) const
    {
        if (tree != nullptr)
            return RunTree(tree, point);
    }

    ComponentIt begin()
    {
        return comps.begin();
    }
    ComponentIt end()
    {
        return comps.end();
    }
    size_t size() const
    {
        return comps.size();
    }
    size_t size(size_t pos) const
    {
        return comps[pos % comps.size()].size();
    }
    friend Polygon operator && (const Polygon & a, const Polygon & b);
    friend Polygon operator || (const Polygon & a, const Polygon & b);
};

Polygon operator && (const Polygon & a, const Polygon & b)
{
    if (a.tree == nullptr)
        return a;
    if (b.tree == nullptr)
        return b;
    Polygon res;
    res.comps = a.comps;
    res.comps.insert(res.comps.end(), b.comps.begin(), b.comps.end());
    res.tree = new AST(*a.tree && *b.tree);
    res.tree->right->Incr(a.size());
    return res;
}
Polygon operator || (const Polygon & a, const Polygon & b)
{
    if (a.tree == nullptr)
        return b;
    if (b.tree == nullptr)
        return a;
    Polygon res;
    res.comps = a.comps;
    res.comps.insert(res.comps.end(), b.comps.begin(), b.comps.end());
    res.tree = new AST(*a.tree || *b.tree);
    res.tree->right->Incr(a.size());
    return res;
}

#endif //GEOM_HPP