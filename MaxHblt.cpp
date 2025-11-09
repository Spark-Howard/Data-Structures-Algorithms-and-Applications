//
// Created by 霍华德 on 2025/11/9.
//
#include<iostream>
#include<queue>
using namespace std;

class treeEmpty : public std::exception {
    const char* what() const noexcept override{ return "The tree is empty!"; }
};

template <class T>
struct binaryTreeNode {
    T element;
    binaryTreeNode<T> *leftChild;
    binaryTreeNode<T> *rightChild;
    binaryTreeNode(const T& theElement) {
        element = theElement;
        leftChild = NULL;
        rightChild = NULL;
    }
    binaryTreeNode(const T& theElement, binaryTreeNode<T>* l, binaryTreeNode<T>* r) {
        element = theElement;
        leftChild = l;
        rightChild = r;
    }
};

/// 最大高度优先左高树(maxHblt)满足:
/// 1. 对树中所有节点，左子树根节点的s(x)大于等于右子树根节点；
/// 2. 且整棵树满足大根树的性质，即所有根节点的数据大于等于所有其左右子节点的数据值
template <class T>
class maxHblt {
public:
    maxHblt() {
        root = NULL;
        treeSize = 0;
    }
    bool empty() const { return treeSize == 0; }
    T front() { return root->element->second; }
    void initialize(T* theElements, int theSize);
    void meld(maxHblt<T>& theHblt);
    void push(const T&);
    void pop();


private:
    void meld(binaryTreeNode<pair<int, T>>* &x,
              binaryTreeNode<pair<int, T>>* &y);
    binaryTreeNode<pair<int, T>>* root;     //前项表示s(x)，后项存储该节点的元素值
    int treeSize;

};

template<class T>
void maxHblt<T>::meld(binaryTreeNode<pair<int, T>>* &x, binaryTreeNode<pair<int, T>>* &y) {
    if (y == NULL) {
        return;
    }
    if (x == NULL) {
        x = y;
        return;
    }
    /* 始终让左高树以x为根，这里是要保证x根节点存储的数据一定要比y根节点存储的数据要大，
     * 因为下一步一定是让根节点小的树与根节点大的树的右子树合并，进而形成递归
     */
    if (x->element.second < y->element.second) {
        swap(x,y);
    }
    meld(x->rightChild,y);

    //递归结束后对左高树的形状以及所有节点的s(x)进行维护
    //if分支是在对末端内部节点进行维护，因为右子树一定不为空，所以没有讨论
    if (x->leftChild == NULL) {
        x->leftChild = x->rightChild;
        x->rightChild = NULL;
        x->element.first = 1;
    }else {
        if (x->leftChild->element.first < x->rightChild->element.first) {
            swap(x->rightChild, x->rightChild);
        }
        x->element.first = x->rightChild->element.first + 1;    //右子树根节点的s(x)一定小于等于左子树根节点的s(x)
    }
}

//创建n棵只有1个节点的树，装入队列中，依次两两合并，合并(n-1)次最终只剩一棵树
template<class T>
void maxHblt<T>::initialize(T *theElements, int theSize) {
    queue<binaryTreeNode<pair<int, T>> *> q(theSize);
    delete root;

    for (int i = 1; i <= theSize; i++) {
        q.push(binaryTreeNode<pair<int, T>> (pair(1,theElements[i])));
    }
    for (int i = 1; i <= theSize - 1; i++) {
        binaryTreeNode<pair<int, T>> *node1 = q.front();
        q.pop();
        binaryTreeNode<pair<int, T>> *node2 = q.front();
        q.pop();
        meld(node1, node2);
        q.push(node1);
    }
}

template<class T>
void maxHblt<T>::meld(maxHblt<T>& theHblt) {
    meld(root, theHblt.root);
    treeSize += theHblt.treeSize;
    theHblt.root = NULL;
    theHblt.treeSize = 0;
}
template<class T>
void maxHblt<T>::push(const T& theElement) {
    binaryTreeNode<pair<int, T>> *q = new binaryTreeNode<pair<int, T>>(pair<int, T>(1,theElement));
    meld(root, q);
    treeSize++;
}
template<class T>
void maxHblt<T>::pop() {
    if (root == NULL) {
        throw treeEmpty();
    }
    binaryTreeNode<pair<int, T>> *left = root->leftChild;
    binaryTreeNode<pair<int, T>> *right = root->rightChild;
    delete root;
    root = left;
    meld(left, right);
    treeSize--;
}