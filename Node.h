#ifndef NODE_H_
#define NODE_H_

#include <string>
#include <vector>

#include "Point.h"
#include "Leaf.h"
#include "Box.h"
#include "SearchResult.h"

#define BUCKET_MAX_SIZE 30

struct SearchResult;

struct Node {
    Node(Node* parent, Box box);
    ~Node();
    Node* parent;
    Node* children[4];
    Box box;
    Leaf* leaf;
    std::string to_string();
    void add(Point p);
    SearchResult find_point_closest_to(Point target);
    void split();
    void collapse();
    int number_of_points();
    Node* top_down_search_for(Point p);
private:
    SearchResult find_closest_helper(Point target, Box search_box);
};

// children:
#define TOP_LEFT      0
#define TOP_RIGHT     1
#define BOTTOM_LEFT   2
#define BOTTOM_RIGHT  3

// TEST ONLY
extern Box last_search;

#endif
