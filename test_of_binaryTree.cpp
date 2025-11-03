//
// Created by 霍华德 on 2025/10/31.
//
#include<iostream>
#include<vector>
#include "BinaryTree.h"
using namespace std;

int main() {
    binaryTree<char> tree1;
    cout << "请输入二叉树创建序列：";
    string level;
    cin >> level;
    const char* levelSeq = level.c_str();
    tree1.createTree(levelSeq, level.length());
    tree1.preOrder(tree1.root());
    cout << endl;
    tree1.inOrder(tree1.root());
    cout << endl;
    tree1.postOrder(tree1.root());
    cout << endl;
    cout << "此二叉树的节点总数为：" << tree1.numberOfNodes(tree1.root()) << endl;
    cout << "此二叉树的高度为：" << tree1.height(tree1.root()) << endl;

    binaryTree<char> tree2;
    string preOrder1;
    string inOrder1;
    cout << "请输入前序序列：" << endl;
    cin >> preOrder1;
    cout << "请输入中序序列：" << endl;
    cin >> inOrder1;
    vector<char> preSequence1(preOrder1.begin(), preOrder1.end());
    vector<char> inSequence1(inOrder1.begin(), inOrder1.end());
    unordered_map<char, int> keyMap2;
    createMapOfInOrder(&inSequence1, keyMap2);

    binaryTreeNode<char> *root2 = tree2.createTreeByPreAndIn(&preSequence1, &inSequence1, &keyMap2, 0, static_cast<int>(preSequence1.size() - 1), 0, static_cast<int>(inSequence1.size() - 1));
    tree2.setRoot(root2);
    tree2.preOrder(tree2.root());
    cout << endl;
    tree2.inOrder(tree2.root());
    cout << endl;
    tree2.postOrder(tree2.root());
    cout << endl;

    binaryTree<char> tree3;
    string postOrder2;
    string inOrder2;
    cout << "请输入后序序列：" << endl;
    cin >> postOrder2;
    cout << "请输入中序序列：" << endl;
    cin >> inOrder2;
    vector<char> postSequence2(postOrder2.begin(), postOrder2.end());
    vector<char> inSequence2(inOrder2.begin(), inOrder2.end());
    unordered_map<char, int> keyMap3;
    createMapOfInOrder(&inSequence2, keyMap3);

    binaryTreeNode<char> *root3 = tree3.createTreeByPreAndIn(&postSequence2, &inSequence2, &keyMap3, 0, static_cast<int>(postSequence2.size() - 1), 0, static_cast<int>(inSequence2.size() - 1));
    tree2.setRoot(root2);
    tree2.preOrder(tree2.root());
    cout << endl;
    tree2.inOrder(tree2.root());
    cout << endl;
    tree2.postOrder(tree2.root());
    cout << endl;
    return 0;
}