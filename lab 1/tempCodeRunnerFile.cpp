#include<iostream>

using namespace std;

bool exp1(string input){
     int i=0;
    int n=input.size();

    if(i<n && input[i]=='b'){
        i++;
    }else{
       
        return 0;
    }

    while(i<n && input[i]=='b'){
        i++;
    }
     
    while(i<n && input[i]=='a'){
        i++;
    }
    if(i==n){
    
        return 1;
    }
  
    return 0;

}

bool exp2(string input){
     int i=0;
    int n =input.size();

    while(i < n && input[i]=='a'){
        i++;
    }
    if(i==n || input[i]!='b'){
       
        return 0;
    }

    while(i<n && input[i]=='b'){
        i++;
    }

    while(i<n && input[i]=='a'){
        i++;
    }
    if(i==n){  
        return 1;
    }
   

    return 0;
}

int main(){

    cout<<"enter input"<<endl;
    string input;
    cin>>input;

    bool one=exp1(input);
    bool two=exp2(input);

    if(one){
        cout<<input<<" is valid for b+a* "<<endl;
    }else{
        cout<<input<<" is not valid for b+a* "<<endl;

    }

    if(two){
        cout<<input<<" is valid for a*b+a* "<<endl;
    }else{
        cout<<input<<" is not valid for a*b+a* "<<endl;

    }



    return 0;
}