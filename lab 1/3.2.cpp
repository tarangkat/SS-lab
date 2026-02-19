// a*b+a*
#include<iostream>
using namespace std;

int main(){

    cout<<"enter "<<endl;
    string input;
    cin>>input;
    int i=0;
    int n =input.size();

    while(i < n && input[i]=='a'){
        i++;
    }
    if(i==n || input[i]!='b'){
        cout<<"not accepted"<<endl;
        return 0;
    }

    while(i<n && input[i]=='b'){
        i++;
    }

    while(i<n && input[i]=='a'){
        i++;
    }
    if(i==n){
        cout<<"accepted"<<endl;
        return 0;
    }
    cout<<"not accepted"<<endl;

    return 0;
}