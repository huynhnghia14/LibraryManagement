#ifndef BST_H
#define BST_H

// Cây BST – danh mục sách theo tên chuẩn hóa (cài đặt ở tuần 3)
template <typename K, typename V>
class BST {
    struct Node {
        K key;
        V value;
        Node* left;
        Node* right;
    };

    Node* root;
    int count;

public:
    BST() : root(nullptr), count(0) {}
    ~BST();

    void insert(const K& k, const V& v);            // khóa trùng đi sang nhánh phải
    bool remove(const K& k, const V& v);            // xóa đúng nút có khóa k và value v
    V*   find(const K& k);
    template <typename F> void inorder(F visit);
    template <typename F> void prefixSearch(const K& prefix, F visit);   // để so sánh với Trie
    int  height() const;
    int  size() const { return count; }
    void clear();
};

#endif
