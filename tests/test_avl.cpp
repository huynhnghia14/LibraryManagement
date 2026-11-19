#include "AVLTree.h"

#include <cassert>
#include <iostream>
#include <string>

void testInsertSortedStaysBalanced() {
    AVLTree<int, int> t;
    for (int i = 1; i <= 1000; ++i) {
        assert(t.insert(i, i * 10));
        assert(t.isBalanced());
    }
    assert(t.size() == 1000);
    assert(t.height() <= 15);                    // 1.44 * log2(1002) ~ 14.4
}

void testDuplicateRejected() {
    AVLTree<std::string, int> t;
    assert(t.insert("9786040000001", 1));
    assert(!t.insert("9786040000001", 2));
    assert(*t.find("9786040000001") == 1);
}

void testFindAndRemove() {
    AVLTree<int, int> t;
    for (int i = 0; i < 200; ++i) t.insert((i * 37) % 200, i);
    for (int i = 0; i < 200; i += 2) {
        assert(t.remove(i));
        assert(t.isBalanced());
    }
    assert(!t.remove(0));
    assert(t.size() == 100);
    for (int i = 0; i < 200; ++i) assert((t.find(i) != nullptr) == (i % 2 == 1));
}

void testInorderIsSorted() {
    AVLTree<int, int> t;
    int keys[] = {50, 20, 70, 10, 30, 60, 80, 25, 35, 5};
    for (int k : keys) t.insert(k, k);
    int prev = -1, n = 0;
    t.inorder([&](const int& k, int&) {
        assert(k > prev);
        prev = k;
        ++n;
    });
    assert(n == 10);
}

int main() {
    testInsertSortedStaysBalanced();
    testDuplicateRejected();
    testFindAndRemove();
    testInorderIsSorted();
    std::cout << "test_avl: passed" << std::endl;
    return 0;
}
