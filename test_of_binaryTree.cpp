//
// Created by 霍华德 on 2025/10/31.
//
#include<iostream>
#include<vector>
#include "BinaryTree.h"
using namespace std;

int main() {
    binaryTree<char> tree1;
    cout << "请输入二叉树创建序列：" << endl;
    string level;
    cin >> level;
    const char* levelSeq = level.c_str();
    tree1.createTree(levelSeq, level.length());
    tree1.preOrder(tree1.root());
    tree1.inOrder(tree1.root());
    tree1.postOrder(tree1.root());
    cout << "此二叉树的节点总数为：" << tree1.numberOfNodes(tree1.root()) << endl;
    cout << "此二叉树的高度为：" << tree1.height(tree1.root()) << endl;

    binaryTree<char> tree2;
    const int MAX_LEN = 100;
    char preOrder2[MAX_LEN];
    char inOrder2[MAX_LEN];
    cout << "请输入前序序列：" << endl;
    cin >> preOrder2;
    cout << "请输入中序序列：" << endl;
    cin >> inOrder2;

    tree2.createTreeByPreAndIn(preOrder2, inOrder2);
    tree2.preOrder(tree2.root());
    tree2.inOrder(tree2.root());
    tree2.postOrder(tree2.root());

    binaryTree<char> tree3;
    char postOrder3[MAX_LEN];
    char inOrder3[MAX_LEN];
    cout << "请输入后序序列：" << endl;
    cin >> postOrder3;
    cout << "请输入中序序列：" << endl;
    cin >> inOrder3;
    tree3.createTreeByPostAndIn(postOrder3, inOrder3);

    tree3.preOrder(tree3.root());
    tree3.inOrder(tree3.root());
    tree3.postOrder(tree3.root());
    return 0;
}