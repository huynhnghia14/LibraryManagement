#include "DynamicArray.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

void testGrowAndAccess() {
    DynamicArray<int> a;
    for (int i = 0; i < 1000; ++i) a.pushBack(i);
    assert(a.size() == 1000 && a.capacity() >= 1000);
    assert(a.at(999) == 999);
    bool thrown = false;
    try {
        a.at(1000);
    } catch (const std::out_of_range&) {
        thrown = true;
    }
    assert(thrown);
}

void testRemoveAt() {
    DynamicArray<int> a;
    for (int i = 0; i < 5; ++i) a.pushBack(i);
    a.removeAt(0);
    a.removeAt(3);
    assert(a.size() == 3 && a[0] == 1 && a[2] == 3);
}

void testCopyIsIndependent() {
    DynamicArray<std::string> a;
    a.pushBack("x");
    DynamicArray<std::string> b = a;
    b.pushBack("y");
    b[0] = "z";
    assert(a.size() == 1 && a[0] == "x" && b.size() == 2);
}

void testMergeSortStable() {
    struct Item { int key; int order; };
    DynamicArray<Item> a;
    int keys[] = {3, 1, 2, 1, 3, 2, 1};
    for (int i = 0; i < 7; ++i) a.pushBack(Item{keys[i], i});
    a.sort([](const Item& x, const Item& y) { return x.key < y.key; });
    for (int i = 1; i < a.size(); ++i) {
        assert(a[i - 1].key <= a[i].key);
        if (a[i - 1].key == a[i].key) assert(a[i - 1].order < a[i].order);   // giữ thứ tự ban đầu
    }
}

int main() {
    testGrowAndAccess();
    testRemoveAt();
    testCopyIsIndependent();
    testMergeSortStable();
    std::cout << "test_dynarray: passed" << std::endl;
    return 0;
}
