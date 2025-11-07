//
// Created by 霍华德 on 2025/11/7.
//
#include <iostream>
using namespace std;

class heapEmpty : public std::exception {
    const char* what() const noexcept override{ return "The heap is empty!"; }
};

class illegalParameterValue:public exception {
public:
    explicit illegalParameterValue(const std::string& msg)
        : msg_(msg) {}
    const char* what() const noexcept override {
        return msg_.c_str();
    }
private:
    std::string msg_;
};

template<class T>
struct binaryTreeNode {
    binaryTreeNode<T>* leftChild;
    binaryTreeNode<T>* rightChild;
    T element;
    binaryTreeNode<T>() {
        leftChild = rightChild = NULL;
    }
    binaryTreeNode<T>(const T& theElement) {
        element = theElement;
        leftChild = rightChild = NULL;
    }
    binaryTreeNode<T>(const T& theElement, binaryTreeNode<T>* left, binaryTreeNode<T>* right) {
        element = theElement;
        leftChild = left;
        rightChild = right;
    }
};

template<class T>
class maxHeap {
public:
    bool empty() { return heapSize == 0; }
    void push(const T&);
    void pop();

private:
    int heapSize;
    int arrayLength;
    T* heap;
};

//相当于将新插入节点放置于最后一个节点处，然后从该节点处向上起泡
template<class T>
void maxHeap<T>::push(const T& theElement) {
    //防止存放大根堆的数组长度不够
    if (heapSize == arrayLength - 1) {
        changeLength(heap, arrayLength, 2 * arrayLength);
    }
    //把"指针"指向完全二叉树的第一个空节点
    int currentNode = ++heapSize;
    //当当前"指针"指向1或者当前"指针"指向节点的父节点所存储的元素大于等于要插入元素时循环停止，该位置就是新元素的最终位置
    while (currentNode != 1 && heap[currentNode / 2] < theElement) {
        heap[currentNode] = heap[currentNode / 2];
        currentNode /= 2;
    }
    heap[currentNode] = theElement;
}

template<class T>
void maxHeap<T>::pop() {
    if (heapSize == 0) {
        throw heapEmpty();
    }
    heap[1].~T();

    T lastElement = heap[heapSize--];

    int currentNode = 1;
    int child = 2;
    //每次循环开始时确保child"指针"位于currentNode"指针"的左孩子处
    while (child <= heapSize) {
        //如果左孩子处元素小于右孩子处元素，将child"指针"移到右孩子处；如果右孩子处本来就是空的(child == heapSize)，就跳过这一步左右孩子比较
        if (child < heapSize && heap[child] < heap[child + 1]) {
            child++;
        }
        //如果虚指的最后一个元素大于等于两个子节点的元素，说明不用对元素进行交换，退出循环
        if (lastElement >= heap[child]) {
            break;
        }
        //将当前节点处赋值为子节点中较大的元素，之后对应子节点就是新的当前节点，继续下面的调整大根堆的操作
        heap[currentNode] = heap[child];
        currentNode = child;
        child *= 2;
    }
    heap[currentNode] = lastElement;
}

template<class T>
void changeLength(T*& a, int oldLength, int newLength)
{
    if (newLength < 0)
        throw illegalParameterValue("new length must be >= 0");
    T* temp = new T[newLength];
    int number = min(oldLength, newLength);
    copy(a, a + number, temp);
    delete[] a;
    a = temp;
}