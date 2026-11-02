#ifndef BORROW_TICKET_H
#define BORROW_TICKET_H

#include "Date.h"
#include "Book.h"
#include "Reader.h"

enum TicketStatus { TICKET_BORROWING = 0, TICKET_RETURNED = 1, TICKET_OVERDUE = 2 };

const int TICKET_ID_LEN = 8;

struct BorrowTicket {
    char id[TICKET_ID_LEN + 1];       // "PM00001"
    char cmnd[CMND_LEN + 1];          // -> Reader
    char isbn[ISBN_LEN + 1];          // -> Book
    Date borrowDate, dueDate, returnDate;   // returnDate = {0,0,0} nếu chưa trả
    int  status;                      // TicketStatus
    long fine;                        // tiền phạt (đồng), tính khi trả
};

#endif
