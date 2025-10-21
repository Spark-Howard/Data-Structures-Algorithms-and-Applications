//
// Created by 霍华德 on 2025/10/11.
//
#include<iostream>
#include "LinearList.h"
using namespace std;

int main() {
    arrayList<int> a(5);
    cout << "请输入数组a，输入除数字外其他字符并回车后自动终止";
    cin >> a;
    cout << a;
    return 0;
}