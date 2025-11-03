//
// Created by 霍华德 on 2025/10/26.
//
#pragma once
#ifndef QUEUE_H
#include<iostream>

#include "LinearList.h"
using namespace std;

template<class T>
class myQueue {
public:
    virtual ~myQueue() {}
    virtual bool empty() const = 0;
    virtual int size() const = 0;
    virtual T& front() = 0;
    virtual T& back() = 0;
    virtual void pop() = 0;
    virtual void push(const T& theElement) = 0;
};

template<class T>
class linkedQueue : public myQueue<T> {
public:
    linkedQueue() {
        frontNode = backNode = NULL;
        queueSize = 0;
    }
    ~linkedQueue();
    bool empty() const { return frontNode == NULL; }
    int size() const { return queueSize; }
    T& front() { return frontNode; }
    T& back() { return backNode; }
    void pop();
    void push(const T&);

private:
    chainNode<T>* frontNode;
    chainNode<T>* backNode;
    int queueSize;
};

template<class T>
linkedQueue<T>::~linkedQueue() {
    while (frontNode != NULL) {
        chainNode<T> *nextNode = new chainNode<T>(frontNode->next);
        delete frontNode;
        frontNode = nextNode;
        queueSize--;
    }
}

template<class T>
void linkedQueue<T>::pop() {
    chainNode<T> *nextNode = new chainNode<T>(frontNode->next);
    delete frontNode;
    frontNode = nextNode;
    queueSize--;
}
template<class T>
void linkedQueue<T>::push(const T& theElement) {
    chainNode<T> *newNode = new chainNode<T>(theElement, NULL);
    if (queueSize == 0) {
        frontNode = backNode = newNode;
    }else {
        backNode->next = newNode;
        backNode = newNode;
    }
    queueSize++;
}

#endif
