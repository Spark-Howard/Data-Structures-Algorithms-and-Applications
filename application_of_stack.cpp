//
// Created by 霍华德 on 2025/10/21.
//
#include<iostream>
#include "Stack.h"
using namespace std;

void printMatchedPairs(string expr) {
    arrayStack<int> s;
    int length = (int) expr.size();
    for (int i = 0; i < length; i++) {
        if (expr.at(i) == '(') {
            s.push(i);
        }else if (expr.at(i) == ')') {
            try {
                cout << s.top() << " " << i << endl;
            }catch (stackEmpty) {
                cout << "No match for right parenthesis." <<  " at " << i << endl;
            }
        }
    }

    while (!s.empty()) {
        cout << "No match for left parenthesis at " << s.top() << endl;
    }

}
