#ifndef POINT_H_
#define POINT_H_

#include <string>

class Point {
public:
    Point(double x, double y);
    ~Point();
    double x;
    double y;
    double distance(Point& other);
    double distance_squared(Point& other);
    std::string to_string();
};

#endif
