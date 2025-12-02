//
// Created by 霍华德 on 2025/11/30.
//
#include <iostream>
#include <sstream>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;

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

class undefinedMethod : public std::exception {
public:
    explicit undefinedMethod(const std::string& msg)
        : msg_(msg) {}
    const char* what() const noexcept override {
        return msg_.c_str();
    }
private:
    std::string msg_;
};

template <class T>
void make2dArray(T**& a, int rows, int cols) {  // n 表示最大索引，数组大小为 (n+1)*(n+1)
    // 1. 分配行指针数组（0~n 行）
    a = new T*[rows];
    // 2. 分配连续元素池：(n+1)*(n+1) 个元素
    a[0] = new T[rows * cols];
    // 3. 映射每行首地址（i从0到n）
    for (int i = 0; i < rows; i++) {
        a[i] = a[0] + i * cols;  // 第i行首地址 = 元素池起始 + i*(n+1)
    }
}

template <class T>
void delete2dArray(T**& a) {
    if (a != nullptr) {
        delete[] a[0];  // 释放元素池
        delete[] a;     // 释放行指针数组
        a = nullptr;    // 避免悬空指针
    }
}

template<class T>
class vertexIterator {
public:
    virtual ~vertexIterator() = default;
    virtual bool hasNext() const = 0;
    virtual int next() = 0;   // 返回下一个邻接顶点编号
    virtual T weight() const = 0;   // 返回当前边的权值
};

template<class T>
class myIterator : public vertexIterator<T> {
public:
    myIterator(T* theRow, T theNoEdge, int numberOfVertices) {
        row  = theRow;
        noEdge = theNoEdge;
        n = numberOfVertices;
        currentVertex = 1;
    }

    ~myIterator() {}
    int next(T& theWeight) {
        for (int j = currentVertex; j <= n; j++) {
            if (row[j] != noEdge) {
                currentVertex = j + 1;
                theWeight = row[j];
                return j;
            }
        }
        currentVertex = n + 1;
        return 0;
    }

protected:
    T* row;
    T noEdge;
    int n;
    int currentVertex;
};

template<class T>
struct edge {
    int v1;
    int v2;
    int w;
    int vertex1() const { return v1; }
    int vertex2() const { return v2; }
    int weight() const { return w; }
};

template<class T>
class Graph {
public:
    virtual ~Graph() {}
    virtual int numberOfVertices() const = 0;
    virtual int numberOfEdges() const = 0;
    virtual bool existsEdge(int, int) const = 0;
    virtual void insertEdge(edge<T>*) const = 0;
    virtual void eraseEdge(int, int) = 0;
    virtual int degree(int) const = 0;
    virtual int inDegree(int) const = 0;
    virtual int outDegree(int) const = 0;

    virtual bool directed() const = 0;
    virtual bool weighted() const = 0;

    virtual void bfs(int v, int reach[], int label);
    virtual void dfs(int v, int reach[], int label);
    virtual vertexIterator<T>* iterator(int) = 0;
};

template<class T>
void Graph<T>::bfs(int v, int reach[], int label) {
    queue<int> q;
    reach[v] = label;
    q.push(v);
    while (!q.empty()) {
        int w = q.front();
        q.pop();
        vertexIterator<T> *iw = iterator(w);
        int u;
        while ((u = iw->next()) != 0) {
            if (reach[u] == 0) {
                q.push(u);
                reach[u] = label;
            }
        }
        delete iw;
    }
}

template<class T>
void Graph<T>::dfs(int v, int reach[], int label) {
    reach[v] = label;
    vertexIterator<T> *iv = iterator(v);
    int u;
    while ((u = iv->next()) != 0) {
        if (reach[u] == 0) {
            dfs(u, reach, label);
        }
    }
    delete iv;
}

template<class T>
class adjacencyWDigraph : public Graph<T> {
protected:
    int n;
    int e;
    T **a;
    T noEdge;

public:
    adjacencyWDigraph(int numberOfVertices = 0, T theNoEdge = 0);

    ~adjacencyWDigraph() { delete2dArray(a); }
    int numberOfVertices() const { return n; }
    int numberOfEdges() const { return e; }
    bool directed() const { return true; }
    bool weighted() const { return true; }
    bool existsEdge(int, int) const;

    void insertEdge(edge<T>* theEdge);
    void eraseEdge(int i, int j);
    void checkVertex(int theVertex) const;

    int degree(int theVertex) const { throw undefinedMethod("degree() undefined"); }    //有向图没有度的概念，只有出度和入度
    int outDegree(int theVertex) const;
    int inDegree(int theVertex) const;

    void bfs(int v, int reach[], int label);
    void dfs(int v, int reach[], int label);

    void dijkstra(int source, T* dist[], int* parent);
    void printShortestPath(int source, int target, int* parent, T* dist);
    myIterator<T>* iterator(int theVertex) {
        checkVertex(theVertex);
        return new myIterator<T>(a[theVertex], noEdge, n);
    }
};

template<class T>
adjacencyWDigraph<T>::adjacencyWDigraph(int numberOfVertices, T theNoEdge) {
    if (numberOfVertices < 0) {
        throw illegalParameterValue("Number of vertices must be >= 0");
    }
    n = numberOfVertices;
    e = 0;
    noEdge = theNoEdge;
    make2dArray(a, n + 1, n + 1);
    for (int i = 1; i <= n; i++) {
        fill(a[i] + 1, a[i] + n + 1, noEdge);
    }
}

template<class T>
bool adjacencyWDigraph<T>::existsEdge(int i, int j) const {
    if (i < 1 || j < 1 || i > n || j > n || a[i][j] == noEdge) {
        return false;
    }else {
        return true;
    }
}

template<class T>
void adjacencyWDigraph<T>::insertEdge(edge<T>* theEdge) {
    int v1 = theEdge->vertex1();
    int v2 = theEdge->vertex2();
    if (v1 < 1 || v2 < 1 || v1 > n || v2 > n || v1 == v2) {
        ostringstream s;
        s << "(" << v1 << "," << v2 << ") is not a permissible edge";
        throw illegalParameterValue(s.str());
    }

    if (a[v1][v2] == noEdge) {
        e++;
    }
    a[v1][v2] = theEdge->weight();
}

template<class T>
void adjacencyWDigraph<T>::eraseEdge(int i, int j) {
    if (i >= 1 && j >= 1 && i <= n && j <= n && a[i][j] != noEdge) {
        a[i][j] = noEdge;
        e--;
    }
}

template<class T>
void adjacencyWDigraph<T>::checkVertex(int theVertex) const {
    if (theVertex < 1 || theVertex > n) {
        ostringstream s;
        s << "no vertex " << theVertex;
        throw illegalParameterValue(s.str());
    }
}


template<class T>
int adjacencyWDigraph<T>::outDegree(int theVertex) const {
    checkVertex(theVertex);
    int sum = 0;
    for (int j = 1; j <= n; j++) {
        if (a[theVertex][j] != noEdge) {
            sum++;
        }
    }
    return sum;
}

template<class T>
int adjacencyWDigraph<T>::inDegree(int theVertex) const {
    checkVertex(theVertex);
    int sum = 0;
    for (int j = 1; j <= n; j++) {
        if (a[j][theVertex] != noEdge) {
            sum++;
        }
    }
    return sum;
}

template<class T>
void adjacencyWDigraph<T>::bfs(int v, int reach[], int label) {
    queue<int> q;
    reach[v] = label;
    q.push(v);
    while (!q.empty()) {
        int w = q.front();
        q.pop();
        for (int u = 1; u <= n; u++) {
            if (a[w][u] != noEdge && reach[u] == 0) {
                q.push(u);
                reach[u] = label;
            }
        }
    }
}
template<class T>
void adjacencyWDigraph<T>::dfs(int v, int reach[], int label) {
    reach[v] = label;
    for (int u = 1; u <= n; u++) {
        if (a[v][u] != noEdge && reach[u] == 0) {
            dfs(u, reach, label);
        }
    }
}

//dist数组记录了到目前为止，起点到每个顶点的距离的最小值，这些值可能会进行更新
template<class T>
void adjacencyWDigraph<T>::dijkstra(int source, T *dist[], int* parent) {
    checkVertex(source);
    int n = numberOfVertices();
    bool* visited = new bool[n + 1];
    T INF = (T)INT_MAX;

    //初始化距离和前驱
    for (int i = 1; i <= n; i++) {
        if (i == source) {
            dist[i] = 0;
        }else {
            dist[i] = INF;
        }
        parent[i] = -1;
    }

    for (int i = 1; i <= n; i++) {
        //第 1 步：选择未访问且距离最小的顶点u
        T minDist = INF;
        int u = -1;
        for (int j = 1; j <= n; j++) {
            if (!visited[j] && dist[j] < minDist) {
                minDist = dist[j];
                u = j;
            }
        }
        //剩余节点均已访问过
        if (u == -1) {
            break;
        }
        //将u加入已求得最短路径的顶点的集合
        visited[u] = true;

        //第 2 步：松弛u的所有邻接顶点
        for (int v = 1; v <= n; v++) {
            if (!visited[v] && existsEdge(u,v)) {
                T newDist = dist[u] + a[u];
                if (newDist < dist[v]) {
                    dist[v] = newDist;
                    parent[v] = u;
                }
            }
        }
    }
    delete[] visited;
}
template<class T>
void adjacencyWDigraph<T>::printShortestPath(int source, int target, int *parent, T *dist) {
    if (dist[target] == (T)INT_MAX) {
        cout << "起点" << source << "到顶点" << target << "无路径" << endl;
        return;
    }

    int* path = new int[numberOfVertices() + 1];
    int length = 0;
    for (int v = target; v != -1; v = parent[v]) {
        path[length++] = v;
    }

    cout << "起点" << source << "到顶点" << target << "的最短路径为：";
    for (int i = length - 1; i >= 0; i--) {
        cout << path[i];
        if (i > 0) {
            cout << "->";
        }
    }
    cout << "总长度：" << dist[target] << endl;

    delete[] path;
}
