#include <iostream>
#include <string>
#include <cctype>

using namespace std;

/*
Grammar:
E -> TE'
E' -> +TE' | ε
T -> FT'
T' -> *FT' | ε
F -> id | (E)
*/

string input;
int pos = 0;
bool error = false;

void E();
void Eprime();
void T();
void Tprime();
void F();

void advance() {
    pos++;
}

// E -> TE'
void E() {
    T();
    Eprime();
}

// E' -> +TE' | ε
void Eprime() {
    if (input[pos] == '+') {
        advance();
        T();
        Eprime();
    }
    // ε transition: do nothing
}

// T -> FT'
void T() {
    F();
    Tprime();
}

// T' -> *FT' | ε
void Tprime() {
    if (input[pos] == '*') {
        advance();
        F();
        Tprime();
    }
    // ε transition: do nothing
}

// F -> id | (E)
void F() {
    if (isalnum(input[pos])) { // treats any alphanumeric char as 'id'
        advance();
    } else if (input[pos] == '(') {
        advance();
        E();
        if (input[pos] == ')') {
            advance();
        } else {
            error = true;
        }
    } else {
        error = true;
    }
}

int main() {
    cout << "Recursive Descent Parsing\n";
    cout << "Enter input string: ";
    cin >> input;

    // Call start symbol
    E();

    if (!error && pos == input.length()) {
        cout << "\nString Accepted\n";
    } else {
        cout << "\nString Rejected\n";
    }

    return 0;
}
