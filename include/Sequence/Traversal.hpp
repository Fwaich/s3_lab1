#pragma once
#include <functional> 
#include "BinaryTree.hpp"
#include "TreeNode.hpp"

template <typename T>
class TreeTraversal {
public:
    virtual void traverse(TreeNode<T>* root, std::vector<TreeNode<T>*>& result) = 0;
    virtual TreeTraversal<T>* clone() = 0;
    virtual ~TreeTraversal() = default;
};

template <typename T>
class InOrderTraversal : public TreeTraversal<T> {
public:
    void traverse(TreeNode<T>* node, std::vector<TreeNode<T>*>& result) override {
        if (!node) return;
        traverse(node->left, result);
        result.push_back(node);
        traverse(node->right, result);
    }

    TreeTraversal<T>* clone() override {
        return new InOrderTraversal<T>();
    }
};

template <typename T>
class PreOrderTraversal : public TreeTraversal<T> {
public:
    void traverse(TreeNode<T>* node, std::vector<TreeNode<T>*>& result) override {
        if (!node) return;
        result.push_back(node);
        traverse(node->left, result);
        traverse(node->right, result);
    }

    TreeTraversal<T>* clone() override {
        return new PreOrderTraversal<T>();
    }
};

template <typename T>
class PostOrderTraversal : public TreeTraversal<T> {
public:
    void traverse(TreeNode<T>* node, std::vector<TreeNode<T>*>& result) override {
        if (!node) return;
        traverse(node->left, result);
        traverse(node->right, result);
        result.push_back(node);
    }

    TreeTraversal<T>* clone() override {
        return new PostOrderTraversal<T>();
    }
};
