#pragma once
#include "Traversal.hpp"
#include "TreeNode.hpp"

template<typename T>
class TreeTraversal;

template <typename T>
class TreeIterator {
    std::vector<TreeNode<T>*> nodes;
    size_t index = 0;

public:
    TreeIterator(TreeNode<T>* root, TreeTraversal<T>* traversal) {
        traversal->traverse(root, nodes);
    }

    bool has_next() const {
        return index < nodes.size();
    }

    T& next() {
        return nodes[index++]->data;
    }

    size_t get_size() {
        return nodes.size();
    }

    T& get_at(size_t idx) {
        if (idx >= nodes.size()) {
            throw std::out_of_range("Index out of range");
        }
        return nodes[idx]->data;
    }

    const T& get_at(size_t idx) const {
        if (idx >= nodes.size()) {
            throw std::out_of_range("Index out of range");
        }
        return nodes[idx]->data;
    }

    void reset() { index = 0; }
};
