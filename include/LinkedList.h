#ifndef LINKED_LIST_H
#define LINKED_LIST_H

// Danh sách liên kết đơn – danh sách phiếu mượn; cũng dùng làm hàng đợi cho BFS
template <typename T>
class LinkedList {
    struct Node {
        T data;
        Node* next;

        Node(const T& d) : data(d), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int count;

public:
    LinkedList() : head(nullptr), tail(nullptr), count(0) {}
    ~LinkedList() { clear(); }
    LinkedList(const LinkedList&) = delete;
    LinkedList& operator=(const LinkedList&) = delete;

    void pushBack(const T& x) {                      // O(1) nhờ con trỏ tail
        Node* n = new Node(x);
        if (tail) tail->next = n;
        else head = n;
        tail = n;
        ++count;
    }

    bool popFront(T& out) {                          // dùng khi làm hàng đợi
        if (!head) return false;
        Node* n = head;
        out = n->data;
        head = n->next;
        if (!head) tail = nullptr;
        delete n;
        --count;
        return true;
    }

    template <typename P>
    T* find(P pred) {                                // phần tử đầu tiên thỏa pred, O(m)
        for (Node* n = head; n; n = n->next) {
            if (pred(n->data)) return &n->data;
        }
        return nullptr;
    }

    template <typename P>
    bool remove(P pred) {                            // xóa phần tử đầu tiên thỏa pred
        Node* prev = nullptr;
        for (Node* n = head; n; prev = n, n = n->next) {
            if (!pred(n->data)) continue;
            if (prev) prev->next = n->next;
            else head = n->next;
            if (n == tail) tail = prev;
            delete n;
            --count;
            return true;
        }
        return false;
    }

    template <typename F>
    void forEach(F visit) {
        for (Node* n = head; n; n = n->next) visit(n->data);
    }

    template <typename P>
    int countIf(P pred) {
        int c = 0;
        for (Node* n = head; n; n = n->next) {
            if (pred(n->data)) ++c;
        }
        return c;
    }

    int  size() const { return count; }
    bool empty() const { return count == 0; }

    void clear() {
        while (head) {
            Node* next = head->next;
            delete head;
            head = next;
        }
        tail = nullptr;
        count = 0;
    }
};

#endif
