#pragma once
#include <sstream>
#include <functional>
#include <stdexcept>
#include "TreeNode.hpp"
#include "Traversal.hpp"
#include "TreeIterator.hpp"

template<typename T>
class TreeTraversal;

template<typename T>
class InOrderTraversal;

template <typename T>
class BinaryTree {
public:
    using tnode = TreeNode<T>;

private:
    tnode* root;
    TreeTraversal<T>* traversal;

public:
    BinaryTree(TreeTraversal<T>* new_traversal = new InOrderTraversal<T>())
    : root(nullptr), traversal(new_traversal) {}

    ~BinaryTree() {
        clear_r(root);
        root = nullptr;
        delete traversal;
    }

    void add(T value) {
        root = add_r(root, value);
    }

    TreeIterator<T> iterator() const {
        return TreeIterator<T>(root, traversal);
    }

    void remove(const T& value) {
        bool found = false;
        root = remove_r(root, value, found);
        if (!found) {
            throw std::runtime_error("No such element");
        }
    }

    void set_traversal(TreeTraversal<T>* new_traversal){
        delete traversal;
        traversal = new_traversal;
    }

    bool contains(const T& value) {
        auto it = iterator();
        while (it.has_next()) {
            if (it.next() == value) {
                return true;
            }
        }
        return false;
    }
    
    BinaryTree<T>* extract_subtree(const T& value) {
        if (!root) throw std::runtime_error("root isn't init");

        tnode* target_node = find_node_r(root, value);
        if (!target_node) throw std::runtime_error("No such element");

        BinaryTree<T>* subtree = new BinaryTree<T>(traversal->clone());
        subtree->root = clone_subtree_r(target_node);
        return subtree;
    }


private:
    tnode* add_r(tnode* node, T value) {
        if (node == nullptr) return new tnode(value);
        if (value < node->data) node->left = add_r(node->left, value);
        else if (value > node->data) node->right = add_r(node->right, value);
        return node;
    }

    void clear_r(tnode* node) {
        if (!node) return;
        clear_r(node->left);
        clear_r(node->right);
        delete node;
    }

    tnode* find_node_r(tnode* node, const T& value) {
        if (!node) return nullptr;
        if (value == node->data) return node;
        if (value < node->data) return find_node_r(node->left, value);
        return find_node_r(node->right, value);
    }

    tnode* clone_subtree_r(tnode* node) {
        if (!node) return nullptr;
        tnode* new_node = new tnode(node->data);
        new_node->left = clone_subtree_r(node->left);
        new_node->right = clone_subtree_r(node->right);
        return new_node;
    }

    tnode* remove_r(tnode* node, const T& value, bool& found) {
        if (!node) return nullptr;
    
        if (value < node->data) {
            node->left = remove_r(node->left, value, found);
        } else if (value > node->data) {
            node->right = remove_r(node->right, value, found);
        } else {
            found = true;
    
            if (!node->left) {
                tnode* right = node->right;
                delete node;
                return right;
            } else if (!node->right) {
                tnode* left = node->left;
                delete node;
                return left;
            }
    
            tnode* min_node = find_min_node(node->right);
            node->data = min_node->data;
            node->right = remove_r(node->right, min_node->data, found);
        }
        return node;
    }

};

template <typename T>
TreeNode<T>* find_min_node(TreeNode<T>* node) {
    while (node->left) node = node->left;
    return node;
}
