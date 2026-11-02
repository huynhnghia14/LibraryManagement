#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "BorrowTicket.h"

enum TransType { TRANS_BORROW = 0, TRANS_RETURN = 1, TRANS_FINE = 2 };

struct Transaction {
    long id;                          // tự tăng
    int  type;                        // TransType
    char ticketId[TICKET_ID_LEN + 1];
    char cmnd[CMND_LEN + 1];
    char isbn[ISBN_LEN + 1];
    Date date;
    long amount;                      // tiền phạt với TRANS_FINE, 0 với loại khác
};

#endif
