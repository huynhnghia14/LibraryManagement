#ifndef LINKED_LIST_H
#define LINKED_LIST_H

// Danh sách liên kết đơn – danh sách phiếu mượn (cài đặt ở tuần 5)
template <typename T>
class LinkedList {
    struct Node {
        T data;
        Node* next;
    };

    Node* head;
    Node* tail;
    int count;

public:
    LinkedList() : head(nullptr), tail(nullptr), count(0) {}
    ~LinkedList();

    void pushBack(const T& x);                      // O(1) nhờ con trỏ tail
    template <typename P> T*   find(P pred);        // phần tử đầu tiên thỏa pred
    template <typename P> bool remove(P pred);
    template <typename F> void forEach(F visit);
    template <typename P> int  countIf(P pred);
    int  size() const { return count; }
    void clear();
};

#endif
