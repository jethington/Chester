#include "Box.h"

static Point find_center(Point bottom_left, float width, float height);

// would be easier if constructor took center point and calculated bottom_left
Box::Box(Point bottom_left, float width, float height) :
bottom_left(bottom_left), width(width), height(height), center(find_center(bottom_left, width, height)) {

}

Box::~Box() {

}

bool Box::contains(Point& p) {
    return !((p.x < bottom_left.x) || (p.y < bottom_left.y) || (p.x > (bottom_left.x + width)) || (p.y > (bottom_left.y + height)));
}

bool Box::overlaps(Box& other) {
    if (bottom_left.x > other.bottom_left.x + other.width) {
        return false;
    }
    if (bottom_left.x + width < other.bottom_left.x) {
        return false;
    }
    if (bottom_left.y + height < other.bottom_left.y) {
        return false;
    }
    if (bottom_left.y > other.bottom_left.y + other.height) {
        return false;
    }
    return true;
}

// used in constructor
static Point find_center(Point bottom_left, float width, float height) {
    float center_x = bottom_left.x + width/2.0;
    float center_y = bottom_left.y + height/2.0;
    return Point(center_x, center_y);
}

std::string Box::to_string() {
    std::string result;
    result.append("bottom left: ").append(bottom_left.to_string());
    result.append("  width: ").append(std::to_string(width));
    result.append("  height: ").append(std::to_string(height));
    return result;
}
