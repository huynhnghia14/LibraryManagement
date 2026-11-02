#ifndef AUTH_H
#define AUTH_H

#include <string>
#include "Account.h"

class Library;

enum Action {
    ACT_MANAGE_DATA, ACT_BORROW_RETURN, ACT_STATISTICS,
    ACT_SEARCH, ACT_VIEW_OWN_TICKETS, ACT_BACKUP, ACT_RESTORE, ACT_MANAGE_ACCOUNTS
};

struct Session {
    bool loggedIn;
    std::string username;
    int role;                       // Role
    std::string cmnd;
};

// Đăng nhập và phân quyền Admin / Thủ thư / Độc giả
class Auth {
public:
    static unsigned long long hashPassword(const std::string& password);   // FNV-1a + salt
    static bool login(Library& lib, Session& session);
    static bool can(const Session& session, Action action);
    static void ensureDefaultAdmin(Library& lib);
};

#endif
