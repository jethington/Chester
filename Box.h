#ifndef BOX_H_
#define BOX_H_

#include <string>
#include "Point.h"

class Box {
public:
    Box(Point bottom_left, float width, float height);
    ~Box();
    bool contains(Point& p);
    bool overlaps(Box& other);
    Point bottom_left;
    float width;
    float height;
    Point center;
    std::string to_string();
};

#endif
