#include "Leaf.h"

Leaf::Leaf() {
    
}

Leaf::~Leaf() {

}

void Leaf::add(Point p) {
    bucket.push_back(p);
}

std::string Leaf::to_string() {
    std::string s;
    for (auto &p: bucket) {
        s.append(p.to_string()).append("\n");
    }
    return s;
}
