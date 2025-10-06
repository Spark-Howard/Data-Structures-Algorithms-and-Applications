//
// Created by 霍华德 on 2025/10/5.
//
#include <iostream>

#include "AlternativeMatrix.h"

int main() {
    //测试创建对角矩阵
    diagonalMatrix<int> m1(5);
    //测试输入和输出对角矩阵
    cin >> m1;
    cout << m1;
    //测试get函数
    cout << m1.get(2,2) << endl;
    cout << m1.get(3,4) << endl;
    //测试set函数
    try {
        m1.set(0,4,10);
    }catch (matrixIndexOutOfBounds) {
        cout << "set函数成功抛出矩阵越界异常" << endl;
    }
    try {
        m1.set(4,3,10);
    }catch (illegalParameterValue) {
        cout << "set函数成功抛出错误索引异常" << endl;
    }
    m1.set(2,2,10);
    cout << m1;
    cout << m1.get(2,2) << endl;

    //测试三对角矩阵
    tridiagonalMatrix<int> m2(6);
    //测试输入和输出三对角矩阵
    cin >> m2;
    cout << m2;
    //测试get函数
    cout << m2.get(2,2) << endl;
    cout << m2.get(3,4) << endl;
    cout << m2.get(5,3) << endl;
    //测试set函数
    try {
        m2.set(-2,1,8);
    }catch (matrixIndexOutOfBounds) {
        cout << "set函数抛出矩阵越界异常成功" << endl;
    }
    try {
        m2.set(5,3,7);
    }catch (illegalParameterValue) {
        cout << "set函数抛出错误索引异常成功" << endl;
    }
    m2.set(2,2,9);
    cout << m2;
    cout << m2.get(2,2) << endl;

    //测试创建下三角矩阵
    lowerTriangularMatrix<int> m3(5);
    //测试输入和输出下三角矩阵
    cin >> m3;
    cout << m3;
    //测试get函数
    cout << m3.get(2,2) << endl;
    cout << m3.get(3,4) << endl;
    cout << m3.get(5,3) << endl;
    //测试set函数
    try {
        m3.set(5,-1,7);
    }catch (matrixIndexOutOfBounds) {
        cout << "set函数抛出矩阵越界异常成功" << endl;
    }
    try {
        m3.set(1,5,10);
    }catch (illegalParameterValue) {
        cout << "set函数抛出错误索引异常成功" << endl;
    }
    m3.set(2,2,9);
    cout << m3;
    cout << m3.get(2,4) << endl;
    cout << m3.get(4,2) << endl;

    //测试对称矩阵
    symmetricMatrix<int> m4(6);
    cin >> m4;
    cout << m4;

    //测试get函数
    cout << m4.get(2,2) << endl;
    cout << m4.get(3,5) << endl;
    cout << m4.get(5,3) << endl;
    //测试set函数
    try {
        m4.set(5,-1,7);
    }catch (matrixIndexOutOfBounds) {
        cout << "set函数抛出矩阵越界异常成功" << endl;
    }
    m4.set(2,4,9);
    cout << m4;
    cout << m4.get(2,4) << endl;
    cout << m4.get(4,2) << endl;
    return 0;
}