#include<bits/stdc++.h>
using namespace std;

map<int, <pair<int, setchar>>> transitions;
set<int> acceptStates;

void buildDFAFromRegExp(const string &regExp) {
    int currentState = 0;
    int nextState = 1;

    for (int i = 0; i < regExp.length(); i++) {
        char c = regExp[i];

        if (c != '*' && c != '+') {
            transitions[currentState].insert({nextState, c});
            
            if (i + 1 < regExp.length()) {
                if (regExp[i + 1] == '*') {
                    transitions[nextState].insert({nextState, c});
                    i++; 
                } 
                else if (regExp[i + 1] == '+') {
                    transitions[nextState].insert({nextState, c});
                    i++; 
                }
            }
            
            currentState = nextState;
            nextState++;
        }
    }
    acceptStates.insert(currentState);
}


void printTransitionTable() {
    set<char> alphabet;
    for (auto const& y : transitions) {
        const auto& transSet = y.second;
        
        for (auto const& target : transSet) {
            alphabet.insert(target.second);
        }
    }

    cout << "\n--- DFA Transition Table ---\n";
    cout << "State\t| ";
    for (char c : alphabet) cout << c << "\t| ";
    cout << "Accept?";
    cout << "\n--------------------------------------------\n";

    int maxState = acceptStates.empty() ? 0 : *acceptStates.rbegin();
    
    for (int i = 0; i <= maxState; i++) {
        cout << i << "\t| ";
        for (char c : alphabet) {
            bool found = false;
            for (auto const& target : transitions[i]) {
                if (target.second == c) {
                    cout << target.first << "\t| ";
                    found = true;
                    break;
                }
            }
            if (!found) cout << "-\t| ";
        }
        if (acceptStates.count(i)) cout << "Yes";
        else cout << "No";
        cout << endl;
    }
}

int main() {
    string regExp;
    cout << "Enter a simple Regular Expression (e.g., a*b+c): ";
    cin >> regExp;

    buildDFAFromRegExp(regExp);
    printTransitionTable();

    return 0;
}