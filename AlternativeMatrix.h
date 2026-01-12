//
// Created by 霍华德 on 2025/10/4.
//
#pragma once
#ifndef ALTERNATIVEMATRIX_H
#define ALTERNATIVEMATRIX_H
#include <exception>
#include <iostream>
#include <iomanip>
using namespace std;

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

class matrixIndexOutOfBounds : public std::exception {
    const char* what() const noexcept override { return "Matrix index out of bounds"; }
};

template <class T>
class alternativeMatrix;

template <class T>
ostream& operator <<(ostream&, const alternativeMatrix<T> &);

template <class T>
istream& operator >>(istream&, alternativeMatrix<T> &);

template <class T>
class alternativeMatrix {
public:
    friend ostream& operator << <T>(ostream&, const alternativeMatrix<T> &);
    friend istream& operator >> <T>(istream&, alternativeMatrix<T> &);
    virtual ~alternativeMatrix() {};
    virtual T get(int, int) const = 0;
    virtual void set(int, int, const T&) = 0;

private:
    virtual void print(ostream &) const = 0;
    virtual void scan(istream &) = 0;
};

template <class T>
ostream& operator <<(ostream& out, const alternativeMatrix<T> &m) {
    m.print(out);
    return out;
}

template <class T>
istream& operator >>(istream& in, alternativeMatrix<T> &m) {
    m.scan(in);
    return in;
}

//对角矩阵
template<class T>
class diagonalMatrix:public alternativeMatrix<T>{
public:
    diagonalMatrix(int theN = 10);
    ~diagonalMatrix() { delete [] element; }
    T get(int, int) const;
    void set(int, int, const T&);
private:
    int n;
    T *element;
    void print(ostream &) const;
    void scan(istream &);
};

template<class T>
diagonalMatrix<T>::diagonalMatrix(int theN) {
    if (theN < 1) {
        throw illegalParameterValue("Matrix size must be > 0");
    }
    n = theN;
    element = new T [n];
}

//只映射主对角线元素
template<class T>
T diagonalMatrix<T>::get(int i, int j) const {
    if (i < 1 || j < 1 || i > n || j > n) {
        throw matrixIndexOutOfBounds();
    }
    if (i == j) {
        return element[i - 1];
    }else {
        return 0;
    }
}

template<class T>
void diagonalMatrix<T>::set(int i, int j, const T& newValue) {
    if (i < 1 || j < 1 || i > n || j > n) {
        throw matrixIndexOutOfBounds();
    }
    if (i == j) {
        element[i - 1] = newValue;
    }else {
        if (newValue != 0) {
            throw illegalParameterValue("Nondiagonal elements must be zero!");
        }
    }
}

template<class T>
void diagonalMatrix<T>::print(ostream &out) const {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (i == j) {
                out << setw(6) << element[i - 1];
            }else {
                out << setw(6) << 0;
            }
        }
        out << endl;
    }
}

template<class T>
void diagonalMatrix<T>::scan(istream &in) {
    for (int i = 1; i <= n; i++) {
        in >> element[i - 1];
    }
}


//三对角矩阵
template<class T>
class tridiagonalMatrix:public alternativeMatrix<T> {
public:
    tridiagonalMatrix(int theN = 10);
    ~tridiagonalMatrix() { delete [] element; }
    T get(int, int) const;
    void set(int, int, const T&);

private:
    int n;
    T *element;
    void print(ostream &) const;
    void scan(istream &);
};

template<class T>
tridiagonalMatrix<T>::tridiagonalMatrix(int theN) {
    if (theN < 1) {
        throw illegalParameterValue("Matrix size must be > 0");
    }
    n = theN;
    element = new T [3 * n - 2];
}

//element[]采用逐条对角线映射，即先映射下对角线，再映射主对角线，最后映射上对角线。
template<class T>
T tridiagonalMatrix<T>::get(int i, int j) const {
    if (i < 1 || j < 1 || i > n || j > n) {
        throw matrixIndexOutOfBounds();
    }
    switch (i - j) {
        case 1:     //下对角线
            return element[i - 2];
        case 0:     //主对角线
            return element[n + i - 2];
        case -1:    //上对角线
            return element[2 * n + i - 2];
        default:
            return 0;
    }
}

template<class T>
void tridiagonalMatrix<T>::set(int i, int j, const T& newValue) {
    if (i < 1 || j < 1 || i > n || j > n) {
        throw matrixIndexOutOfBounds();
    }
    switch (i - j) {
        case 1:     //下对角线
            element[i - 2] = newValue;
            break;
        case 0:     //主对角线
            element[n + i - 2] = newValue;
            break;
        case -1:    //上对角线
            element[2 * n + i - 2] = newValue;
            break;
        default:
            throw illegalParameterValue("Elements not in triangular matrix must be zero!");
    }
}

template<class T>
void tridiagonalMatrix<T>::print(ostream &out) const {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            switch (i - j) {
                case 1:     //下对角线
                    out << setw(6) << element[i - 2];
                    break;
                case 0:     //主对角线
                    out << setw(6) << element[n + i - 2];
                    break;
                case -1:    //上对角线
                    out << setw(6) << element[2 * n + i - 2];
                    break;
                default:
                    out << setw(6) << 0;
            }
        }
        out << endl;
    }
}

template<class T>
void tridiagonalMatrix<T>::scan(istream &in) {
    for (int i = 0; i < 3 * n - 2; i++) {
        in >> element[i];
    }
}


//下三角矩阵
template <class T>
class lowerTriangularMatrix:public alternativeMatrix<T> {
public:
    lowerTriangularMatrix(int theN = 10);
    ~lowerTriangularMatrix() { delete [] element; }

    T get(int, int) const;
    void set(int, int, const T&);

private:
    int n;
    T *element;
    void print(ostream &) const;
    void scan(istream &);
};

template<class T>
lowerTriangularMatrix<T>::lowerTriangularMatrix(int theN) {
    if (theN < 1) {
        throw illegalParameterValue("Matrix size must be > 0");
    }
    n = theN;
    element = new T [n * (n + 1) / 2];
}

template<class T>
T lowerTriangularMatrix<T>::get(int i, int j) const {
    if (i < 1 || j < 1 || i > n || j > n) {
        throw matrixIndexOutOfBounds();
    }else if (i < j) {
        return 0;
    }else {
        return element[i * (i - 1) / 2 + j - 1];
    }
}

template <class T>
void lowerTriangularMatrix<T>::set(int i, int j, const T& newValue) {
    if (i < 1 || j < 1 || i > n || j > n) {
        throw matrixIndexOutOfBounds();
    }
    if (i >= j) {
        element[i * (i - 1) / 2 + j - 1] = newValue;
    }else {
        throw illegalParameterValue("Elements not in lower triangle must be zero!");
    }
}

template<class T>
void lowerTriangularMatrix<T>::print(ostream &out) const{
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (i >= j) {
                out << setw(6) << element[i * (i - 1) / 2 + j - 1];
            }else {
                out << setw(6) << 0;
            }
        }
        out << endl;
    }
}

template<class T>
void lowerTriangularMatrix<T>::scan(istream &in) {
    for (int i = 0; i < n * (n + 1) / 2; i++) {
        in >> element[i];
    }
}


//对称矩阵
template <class T>
class symmetricMatrix:public alternativeMatrix<T> {
public:
    symmetricMatrix(int theN = 10);
    ~symmetricMatrix() { delete [] element; }

    T get(int, int) const;
    void set(int, int, const T&);
private:
    int n;
    T *element;
    void print(ostream &) const;
    void scan(istream &);
};

//用下三角矩阵来存储
template<class T>
symmetricMatrix<T>::symmetricMatrix(int theN) {
    if (theN < 1) {
        throw illegalParameterValue("Matrix size must be > 0");
    }
    n = theN;
    element = new T [n * (n + 1) / 2];
}

template <class T>
T symmetricMatrix<T>::get(int i, int j) const {
    if (i < 1 || j < 1 || i > n || j > n) {
        throw matrixIndexOutOfBounds();
    }else if (i < j) {
        return element[j * (j - 1) / 2 + i - 1];
    }else {
        return element[i * (i - 1) / 2 + j - 1];
    }
}

template <class T>
void symmetricMatrix<T>::set(int i, int j,const T& newValue) {
    if (i < 1 || j < 1 || i > n || j > n) {
        throw matrixIndexOutOfBounds();
    }else if (i < j) {
        element[j * (j - 1) / 2 + i - 1] = newValue;
    }else {
        element[i * (i - 1) / 2 + j - 1] = newValue;
    }
}

template<class T>
void symmetricMatrix<T>::print(ostream &out) const {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (i >= j) {
                out << setw(6) << element[i * (i - 1) / 2 + j - 1];
            }else {
                out << setw(6) << element[j * (j - 1) / 2 + i - 1];
            }
        }
        out << endl;
    }
}

template<class T>
void symmetricMatrix<T>::scan(istream &in) {
    for (int i = 0; i < n * (n + 1) / 2; i++) {
        in >> element[i];
    }
}

#endif
