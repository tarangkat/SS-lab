#include <iostream>
#include <string>
#include <cctype>

using namespace std;

int main() {
    string expr;
    string identifiers = "";
    string operators = "";
    
 
    bool expectIdentifier = true;
    bool isValid = true;

    cout << "Enter simple arithmetic expression (e.g., a + b * c): ";
    getline(cin, expr);

    for (int i = 0; i < expr.length(); i++) {
        char c = expr[i];

       
        if (c == ' ') continue;

     
        if (isalpha(c)) {
            if (!expectIdentifier) {
               
                isValid = false; 
                break;
                }
            identifiers += c;
            identifiers += " ";
            expectIdentifier = false;  
        }

        
         
        else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%') {
            if (expectIdentifier) {
              
                isValid = false;
                break;
            }
            operators += c;
            operators += " ";
            expectIdentifier = true; 
        }
        
        
        else {
            isValid = false;
            break;
        }
    }

   
    if (expectIdentifier) {
        isValid = false;
    }

 
    cout << "\n----------------------------\n";
    if (isValid) {
        cout << "Status: Valid Expression\n";
        cout << "Identifiers: " << identifiers << endl;
        cout << "Operators:   " << operators << endl;
    } else {
        cout << "Status: Invalid Expression" << endl;
    }
    cout << "----------------------------\n";

    return 0;
}