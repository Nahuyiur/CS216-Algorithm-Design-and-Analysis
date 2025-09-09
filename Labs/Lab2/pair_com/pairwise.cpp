#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
using namespace std;  
int main()  
{  
    int t=10;
    while(t)  
    {  
      	t--;  
        system("./random_generator > data.txt");  
        system("./fangshen < data.txt > fangshen.txt");  
         system("./lab2a < data.txt > lab2a.txt");  
        if(system("diff lab2a.txt fangshen.txt"))   break;  
    }  
    if(t==0) cout<<"no error"<<endl;  
    else cout<<"error"<<endl;  
    
    cout<<10-t<<endl;
    return 0;  
}  