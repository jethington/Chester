#include "Node.h"

#include <iostream>
#include <cassert>

Node::Node(Node* parent, Box box) :
parent(parent), box(box) {
    for (Node* &node_ptr: children) {
        node_ptr = nullptr;
    }
    leaf = new Leaf();
}

Node::~Node() {
    for (Node* &node_ptr: children) {
        delete node_ptr;
    }
    delete leaf;
}

void Node::collapse() {
    leaf = new Leaf();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < children[i]->leaf->bucket.size(); j++) {
            leaf->bucket.push_back(children[i]->leaf->bucket[j]);
        }        
        delete children[i];
        children[i] = nullptr;
    }
}

// this function assumes that the point is in this Node's box, or the function would not get called
Node* Node::top_down_search_for(Point p) {    
    if (leaf != nullptr) {
        return this;
    }
    else {
        for (Node* &node_ptr: children) {
            if (node_ptr->box.contains(p)) {
                return node_ptr->top_down_search_for(p);
            }
        }
        // this should be unreachable
        assert(0);
        return nullptr; 
    }
}

// TEST ONLY
Box last_search(Point(0.0, 0.0), 1.0, 1.0);

// should only be called on top node in tree
SearchResult Node::find_point_closest_to(Point target) {  
    Node* contains_target = top_down_search_for(target);
    Box b = contains_target->box;
    // now center the box on the target
    Point new_bottom_left = target;
    new_bottom_left.x -= b.width/2.0;
    new_bottom_left.y -= b.height/2.0;
    Box target_box = Box(new_bottom_left, b.width, b.height);
        
    SearchResult result;

    if (leaf == nullptr) {
        while (result.node_ptr == nullptr) { 
            for (Node* &node_ptr: children) {           
                if (node_ptr->box.overlaps(target_box)) {                        
                    SearchResult found = find_closest_helper(target, target_box);
                    if (found.node_ptr != nullptr) { // found one               
                        if (result.node_ptr == nullptr) { // no result yet, so this one must be the best
                            result = found;
                        }
                        else { // compare the one found in this branch to the best one so far
                            Point point_found = found.node_ptr->leaf->bucket[found.index];                    
                            Point result_point = result.node_ptr->leaf->bucket[result.index];
                            if (target.distance(point_found) < target.distance(result_point)) {
                                result = found;
                            }
                        }
                    }
                }
            }

            if (result.none_found()) {
                // wouldn't a Box::scale function be nice?
                target_box.bottom_left.x -= target_box.width/2.0;
                target_box.bottom_left.y -= target_box.height/2.0;
                target_box.width *= 2.0;
                target_box.height *= 2.0;
            }
        } 
    }
    else {
        // down to one node
        int index = 0;
        double smallest = 4.0; // you know 4.0 is greater than distance_squared for a 1.0 * 1.0 square
        for (int i = 0; i < leaf->bucket.size(); i++) {
            double d = target.distance_squared(leaf->bucket[i]);
            if (d < smallest) {
                smallest = d;
                index = i;                    
            }
        }
        result = SearchResult(this, index);
    }
             
    return result;
}

// this function should only be called if the boxes intersect
SearchResult Node::find_closest_helper(Point target, Box search_box) {    
    if (leaf != nullptr) {
        double smallest = 4.0;
        int index = 0;
        SearchResult result;
        double min_distance_squared = search_box.width * search_box.width / 4.0; // (w/2)^2 radius
        for (int i = 0; i < leaf->bucket.size(); i++) {
            double d = target.distance_squared(leaf->bucket[i]);
            if ((d < min_distance_squared) && (d < smallest)) {
                smallest = d;
                index = i;                    
            }
        }
        if (smallest < 3.5) {
            result = SearchResult(this, index);
        }        
        return result;          
    }
    else {
        SearchResult result;        
        for (Node* &node_ptr: children) {
            if (node_ptr->box.overlaps(search_box)) {
                SearchResult found = node_ptr->find_closest_helper(target, search_box);
                if (found.node_ptr != nullptr) { // found one               
                    if (result.node_ptr == nullptr) { // no result yet, so this one must be the best
                        result = found;
                    }
                    else { // compare the one found in this branch to the best one so far
                        Point point_found = found.node_ptr->leaf->bucket[found.index];                    
                        Point result_point = result.node_ptr->leaf->bucket[result.index];
                        if (target.distance(point_found) < target.distance(result_point)) {
                            result = found;
                        }
                    }
                }
            }
        }
        return result;
    }
}

int Node::number_of_points() {
    if (leaf != nullptr) {
        return leaf->bucket.size();
    }
    else {
        int result = 0;        
        for (Node* &node_ptr: children) {
            result += node_ptr->number_of_points();
        }
        return result;
    }
}

void Node::add(Point p) {
    if (leaf == nullptr) {
        // keep going
        int next;
        if (p.y > box.center.y) {
            if (p.x > box.center.x) {
              next = TOP_RIGHT;
            }
            else {
              next = TOP_LEFT;
            }
        }
        else {
            if (p.x > box.center.x) {
              next = BOTTOM_RIGHT;
            }
            else {
              next = BOTTOM_LEFT;
            }
        }
        children[next]->add(p);
    }
    else {
        leaf->add(p);
        if (leaf->bucket.size() >= BUCKET_MAX_SIZE) {
            split();
        }
    }
}

void Node::split() {
    double w = box.width/2.0;
    double h = box.height/2.0;
    double x = box.bottom_left.x;
    double y = box.bottom_left.y;

    children[BOTTOM_LEFT] = new Node(this, Box(box.bottom_left, w, h));

    Point p1(x, y+h);
    children[TOP_LEFT] = new Node(this, Box(p1, w, h));

    Point p2(x+w, y);
    children[BOTTOM_RIGHT] = new Node(this, Box(p2, w, h));

    Point p3(x+w, y+h);
    children[TOP_RIGHT] = new Node(this, Box(p3, w, h));

    Leaf* temp = leaf; // want to set it to null so I can use add() function, but I can't delete it yet
    leaf = nullptr;

    for (Point &p: temp->bucket) {
        add(p);
    }

    delete temp;
    temp = nullptr;
}

std::string Node::to_string() {
    std::string s;    
    if (leaf != nullptr) {
        s.append(leaf->to_string()).append("\n"); // note that I should remove the \n if I write a Tree::to_string()       
    }
    else {
        for (Node* &node_ptr: children) {
            s.append(node_ptr->to_string());
        }
    }
    return s;
}
