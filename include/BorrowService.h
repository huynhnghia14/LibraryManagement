#ifndef BORROW_SERVICE_H
#define BORROW_SERVICE_H

#include <string>
#include "Library.h"
#include "Result.h"

// Mượn – trả sách, tính tiền phạt (FR-L-01..06)
class BorrowService {
    Library& lib;

public:
    explicit BorrowService(Library& l) : lib(l) {}

    Result checkConstraints(const std::string& cmnd, const std::string& isbn, const Date& today);
    Result borrow(const std::string& cmnd, const std::string& isbn, const Date& today);
    Result returnBook(const std::string& ticketId, const Date& today);
    long   calcFine(const BorrowTicket& t, const Date& returnDate) const;
    int    refreshOverdue(const Date& today);           // cập nhật trạng thái quá hạn
    template <typename F> void listActive(F visit);
    template <typename F> void listOverdue(F visit);
    template <typename F> void historyOf(const std::string& cmnd, F visit);
};

#endif
