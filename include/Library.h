#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include "Book.h"
#include "Reader.h"
#include "Config.h"
#include "AVLTree.h"
#include "BST.h"
#include "HashTable.h"

// Gom toàn bộ CTDL của chương trình (facade); logic nghiệp vụ nằm trong các Service
class Library {
public:
    AVLTree<std::string, Book>        books;        // ISBN -> sách (nguồn dữ liệu chính)
    BST<std::string, std::string>     byTitle;      // tên chuẩn hóa -> ISBN (in danh mục theo tên)
    HashTable<std::string, Reader>    readers;      // CMND -> độc giả
    Config                            config;

    bool dirty;

    Library() : dirty(false) {}

    void markDirty() { dirty = true; }
};

#endif
