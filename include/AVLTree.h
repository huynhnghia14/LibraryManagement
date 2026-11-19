#ifndef AVL_TREE_H
#define AVL_TREE_H

// Cây AVL – quản lý sách theo ISBN
// Mọi thao tác insert / remove / find O(log n) nhờ giữ |balanceFactor| <= 1 tại mọi nút
template <typename K, typename V>
class AVLTree {
    struct Node {
        K key;
        V value;
        Node* left;
        Node* right;
        int height;

        Node(const K& k, const V& v) : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;
    int count;

    static int h(Node* n) { return n ? n->height : 0; }
    static int balanceFactor(Node* n) { return h(n->left) - h(n->right); }

    static void updateHeight(Node* n) {
        int hl = h(n->left), hr = h(n->right);
        n->height = 1 + (hl > hr ? hl : hr);
    }

    static Node* rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        x->right = y;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    static Node* rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    // Cân bằng lại nút n sau khi chèn / xóa ở cây con (4 trường hợp LL, LR, RR, RL)
    static Node* rebalance(Node* n) {
        updateHeight(n);
        int bf = balanceFactor(n);
        if (bf > 1) {
            if (balanceFactor(n->left) < 0) n->left = rotateLeft(n->left);    // LR
            return rotateRight(n);                                            // LL
        }
        if (bf < -1) {
            if (balanceFactor(n->right) > 0) n->right = rotateRight(n->right); // RL
            return rotateLeft(n);                                             // RR
        }
        return n;
    }

    static Node* insertRec(Node* n, const K& k, const V& v, bool& inserted) {
        if (!n) {
            inserted = true;
            return new Node(k, v);
        }
        if (k < n->key)      n->left = insertRec(n->left, k, v, inserted);
        else if (n->key < k) n->right = insertRec(n->right, k, v, inserted);
        else {
            inserted = false;                     // khóa đã tồn tại
            return n;
        }
        return rebalance(n);
    }

    static Node* minNode(Node* n) {
        while (n->left) n = n->left;
        return n;
    }

    static Node* removeRec(Node* n, const K& k, bool& removed) {
        if (!n) return nullptr;
        if (k < n->key)      n->left = removeRec(n->left, k, removed);
        else if (n->key < k) n->right = removeRec(n->right, k, removed);
        else {
            removed = true;
            if (!n->left || !n->right) {          // 0 hoặc 1 con: nối con lên thay
                Node* child = n->left ? n->left : n->right;
                delete n;
                return child;
            }
            Node* succ = minNode(n->right);       // 2 con: thay bằng nút nhỏ nhất bên phải
            n->key = succ->key;
            n->value = succ->value;
            bool dummy = false;
            n->right = removeRec(n->right, succ->key, dummy);
        }
        return rebalance(n);
    }

    template <typename F>
    static void inorderRec(Node* n, F& visit) {
        if (!n) return;
        inorderRec(n->left, visit);
        visit(n->key, n->value);
        inorderRec(n->right, visit);
    }

    static void destroy(Node* n) {
        if (!n) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

    static bool checkRec(Node* n, int& height) {
        if (!n) {
            height = 0;
            return true;
        }
        int hl, hr;
        if (!checkRec(n->left, hl) || !checkRec(n->right, hr)) return false;
        if (n->left && !(n->left->key < n->key)) return false;
        if (n->right && !(n->key < n->right->key)) return false;
        int diff = hl - hr;
        height = 1 + (hl > hr ? hl : hr);
        return diff >= -1 && diff <= 1 && height == n->height;
    }

public:
    AVLTree() : root(nullptr), count(0) {}
    ~AVLTree() { destroy(root); }
    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;

    bool insert(const K& k, const V& v) {         // false nếu khóa đã tồn tại
        bool inserted = false;
        root = insertRec(root, k, v, inserted);
        if (inserted) ++count;
        return inserted;
    }

    bool remove(const K& k) {
        bool removed = false;
        root = removeRec(root, k, removed);
        if (removed) --count;
        return removed;
    }

    V* find(const K& k) {                         // nullptr nếu không có
        Node* n = root;
        while (n) {
            if (k < n->key)      n = n->left;
            else if (n->key < k) n = n->right;
            else return &n->value;
        }
        return nullptr;
    }

    template <typename F>
    void inorder(F visit) { inorderRec(root, visit); }   // visit(const K&, V&) theo khóa tăng dần

    int  size() const { return count; }
    int  height() const { return h(root); }
    bool isBalanced() const { int hh; return checkRec(root, hh); }   // dùng trong unit test

    void clear() {
        destroy(root);
        root = nullptr;
        count = 0;
    }
};

#endif
