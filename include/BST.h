#ifndef BST_H
#define BST_H

#include <string>

// Cây nhị phân tìm kiếm – danh mục sách theo tên chuẩn hóa
// Cho phép khóa trùng (hai sách cùng tên): khóa bằng nhau đi sang nhánh phải
template <typename K, typename V>
class BST {
    struct Node {
        K key;
        V value;
        Node* left;
        Node* right;

        Node(const K& k, const V& v) : key(k), value(v), left(nullptr), right(nullptr) {}
    };

    Node* root;
    int count;

    template <typename F>
    static void inorderRec(Node* n, F& visit) {
        if (!n) return;
        inorderRec(n->left, visit);
        visit(n->key, n->value);
        inorderRec(n->right, visit);
    }

    // Chỉ đi vào nhánh có thể chứa khóa bắt đầu bằng prefix
    template <typename F>
    static void prefixRec(Node* n, const std::string& prefix, F& visit) {
        if (!n) return;
        int cmp = n->key.compare(0, prefix.size(), prefix);
        if (cmp > 0) {
            prefixRec(n->left, prefix, visit);
        } else if (cmp < 0) {
            prefixRec(n->right, prefix, visit);
        } else {
            prefixRec(n->left, prefix, visit);
            visit(n->key, n->value);
            prefixRec(n->right, prefix, visit);
        }
    }

    static int heightRec(Node* n) {
        if (!n) return 0;
        int hl = heightRec(n->left), hr = heightRec(n->right);
        return 1 + (hl > hr ? hl : hr);
    }

    static void destroy(Node* n) {
        if (!n) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

public:
    BST() : root(nullptr), count(0) {}
    ~BST() { destroy(root); }
    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;

    // Chèn bằng vòng lặp (không đệ quy) để không tràn stack khi cây bị lệch
    void insert(const K& k, const V& v) {
        Node** link = &root;
        while (*link) link = (k < (*link)->key) ? &(*link)->left : &(*link)->right;
        *link = new Node(k, v);
        ++count;
    }

    // Xóa đúng nút có khóa k và giá trị v (3 trường hợp: lá, 1 con, 2 con)
    bool remove(const K& k, const V& v) {
        Node** link = &root;
        while (*link) {
            Node* cur = *link;
            if (k < cur->key)                         link = &cur->left;
            else if (cur->key < k || !(cur->value == v)) link = &cur->right;
            else break;
        }
        Node* n = *link;
        if (!n) return false;

        if (!n->left) {
            *link = n->right;
        } else if (!n->right) {
            *link = n->left;
        } else {
            Node** succLink = &n->right;              // nút kế tiếp: nhỏ nhất của cây con phải
            while ((*succLink)->left) succLink = &(*succLink)->left;
            Node* succ = *succLink;
            *succLink = succ->right;
            succ->left = n->left;
            succ->right = n->right;
            *link = succ;
        }
        delete n;
        --count;
        return true;
    }

    V* find(const K& k) {
        Node* n = root;
        while (n) {
            if (k < n->key)      n = n->left;
            else if (n->key < k) n = n->right;
            else return &n->value;
        }
        return nullptr;
    }

    template <typename F>
    void inorder(F visit) { inorderRec(root, visit); }   // visit(const K&, V&) theo thứ tự tên

    template <typename F>
    void prefixSearch(const std::string& prefix, F visit) { prefixRec(root, prefix, visit); }

    int  height() const { return heightRec(root); }
    int  size() const { return count; }

    void clear() {
        destroy(root);
        root = nullptr;
        count = 0;
    }
};

#endif
