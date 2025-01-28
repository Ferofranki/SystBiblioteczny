#include <iostream>
#include <string>

using namespace std;

struct User {
    int id;
    string firstName;
    string lastName;
    string birthDate;
    string adres;
};

struct Book {
    int id;
    string title;
    string author;
    int volume;
    string publisher;
    string releaseDate;
    User* pazycz;
};

struct Library {
    User** users;
    Book** books;
    int userCount;
    int bookCount;
    int maxUsers;
    int maxBooks;
};

Library* stwarLibrary(int initialUsers = 2, int initialBooks = 2) {
    Library* lib = new Library;
    lib->users = new User * [initialUsers];
    lib->books = new Book * [initialBooks];
    lib->userCount = 0;
    lib->bookCount = 0;
    lib->maxUsers = initialUsers;
    lib->maxBooks = initialBooks;
    return lib;
}

void addUser(Library* lib, string firstName, string lastName, string birthDate, string adres) {
    if (lib->userCount >= lib->maxUsers) {
        int newSize = lib->maxUsers * 2;
        User** newUsers = new User * [newSize];
        for (int i = 0; i < lib->userCount; ++i) {
            newUsers[i] = lib->users[i];
        }
        delete[] lib->users;
        lib->users = newUsers;
        lib->maxUsers = newSize;
    }

    User* newUser = new User;
    newUser->id = lib->userCount + 1;
    newUser->firstName = firstName;
    newUser->lastName = lastName;
    newUser->birthDate = birthDate;
    newUser->adres = adres;

    lib->users[lib->userCount++] = newUser;
}

void addBook(Library* lib, string title, string author, int volume, string publisher, string releaseDate) {
    if (lib->bookCount >= lib->maxBooks) {
        int newSize = lib->maxBooks * 2;
        Book** newBooks = new Book * [newSize];
        for (int i = 0; i < lib->bookCount; ++i) {
            newBooks[i] = lib->books[i];
        }
        delete[] lib->books;
        lib->books = newBooks;
        lib->maxBooks = newSize;
    }

    Book* newBook = new Book;
    newBook->id = lib->bookCount + 1;
    newBook->title = title;
    newBook->author = author;
    newBook->volume = volume;
    newBook->publisher = publisher;
    newBook->releaseDate = releaseDate;
    newBook->pazycz = nullptr;

    lib->books[lib->bookCount++] = newBook;
}

User* findUserById(Library* lib, int userId) {
    for (int i = 0; i < lib->userCount; ++i) {
        if (lib->users[i]->id == userId) {
            return lib->users[i];
        }
    }
    return nullptr;
}

Book* findBookById(Library* lib, int bookId) {
    for (int i = 0; i < lib->bookCount; ++i) {
        if (lib->books[i]->id == bookId) {
            return lib->books[i];
        }
    }
    return nullptr;
}

bool pazyczBook(Library* lib, int userId, int bookId) {
    User* user = findUserById(lib, userId);
    Book* book = findBookById(lib, bookId);

    if (user && book && !book->pazycz) {
        book->pazycz = user;
        return true;
    }
    return false;
}

bool returnBook(Library* lib, int bookId) {
    Book* book = findBookById(lib, bookId);
    if (book && book->pazycz) {
        book->pazycz = nullptr;
        return true;
    }
    return false;
}

void pokazAllBooks(Library* lib) {
    cout << "\n==== Wszystkie ksiazki ====\n";
    for (int i = 0; i < lib->bookCount; ++i) {
        Book* book = lib->books[i];
        cout << "ID: " << book->id
            << "\nTytul: " << book->title
            << "\nAutor: " << book->author
            << "\nTom: " << book->volume
            << "\nWydawca: " << book->publisher
            << "\nData wydania: " << book->releaseDate
            << "\nStatus: ";
        if (book->pazycz) {
            cout << "Wypozyczona przez (ID: " << book->pazycz->id
                << ") " << book->pazycz->firstName << " " << book->pazycz->lastName;
        }
        else {
            cout << "Dostepna";
        }
        cout << "\n\n";
    }
}

void pokazUserBooks(Library* lib, int userId) {
    User* user = findUserById(lib, userId);
    if (!user) {
        cout << "Uzytkownik nie znaleziony!\n";
        return;
    }

    cout << "\n==== ksiazki wypożyczone przez (ID: " << user->id << ") "
        << user->firstName << " " << user->lastName << " ====\n";

    bool hasBooks = false;
    for (int i = 0; i < lib->bookCount; ++i) {
        if (lib->books[i]->pazycz == user) {
            hasBooks = true;
            cout << "ID ksiazki: " << lib->books[i]->id
                << "\nTytul: " << lib->books[i]->title
                << "\nAutor: " << lib->books[i]->author
                << "\nTom: " << lib->books[i]->volume
                << "\n\n";
        }
    }

    if (!hasBooks) cout << "Brak wypozyczonych ksiaek\n";
}

void freeLibrary(Library* lib) {
    for (int i = 0; i < lib->userCount; ++i) delete lib->users[i];
    for (int i = 0; i < lib->bookCount; ++i) delete lib->books[i];
    delete[] lib->users;
    delete[] lib->books;
    delete lib;
}

void displayMenu() {
    cout << "\n==== System Biblioteczny ====\n";
    cout << "1. Dodaj nowego uzytkownika\n";
    cout << "2. Dodaj nowa ksiazke\n";
    cout << "3. Wypozycz ksiazke\n";
    cout << "4. Zwroc ksiazke\n";
    cout << "5. Wyswietl wszystkie ksiazki\n";
    cout << "6. Wyswietl ksiazki uzytkownika\n";
    cout << "7. Wyjdz\n";
    cout << "Wybierz opcje: ";
}

void Input(Library* lib) {
    int choice;
    while (true) {
        displayMenu();

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore();
            cout << "Nieprawidlowy wybor!\n";
            continue;
        }

        switch (choice) {
        case 1: {
            string firstName, lastName, birthDate, adres;
            cout << "\nNowy uzytkownik:\n";
            cout << "Imie: "; cin >> firstName;
            cout << "Nazwisko: "; cin >> lastName;
            cout << "Data urodzenia (DD-MM-YYYY): "; cin >> birthDate;
            cin.ignore();
            cout << "Adres: "; getline(cin, adres);

            addUser(lib, firstName, lastName, birthDate, adres);
            cout << "Uzytkownik dodany pomyslnie! ID: " << lib->userCount << "\n";
            break;
        }

        case 2: {
            string title, author, publisher, releaseDate;
            int volume;
            cout << "\nNowa ksiazka:\n";
            cout << "Tytul: "; cin.ignore(); getline(cin, title);
            cout << "Autor: "; getline(cin, author);
            cout << "Tom: ";
            while (!(cin >> volume)) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Nieprawidlowy tom! Wpisz liczbe: ";
            }
            cin.ignore(1000, '\n');
            cout << "Wydawca: "; getline(cin, publisher);
            cout << "Data wydania (RRRR-MM-DD): "; cin >> releaseDate;

            addBook(lib, title, author, volume, publisher, releaseDate);
            cout << "Ksiazka dodana pomyslnie! ID: " << lib->bookCount << "\n";
            break;
        }

        case 3: {
            int userId, bookId;
            cout << "Podaj ID uzytkownika: "; cin >> userId;
            cout << "Podaj ID ksiazki: "; cin >> bookId;

            if (pazyczBook(lib, userId, bookId)) {
                cout << "Ksiazka wypozyczona pomyslnie!\n";
            }
            else {
                cout << "Blad wypozyczenia! Sprawdz ID lub dostepnosc ksiazki.\n";
            }
            break;
        }

        case 4: {
            int bookId;
            cout << "Podaj ID ksiazki do zwrotu: "; cin >> bookId;

            if (returnBook(lib, bookId)) {
                cout << "Ksiazka zwrocona pomyslnie!\n";
            }
            else {
                cout << "Blad zwrotu ksiazki!\n";
            }
            break;
        }

        case 5:
            pokazAllBooks(lib);
            break;

        case 6: {
            int userId;
            cout << "Podaj ID uzytkownika: "; cin >> userId;
            pokazUserBooks(lib, userId);
            break;
        }

        case 7:
            cout << "Koniec programu.\n";
            return;

        default:
            cout << "Nieprawidlowa opcja!\n";
        }

        cin.ignore();
    }
}

int main() {
    Library* library = stwarLibrary();
    Input(library);
    freeLibrary(library);
    return 0;
}