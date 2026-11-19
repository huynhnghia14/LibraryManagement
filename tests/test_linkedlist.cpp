#include "LinkedList.h"

#include <cassert>
#include <iostream>

void testPushBackKeepsOrder() {
    LinkedList<int> l;
    for (int i = 1; i <= 5; ++i) l.pushBack(i);
    int expect = 1;
    l.forEach([&](int x) { assert(x == expect++); });
    assert(l.size() == 5);
}

void testFindRemoveCount() {
    LinkedList<int> l;
    for (int i = 1; i <= 10; ++i) l.pushBack(i);
    assert(*l.find([](int x) { return x > 7; }) == 8);
    assert(!l.find([](int x) { return x > 100; }));
    assert(l.countIf([](int x) { return x % 2 == 0; }) == 5);
    assert(l.remove([](int x) { return x == 1; }));      // xóa đầu
    assert(l.remove([](int x) { return x == 10; }));     // xóa cuối -> tail phải cập nhật
    l.pushBack(11);
    int last = 0;
    l.forEach([&](int x) { last = x; });
    assert(last == 11 && l.size() == 9);
}

void testQueueBehaviour() {
    LinkedList<int> q;
    q.pushBack(1);
    q.pushBack(2);
    int x;
    assert(q.popFront(x) && x == 1);
    q.pushBack(3);
    assert(q.popFront(x) && x == 2);
    assert(q.popFront(x) && x == 3);
    assert(!q.popFront(x) && q.empty());
    q.pushBack(4);                                        // sau khi rỗng vẫn thêm được
    assert(q.popFront(x) && x == 4);
}

int main() {
    testPushBackKeepsOrder();
    testFindRemoveCount();
    testQueueBehaviour();
    std::cout << "test_linkedlist: passed" << std::endl;
    return 0;
}
