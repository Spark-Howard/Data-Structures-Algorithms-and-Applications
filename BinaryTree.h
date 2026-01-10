//
// Created by 霍华德 on 2025/10/26.
//
#pragma once
#ifndef BINARYTREE_H
#include<iostream>
#include<queue>
using namespace std;

template <class T>
struct binaryTreeNode {
    T element;
    binaryTreeNode<T> *leftChild;
    binaryTreeNode<T> *rightChild;

    binaryTreeNode() {
        leftChild = rightChild = NULL;
    }
    binaryTreeNode(const T& theElement) {
        element = theElement;
        leftChild = rightChild = NULL;
    }
    binaryTreeNode(const T& theElement, binaryTreeNode<T> *l, binaryTreeNode<T> *r) {
        element = theElement;
        leftChild = l;
        rightChild = r;
    }
};

template <class T>
void visit(binaryTreeNode<T>* current, bool& isFirst) {
    if (isFirst) {
        cout << current->element;
    }else {
        cout << "," << current->element;
    }
}

template <class T>
class binaryTree {
public:
    binaryTree() {
        origin = NULL;
        nodeNumber = 0;
    }
    binaryTreeNode<T>* root();
    void setRoot(binaryTreeNode<T>*);
    void preOrder(binaryTreeNode<T>*);
    void inOrder(binaryTreeNode<T>*);
    void postOrder(binaryTreeNode<T>*);
    void levelOrder(binaryTreeNode<T>*);
    void createTree(const char*, int);
    int numberOfNodes(binaryTreeNode<T>*);
    int height(binaryTreeNode<T>*);
    void createTreeByPreAndIn(const char*, const char*);
    void createTreeByPostAndIn(const char*, const char*);
private:
    void preOrderHelper(binaryTreeNode<T>*, bool&);
    void inOrderHelper(binaryTreeNode<T>*, bool&);
    void postOrderHelper(binaryTreeNode<T>*, bool&);
    binaryTreeNode<T>* buildTreeByPreAndIn(vector<T>*, vector<T>*, unordered_map<T, int>*, int, int, int, int);
    binaryTreeNode<T>* buildTreeByPostAndIn(vector<T>*, vector<T>*, unordered_map<T, int>*, int, int, int, int);
    int nodeNumber;
    binaryTreeNode<T> *origin;
};

template<class T>
binaryTreeNode<T> *binaryTree<T>::root() {
    return origin;
}

template<class T>
void binaryTree<T>::setRoot(binaryTreeNode<T> *newRoot) {
    origin = newRoot;
}

// 二叉树前序遍历
template<class T>
void binaryTree<T>::preOrder(binaryTreeNode<T>* current) {
    bool first = true;
    preOrderHelper(current, first);
    cout << endl;
}
template<class T>
void binaryTree<T>::preOrderHelper(binaryTreeNode<T> *current, bool& first) {
    if (!current) {
        return;
    }
    visit(current, first);
    first = false;
    preOrderHelper(current->leftChild, first);
    preOrderHelper(current->rightChild, first);
}

// 二叉树中序遍历
template<class T>
void binaryTree<T>::inOrder(binaryTreeNode<T>* current) {
    bool first = true;
    inOrderHelper(current, first);
    cout << endl;
}
template<class T>
void binaryTree<T>::inOrderHelper(binaryTreeNode<T> *current, bool& first) {
    if (!current) {
        return;
    }
    inOrderHelper(current->leftChild, first);
    visit(current, first);
    first = false;
    inOrderHelper(current->rightChild, first);
}

// 二叉树后序遍历
template<class T>
void binaryTree<T>::postOrder(binaryTreeNode<T>* current) {
    bool first = true;
    postOrderHelper(current, first);
    cout << endl;
}
template<class T>
void binaryTree<T>::postOrderHelper(binaryTreeNode<T> *current, bool& first) {
    if (!current) {
        return;
    }
    postOrderHelper(current->leftChild, first);
    postOrderHelper(current->rightChild, first);
    visit(current, first);
    first = false;
}

// 二叉树层序遍历
template<class T>
void binaryTree<T>::levelOrder(binaryTreeNode<T> *current) {
    queue<binaryTreeNode<T>*> q;
    q.push(current);
    while (!q.empty()) {
        binaryTreeNode<T>* node = q.front();
        q.pop();
        visit(node);
        if (node->leftChild) {
            q.push(node->leftChild);
        }
        if (node->rightChild) {
            q.push(node->rightChild);
        }
    }
}

template<class T>
void binaryTree<T>::createTree(const char* str, int size) {
    if (size <= 0) {
        origin = NULL;
        return;
    }
    vector<binaryTreeNode<T>*> nodes(size);
    for (int i = 0; i < size; i++) {
        if (str[i] == '#') {
            nodes[i] = NULL;
        }else {
            nodes[i] = new binaryTreeNode<T>(str[i]);
        }
    }

    for (int i = 0; i < size; i++) {
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        if (l < size) {
            nodes[i]->leftChild = nodes[l];
        }
        if (r < size) {
            nodes[i]->rightChild = nodes[r];
        }
    }
    origin = nodes[0];
}

template<class T>
int binaryTree<T>::numberOfNodes(binaryTreeNode<T> *current) {
    if (!current) {
        return 0;
    }
    return 1 + numberOfNodes(current->leftChild) + numberOfNodes(current->rightChild);
}

template <class T>
int binaryTree<T>::height(binaryTreeNode<T> *current) {
    if (!current) {
        return 0;
    }
    int leftHeight = height(current->leftChild);
    int rightHeight = height(current->rightChild);
    if (leftHeight > rightHeight) {
        return ++leftHeight;
    }else {
        return ++rightHeight;
    }
}

template<class T>
void createMapOfInOrder(vector<T>* inOrder, unordered_map<T, int> &idx) {
    for (int i = 0; i < inOrder->size(); i++) {
        idx[(*inOrder)[i]] = i;
    }
}

template<class T>
void binaryTree<T>::createTreeByPreAndIn(const char* pre, const char* in) {
    vector<T> preSequence(pre, pre+strlen(pre));
    vector<T> inSequence(in, in+strlen(in));
    unordered_map<T, int> idx;
    createMapOfInOrder(&inSequence, idx);
    origin = this->buildTreeByPreAndIn(&preSequence, &inSequence, &idx, 0, static_cast<int>(preSequence.size() - 1), 0, static_cast<int>(inSequence.size() - 1));
}

template<class T>
void binaryTree<T>::createTreeByPostAndIn(const char* post, const char* in) {
    vector<T> postSequence(post, post+strlen(post));
    vector<T> inSequence(in, in+strlen(in));
    unordered_map<T, int> idx;
    createMapOfInOrder(&inSequence, idx);
    origin = this->buildTreeByPostAndIn(&postSequence, &inSequence, &idx, 0, static_cast<int>(postSequence.size() - 1), 0, static_cast<int>(inSequence.size() - 1));
}

template<class T>
binaryTreeNode<T> *binaryTree<T>::buildTreeByPreAndIn(vector<T>* preOrder, vector<T>* inOrder, unordered_map<T, int>* key, int preL, int preR, int inL, int inR) {
    if (preL > preR) {
        return nullptr;
    }
    T rootVal = (*preOrder)[preL];
    int rootIndex = (*key)[rootVal];
    binaryTreeNode<T> *current = new binaryTreeNode<T>(rootVal);
    current->leftChild = buildTreeByPreAndIn(preOrder, inOrder, key, preL + 1, rootIndex + preL - inL, inL, rootIndex - 1);
    current->rightChild = buildTreeByPreAndIn(preOrder, inOrder, key, rootIndex + preL - inL + 1, preR, rootIndex + 1, inR);
    return current;
}
template<class T>
binaryTreeNode<T> *binaryTree<T>::buildTreeByPostAndIn(vector<T> *postOrder, vector<T> *inOrder, unordered_map<T, int> *key, int postL, int postR, int inL, int inR) {
    if (postL > postR) {
        return nullptr;
    }
    T rootVal = (*postOrder)[postR];
    int rootIndex = (*key)[rootVal];
    binaryTreeNode<T> *current = new binaryTreeNode<T>(rootVal);
    current->leftChild = buildTreeByPostAndIn(postOrder, inOrder, key, postL, rootIndex + postL - inL - 1, inL, rootIndex - 1);
    current->rightChild = buildTreeByPostAndIn(postOrder, inOrder, key, rootIndex + postL - inL, postR - 1, rootIndex + 1, inR);
    return current;
}



#endif
