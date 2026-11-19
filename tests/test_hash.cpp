#include "HashTable.h"

#include <cassert>
#include <iostream>
#include <string>

void testInsertFindRemove() {
    HashTable<std::string, int> t(11);
    assert(t.insert("079012345678", 1));
    assert(!t.insert("079012345678", 2));
    assert(*t.find("079012345678") == 1);
    assert(t.remove("079012345678"));
    assert(!t.find("079012345678") && !t.remove("079012345678"));
}

void testChainsWithoutRehash() {
    HashTable<std::string, int> t(7);             // 5 khóa / 7 bucket = 0.71 -> chưa rehash, chắc chắn có va chạm
    const char* keys[] = {"111111111", "222222222", "333333333", "444444444", "555555555"};
    for (int i = 0; i < 5; ++i) assert(t.insert(keys[i], i));
    assert(t.bucketCount() == 7 && t.size() == 5);
    for (int i = 0; i < 5; ++i) {                 // xóa từng khóa, các khóa còn lại vẫn tìm thấy
        assert(t.remove(keys[i]));
        for (int j = i + 1; j < 5; ++j) assert(*t.find(keys[j]) == j);
    }
    assert(t.size() == 0);
}

void testRehashKeepsAllKeys() {
    HashTable<std::string, int> t(5);
    for (int i = 0; i < 1000; ++i) assert(t.insert("CMND" + std::to_string(i), i));
    assert(t.size() == 1000);
    assert(t.loadFactor() <= 0.75);
    assert(t.bucketCount() > 1000);
    for (int i = 0; i < 1000; ++i) assert(*t.find("CMND" + std::to_string(i)) == i);
}

void testForEachVisitsAll() {
    HashTable<std::string, int> t;
    int sum = 0;
    for (int i = 1; i <= 100; ++i) t.insert(std::to_string(i), i);
    t.forEach([&](const std::string&, int& v) { sum += v; });
    assert(sum == 5050);
}

int main() {
    testInsertFindRemove();
    testChainsWithoutRehash();
    testRehashKeepsAllKeys();
    testForEachVisitsAll();
    std::cout << "test_hash: passed" << std::endl;
    return 0;
}
