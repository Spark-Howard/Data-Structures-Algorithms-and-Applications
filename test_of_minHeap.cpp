//
// Created by 霍华德 on 2026/1/19.
//
#include<iostream>
#include "MinHeap.h"
using namespace std;

int main() {
    MinHeap<int> h;
    int a[12] = {100, 4, 2, 8, 7, 10, 6, 3, 1, 11, 5, 9};
    h.initialize(a, 11);
    h.print();
    cout << h.arrayLength << endl;
    h.push(12);
    cout << h.arrayLength << endl;
    h.push(13);
    cout << h.arrayLength << endl;

    for (int i = 1; i <= 8; i++) {
        h.pop();
        cout << h.arrayLength << endl;
    }
    h.print();
    return 0;
}
