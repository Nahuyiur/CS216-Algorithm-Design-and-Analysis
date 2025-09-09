#include <iostream>
using namespace std;
 
int main()
{
	srand(time(0));
	int n,m,S;
	n=rand()%100+1;
	m=rand()%200+1;
	S=1;
	printf("%d %d %d\n",n,m,S);
	for (int i=1;i<=m;i++){
		int u,v,w;
		u=rand()%n+1;
		v=rand()%n+1;
		w=rand()%10000;
		printf("%d %d %d\n",u,v,w);
	}
	
	return 0;
}