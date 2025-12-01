//
// Created by 霍华德 on 2025/11/10.
//
#include<iostream>
#include"BinarySearchTree.h"
using namespace std;

int main() {
    BinarySearchTree<int> bst;
    vector<int> a = {6,7,4,5,2,3,1};
    bst.createBinarySearchTree(&a);
    bst.inOrder();
    return 0;
}