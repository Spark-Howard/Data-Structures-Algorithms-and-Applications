//
// Created by 霍华德 on 2025/10/10.
//
#pragma once
#ifndef STACK_H
#define STACK_H
#include <iostream>
#include <iomanip>
#include "LinearList.h"
using namespace std;

class stackEmpty : public std::exception {
    const char* what() const noexcept override{ return "The stack is empty!"; }
};
class illegalParameterValue : public std::exception {
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

template <class T>
class myStack {
public:
    virtual ~myStack() {}
    virtual bool empty() const = 0;
    virtual int size() const = 0;
    virtual T& top() = 0;
    virtual void pop() = 0;
    virtual void push(const T& theElement) = 0;
};

template <class T>
class derivedArrayStack : private arrayList<T>,
                          public myStack<T>
{
public:
    derivedArrayStack(int initialCapacity = 10)
        : arrayList<T> (initialCapacity) {}
    bool empty() const { return arrayList<T>::empty(); }
    int size() const { return arrayList<T>::size(); }
    T& top() {
        try {
            return get(arrayList<T>::size() - 1);
        } catch (illegalIndex){
            throw stackEmpty();
        }
    }
    void pop() {
        try {
            erase(arrayList<T>::size() - 1);
        } catch (illegalIndex) {
            throw stackEmpty();
        }
    }
    void push(const T& theElement) {
        insert(arrayList<T>::size(), theElement);
    }
};

template<class T>
class arrayStack : public myStack<T> {
public:
    arrayStack(int initialCapacity = 10);
    ~arrayStack() { delete [] stack; }
    bool empty() const { return stackTop == -1; }
    int size() const {
        return stackTop + 1;
    }
    T& top() {
        if (stackTop == -1) {
            throw stackEmpty();
        }
        return stack[stackTop];
    }
    void pop() {
        if (stackTop == -1) {
            throw stackEmpty();
        }
        stack[stackTop--].~T();
    }
    void push(const T& theElement);
private:
    int stackTop;   //标记栈顶元素的索引，若为-1表示为空栈
    int arrayLength;
    T *stack;
};

template<class T>
arrayStack<T>::arrayStack(int initialCapacity) {
    if (initialCapacity < 1) {
        ostringstream s;
        s << "Initial capacity = " << initialCapacity << "Must be > 0";
        throw illegalParameterValue(s.str());
    }
    arrayLength = initialCapacity;
    stack = new T[arrayLength];
    stackTop = -1;
}

template<class T>
void arrayStack<T>::push(const T& theElement) {
    if (stackTop == arrayLength - 1) {
        changeLength(stack, arrayLength, 2*arrayLength);
        arrayLength *= 2;
    }
    stack[++stackTop] = theElement;
}

template<class T>
class linkedStack : public myStack<T> {
public:
    linkedStack(int initialCapacity = 10) {
        stackTop = NULL;
        stackSize = 0;
    }
    ~linkedStack();
    bool empty() const { return stackSize == 0; }
    int size () { return stackSize; }
    T& top() {
        if (stackSize == 0) {
            throw stackEmpty();
        }
        return stackTop->element;
    }
    void pop();
    void push(const T& theElement) {
        stackTop = new chainNode<T>(theElement, stackTop);
        stackSize++;
    }

private:
    chainNode<T>* stackTop;
    int stackSize;
};

template<class T>
linkedStack<T>::~linkedStack() {
    while (stackTop != NULL) {
        chainNode<T> *nextNode = stackTop->next;
        delete stackTop;
        stackTop = nextNode;
    }
}

template<class T>
void linkedStack<T>::pop() {
    if (stackSize == 0) {
        throw stackEmpty();
    }
    chainNode<T> *nextNode = stackTop->next;
    delete stackTop;
    stackTop = nextNode;
    stackSize--;
}

#endif