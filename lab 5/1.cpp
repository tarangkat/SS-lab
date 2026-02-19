#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <stack>
using namespace std;

map<char, vector<string>> grammar;
map<char, set<char>> FIRST, FOLLOW;
map<char, map<char, string>> table;

bool isTerminal(char c) {
    return !(c >= 'A' && c <= 'Z');
}


void findFirst(char c) {

    for (auto prod : grammar[c]) {

        if (prod == "#") {
            FIRST[c].insert('#');
        }
        else {
            for (int i = 0; i < prod.length(); i++) {

                char symbol = prod[i];

                if (isTerminal(symbol)) {
                    FIRST[c].insert(symbol);
                    break;
                }
                else {
                    findFirst(symbol);

                    for (auto x : FIRST[symbol]) {
                        if (x != '#')
                            FIRST[c].insert(x);
                    }

                    if (FIRST[symbol].count('#') == 0)
                        break;

                    if (i == prod.length() - 1)
                        FIRST[c].insert('#');
                }
            }
        }
    }
}


void findFollow(char c) {
 
    if (c == 'E')   
        FOLLOW[c].insert('$');

    for (auto g : grammar) {

        char lhs = g.first;

        for (auto prod : g.second) {

            for (int i = 0; i < prod.length(); i++) {

                if (prod[i] == c) {

                    if (i + 1 < prod.length()) {

                        char next = prod[i + 1];

                        if (isTerminal(next))
                            FOLLOW[c].insert(next);
                        else {
                            for (auto x : FIRST[next]) {
                                if (x != '#')
                                    FOLLOW[c].insert(x);
                            }

                            if (FIRST[next].count('#'))
                                for (auto x : FOLLOW[lhs])
                                    FOLLOW[c].insert(x);
                        }
                    }
                    else {
                        if (lhs != c)
                            for (auto x : FOLLOW[lhs])
                                FOLLOW[c].insert(x);
                    }
                }
            }
        }
    }
}


void constructTable() {

    for (auto g : grammar) {

        char lhs = g.first;

        for (auto prod : g.second) {

            set<char> firstSet;

            if (prod == "#")
                firstSet.insert('#');
            else {
                for (int i = 0; i < prod.length(); i++) {

                    char symbol = prod[i];

                    if (isTerminal(symbol)) {
                        firstSet.insert(symbol);
                        break;
                    }
                    else {
                        for (auto x : FIRST[symbol]) {
                            if (x != '#')
                                firstSet.insert(x);
                        }

                        if (FIRST[symbol].count('#') == 0)
                            break;

                        if (i == prod.length() - 1)
                            firstSet.insert('#');
                    }
                }
            }

            for (auto t : firstSet) {
                if (t != '#')
                    table[lhs][t] = prod;
            }

            if (firstSet.count('#')) {
                for (auto f : FOLLOW[lhs])
                    table[lhs][f] = "#";
            }
        }
    }
}

 
void displayTable() {

    cout << "\nLL(1) Parsing Table:\n\n";
    cout << "\t i\t+\t*\t(\t)\t$\n";

    for (auto g : grammar) {
        char nt = g.first;
        cout << nt << "\t";

        char terminals[] = {'i','+','*','(',')','$'};

        for (char t : terminals) {
            if (table[nt][t] != "")
                cout << nt << "->" << table[nt][t] << "\t";
            else
                cout << "-\t";
        }
        cout << endl;
    }
}

void parseString(string input) {

    stack<char> st;
    st.push('$');
    st.push('E');

    input += "$";
    int i = 0;

    while (!st.empty()) {

        char top = st.top();
        char current = input[i];

        if (top == current) {
            st.pop();
            i++;
        }
        else if (isTerminal(top)) {
            cout << "\n String Rejected\n";
            return;
        }
        else {
            if (table[top][current] != "") {

                string prod = table[top][current];
                st.pop();

                if (prod != "#") {
                    for (int j = prod.length()-1; j >= 0; j--)
                        st.push(prod[j]);
                }
            }
            else {
                cout << "\n String Rejected\n";
                return;
            }
        }
    }

    if (i == input.length())
        cout << "\n String Accepted\n";
    else
        cout << "\n String Rejected\n";
}

int main() {

    
    grammar['E'] = {"TE"};
    grammar['T'] = {"FT"};
    grammar['F'] = {"i", "(E)"};
    grammar['E'] = {"TE"};
    grammar['T'] = {"FT"};
    
    grammar['R'] = {"+TE", "#"};  
    grammar['Y'] = {"*FT", "#"};  

    
    grammar.clear();
    grammar['E'] = {"TR"};
    grammar['R'] = {"+TR", "#"};
    grammar['T'] = {"FY"};
    grammar['Y'] = {"*FY", "#"};
    grammar['F'] = {"i", "(E)"};

    for (auto g : grammar)
        findFirst(g.first);

    for (auto g : grammar)
        findFollow(g.first);

    constructTable();

    displayTable();

    string input;
    cout << "\nEnter input string (use i for id): ";
    cin >> input;

    parseString(input);

    return 0;
}
