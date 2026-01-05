#pragma once

template <typename T>
struct TreeNode 
{
    T data;
    TreeNode* left;
    TreeNode* right;
    
    TreeNode(T new_value, TreeNode* new_left = nullptr, TreeNode* new_right = nullptr)
    : data(new_value), left(new_left), right(new_right) {}
};