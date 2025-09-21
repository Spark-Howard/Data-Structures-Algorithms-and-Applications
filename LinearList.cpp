//
// Created by 霍华德 on 2025/9/19.
//
#include <iostream>
using namespace std;

template<class T>
class linearList {
public:
    virtual ~linearList();
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
        cerr << "The index is illegal.";
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
        cout << "index = " << theIndex << " size = " << listSize;
        // throw illegalIndex();
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

template<class T>
class doublyLinkedList : linearList<T> {
    doublyLinkedList(int initialCapacity);
    doublyLinkedList(const chain<T> &);
    ~doublyLinkedList();

    bool empty() const {return listSize == 0;}  //判断链表是否为空
    int size() const {return listSize;}
    T& get(int theIndex) const;     //返回指定下标值的元素
    int indexOf(const T& theElement) const; //返回指定元素的下标
    void erase(int theIndex);
    void insert(int theIndex, const T& theElement);
    void output(ostream& out) const;

    void checkIndex(int theIndex) const;
    chainNode<T>* firstNode;
    chainNode<T>* lastNode;
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

template<class T>
doublyLinkedList<T>::doublyLinkedList(const chain<T> &theList) {
    listSize = theList.listSize;

    if (listSize == 0) {
        firstNode = lastNode = NULL;
    }

    chainNode<T>* sourceNode = theList.firstNode;
    firstNode = new chainNode<T>(sourceNode->element);
    sourceNode = sourceNode->next;      //让sourceNode多走一个，sourceNode负责在老链表取值，targetNode负责从新链表上一个节点链接
    chainNode<T>* targetNode = firstNode;
    while (sourceNode != NULL) {
        targetNode->next = sourceNode;
        sourceNode = sourceNode->next;
        targetNode = targetNode->next;
    }
    lastNode = targetNode->next;
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

    chainNode<T> currentNode = new chainNode<T>;
    for (int i = 0; i < theIndex; i++) {
        currentNode = currentNode->next;
    }
    return currentNode->element;
}

template<class T>
int doublyLinkedList<T>::indexOf(const T& theElement)const {
    chainNode<T> currentNode = firstNode;
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
    chainNode<T>* deleteNode;
    if (theIndex == 0) {
        deleteNode = firstNode;
        firstNode = firstNode->next;
    }else {
        chainNode<T> currentNode = firstNode;
        for (int i = 0; i < theIndex - 1; i++) {
            currentNode = currentNode->next;
        }
        deleteNode = currentNode->next;
        currentNode->next = currentNode->next->next;
    }
    delete deleteNode;
}

template<class T>
void doublyLinkedList<T>::insert(int theIndex, const T& theElement) {
    if (theIndex < 0 || theIndex > listSize) {
        cerr << "Illegal Index!!!";
    }else if (theIndex == 0) {
        firstNode = new chainNode<T>(theElement, firstNode);
    }else {
        chainNode<T> currentNode;
        for (int i = 0; i < theIndex - 1; i++) {
            currentNode->next = new chainNode<T>(theElement, currentNode->next);
        }
    }
}

template<class T>
void doublyLinkedList<T>::output(ostream &out) const {
    for (chainNode<T>* currentNode = firstNode; currentNode != NULL; currentNode = currentNode->next) {
        out << currentNode->element << "  ";
    }
}