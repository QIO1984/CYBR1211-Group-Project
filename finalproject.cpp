// finalproject.cpp
// Simple library inventory with persistence (inventory.txt)
// Compile: g++ -std=c++17 finalproject.cpp -o finalproject

#include <bits/stdc++.h>
using namespace std;

const string FILENAME = "inventory.txt";
struct Book {
    int id;
    string title;
    string author;
    bool rented;
};

vector<Book> loadInventory() {
    vector<Book> books;
    ifstream in(FILENAME);
    if (!in) return books;
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        // expected format: id|title|author|rented(0/1)
        stringstream ss(line);
        string token;
        Book b{};
        if (!getline(ss, token, '|')) continue;
        b.id = stoi(token);
        if (!getline(ss, b.title, '|')) continue;
        if (!getline(ss, b.author, '|')) continue;
        if (!getline(ss, token, '|')) continue;
        b.rented = (token == "1");
        books.push_back(b);
    }
    return books;
}

void saveInventory(const vector<Book>& books) {
    ofstream out(FILENAME, ios::trunc);
    for (const auto& b : books) {
        out << b.id << '|' << b.title << '|' << b.author << '|' << (b.rented ? '1' : '0') << '\n';
    }
}

int nextId(const vector<Book>& books) {
    int mx = 0;
    for (auto &b : books) if (b.id > mx) mx = b.id;
    return mx + 1;
}

void addBook(vector<Book>& books) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    Book b;
    b.id = nextId(books);
    cout << "Title: ";
    getline(cin, b.title);
    cout << "Author: ";
    getline(cin, b.author);
    b.rented = false;
    books.push_back(b);
    saveInventory(books);
    cout << "Added book ID " << b.id << ".\n";
}

void removeBook(vector<Book>& books) {
    cout << "Enter book ID to remove: ";
    int id; if (!(cin >> id)) { cin.clear(); cin.ignore(10000,'\n'); return; }
    auto it = find_if(books.begin(), books.end(), [&](const Book& b){ return b.id == id; });
    if (it == books.end()) {
        cout << "Book not found.\n";
        return;
    }
    if (it->rented) {
        cout << "Cannot remove a rented book. Return it first.\n";
        return;
    }
    books.erase(it);
    saveInventory(books);
    cout << "Removed book ID " << id << ".\n";
}

void listBooks(const vector<Book>& books, bool showRented) {
    cout << (showRented ? "Rented books:\n" : "Available books:\n");
    bool any = false;
    for (const auto& b : books) {
        if (b.rented == showRented) {
            any = true;
            cout << "ID: " << b.id << " | " << b.title << " by " << b.author << '\n';
        }
    }
    if (!any) cout << "(none)\n";
}

void rentBook(vector<Book>& books) {
    cout << "Enter book ID to rent: ";
    int id; if (!(cin >> id)) { cin.clear(); cin.ignore(10000,'\n'); return; }
    auto it = find_if(books.begin(), books.end(), [&](const Book& b){ return b.id == id; });
    if (it == books.end()) {
        cout << "Book not found.\n";
        return;
    }
    if (it->rented) {
        cout << "Book is already rented.\n";
        return;
    }
    it->rented = true;
    saveInventory(books);
    cout << "Book ID " << id << " rented.\n";
}

void returnBook(vector<Book>& books) {
    cout << "Enter book ID to return: ";
    int id; if (!(cin >> id)) { cin.clear(); cin.ignore(10000,'\n'); return; }
    auto it = find_if(books.begin(), books.end(), [&](const Book& b){ return b.id == id; });
    if (it == books.end()) {
        cout << "Book not found.\n";
        return;
    }
    if (!it->rented) {
        cout << "Book is not rented.\n";
        return;
    }
    it->rented = false;
    saveInventory(books);
    cout << "Book ID " << id << " returned.\n";
}

void librarianMenu(vector<Book>& books) {
    while (true) {
        cout << "\nLibrarian Menu:\n"
             << "1) Add book\n"
             << "2) Remove book\n"
             << "3) List all books\n"
             << "4) Back\n"
             << "Choice: ";
        int c; if (!(cin >> c)) { cin.clear(); cin.ignore(10000,'\n'); continue; }
        switch (c) {
            case 1: addBook(books); break;
            case 2: removeBook(books); break;
            case 3:
                for (const auto& b : books) {
                    cout << "ID:" << b.id << " | " << (b.rented ? "[Rented] " : "[Available] ")
                         << b.title << " by " << b.author << '\n';
                }
                break;
            case 4: return;
            default: cout << "Invalid.\n";
        }
    }
}

void userMenu(vector<Book>& books) {
    while (true) {
        cout << "\nUser Menu:\n"
             << "1) View available books\n"
             << "2) View rented books\n"
             << "3) Rent a book\n"
             << "4) Return a book\n"
             << "5) Back\n"
             << "Choice: ";
        int c; if (!(cin >> c)) { cin.clear(); cin.ignore(10000,'\n'); continue; }
        switch (c) {
            case 1: listBooks(books, false); break;
            case 2: listBooks(books, true); break;
            case 3: rentBook(books); break;
            case 4: returnBook(books); break;
            case 5: return;
            default: cout << "Invalid.\n";
        }
    }
}

int main() {
    vector<Book> books = loadInventory();
    while (true) {
        cout << "\nMain Menu:\n1) Librarian\n2) User\n3) Quit\nChoice: ";
        int c; if (!(cin >> c)) { cin.clear(); cin.ignore(10000,'\n'); continue; }
        if (c == 1) {
            // simple password check
            cout << "Enter librarian password: ";
            string pass;
            cin >> pass;
            if (pass != "admin") {
                cout << "Wrong password.\n";
            } else {
                librarianMenu(books);
            }
        } else if (c == 2) {
            userMenu(books);
        } else if (c == 3) {
            cout << "Exiting.\n";
            break;
        } else {
            cout << "Invalid.\n";
        }
    }
    return 0;
}