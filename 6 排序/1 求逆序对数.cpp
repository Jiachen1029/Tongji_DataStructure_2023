//超时算法
//#include <iostream>
//using namespace std;
//
//int countInversions(int* arr, int n) 
//{
//    int invCount = 0;
//    // 遍历数组的每一个元素
//    for (int i = 0; i < n - 1; ++i) 
//    {
//        // 对于每一个元素，检查其后面的元素
//        for (int j = i + 1; j < n; ++j) 
//        {
//            // 如果前面的元素大于后面的元素，则是一个逆序对
//            if (arr[i] > arr[j])
//            {
//                ++invCount;
//            }
//        }
//    }
//    return invCount;
//}
//
//int main()
//{
//    while (1)
//    {
//        int N;
//        cin >> N;//输入数组大小
//        if (N == 0)
//            break;
//
//        int* arr = new int[N];//创建动态数组
//        for (int i = 0; i < N; ++i)
//        {
//            cin >> arr[i];
//        }
//
//        cout << countInversions(arr, N) << endl;
//
//        delete[] arr;
//    }
//    return 0;
//}
#include <iostream>
using namespace std;

// 合并两个子数组并计算逆序对的数量
long long mergeAndCount(int* arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // 动态分配两个子数组
    int* leftSub = new int[n1];
    int* rightSub = new int[n2];

    // 将数据复制到子数组
    for (int i = 0; i < n1; ++i)
        leftSub[i] = arr[left + i];
    for (int i = 0; i < n2; ++i)
        rightSub[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    long long invCount = 0;

    // 合并两个子数组并计算逆序对
    while (i < n1 && j < n2) {
        if (leftSub[i] <= rightSub[j]) {
            arr[k++] = leftSub[i++];
        }
        else {
            arr[k++] = rightSub[j++];
            invCount += (n1 - i); // 计算逆序对
        }
    }

    // 复制剩余的元素
    while (i < n1) {
        arr[k++] = leftSub[i++];
    }

    while (j < n2) {
        arr[k++] = rightSub[j++];
    }

    // 释放动态分配的内存
    delete[] leftSub;
    delete[] rightSub;

    return invCount;
}

// 使用归并排序计算逆序对的数量
long long mergeSortAndCount(int* arr, int left, int right) {
    long long invCount = 0;
    if (left < right) {
        int mid = left + (right - left) / 2;

        // 递归排序并计算逆序对
        invCount += mergeSortAndCount(arr, left, mid);
        invCount += mergeSortAndCount(arr, mid + 1, right);

        // 合并两个子数组并计算逆序对
        invCount += mergeAndCount(arr, left, mid, right);
    }
    return invCount;
}

int main() {
    while (true) {
        int N;
        cin >> N;
        if (N == 0) break;

        int* arr = new int[N];
        for (int i = 0; i < N; ++i) {
            cin >> arr[i];
        }

        cout << mergeSortAndCount(arr, 0, N - 1) << endl;

        delete[] arr;
    }
    return 0;
}
