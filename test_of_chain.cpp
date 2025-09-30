//
// Created by 霍华德 on 2025/9/29.
//
#include<iostream>
#include "LinearList.h"
using namespace std;

int main () {
    /*  输入测试数据(不要复制)：
     *  5
     *  1 2 3 4 5
     */
    chain<int> a;
    int n;
    cout << "请输入a链表元素个数" << endl;
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> a;
    }
    cout << "链表a为：" << endl;
    cout << a << endl;

    //
    cout << "a.empty()的取值为：" << a.empty() << endl;
    cout << "a是否为空："<< endl;
    if (a.empty()) {
        cout << "a是空的" << endl;
    }else {
        cout << "a非空，" << "且a的长度为" << a.size() << endl;
    }

    //
    cout << "链表下标为0的节点中存储的元素为：" << a.get(0) << endl;
    cout << "链表下标为3的节点中存储的元素为：" << a.get(3) << endl;
    try {
        cout << a.get(10) << endl;
    }catch (illegalIndex) {
        cout << "抛出访问非法链表索引异常测试成功！" << endl;
    }

    if (a.indexOf(4) == -1) {
        cout << "该链表中不存在所查元素" << 4 << endl;
    }else {
        cout << "该节点存在，且链表中存储元素为4的节点的索引值为：" << a.indexOf(4) << endl;
    }

    if (a.indexOf(100) == -1) {
        cout << "该链表中不存在所查元素" << 100 << endl;
    }else {
        cout << "该节点存在，且链表中存储元素为4的节点的索引值为：" << a.indexOf(100) << endl;
    }

    //
    cout << a << endl;
    a.insert(0, -10);
    cout << a << endl;
    a.insert(3, 100);
    cout << a << endl;
    a.insert(7, -7);
    cout << a << endl;
    try {
        a.insert(100, 100);
    }catch (illegalInsertIndex) {
        cout << "抛出非法插入节点异常测试成功！" << endl;
    }


    return 0;
}