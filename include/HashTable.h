#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <string>

// Hàm băm đa thức cơ số 31 cho chuỗi (CMND, username, thể loại)
inline unsigned hashKey(const std::string& key) {
    unsigned h = 0;
    for (size_t i = 0; i < key.size(); ++i) h = h * 31 + (unsigned char)key[i];
    return h;
}

inline unsigned hashKey(int key) {
    return (unsigned)key * 2654435761u;
}

// Bảng băm xử lý va chạm bằng separate chaining – quản lý độc giả theo CMND
// Tìm / thêm / xóa trung bình O(1); tự rehash khi hệ số tải > 0.75
template <typename K, typename V>
class HashTable {
    struct Node {
        K key;
        V value;
        Node* next;

        Node(const K& k, const V& v, Node* n) : key(k), value(v), next(n) {}
    };

    Node** buckets;
    int tableSize;                                  // luôn là số nguyên tố
    int count;

    static constexpr double MAX_LOAD = 0.75;

    static bool isPrime(int n) {
        if (n < 2) return false;
        for (int d = 2; (long)d * d <= n; ++d) {
            if (n % d == 0) return false;
        }
        return true;
    }

    static int nextPrime(int n) {
        while (!isPrime(n)) ++n;
        return n;
    }

    int indexOf(const K& key) const { return (int)(hashKey(key) % (unsigned)tableSize); }

    // Gấp đôi số bucket (làm tròn lên số nguyên tố) rồi nối lại từng nút, không cấp phát nút mới
    void rehash() {
        int oldSize = tableSize;
        Node** old = buckets;
        tableSize = nextPrime(oldSize * 2 + 1);
        buckets = new Node*[tableSize]();
        for (int i = 0; i < oldSize; ++i) {
            Node* n = old[i];
            while (n) {
                Node* next = n->next;
                int idx = indexOf(n->key);
                n->next = buckets[idx];
                buckets[idx] = n;
                n = next;
            }
        }
        delete[] old;
    }

    void destroyAll() {
        for (int i = 0; i < tableSize; ++i) {
            Node* n = buckets[i];
            while (n) {
                Node* next = n->next;
                delete n;
                n = next;
            }
            buckets[i] = nullptr;
        }
        count = 0;
    }

public:
    explicit HashTable(int initialSize = 1009)
        : buckets(nullptr), tableSize(nextPrime(initialSize < 3 ? 3 : initialSize)), count(0) {
        buckets = new Node*[tableSize]();
    }

    ~HashTable() {
        destroyAll();
        delete[] buckets;
    }

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    bool insert(const K& k, const V& v) {           // false nếu trùng khóa
        if (find(k)) return false;
        if ((double)(count + 1) / tableSize > MAX_LOAD) rehash();
        int idx = indexOf(k);
        buckets[idx] = new Node(k, v, buckets[idx]);  // chèn đầu chuỗi: O(1)
        ++count;
        return true;
    }

    bool remove(const K& k) {
        Node** link = &buckets[indexOf(k)];
        while (*link) {
            if ((*link)->key == k) {
                Node* dead = *link;
                *link = dead->next;
                delete dead;
                --count;
                return true;
            }
            link = &(*link)->next;
        }
        return false;
    }

    V* find(const K& k) {
        for (Node* n = buckets[indexOf(k)]; n; n = n->next) {
            if (n->key == k) return &n->value;
        }
        return nullptr;
    }

    template <typename F>
    void forEach(F visit) {                          // visit(const K&, V&)
        for (int i = 0; i < tableSize; ++i) {
            for (Node* n = buckets[i]; n; n = n->next) visit(n->key, n->value);
        }
    }

    void clear() { destroyAll(); }

    double loadFactor() const { return (double)count / tableSize; }
    int  size() const { return count; }
    int  bucketCount() const { return tableSize; }

    int longestChain() const {                       // phục vụ đo hiệu năng
        int best = 0;
        for (int i = 0; i < tableSize; ++i) {
            int len = 0;
            for (Node* n = buckets[i]; n; n = n->next) ++len;
            if (len > best) best = len;
        }
        return best;
    }
};

#endif
