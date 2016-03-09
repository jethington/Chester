#ifndef LEAF_H_
#define LEAF_H_

#include <vector>
#include <string>
#include "Point.h"

struct Leaf {
    Leaf();
    ~Leaf();
    std::vector<Point> bucket;
    void add(Point p);
    std::string to_string();
};

#endif
