#ifndef BOOK_H
#define BOOK_H

const int ISBN_LEN = 13, TITLE_LEN = 100, AUTHOR_LEN = 50, CATEGORY_LEN = 30;

struct Book {
    char isbn[ISBN_LEN + 1];          // khóa chính, 13 chữ số
    char title[TITLE_LEN + 1];
    char author[AUTHOR_LEN + 1];
    char category[CATEGORY_LEN + 1];
    int  year;                        // 1900..năm hiện tại
    int  quantity;                    // tổng số bản
    int  available;                   // số bản còn trong kho (0..quantity)
};

#endif
