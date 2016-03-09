#ifndef SEARCH_RESULT_H_
#define SEARCH_RESULT_H_

#include "Node.h"

struct Node;

// yep, std::tuple was enough of a pain that I gave up and wrote this instead
struct SearchResult {
    SearchResult(Node* node_ptr, int index);
    SearchResult();
    ~SearchResult();    
    Node* node_ptr;
    int index;
    bool none_found();
};

#endif
