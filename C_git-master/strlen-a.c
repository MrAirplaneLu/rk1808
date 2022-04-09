#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char a[]="xyz",b[]={'x','y','z'};
	char c[]="xyz";
	int  d[]={'x','y','z'};
	char e[10];
	printf("%d,%d\n",(int)strlen(a),(int)strlen(b));
	printf("%d,%d\n",(int)sizeof(a),(int)sizeof(b));
	printf("%d,%d\n",(int)sizeof(c),(int)sizeof(d));
	printf("%d,%d\n",(int)strlen(e),(int)sizeof(e));
	

	memset(e,0,sizeof(e));
	printf("%d,%d\n",(int)strlen(c),(int)strlen(d));
	printf("%d,%d\n",(int)strlen(e),(int)sizeof(e));
	
	e[0]='a';
	printf("%d,%d\n",(int)strlen(e),(int)sizeof(e));
	return 0;
}
