//
// Created by 霍华德 on 2025/11/10.
//
#pragma once
#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H
#include<iostream>
using namespace std;

class treeEmpty : public std::exception {
    const char* what() const noexcept override { return "The tree is empty"; }
};
template<class T>
struct binaryTreeNode {
    binaryTreeNode() {
        leftChild = rightChild = NULL;
    }
    binaryTreeNode(const T& theElement) {
        element = theElement;
        leftChild = rightChild = NULL;
    }
    binaryTreeNode(const T& theElement, binaryTreeNode<T>* l, binaryTreeNode<T>* r) {
        element = theElement;
        leftChild = l;
        rightChild = r;
    }

    binaryTreeNode<T>* leftChild;
    binaryTreeNode<T>* rightChild;
    T element;
};

template<class T>
void visit(binaryTreeNode<T>* current, bool& isFirst) {
    if (isFirst) {
        cout << current->element;
    }else {
        cout << "," << current->element;
    }
}

template<class T>
class BinarySearchTree {
public:
    BinarySearchTree() { root = NULL; }
    bool empty() { return root == NULL; }
    void insertNode(const T&);
    void removeNode(const T&);
    bool searchNode(const T&);
    void inOrder();
    void createBinarySearchTree(vector<T>* vec);

private:
    binaryTreeNode<T>* root;
    void inOrder(binaryTreeNode<T>* currentNode, bool& isFirst);
};

template<class T>
void BinarySearchTree<T>::insertNode(const T& num) {
    if (root == NULL) {
        root = new binaryTreeNode<T>(num);
        return;
    }
    binaryTreeNode<T>* current = root;
    binaryTreeNode<T>* pre = NULL;
    while (current != NULL) {
        if (current->element == num) {
            return;
        }
        pre = current;
        if (current->element < num) {
            current = current->rightChild;
        }else if (current->element > num) {
            current = current->leftChild;
        }
    }

    binaryTreeNode<T>* newNode = new binaryTreeNode<T>(num);
    if (pre->element < num) {
        pre->rightChild = newNode;
    }else {
        pre->leftChild = newNode;
    }
}

template<class T>
void BinarySearchTree<T>::removeNode(const T& num) {
    if (root == NULL) {
        throw treeEmpty();
    }
    binaryTreeNode<T>* current = root;
    binaryTreeNode<T>* pre = NULL;
    while (current != NULL) {
        if (current->element == num) {
            break;
        }
        pre = current;
        if (current->element < num) {
            current = current->rightChild;
        }else {
            current = current->leftChild;
        }
    }

    if (current == NULL) {
        return;
    }
    if (current->leftChild == NULL || current->rightChild == NULL) {
        binaryTreeNode<T>* childNode = current->leftChild != NULL ? current->leftChild : current->rightChild;
        if (pre == NULL){
            root = childNode;
        }else if (pre->leftChild == current) {
            pre->leftChild = childNode;
        }else {
            pre->rightChild = childNode;
        }
        delete current;
    }else {
        binaryTreeNode<T>* tempNode = current->rightChild;
        while (tempNode->leftChild != NULL) {
            tempNode = tempNode->leftChild;
        }
        T tempElement = tempNode->element;
        removeNode(tempElement);
        current->element = tempElement;
    }
}

template<class T>
bool BinarySearchTree<T>::searchNode(const T& num) {
    if (root == NULL) {
        return false;
    }
    binaryTreeNode<T>* current = root;
    while (current != NULL) {
        if (current->element == num) {
            return true;
        }else if (current->element > num) {
            current = current->leftChild;
        }else {
            current = current->rightChild;
        }
    }
    return false;
}

template<class T>
void BinarySearchTree<T>::inOrder() {
    bool first = true;
    if (root == NULL) {
        return;
    }
    inOrder(root, first);
}

template<class T>
void BinarySearchTree<T>::inOrder(binaryTreeNode<T> *currentNode, bool& isFirst) {
    if (!currentNode) {
        return;
    }
    inOrder(currentNode->leftChild, isFirst);
    visit(currentNode, isFirst);
    isFirst = false;
    inOrder(currentNode->rightChild, isFirst);
}

template<class T>
void BinarySearchTree<T>::createBinarySearchTree(vector<T>* vec) {
    for (int i = 0; i < vec->size(); i++) {
        insertNode((*vec)[i]);
    }
}

#endif