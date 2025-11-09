#include<vector>
#include<iostream>
#include"MaxHeap.h"
using namespace std;

class Sorting {
public:
    int rounds = 0;     //外层循环的执行次数
    //这是选择排序的交换版本，选择排序是每轮经过一一比较挑出最大的放在最后面
    vector<int> Bubble_Sort(vector<int> nums) {
        int n = nums.size();
        for (int i = 0; i < n - 1; i++) {
            rounds++;
            for (int j = i + 1; j < n; j++) {
                //若某次靠左的数小于等于右边的数了，下次比较的就不是相邻的两个元素了，因为j照样+1，而i没变
                if (nums[i] > nums[j]) {
                    int temp = nums[j];
                    nums[j] = nums[i];
                    nums[i] = temp;
                }
            }
        }
        return nums;
    }

    //冒泡排序：每轮两两相邻元素进行比较，若左边的数大于右边的数，则交换位置，第一遍会将最大的数移到最后面，第二遍将第二大的数移到倒数第二位，以此类推。
    vector<int> Bubble_sort2(vector<int> nums) {
        int n = nums.size();
        for (int i = 0; i < n - 1; i++) {
            rounds++;
            //只对相邻元素进行比较，这是冒泡排序的核心思想
            for (int j = 0; j < n - i - 1; j++) {
                if (nums[j] > nums[j + 1]) {
                    int temp = nums[j];
                    nums[j] = nums[j + 1];
                    nums[j + 1] = temp;
                }
            }
        }
        return nums;
    }

    /*优化版本1：为防止排序已经完成但外层循环依然徒然继续，设置bool变量flag,
     *若某次外层循环没有对数组进行改动说明排序已经完成，不再进入下次外层循环
     */
    vector<int> Bubble_sort21(vector<int> nums) {
        int n = nums.size();
        bool flag = true;       //flag变量用于表示标记是否进入内层循环，即上次外层循环内是否执行冒泡排序操作
        for (int i = 0; i < n - 1 && flag; i++) {
            flag = false;
            rounds++;
            for (int j = 0; j < n - i - 1; j++) {
                if (nums[j] > nums[j+1]) {
                    int temp = nums[j];
                    nums[j] = nums[j + 1];
                    nums[j + 1] = temp;
                    flag = true;
                }
            }
        }
        return nums;
    }

    //选择排序：依次寻找最小、第二小、第三小......最大的元素放置到相应位置
    vector<int> Simple_Selection_Sort(vector<int> nums) {
        int n = nums.size();
        int min;
        for (int i = 0; i < n; i++) {
            min = i;        //设置初始最小下标为当前需要插入位置下标
            for (int j = i + 1; j < n; j++) {
                if (nums[j] < nums[min]) {
                    min = j;    //不断更新最小下标
                }
            }
            //将最小下标指向元素与需要插入位置的元素进行位置交换
            int temp = nums[i];
            nums[i] = nums[min];
            nums[min] = temp;
        }
        return nums;
    }

    //插入排序(稳定)：不断将后面无序队列中的数插入到已经排好序的有序队列中，就像给卷子按成绩排序一样
    vector<int> InsertSort(vector<int> nums) {
        for (int i = 1; i < nums.size(); i++) {
            int temp = nums[i];
            int j = i - 1;
            //新插入的数不断向前移动，直至恰当位置
            for (; j >= 0 && nums[j] > temp; j--) {
                nums[j + 1] = nums[j];
            }
            nums[j + 1] = temp;
        }
        return nums;
    }

    //堆排序(不稳定)：
    vector<int> HeapSort(vector<int> nums) {
        int* sequence = new int[nums.size() + 1];
        for (int i = 1; i <= nums.size(); i++) {
            sequence[i] = nums[i - 1];
        }
        // vector<int> sequence(nums.size() + 1);
        // copy(nums.begin(), nums.end(), sequence.data() + 1);
        maxHeap<int> heap(nums.size());
        heap.initialize(sequence, nums.size());
        for (int i = nums.size() - 1; i >= 0; i--) {
            nums[i] = heap.top();
            heap.pop();
        }
        return nums;
    }

    /*希尔排序(不稳定)：改进了插入排序，由于插入排序在数据量较小效率较高，
     *所以我们设置增量对数据进行分组进行插入排序,并不断减小增量，最终实现整个数组的排序
     */
    vector<int> ShellSort(vector<int> nums) {
        int n = nums.size();
        int gap = n;

        while (gap > 1) {
            gap = gap/3 + 1;
            for (int i = gap; i < n; i++) {
                int temp = nums[i];
                int j = i - gap;

                for (; j >= 0 && nums[j] > temp; j = j - gap) {
                    nums[j + gap] = nums[j];
                }
                nums[j + gap] = temp;
            }
        }
        return nums;
    }
};

int main() {
    vector<int> a = {7,1,2,3,6,4,5};
    Sorting sort;

    vector<int> sorted1 = sort.Bubble_Sort(a);
    for (int i = 0; i <= a.size() - 1; i++) {
        cout << sorted1[i] << " ";
    }
    cout << endl << "排序轮数：" << sort.rounds << endl;
    sort.rounds = 0;

    vector<int> sorted2 = sort.Bubble_sort2(a);
    for (int i = 0; i <= a.size() - 1; i++) {
        cout << sorted2[i] << " ";
    }
    cout << endl << "排序轮数：" << sort.rounds << endl;
    sort.rounds = 0;

    vector<int> sorted3 = sort.Bubble_sort21(a);
    for (int i = 0; i <= a.size() - 1; i++) {
        cout << sorted3[i] << " ";
    }
    cout << endl << "排序轮数：" << sort.rounds << endl;
    sort.rounds = 0;

    vector<int> sorted4 = sort.Simple_Selection_Sort(a);
    for (int i = 0; i <= a.size() - 1; i++) {
        cout << sorted4[i] << " ";
    }
    cout << endl;

    vector<int> sorted5 = sort.InsertSort(a);
    for (int i = 0; i <= a.size() - 1; i++) {
        cout << sorted5[i] << " ";
    }
    cout << endl;

    vector<int> sorted6 = sort.ShellSort(a);
    for (int i = 0; i <= a.size() - 1; i++) {
        cout << sorted6[i] << " ";
    }
    cout << endl;

    vector<int> b = {9,2,3,5,8,1,6,4,7};
    vector<int> sorted7 = sort.HeapSort(b);
    for (int i = 0; i <= b.size() - 1; i++) {
        cout << sorted7[i] << " ";
    }
    cout << endl;
    return 0;
}