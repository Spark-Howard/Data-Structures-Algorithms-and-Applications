//
// Created by 霍华德 on 2025/10/10.
//
#include <iostream>
#include <iomanip>
using namespace std;
template <class T>
class Stack {
public:
    virtual ~stack() {}
    virtual bool empty() const = 0;
    virtual int size() const = 0;
    virtual T& top() = 0;
    virtual void pop() = 0;
    virtual void push(const T& theElement) = 0;
};

template <class T>
class derivedArrayStack : private arrayList<T>