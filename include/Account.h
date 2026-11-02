#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "Reader.h"

enum Role { ROLE_ADMIN = 0, ROLE_LIBRARIAN = 1, ROLE_READER = 2 };

const int USERNAME_LEN = 20;

struct Account {
    char username[USERNAME_LEN + 1];
    unsigned long long passwordHash;  // FNV-1a(salt + password)
    int  role;                        // Role
    char cmnd[CMND_LEN + 1];          // liên kết độc giả (rỗng với Admin / Thủ thư)
};

#endif
