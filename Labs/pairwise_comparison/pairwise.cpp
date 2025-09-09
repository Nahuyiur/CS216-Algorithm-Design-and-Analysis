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
        system("./Floyd < data.txt > Floyd.txt");  
         system("./Dijkstra < data.txt > Dijkstra.txt");  
        if(system("diff Dijkstra.txt Floyd.txt"))   break;  
    }  
    if(t==0) cout<<"no error"<<endl;  
    else cout<<"error"<<endl;  
 
    return 0;  
}  