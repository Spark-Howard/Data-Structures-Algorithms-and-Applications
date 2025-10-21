//
// Created by 霍华德 on 2025/9/19.
//
#pragma once
#ifndef LINEARLIST_H
#include<iostream>
#include<sstream>
#include<vector>
using namespace std;

//定义异常
class illegalIndex : public std::exception {
    const char* what() const noexcept override { return "The index of the Node is illegal!"; }
};

class illegalInsertIndex : public std::exception {
    const char* what() const noexcept override { return "The index of the node to insert is illegal!"; }
};

template<class T>
class linearList {
public:
    virtual ~linearList() {};
    virtual bool empty() const = 0;
    virtual int size() const = 0;
    virtual T& get(int theIndex) const = 0;
    virtual int indexOf(const T& theElement) const = 0;
    virtual void erase(int theIndex) = 0;
    virtual void insert(int theIndex, const T& theElement) = 0;
    virtual void output(ostream& out) const = 0;
};

template <class T>
struct chainNode {
    T element;
    chainNode<T> *next;

    chainNode() {}      //默认构造函数
    //构造函数
    chainNode(const T& element) {
        this->element = element;
    }
    //拷贝构造函数
    chainNode(const T& element, chainNode<T>* next) {
        this->element = element;
        this->next = next;
    }
};

template <class T>
struct chainNode2 {
    T element;
    chainNode<T> *next;
    chainNode<T> *prev;

    chainNode2() {}      //默认构造函数
    //构造函数
    chainNode2(const T& element) {
        this->element = element;
    }
    //拷贝构造函数
    chainNode2(const T& element, chainNode<T>* next, chainNode<T>* prev) {
        this->element = element;
        this->next = next;
        this->prev = prev;
    }
};

template <class T>
class chain : public linearList<T> {
public:
    chain(int initialCapacity = 10);
    chain(const chain<T>&);
    ~chain();

    bool empty() const {return listSize == 0;}  //判断链表是否为空
    int size() const {return listSize;}
    T& get(int theIndex) const;     //返回指定下标值的元素
    int indexOf(const T& theElement) const; //返回指定元素的下标
    void erase(int theIndex);
    void insert(int theIndex, const T& theElement);
    void output(ostream& out) const;
    friend istream& operator>>(istream& in, chain& x) {
        int ele;
        in >> ele;
        x.insert(x.size(), ele);
        return in;
    }

    void clear();
    void push_back(const T& theElement);

protected:
    void checkIndex(int theIndex) const;
    chainNode<T>* firstNode;
    int listSize;
};

//构造函数
template<class T>
chain<T>::chain(int initialCapacity) {
    if (initialCapacity < 1) {
        cout << "Initial capacity = " << initialCapacity << " Must be > 0";
    }
    firstNode = NULL;
    listSize = 0;
}

//拷贝构造函数
template<class T>
chain<T>::chain(const chain<T>& theList) {
    listSize = theList.listSize;

    //若已知链表为空链表
    if (listSize == 0) {
        firstNode = NULL;
        return;
    }

    //定义了一个指向chainNode<T>结构体的指针变量sourceNode，并用theList.firstNode里的地址给它初始化。
    chainNode<T>* sourceNode = theList.firstNode;
    firstNode = new chainNode<T>(sourceNode->element);
    sourceNode = sourceNode->next;      //让sourceNode多走一个，sourceNode负责在老链表取值，targetNode负责从新链表上一个节点链接
    chainNode<T>* targetNode = firstNode;
    while (sourceNode != NULL) {
        targetNode->next = new chainNode<T>(sourceNode->element);
        targetNode = targetNode->next;
        sourceNode = sourceNode->next;
    }
    targetNode->next = NULL;
}

template<class T>
chain<T>::~chain() {
    while (firstNode != NULL) {
        chainNode<T>* nextNode = firstNode->next;
        delete firstNode;
        firstNode = nextNode;
    }
}

template<class T>
void chain<T>::checkIndex(int theIndex) const {
    if (theIndex < 0 || theIndex > listSize - 1) {
        throw illegalIndex();
    }
}
template<class T>
T& chain<T>::get(int theIndex) const {
    checkIndex(theIndex);       //先确认链表中存在该节点

    chainNode<T>* currentNode = firstNode;
    //取值操作一般需要进行theIndex次移动，而插入或删除操作一般进行(theIndex-1)次移动
    for (int i = 0; i < theIndex; i++) {
        currentNode = currentNode->next;
    }
    return currentNode->element;
}

template<class T>
int chain<T>::indexOf(const T& theElement) const {
    chainNode<T>* currentNode = firstNode;
    int index = 0;
    //链表到达尾部和找到特定元素都会使得循环停止
    while (currentNode != NULL && currentNode->element != theElement) {
        currentNode = currentNode->next;
        index++;
    }
    //当currentNode移动到链表尾部时说明没有找到对应元素
    if (currentNode == NULL) {
        return -1;
    }else {
        return index;
    }
}

template<class T>
void chain<T>::erase(int theIndex) {
    checkIndex(theIndex);   //查看要删除节点下标是否合理
    chainNode<T>* deleteNode;
    //删除头结点需要单独处理
    if (theIndex == 0) {
        deleteNode = firstNode;
        firstNode = firstNode->next;
    }else {
        chainNode<T>* p = firstNode;
        for (int i = 0; i < theIndex - 1; i++) {
            p = p->next;
        }
        deleteNode = p->next;
        p->next = p->next->next;
    }
    listSize--;
    delete deleteNode;
}

template<class T>
void chain<T>::insert(int theIndex, const T& theElement) {
    if (theIndex < 0 || theIndex > listSize) {
        throw illegalInsertIndex();
    }
    if (theIndex == 0) {
        //先执行右侧创建新节点操作，让头结点指向该节点
        firstNode = new chainNode<T>(theElement, firstNode);
    }else {
        chainNode<T>* p = firstNode;
        for (int i = 0; i < theIndex - 1; i++) {
            p = p->next;
        }
        p->next = new chainNode<T>(theElement, p->next);
    }
    listSize++;
}

template<class T>
void chain<T>::output(ostream& out)const {
    for (chainNode<T>* currentNode = firstNode; currentNode != NULL; currentNode = currentNode->next) {
        out << currentNode->element << "  ";
    }
}

//运算符重载，用于cout << someChain;
template<class T>
ostream& operator<<(ostream& out, const chain<T>& x) {
    x.output(out);
    return out;
}

/*使用迭代器访问第i个元素时，只有第一次需要i次操作，之后迭代器会把上一步的信息留在自己那里，之后再访问可以1步操作直接完成
 *也就是说用迭代器访问多个元素时比每次从头访问更快
 */
template<class T>
class iterator {
public:
    typedef std::forward_iterator_tag iterator_category;
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::ptrdiff_t difference_type;

    iterator(chainNode<T>* theNode = NULL) {
        node = theNode;
    }

    T& operator*() const {
        return node->element;
    }
    T* operator->() const {
        return &node->element;
    }

    iterator& operator++() {
        node = node->next;
        return *this;
    }
    iterator operator++(int) {
        iterator old = *this;
        node = node->next;
        return old;
    }
    bool operator!=(const iterator right) const {
        return node != right.node;
    }
    bool operator==(const iterator right) const {
        return node == right.node;
    }
protected:
    chainNode<T>* node;
};

//为了实现抽象数据类型的一些基本操作，我们应当对现有的线性表进行一些ADT(Abstract Data Type)扩展
template<class T>
class extendedLinearList : linearList<T> {
public:
    virtual ~extendedLinearList() {}
    virtual void clear() = 0;
    virtual void push_back(const T& theElement) = 0;
};



template <class T>
void chain<T>:: clear() {
    while (firstNode != NULL) {
        chainNode<T> nextNode = firstNode->next;
        delete firstNode;
        firstNode = nextNode;
    }
    listSize = 0;
}

//这里其实这么写没有任何意义，并没有设置尾节点以简化尾插操作，而依然是从头到尾遍历后再插入新节点
template<class T>
void chain<T>::push_back(const T &theElement) {
    chainNode<T>* newNode = new chainNode<T>(theElement, NULL);
    if (firstNode == NULL) {
        firstNode = newNode;
    }else {
        chainNode<T>* currentNode = firstNode;
        while (currentNode->next != NULL) {
            currentNode = currentNode->next;
        }
        currentNode->next = newNode;
    }
    listSize++;
}


//循环链表
template<class T>
class circularListWithHeader : public linearList<T>{
public:
    circularListWithHeader();
    int indexOf(const T& theElement) const;
    chainNode<T>* headerNode;
    int listSize;
};

//创建一个只含有头节点的自环
template<class T>
circularListWithHeader<T>::circularListWithHeader() {
    headerNode = new chainNode<T>();
    headerNode->next = headerNode;
    listSize = 0;
}

//将头节点设置为要查找的元素，无论怎样，循环都会结束，只是当在头节点停止时说明链表中不含该元素。
template<class T>
int circularListWithHeader<T>::indexOf(const T& theElement) const {
    headerNode->element = theElement;

    chainNode<T>* currentNode = headerNode->next;
    int index = 0;
    while (currentNode->next != theElement) {
        currentNode = currentNode->next;
        index++;
    }

    if (currentNode == headerNode) {
        return -1;
    }else {
        return index;
    }
}

//双向链表
template<class T>
class doublyLinkedList : linearList<T> {
    doublyLinkedList(int initialCapacity);
    explicit doublyLinkedList(const doublyLinkedList<T> &);
    ~doublyLinkedList();

    bool empty() const {return listSize == 0;}  //判断链表是否为空
    int size() const {return listSize;}
    T& get(int theIndex) const;     //返回指定下标值的元素
    int indexOf(const T& theElement) const; //返回指定元素的下标
    void erase(int theIndex);
    void insert(int theIndex, const T& theElement);
    void output(ostream& out) const;

    void checkIndex(int theIndex) const;
    chainNode2<T>* firstNode;
    chainNode2<T>* lastNode;
    int listSize;
};

template<class T>
doublyLinkedList<T>::doublyLinkedList(int initialCapacity) {
    if (initialCapacity < 1) {
        cout << "Initial capacity = " << initialCapacity << " Must be > 0";
    }
    firstNode = lastNode = NULL;
    listSize = 0;
}

/*拷贝构造函数
 *在传参时一定要传要复制的链表的引用，相当于别名，如果传值的话，编译器会调用doublyLinkedList的复制构造函数从而发生无限递归调用
 */
template<class T>
doublyLinkedList<T>::doublyLinkedList(const doublyLinkedList<T> &theList) {
    listSize = theList.listSize;

    if (listSize == 0) {
        firstNode = lastNode = NULL;
    }

    chainNode2<T>* sourceNode = theList.firstNode;
    firstNode = new chainNode2<T>(sourceNode->element);
    firstNode->prev = NULL;
    sourceNode = sourceNode->next;      //让sourceNode多走一个，sourceNode负责在老链表取值，targetNode负责从新链表上一个节点链接
    chainNode2<T>* targetNode = firstNode;
    while (sourceNode != NULL) {
        targetNode->next = new chainNode2<T>(sourceNode->element);  //一定要重新创建节点并为他们把element拷贝过来
        targetNode->next->prev = targetNode;        //一定别忘了操作prev指针让其回指上一个节点
        sourceNode = sourceNode->next;
        targetNode = targetNode->next;
    }
    targetNode->next = NULL;
    lastNode = targetNode;
}

template<class T>
void doublyLinkedList<T>::checkIndex(int theIndex) const {
    if (theIndex < 0 || theIndex > listSize - 1) {
        cerr << "The index is illegal.";
    }
}

template<class T>
T& doublyLinkedList<T>::get(int theIndex) const {
    checkIndex(theIndex);
    chainNode2<T> currentNode = new chainNode2<T>;
    if (theIndex < listSize/2) {
        for (int i = 0; i < theIndex; i++) {
            currentNode = currentNode->next;
        }
    }else {
        for (int i = listSize - 1; i > theIndex; i--) {
            currentNode = currentNode->prev;
        }
    }
    return currentNode->element;
}

template<class T>
int doublyLinkedList<T>::indexOf(const T& theElement)const {
    chainNode2<T> currentNode = firstNode;
    int index = 0;
    while (currentNode != NULL || currentNode->element != theElement) {
        currentNode = currentNode->next;
        index++;
    }
    if (currentNode == NULL) {
        return -1;
    }else {
        return index;
    }
}

template<class T>
void doublyLinkedList<T>::erase(int theIndex){
    checkIndex(theIndex);
    chainNode2<T>* deleteNode;
    /*分支1：删头操作，需要讨论链表是否只有一个节点
     *分支2：删尾操作，但是需要排除掉链表只有一个节点即既是头又是尾的情况
     *分支3：删除中间节点操作，根据中间节点位置确定是从头开始遍历还是从尾开始遍历
     */
    if (theIndex == 0) {
        deleteNode = firstNode;
        firstNode = firstNode->next;
        if (listSize == 1) {
            lastNode = NULL;
        }else {
            firstNode->prev = NULL;
        }
    }else if (theIndex == listSize - 1 && theIndex != 0) {
        deleteNode = lastNode;
        lastNode = lastNode->prev;
        lastNode->next = NULL;
    }else {
        chainNode2<T>* currentNode;
        if (theIndex < listSize/2) {
            currentNode = firstNode;
            //为了能够处理前序节点指针prev的连接，我们这次让currentNode移动到要删除的节点的位置
            for (int i = 0; i < theIndex; i++) {
                currentNode = currentNode->next;
            }
        }else {
            currentNode = lastNode;
            for (int i = listSize - 1; i > theIndex; i--) {
                currentNode = currentNode->prev;
            }
        }
        deleteNode = currentNode;
        currentNode->prev->next = currentNode->next;
        currentNode->next->prev = currentNode->prev;
    }
    delete deleteNode;
    listSize--;
}

template<class T>
void doublyLinkedList<T>::insert(int theIndex, const T& theElement) {
    /*分支1：处理异常下标值
     *分支2：处理插头情况，但需要特别处理listSize=0条件下lastNode的设置问题
     *分支3：处理插尾情况，但需要排除listSize=0的情况，因为这相当于插头操作，已经考虑到了
     *分支4：处理插入中间节点情况，根据中间节点的位置判断从链表的哪一头开始遍历
     */
    if (theIndex < 0 || theIndex > listSize) {
        cerr << "Illegal Index!!!";
        return;
    }else if (theIndex == 0) {
        chainNode2<T>* newNode = new chainNode2<T>(theElement, firstNode, NULL);
        if (listSize != 0) {
            firstNode->prev = newNode;
            firstNode = newNode;
        }else {
            firstNode = newNode;
            lastNode = newNode;
        }
    }else if (theIndex == listSize && listSize != 0) {
        chainNode2<T>* newNode = new chainNode2<T>(theElement, NULL, lastNode);
        lastNode->next = newNode;
        lastNode = newNode;
    }
    else {
        chainNode2<T>* currentNode;
        if (theIndex < listSize/2) {
            currentNode = firstNode;
            for (int i = 0; i < theIndex; i++) {
                currentNode = currentNode->next;
            }
        }else {
            currentNode = lastNode;
            for (int i = listSize - 1; i > theIndex; i--) {
                currentNode = currentNode->prev;
            }
        }
        chainNode2<T>* newNode = new chainNode2<T>(theElement, currentNode, currentNode->prev);
        currentNode->prev = newNode;
        newNode->prev->next = newNode;
    }
    listSize++;
}

template<class T>
void doublyLinkedList<T>::output(ostream &out) const {
    for (chainNode2<T>* currentNode = firstNode; currentNode != NULL; currentNode = currentNode->next) {
        out << currentNode->element << "  ";
    }
}

template<class T>
class arrayList;

template <class T>
istream& operator >>(istream&, arrayList<T>&);

// arrayList类
template <class T>
class arrayList : public linearList<T> {
public:
    arrayList(int initialCapacity = 10);
    arrayList(const arrayList<T>&);
    ~arrayList() { delete [] element; }
    bool empty() const { return listSize == 0; }
    int size() const {return listSize; }
    T& get(int theIndex) const;
    int indexOf (const T& theElement) const;
    void erase(int theIndex);
    void insert(int theIndex, const T& theElement);
    void changeLength(T*&, int, int);
    void output(ostream& out) const;
    friend istream& operator>> <T>(istream&, arrayList&);

    int capacity() const { return arrayLength; }

protected:
    void checkIndex(int theIndex) const;
    T* element;
    int arrayLength;
    int listSize;
};

template<class T>
arrayList<T>::arrayList(int initialCapacity) {
    if (initialCapacity < 1) {
        ostringstream s;
        s << "Initial capacity = " << initialCapacity << " Must be > 0";
        throw illegalIndex();
    }
    arrayLength = initialCapacity;
    element = new T[arrayLength];
    listSize = 0;
}

//拷贝构造函数
template<class T>
arrayList<T>::arrayList(const arrayList<T>& theList) {
    arrayLength = theList.arrayLength;
    listSize = theList.listSize;
    element = new T[arrayLength];
    copy(theList.element, theList.element + listSize, element);
}

template<class T>
void arrayList<T>::checkIndex(int theIndex) const {
    if (theIndex < 0 || theIndex >= listSize) {
        ostringstream s;
        s << "index = " << theIndex << " size = " << listSize;
        throw illegalIndex();
    }
}

template<class T>
T& arrayList<T>::get(int theIndex) const {
    checkIndex(theIndex);
    return element[theIndex];
}

template<class T>
int arrayList<T>::indexOf(const T& theElement) const {
    int theIndex = (int) (find(element, element + listSize, theElement) - element);     //find函数传入什么类型的迭代器就会返回什么类型的迭代器，如果find函数没有找到对应元素，迭代器会停留在对应尾结点
    if (theIndex == listSize) {
        return -1;
    }else {
        return theIndex;
    }
}

template<class T>
void arrayList<T>::erase(int theIndex) {
    checkIndex(theIndex);
    //从element + index位置开始逐个把element + index后的元素复制过来
    copy(element + theIndex + 1, element + listSize, element + theIndex);
    //将数组最后的空位置回收
    element[--listSize].~T();
}

template<class T>
void arrayList<T>::insert(int theIndex, const T& theElement) {
    if (theIndex < 0 || theIndex > listSize) {
        ostringstream s;
        s << "index = " << theIndex << " size = " << listSize;
        throw illegalInsertIndex();
    }

    if (listSize == arrayLength) {
        changeLength(element, arrayLength, 2 * arrayLength);
        arrayLength *= 2;
    }
    //把元素按原序复制到目标区间的尾部，第三个参数为尾后迭代器，这一步相当于将插入位置后所有元素向后移动一个位置
    copy_backward(element + theIndex, element + listSize, element + listSize + 1);

    element[theIndex] = theElement;
    listSize++;
}

template<class T>
void arrayList<T>::output(ostream &out) const {
    copy(element, element + listSize, ostream_iterator<T>(cout, " "));
}
template<class T>
ostream& operator<<(ostream& out, const arrayList<T>& x) {
    x.output(out);
    return out;
}

/*
作用：将数组的长度加倍
输入：指针a指向需要改变长度的数组，oldLength表示数组原来的长度，newLength表示需要改变的新长度
结果：将数组扩容/缩容 为newLength
*/
template<class T>
void arrayList<T>::changeLength(T*& a, int oldLength, int newLength)
{
    if (newLength < 0) {
        throw illegalIndex();
    }
    T* temp = new T[newLength];
    int number = min(oldLength, newLength);
    copy(a, a + number, temp);
    delete[] a;
    a = temp;
}

template<class T>
istream& operator >>(istream& in, arrayList<T>& x) {
    T val;
    while (in >> val) {
        x.insert(x.listSize, val);
    }
    return in;
}

template<class iterator>
void copy(iterator start, iterator end, iterator to) {
    while (start != end) {
        *to = *start;
        start++;
        to++;
    }
}

#endif