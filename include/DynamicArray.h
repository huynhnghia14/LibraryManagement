#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

// Mảng động – lịch sử giao dịch (cài đặt ở tuần 5)
template <typename T>
class DynamicArray {
    T* data;
    int size_;
    int capacity_;

    void grow() { reserve(capacity_ == 0 ? 8 : capacity_ * 2); }

public:
    DynamicArray() : data(nullptr), size_(0), capacity_(0) {}
    ~DynamicArray();

    void pushBack(const T& x);                      // O(1) khấu hao
    T&   at(int i);                                 // kiểm tra biên, ném std::out_of_range
    const T& at(int i) const;
    void removeAt(int i);
    void reserve(int cap);
    template <typename C> void sort(C cmp);         // merge sort tự cài (top 10)
    int  size() const { return size_; }
    T*   raw() { return data; }                     // để fwrite cả khối
    void clear();
};

#endif
