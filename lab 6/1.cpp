#include <iostream>

#include <string>
using namespace std;
string input;
int pos = 0;
bool E();
bool Eprime();

bool T();

bool Tprime();

bool F();

bool E() {

   if (T()) {
       return Eprime();
   }
   return false;

}

bool Eprime() {

   if (input[pos] == '+') {

       pos++;

       if (T()) {

           return Eprime();

       }

       return false;

   }
  return true;

}
bool T() {

   if (F()) {

       return Tprime();

   }

   return false;

}
bool Tprime() {

   if (input[pos] == '*') {
       pos++;
       if (F()) {
           return Tprime();
       }
       return false;
   }
   return true;

}
bool F() {

   if (isalnum(input[pos])) {
       pos++;
       return true;
   }
   else if (input[pos] == '(') {
      pos++;
       if (E()) {
           if (input[pos] == ')') {

               pos++;
               return true;
           }
       }
       return false;
   }
   return false;
}

int main() {

   cout << "Enter the string to be checked: ";
   cin >> input;

   pos = 0;
   if (E() && pos == input.length()) {
       cout << "String is Accepted.\n";
   } else {
       cout << "String is Rejected.\n";
   }
   return 0;
}




