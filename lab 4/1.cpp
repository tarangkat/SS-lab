#include <iostream>
#include <vector>
#include <set>
#include <map>
using namespace std;

int n;
vector<string> productions;
map<char, set<char>> FIRST, FOLLOW;

bool isTerminal(char c) {
    return !(c >= 'A' && c <= 'Z');
}


void findFirst(char c) {

    if (isTerminal(c)) {
        FIRST[c].insert(c);
        return;
    }

    for (int i = 0; i < n; i++) {

        if (productions[i][0] == c) {

            for (int j = 3; j < productions[i].length(); j++) {

                char next = productions[i][j];

                if (next == '#') {
                    FIRST[c].insert('#');
                    break;
                }

                if (isTerminal(next)) {
                    FIRST[c].insert(next);
                    break;
                }

                else {
                    findFirst(next);

                    for (auto x : FIRST[next]) {
                        if (x != '#')
                            FIRST[c].insert(x);
                    }

                    if (FIRST[next].count('#') == 0)
                        break;
                }

                if (j == productions[i].length() - 1)
                    FIRST[c].insert('#');
            }
        }
    }
}


void findFollow(char c) {

   
    if (c == productions[0][0])
        FOLLOW[c].insert('$');

    for (int i = 0; i < n; i++) {

        for (int j = 3; j < productions[i].length(); j++) {

            if (productions[i][j] == c) {

               
                if (j + 1 < productions[i].length()) {

                    char next = productions[i][j + 1];

                    if (isTerminal(next)) {
                        FOLLOW[c].insert(next);
                    }
                    else {
                        for (auto x : FIRST[next]) {
                            if (x != '#')
                                FOLLOW[c].insert(x);
                        }

                        if (FIRST[next].count('#')) {
                            if (productions[i][0] != c)
                                findFollow(productions[i][0]);

                            for (auto x : FOLLOW[productions[i][0]])
                                FOLLOW[c].insert(x);
                        }
                    }
                }

               
                else {
                    if (productions[i][0] != c) {
                        findFollow(productions[i][0]);
                        for (auto x : FOLLOW[productions[i][0]])
                            FOLLOW[c].insert(x);
                    }
                }
            }
        }
    }
}

int main() {

    cout << "Enter number of productions: ";
    cin >> n;

    cout << "Enter productions (Format: A->BC, use # for epsilon)\n";

    for (int i = 0; i < n; i++) {
        string p;
        cin >> p;
        productions.push_back(p);
    }

    
    for (int i = 0; i < n; i++)
        findFirst(productions[i][0]);

    
    for (int i = 0; i < n; i++)
        findFollow(productions[i][0]);

    cout << "\nFIRST Sets:\n";
    for (auto &p : FIRST) {
        if (p.first >= 'A' && p.first <= 'Z') {
            cout << "FIRST(" << p.first << ") = { ";
            for (auto x : p.second)
                cout << x << " ";
            cout << "}\n";
        }
    }

    cout << "\nFOLLOW Sets:\n";
    for (auto &p : FOLLOW) {
        cout << "FOLLOW(" << p.first << ") = { ";
        for (auto x : p.second)
            cout << x << " ";
        cout << "}\n";
    }

    return 0;
}

// S -> aSbS | bSaS | ∈


// E->TO 
// O->+TO
// O->#            
// T->FM
// M->*FM
// M->#
// F->(E)
// F->id