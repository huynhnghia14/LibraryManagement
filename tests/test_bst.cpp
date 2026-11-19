#include "BST.h"

#include <cassert>
#include <iostream>
#include <string>

void testInorderByName() {
    BST<std::string, std::string> t;
    t.insert("lap trinh c", "3");
    t.insert("cau truc du lieu", "1");
    t.insert("giai thuat", "2");
    t.insert("toan roi rac", "4");
    std::string order;
    t.inorder([&](const std::string&, std::string& v) { order += v; });
    assert(order == "1234");
}

void testDuplicateKeysAndRemoveExact() {
    BST<std::string, std::string> t;
    t.insert("giai tich", "A");
    t.insert("giai tich", "B");
    t.insert("dai so", "C");
    assert(t.size() == 3);
    assert(t.remove("giai tich", "B"));
    assert(!t.remove("giai tich", "B"));
    assert(*t.find("giai tich") == "A");
    assert(t.size() == 2);
}

void testRemoveNodeWithTwoChildren() {
    BST<int, int> t;
    int keys[] = {50, 30, 70, 20, 40, 60, 80, 65};
    for (int k : keys) t.insert(k, k);
    assert(t.remove(70, 70));
    int prev = -1, n = 0;
    t.inorder([&](const int& k, int&) { assert(k > prev); prev = k; ++n; });
    assert(n == 7);
    assert(t.find(65) && t.find(80) && !t.find(70));
}

void testPrefixSearch() {
    BST<std::string, int> t;
    const char* names[] = {"lap trinh c", "lap trinh java", "lich su", "lap trinh python", "giai thuat"};
    for (int i = 0; i < 5; ++i) t.insert(names[i], i);
    int found = 0;
    t.prefixSearch("lap", [&](const std::string& k, int&) { assert(k.compare(0, 3, "lap") == 0); ++found; });
    assert(found == 3);
}

void testSortedInsertDegenerates() {
    BST<int, int> t;
    for (int i = 0; i < 500; ++i) t.insert(i, i);
    assert(t.height() == 500);                    // minh họa BST suy biến (so sánh với AVL)
}

int main() {
    testInorderByName();
    testDuplicateKeysAndRemoveExact();
    testRemoveNodeWithTwoChildren();
    testPrefixSearch();
    testSortedInsertDegenerates();
    std::cout << "test_bst: passed" << std::endl;
    return 0;
}
