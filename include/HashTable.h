#ifndef HASH_TABLE_H
#define HASH_TABLE_H

// Bảng băm xử lý va chạm separate chaining – quản lý độc giả theo CMND (cài đặt ở tuần 4)
template <typename K, typename V>
class HashTable {
    struct Node {
        K key;
        V value;
        Node* next;
    };

    Node** buckets;
    int tableSize;                                  // số nguyên tố: 1009, 2003, 4007, ...
    int count;

    unsigned hash(const K& key) const;              // h = (h * 31 + c) % tableSize
    void rehash();                                  // khi loadFactor() > 0.75

public:
    explicit HashTable(int initialSize = 1009);
    ~HashTable();

    bool insert(const K& k, const V& v);            // false nếu trùng khóa
    bool remove(const K& k);
    V*   find(const K& k);
    template <typename F> void forEach(F visit);
    double loadFactor() const { return (double)count / tableSize; }
    int  size() const { return count; }
    int  bucketCount() const { return tableSize; }
    int  longestChain() const;                      // phục vụ đo hiệu năng
};

#endif
