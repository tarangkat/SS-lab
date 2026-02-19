#include<iostream>
using namespace std;

int main(){ 


    cout<<"enter input"<<endl;
    string input;
    cin>>input;
    
    int i=0;
    int n=input.size();

    if(i<n && input[i]=='b'){
        i++;
    }else{
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
        cout<<"Accepted"<<endl;
        return 0;
    }
    cout<<"not accepted"<<endl;

    return 0;
}