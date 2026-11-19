#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include "Book.h"
#include "Reader.h"
#include "BorrowTicket.h"
#include "Transaction.h"
#include "Config.h"
#include "AVLTree.h"
#include "BST.h"
#include "HashTable.h"
#include "LinkedList.h"
#include "DynamicArray.h"
#include "FileStorage.h"

// Gom toàn bộ CTDL của chương trình (facade) và việc nạp / lưu / sao lưu dữ liệu.
// Logic nghiệp vụ nằm trong các Service.
class Library {
    std::string dataDir;

    std::string path(const std::string& name) const { return dataDir + "/" + name; }
    void clearAll();

public:
    AVLTree<std::string, Book>        books;        // ISBN -> sách (nguồn dữ liệu chính)
    BST<std::string, std::string>     byTitle;      // tên chuẩn hóa -> ISBN (in danh mục theo tên)
    HashTable<std::string, Reader>    readers;      // CMND -> độc giả
    LinkedList<BorrowTicket>          tickets;      // mọi phiếu mượn theo thứ tự lập
    DynamicArray<Transaction>         history;      // lịch sử giao dịch (chỉ thêm vào cuối)
    Config                            config;

    long nextTicketNo;
    long nextTransId;
    bool dirty;

    explicit Library(const std::string& dir = "data");

    const std::string& dir() const { return dataDir; }

    LoadStatus load(std::string& error);            // đọc data/*.dat khi khởi động
    bool save();                                    // ghi lại toàn bộ khi có thay đổi / khi thoát
    bool backup(const Date& today);                 // data/backup/YYYY-MM-DD/
    bool restore(const std::string& backupName, std::string& error);
    DynamicArray<std::string> listBackups() const;
    void rebuildIndexes();                          // dựng lại BST và bộ đếm mã sau khi nạp
    void markDirty() { dirty = true; }
};

#endif
