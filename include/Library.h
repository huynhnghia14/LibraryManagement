#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include "Book.h"
#include "Reader.h"
#include "BorrowTicket.h"
#include "Transaction.h"
#include "Account.h"
#include "Config.h"
#include "AVLTree.h"
#include "BST.h"
#include "HashTable.h"
#include "LinkedList.h"
#include "DynamicArray.h"
#include "Graph.h"
#include "Trie.h"

// Gom toàn bộ CTDL của chương trình (facade); logic nghiệp vụ nằm trong các Service
class Library {
public:
    AVLTree<std::string, Book>                books;        // nguồn dữ liệu sách
    BST<std::string, Book*>                   byTitle;      // chỉ mục theo tên chuẩn hóa
    Trie                                      titleTrie;    // autocomplete
    HashTable<std::string, Reader>            readers;
    HashTable<std::string, DynamicArray<int> > byCategory;  // thể loại -> chỉ số đỉnh
    LinkedList<BorrowTicket>                  tickets;
    DynamicArray<Transaction>                 history;
    Graph                                     bookGraph;
    HashTable<std::string, Account>           accounts;
    Config                                    config;

    long nextTicketNo;
    long nextTransId;
    bool dirty;

    Library();

    bool load();
    bool save();
    bool backup();
    bool restore(const std::string& date);
    void rebuildIndexes();          // dựng byTitle, titleTrie, byCategory, bookGraph sau khi load
    void markDirty() { dirty = true; }
};

#endif
