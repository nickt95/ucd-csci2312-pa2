#include <iostream>
#include "clustering.h"

using namespace std;
using namespace clustering;

int main() {
    int dim = 2;

    double d1[] = {2.5, 4.6};

    point p1;
    point p2(dim);
    point p3 = p2;
    point p4(dim, d1);

    cout << (p1 == p2) << endl;
    cout << (p2 != p3) << endl;
    cout << (p3 > p4) << endl;
    cout << (p3 < p4) << endl;

    p1 += p4;
    p2 -= p4;
    p1 *= 7;
    p2 /= 5;

    cluster c1;
    c1 += p1;
    c1 += p2;
    c1 += p3;
    c1 += p4;

    cluster c2(c1);
    cluster c3 = c1;

    c2 -= p3;

    cout << (c1 == c3) << endl;

    c1 += c2;
    c3 -= c1;

    return 0;
}