#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

struct Book
{
    string title;
    string author;
    bool canBeBorrowed;
    Book() : title(""), author(""), canBeBorrowed(true) {}
};
class Library
{
private:
    string adminUsername;
    string adminPassword;
    Book* books;
    int bookCount;
public:
    Library()
    {
        adminUsername = "admin";
        adminPassword = "admin";
        books = nullptr;
        bookCount = 0;
    }

    bool adminLogin(string username, string password)
    {
        if (username == adminUsername && password == adminPassword)
        {
            cout << "Administrator logged in." << endl;
            return true;
        }
        else
        {
            cout << "Invalid username or password." << endl;
            return false;
        }
    }

    void addBook(string title, string author)
    {
        Book newBook;
        newBook.title = title;
        newBook.author = author;
        newBook.canBeBorrowed = true;

        ofstream outputFile("books.txt", ios::app);
        if (outputFile.is_open())
        {
            outputFile << newBook.title << "," << newBook.author << "," << newBook.canBeBorrowed << endl;
            outputFile.close();
            cout << "Book added: " << title << " - " << author << endl;
        }
        else
        {
            cout << "Unable to open the books file." << endl;
        }
    }

    void removeBook(string title)
    {
        ifstream inputFile("books.txt");
        ofstream outputFile("temp.txt");

        string line;
        bool bookFound = false;

        while (getline(inputFile, line))
        {
            stringstream ss(line);
            string bookTitle, bookAuthor;
            bool canBeBorrowed;

            getline(ss, bookTitle, ',');
            getline(ss, bookAuthor, ',');
            ss >> canBeBorrowed;

            if (bookTitle == title)
            {
                bookFound = true;
                continue;
            }

            outputFile << line << endl;
        }

        inputFile.close();
        outputFile.close();

        remove("books.txt");
        rename("temp.txt", "books.txt");

        if (bookFound)
        {
            cout << "Book removed: " << title << endl;
        }
        else
        {
            cout << "Book not found." << endl;
        }
    }

    void listBooks()
    {
        ifstream inputFile("books.txt");

        if (inputFile.is_open())
        {
            cout << endl << "--------------------------------------------------------------------------------------------------" << endl;

            string line;
            while (getline(inputFile, line))
            {
                stringstream ss(line);
                string bookTitle, bookAuthor;
                bool canBeBorrowed;

                getline(ss, bookTitle, ',');
                getline(ss, bookAuthor, ',');
                ss >> canBeBorrowed;
                cout << "Title: " << setw(20) << bookTitle << "  Author: " << setw(20) << bookAuthor << "  Status: ";
                if (canBeBorrowed) {
                    cout << "Available" << endl;
                }
                else {
                    cout << "Borrowed" << endl;
                }
            }

            cout << "--------------------------------------------------------------------------------------------------" << endl;
            inputFile.close();
        }
        else
        {
            cout << "Unable to open the books file." << endl;
        }
    }

    void borrowBook(string title, string author)
    {
        ifstream inputFile("books.txt");
        ofstream outputFile("temp.txt");

        string line;
        bool bookFound = false;

        while (getline(inputFile, line))
        {
            stringstream ss(line);
            string bookTitle, bookAuthor;
            bool canBeBorrowed;

            getline(ss, bookTitle, ',');
            getline(ss, bookAuthor, ',');
            ss >> canBeBorrowed;

            if (bookTitle == title && bookAuthor == author)
            {
                bookFound = true;
                canBeBorrowed = false;
            }

            outputFile << bookTitle << "," << bookAuthor << "," << canBeBorrowed << endl;
        }

        inputFile.close();
        outputFile.close();

        remove("books.txt");
        rename("temp.txt", "books.txt");

        if (bookFound)
        {
            cout << "Book borrowed: " << title << endl;
        }
        else
        {
            cout << "Book not found or already borrowed." << endl;
        }
    }

    void returnBook(string title, string author)
    {
        ifstream inputFile("books.txt");
        ofstream outputFile("temp.txt");

        if (!inputFile || !outputFile)
        {
            cout << "File could not be opened!" << endl;
            return;
        }

        string line;
        bool bookReturned = false;

        while (getline(inputFile, line))
        {
            stringstream ss(line);
            string bookTitle, bookAuthor;
            bool canBeBorrowed;

            getline(ss, bookTitle, ',');
            getline(ss, bookAuthor, ',');
            ss >> canBeBorrowed;

            if (bookTitle == title && bookAuthor == author)
            {
                bookReturned = true;
                canBeBorrowed = true;
            }

            outputFile << bookTitle << "," << bookAuthor << "," << canBeBorrowed << endl;
        }

        inputFile.close();
        outputFile.close();

        remove("books.txt");
        rename("temp.txt", "books.txt");

        if (bookReturned)
        {
            cout << "Book returned: " << title << endl;
        }
        else
        {
            cout << "Book not found or already returned." << endl;
        }
    }
};

int main()
{
    Library library;

    // Administrator login
    string username;
    string password;
    cout << "Administrator Login" << endl;
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;
    if (!library.adminLogin(username, password))
    {
        return 0;
    }

    int choice;
    string bookTitle, authorName;

    do
    {
        cout << endl;
        cout << "Select an action:" << endl;
        cout << "1. Add Book" << endl;
        cout << "2. Remove Book" << endl;
        cout << "3. List Books" << endl;
        cout << "4. Borrow Book" << endl;
        cout << "5. Return Book" << endl;
        cout << "6. Exit" << endl;
        cout << "Your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "Book Title: ";
            cin.ignore();
            getline(cin, bookTitle);
            cout << "Author Name: ";
            getline(cin, authorName);
            library.addBook(bookTitle, authorName);
            break;
        case 2:
            cout << "Book Title to Remove: ";
            cin.ignore();
            getline(cin, bookTitle);
            library.removeBook(bookTitle);
            break;
        case 3:
            library.listBooks();
            break;
        case 4:
            cout << "Book Title to Borrow: ";
            cin.ignore();
            getline(cin, bookTitle);
            cout << "Author Name: ";
            getline(cin, authorName);
            library.borrowBook(bookTitle, authorName);
            break;
        case 5:
            cout << "Book Title to Return: ";
            cin.ignore();
            getline(cin, bookTitle);
            cout << "Author Name: ";
            getline(cin, authorName);
            library.returnBook(bookTitle, authorName);
            break;
        case 6:
            cout << "Exiting the program..." << endl;
            break;
        default:
            cout << "Invalid choice. Try again." << endl;
            break;
        }
    } while (choice != 6);

    return 0;
}