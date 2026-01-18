//
// Created by 霍华德 on 2026/1/18.
//
#pragma once
#ifndef DATA_STRUCTURES_ALGORITHMS_AND_APPLICATIONS_MINHEAP_H
#define DATA_STRUCTURES_ALGORITHMS_AND_APPLICATIONS_MINHEAP_H
#include <iostream>
#include <algorithm>
#include <sstream>
using namespace std;

class heapEmpty : public std::exception {
    const char* what() const noexcept override { return "The heap is empty!"; }
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
void changeLength(T*& a, int oldLength, int newLength) {
    if (newLength < 0)
        throw illegalParameterValue("new length must be >= 0");
    T* temp = new T[newLength];
    int number = min(oldLength, newLength);
    copy(a, a + number, temp);
    delete [] a;
    a = temp;
}

template<class T>
struct binaryTreeNode {
    binaryTreeNode<T>* leftChild;
    binaryTreeNode<T>* rightChild;
    T element;
    binaryTreeNode<T>() {
        leftChild = rightChild = nullptr;
    }
    binaryTreeNode<T>(const T& theElement) {
        element = theElement;
        leftChild = rightChild = nullptr;
    }
    binaryTreeNode<T>(const T& theElement, binaryTreeNode<T>* left, binaryTreeNode<T>* right) {
        element = theElement;
        leftChild = left;
        rightChild = right;
    }
};

template<class T>
class MinHeap {
public:
    MinHeap(int initialCapacity = 10);
    bool empty() const { return heapSize == 0; }
    void initialize(T* theHeap, int theSize);
    void push(const T&);
    void pop();
    T& top();
    void print();
    int arrayLength;

private:
    void printTree(T arr[], int arrayLength, int curIdx, int indent);
    int heapSize;

    T* heap;
};

template<class T>
MinHeap<T>::MinHeap(int initialCapacity) {
    if (initialCapacity < 1) {
        ostringstream s;
        s << "Initial capacity = " << initialCapacity << " Must be > 0";
        throw illegalParameterValue(s.str());
    }
    arrayLength = initialCapacity + 1;
    heap = new T[arrayLength];
    heapSize = 0;
}
template<class T>
void MinHeap<T>::push(const T& theElement) {
    if (heapSize == arrayLength - 1) {
        changeLength(heap, arrayLength, 2 * arrayLength);
        arrayLength *= 2;
    }
    int currentNode = ++heapSize;
    while (currentNode != 1 && heap[currentNode / 2] > theElement) {
        heap[currentNode] = heap[currentNode / 2];
        currentNode /= 2;
    }
    heap[currentNode] = theElement;
}

template<class T>
void MinHeap<T>::pop() {
    if (empty()) {
        throw heapEmpty();
    }
    T lastElement = heap[heapSize--];

    int currentNode = 1;
    int child = 2;
    while (child <= heapSize) {
        if (child < heapSize && heap[child] > heap[child + 1]) {
            child++;
        }
        if (lastElement <= heap[child]) {
            break;
        }
        heap[currentNode] = heap[child];
        currentNode = child;
        child *= 2;
    }
    heap[currentNode] = lastElement;
    const int MIN_CAPACITY = 11;
    if (heapSize <= (arrayLength - 1) / 4 && arrayLength / 2 >= MIN_CAPACITY) {
        changeLength(heap, arrayLength, arrayLength / 2);
        arrayLength /= 2;
    }
}

template<class T>
void MinHeap<T>::initialize(T *theHeap, int theSize) {
    delete [] heap;
    heap = new T[theSize + 1];
    copy(theHeap, theHeap + theSize + 1, heap);
    heapSize = theSize;
    arrayLength = heapSize + 1;

    for (int root = heapSize / 2; root >= 1; root--) {
        T rootElement = heap[root];

        int child = 2 * root;
        while (child <= heapSize) {
            if (child < heapSize && heap[child] > heap[child + 1]) {
                child++;
            }
            if (rootElement <= heap[child]) {
                break;
            }
            heap[child / 2] = heap[child];
            child *= 2;
        }
        heap[child / 2] = rootElement;
    }
}

template<class T>
T& MinHeap<T>::top() {
    if (empty()) {
        throw heapEmpty();
    }
    return heap[1];
}

template<class T>
void MinHeap<T>::printTree(T arr[], int arrLength, int curIdx, int indent) {
    if (curIdx > arrLength) {
        return;
    }
    for (int i = 0; i < indent; i++) {
        cout << " ";
    }
    cout << arr[curIdx] << endl;
    printTree(arr, arrLength, 2 * curIdx, indent + 4);
    printTree(arr, arrLength, 2 * curIdx + 1, indent + 4);
}

template<class T>
void MinHeap<T>::print() {
    printTree(heap, heapSize, 1, 0);
}

#endif //DATA_STRUCTURES_ALGORITHMS_AND_APPLICATIONS_MINHEAP_H