//
// Created by 霍华德 on 2025/9/26.
//
#include <iostream>
using namespace std;

//先定义常见异常类型
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

class matrixIndexOutOfBounds : public std::exception {
    const char* what() const noexcept override { return "Matrix index out of bounds"; }
};

class matrixSizeMismatch: public std::exception {
    const char* what() const noexcept override { return "Matrix's size is not match."; }
};

template<class T>
class matrix {
    friend ostream& operator << (ostream&, const matrix<T>&);
    friend istream& operator >> (istream&, const matrix<T>&);
public:
    matrix(int theRows = 0, int theColumns = 0);
    matrix(const matrix<T>&);
    ~matrix() {delete [] element;}
    int rows() const {return theRows;}
    int columns() const {return theColumns;}
    T& operator()(int i, int j) const;
    matrix<T>& operator=(const matrix<T>&);
    matrix<T> operator+() const;
    matrix<T> operator+(const matrix<T>&) const;
    matrix<T> operator-() const;
    matrix<T> operator-(const matrix<T>&) const;
    matrix<T> operator*(const matrix<T>&) const;
    matrix<T>& operator+=(const T&);

private:
    int theRows, theColumns;
    T *element;
};

template<class T>
matrix<T>::matrix(int theRows, int theColumns) {
    if (theRows < 0 || theColumns < 0) {
        throw illegalParameterValue("Rows and columns must be >= 0");
    }else if ((theRows == 0 || theColumns == 0) && (theRows != 0 || theColumns != 0)){
        throw illegalParameterValue("Either both or neither rows and columns should be zero");
    }
    this->theRows = theRows;
    this->theColumns = theColumns;
    element = new T [theRows * theColumns];
}

template<class T>
matrix<T>::matrix(const matrix<T>& m) {
    theRows = m.theRows;
    theColumns = m.theColumns;
    element = new T [theRows * theColumns];
    copy(m.element,m.element + theRows * theColumns, element);
}

template<class T>
matrix<T>& matrix<T>::operator=(const matrix<T>& m) {
    if (this != &m) {
        delete[] element;
        theRows = m.theRows;
        theColumns = m.theColumns;
        element = new T [theRows * theColumns];
        copy(m.element, m.element + theColumns * theRows, element);
    }
    return *this;
}

template<class T>
T& matrix<T>::operator()(int i, int j) const {
    if (i < 1 || i > theRows || j < 1 || j > theColumns) {
        throw matrixIndexOutOfBounds();
    }
    return element[(i - 1) * theColumns + j -1];
}

template<class T>
matrix<T> matrix<T>::operator+(const matrix<T>& m) const {
    if (theRows != m.theRows || theColumns != m.theColumns) {
        throw matrixSizeMismatch();
    }
    matrix<T> w(theRows, theColumns);
    for (int i = 0; i < theRows * theColumns; i++) {
        w.element[i] = element[i] + m.element[i];
    }
    return w;
}

template<class T>
matrix<T> matrix<T>::operator*(const matrix<T> &m) const {
    if (theColumns != m.theRows) {
        throw matrixSizeMismatch();
    }
    //创建矩阵w用于盛放结果矩阵
    matrix<T> w(theRows, m.theColumns);
    int ct = 0, cm = 0, cw = 0;
    for (int i = 1; i <= theRows; i++) {
        for (int j = 1; j <= m.theColumns; j++) {
            //初始化sum为第i行第1个元素与m矩阵第j列第1个元素的乘积
            T sum = element[ct] * m.element[cm];
            for (int k = 2; k <= theColumns; k++) {
                ct++;       //原矩阵的映射值每次只需要+1，因为该矩阵是行主映射
                cm += m.theColumns;     //m矩阵的映射值每次要加列数，使得所取元素移动到下一行
                sum += element[ct] * m.element[cm];     //原矩阵的第i行的第k个元素乘以m矩阵的第j列的第k个元素
            }
            w.element[cw++] = sum;      //其实cw就是(i - 1) * theColumns + j
            ct -= theColumns - 1;       //ct回到原矩阵这一行的第一个元素
            cm = j;     //m矩阵取下一列的第一个元素
        }
        ct += theColumns;       //原矩阵取下一行的第一个元素
        cm = 0;     //m矩阵取第一列的第一个元素
    }
    return w;
}

template<class T>
matrix<T> matrix<T>::operator+() const {
    return this;
}

template<class T>
matrix<T> matrix<T>::operator-() const {
    for (int i = 0; i < theColumns * theRows; i++) {
        element[i] = -element[i];
    }
    return *this;
}

template<class T>
matrix<T> matrix<T>::operator-(const matrix<T>& m) const {
    matrix<T> w(theRows, theColumns);
    if (theRows != m.theRows || theColumns != m.theColumns) {
        throw matrixSizeMismatch();
    }
    for (int i = 0; i < theRows * theColumns; i++) {
        w.element[i] = element[i] - m.element[i];
    }
    return w;
}


template<class T>
matrix<T>& matrix<T>::operator+=(const T& t) {
    for (int i = 0; i < theRows * theColumns; i++) {
        element[i] += t;
    }

    return *this;
}

template<class T>
ostream& operator<<(ostream &out, matrix<T>& m) {
    for (int i = 1; i <= m.rows(); i++) {
        for (int j = 1; j <= m.columns(); j++) {
            if (j == m.columns()) {
                out << m(i, j);
            }else {
                out << m(i, j) << "\t";
            }
        }
        out << "\n";
    }
    return out;
}

template<class T>
istream& operator>>(istream& in, matrix<T>& m) {
    // char space0 [] = "";
    // char space1 [] = " ";
    // char space2 [] = "  ";
    // char space3 [] = "   ";
    // char space4 [] = "    ";
    // char space5 [] = "     ";
    for (int i = 1; i <= m.rows(); ++i)
        for (int j = 1; j <= m.columns(); ++j)
            in >> m(i, j);          // 用你已经写好的 operator()
    return in;
}

int main() {
    matrix<int> m1(4, 3);
    cin >> m1;
    cout << m1;

    return 0;
}