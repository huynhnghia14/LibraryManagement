#ifndef READER_H
#define READER_H

#include "Date.h"

enum CardType     { CARD_STUDENT = 0, CARD_LECTURER = 1, CARD_NORMAL = 2 };
enum ReaderStatus { READER_ACTIVE = 0, READER_LOCKED = 1 };

const int CMND_LEN = 12, NAME_LEN = 50, EMAIL_LEN = 50, PHONE_LEN = 11;

struct Reader {
    char cmnd[CMND_LEN + 1];          // khóa chính, 9 hoặc 12 chữ số
    char name[NAME_LEN + 1];
    char email[EMAIL_LEN + 1];
    char phone[PHONE_LEN + 1];        // 10 chữ số
    int  cardType;                    // CardType
    Date regDate, expDate;            // expDate = regDate + 12 tháng
    int  status;                      // ReaderStatus
};

#endif
