#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <vector>

struct Book {
    int id;
    std::string title;
    std::string author;
    int year;
    bool isAvailable;
};

class Library {
private:
    std::vector<Book> books;

public:
    Library();
    void addBook(const Book& book);
    void displayBooks() const;
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const;
};

#endif // LIBRARY_H
