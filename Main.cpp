#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "Node.h"
#include "Box.h"
#include "Point.h"

//#define TEST

int main(void) {
    Point p(0.0, 0.0);
    Box b(p, 1.0, 1.0);
    Node tree(nullptr, b);
    double total_distance = 0.0;
    Point current_position(0.5, 0.5);

    std::ifstream in_file("treats2");
    in_file.ignore(100, '\n'); // skip first line
    int treats = 0;

#ifdef TEST 
    std::vector<Point> test;
#endif

    while (in_file.peek() != EOF) {
        std::string line;
        std::string buff;
        getline(in_file, line);
        std::stringstream ss(line);
        double x;
        double y;
        ss >> x >> y;
        tree.add(Point(x, y));
#ifdef TEST        
        test.push_back(Point(x,y));
#endif
        treats++;        
    }

    while (treats > 0) {        
        SearchResult result = tree.find_point_closest_to(current_position);
        Node* n = result.node_ptr;
        Point p = n->leaf->bucket[result.index];        
        n->leaf->bucket.erase(n->leaf->bucket.begin() + result.index); // could swap it to the back first
        total_distance += p.distance(current_position);
        
        Node* parent = n->parent;
        if ((parent != nullptr) && (parent->number_of_points() < BUCKET_MAX_SIZE)) {           
            parent->collapse();
        }

#ifdef TEST
        // test code: run brute force algorithm and compare results
        // if brute force picks a different treat to eat next than my quad tree, then I have a bug
        int index = 0;
        double closest_distance = 2.0;
        for (int i = 0; i < test.size(); i++) {
            double distance = test[i].distance(current_position);
            if (distance < closest_distance) {
                closest_distance = distance;
                index = i;
            }
        }
        Point temp = current_position;   
#endif

        current_position = p;
        treats--;

#ifdef TEST
        if ((p.x != test[index].x) || (p.y != test[index].y)) { 
            std::cout << "Current: " << temp.to_string() << std::endl;
            std::cout << "Removed: " << p.to_string() << std::endl;
            std::cout << "Correct: " << test[index].to_string() << std::endl;

            Node* contains_correct = tree.top_down_search_for(test[index]);
            std::cout << "Search box:\n" << last_search.to_string() << std::endl << std::endl;
            std::cout << "Correct box: \n" << contains_correct->box.to_string() << std::endl << std::endl;
            std::cout << tree.to_string() << std::endl;

            break;
        }
        test.erase(test.begin() + index);
#endif 
    }

    std::cout << "Total distance: " << total_distance << std::endl;
}
