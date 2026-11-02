#ifndef AVL_TREE_H
#define AVL_TREE_H

// Cây AVL – quản lý sách theo ISBN (cài đặt ở tuần 3)
template <typename K, typename V>
class AVLTree {
    struct Node {
        K key;
        V value;
        Node* left;
        Node* right;
        int height;
    };

    Node* root;
    int count;

    int   h(Node* n) const { return n ? n->height : 0; }
    int   balanceFactor(Node* n) const { return h(n->left) - h(n->right); }
    Node* rotateLeft(Node* x);
    Node* rotateRight(Node* y);
    Node* rebalance(Node* n);                       // 4 trường hợp LL, RR, LR, RL
    Node* insertRec(Node* n, const K& k, const V& v, bool& inserted);
    Node* removeRec(Node* n, const K& k, bool& removed);
    Node* minNode(Node* n);

public:
    AVLTree() : root(nullptr), count(0) {}
    ~AVLTree();

    bool insert(const K& k, const V& v);            // false nếu khóa đã tồn tại
    bool remove(const K& k);
    V*   find(const K& k);                          // nullptr nếu không có
    template <typename F> void inorder(F visit);    // duyệt tăng dần theo khóa
    int  size() const { return count; }
    int  height() const { return h(root); }
    void clear();
};

#endif
