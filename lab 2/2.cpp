#include <bits/stdc++.h>
#define MAX 166
using namespace std;
int main() {
    string str;
    cout<<"enter the line"<<endl;
    cin>>str;
   
    int i = 0;
    for (i = 0; i < str.size(); i++) {
        if (str[i] == ' ')
            continue;
        else
            break;
    }
    if (str[i] == '/' && str[i + 1] == '/') {
        printf("\nThis line is the comment\n");
    } else {
        printf("\nThis line is not the comment\n");
    }
    return 0;
}