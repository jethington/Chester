#include "SearchResult.h"

SearchResult::SearchResult(Node* node_ptr, int index) :
node_ptr(node_ptr), index(index) {

}

SearchResult::SearchResult() {
    node_ptr = nullptr;
    index = 0;
}

SearchResult::~SearchResult() {

}

bool SearchResult::none_found() {
    return node_ptr == nullptr;
}
