#include "convexhull.h"

namespace ConvexHull
{

double min (double a, double b)
{
    return (a < b) ? a : b;
}
bool pcmp (const QPointF & a, const QPointF & b)
{
    return a.x() < b.x() || a.x() == b.x() && a.y() < b.y();
}
double protate(const QPointF & a, const QPointF & b, const QPointF & c)
{
    return (b.x() - a.x()) * (c.y() - b.y()) - (b.y() - a.y()) * (c.x() - b.x());
}

void calculate(QVector<QPointF> & arr)
{
    long long n = arr.size();
    std::sort(arr.begin(), arr.end(), &pcmp);
    QStack<QPointF> st;
    QPointF topst = arr[n - 1];
    for (long long i = n - 2; i >= 0; --i)
    {
        while (!st.empty() && protate(st.top(), topst, arr[i]) < 0)
        {
            topst = st.top();
            st.pop();
        }
        st.push(topst);
        topst = arr[i];
    }
    for (long long i = 1; i < n; ++i)
    {
        while (!st.empty() && protate(st.top(), topst, arr[i]) < 0)
        {
            topst = st.top();
            st.pop();
        }
        st.push(topst);
        topst = arr[i];
    }
    arr.clear();
    while (!st.empty())
    {
        arr.push_back(topst);
        topst = st.top();
        st.pop();
    }
}

}
