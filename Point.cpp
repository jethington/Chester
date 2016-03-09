#include "Point.h"

#include <cmath>

Point::Point(double x, double y) : x(x), y(y) {

}

Point::~Point() {

}

double Point::distance(Point& other) {
    double dx = x - other.x;
    double dy = y - other.y;    
    return std::sqrt(dx*dx + dy*dy);
}

double Point::distance_squared(Point& other) {
    double dx = x - other.x;
    double dy = y - other.y;    
    return dx*dx + dy*dy;
}

std::string Point::to_string() {
    std::string s;
    s.append("(").append(std::to_string(x)).append(", ").append(std::to_string(y)).append(")");
    return s;
}
