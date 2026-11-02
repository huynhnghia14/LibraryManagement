#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <cstddef>
#include "DynamicArray.h"

const int ALPHABET = 37;                            // a-z (26) + 0-9 (10) + ' ' (1)

struct TrieNode {
    TrieNode* children[ALPHABET];
    bool isEnd;
    DynamicArray<int> bookIdx;                      // chỉ số các sách có đúng tên này
};

// Cây Trie – tìm sách theo tiền tố tên (CTDL tự tìm hiểu, cài đặt ở tuần 8)
class Trie {
    TrieNode* root;
    int nodeCount;

    static int charIndex(char c);                   // a-z -> 0..25, 0-9 -> 26..35, ' ' -> 36
    void collect(TrieNode* n, std::string& cur, DynamicArray<std::string>& out, int limit);
    bool removeRec(TrieNode* n, const std::string& w, int depth, int bookIdx);

public:
    Trie();
    ~Trie();

    void insert(const std::string& word, int bookIdx);
    bool remove(const std::string& word, int bookIdx);
    DynamicArray<std::string> startsWith(const std::string& prefix, int limit = 20);
    bool exists(const std::string& word) const;
    int  nodes() const { return nodeCount; }
    size_t memoryBytes() const;
};

#endif
