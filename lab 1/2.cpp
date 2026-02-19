#include<bits/stdc++.h>
using namespace std;

set<string> keywords = {"int", "float", "char", "double", "if", "else", "while", "for", "return", "void", "main"};
set<char> operators = {'+', '-', '*', '/', '=', '<', '>', '%'};
set<char> punctuators = {';', ',', '(', ')', '{', '}', '[', ']'};

bool isKeyword(string s) {
    return keywords.find(s) != keywords.end();
}

bool isOperator(char c) {
    return operators.find(c) != operators.end();
}

bool isPunctuator(char c) {
    return punctuators.find(c) != punctuators.end();
}

bool isNumber(string s) {
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return !s.empty();
}

void detectTokens(string input) {
    string buffer;
    cout << "Token\t\t\tType" << endl;
    cout << "------------------------------------" << endl;

    for (int i = 0; i < input.length(); i++) {
        char c = input[i];

        if (isPunctuator(c)) {
            cout << c << "\t\t\tPunctuator" << endl;
        }
        else if (isOperator(c)) {
            if (i + 1 < input.length() && isOperator(input[i+1])) {
                cout << c << input[i+1] << "\t\t\tOperator" << endl;
                i++;
            } else {
                cout << c << "\t\t\tOperator" << endl;
            }
        }
        else if (isalnum(c) || c == '_') {
            buffer += c;
            if (i + 1 == input.length() || (!isalnum(input[i+1]) && input[i+1] != '_')) {
                if (isKeyword(buffer))
                    cout << buffer << "\t\t\tKeyword" << endl;
                else if (isNumber(buffer))
                    cout << buffer << "\t\t\tConstant" << endl;
                else
                    cout << buffer << "\t\t\tIdentifier" << endl;
                buffer = "";
            }
        }
    }
}


int main() {
    string code;
    cout << "Enter a line of C code: ";
    getline(cin, code);

    detectTokens(code);

    return 0;
}