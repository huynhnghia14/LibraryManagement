#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdexcept>

// Mảng động – lịch sử giao dịch, mảng tạm khi ghi file / thống kê
// pushBack O(1) khấu hao: khi đầy thì cấp phát mảng mới gấp đôi
template <typename T>
class DynamicArray {
    T* data;
    int size_;
    int capacity_;

    void grow() { reserve(capacity_ == 0 ? 8 : capacity_ * 2); }

    template <typename C>
    static void mergeSort(T* a, T* tmp, int lo, int hi, C& cmp) {     // sắp [lo, hi)
        if (hi - lo < 2) return;
        int mid = lo + (hi - lo) / 2;
        mergeSort(a, tmp, lo, mid, cmp);
        mergeSort(a, tmp, mid, hi, cmp);
        int i = lo, j = mid, k = lo;
        while (i < mid && j < hi) tmp[k++] = cmp(a[j], a[i]) ? a[j++] : a[i++];   // ổn định
        while (i < mid) tmp[k++] = a[i++];
        while (j < hi) tmp[k++] = a[j++];
        for (k = lo; k < hi; ++k) a[k] = tmp[k];
    }

public:
    DynamicArray() : data(nullptr), size_(0), capacity_(0) {}
    ~DynamicArray() { delete[] data; }

    DynamicArray(const DynamicArray& o) : data(nullptr), size_(0), capacity_(0) {
        reserve(o.size_);
        for (int i = 0; i < o.size_; ++i) data[i] = o.data[i];
        size_ = o.size_;
    }

    DynamicArray& operator=(const DynamicArray& o) {
        if (this != &o) {
            DynamicArray copy(o);
            swap(copy);
        }
        return *this;
    }

    void swap(DynamicArray& o) {
        T* d = data; data = o.data; o.data = d;
        int s = size_; size_ = o.size_; o.size_ = s;
        int c = capacity_; capacity_ = o.capacity_; o.capacity_ = c;
    }

    void pushBack(const T& x) {
        if (size_ == capacity_) grow();
        data[size_++] = x;
    }

    T& at(int i) {
        if (i < 0 || i >= size_) throw std::out_of_range("DynamicArray::at");
        return data[i];
    }

    const T& at(int i) const {
        if (i < 0 || i >= size_) throw std::out_of_range("DynamicArray::at");
        return data[i];
    }

    T& operator[](int i) { return data[i]; }
    const T& operator[](int i) const { return data[i]; }

    void removeAt(int i) {                           // dịch trái O(n)
        if (i < 0 || i >= size_) throw std::out_of_range("DynamicArray::removeAt");
        for (int k = i; k < size_ - 1; ++k) data[k] = data[k + 1];
        --size_;
    }

    void reserve(int cap) {                          // dùng khi đọc file: biết trước số bản ghi
        if (cap <= capacity_) return;
        T* bigger = new T[cap];
        for (int i = 0; i < size_; ++i) bigger[i] = data[i];
        delete[] data;
        data = bigger;
        capacity_ = cap;
    }

    // Merge sort tự cài: cmp(a, b) == true nếu a đứng trước b
    template <typename C>
    void sort(C cmp) {
        if (size_ < 2) return;
        T* tmp = new T[size_];
        mergeSort(data, tmp, 0, size_, cmp);
        delete[] tmp;
    }

    int  size() const { return size_; }
    int  capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }
    T*   raw() { return data; }                      // để fwrite cả khối
    const T* raw() const { return data; }
    void clear() { size_ = 0; }
};

#endif
