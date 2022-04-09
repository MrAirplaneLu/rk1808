#include <stdio.h>
#include <string.h>
int main(int argc ,char **argv  )
{ 
    char a[10] = "abc" ,b[10] = "012",c[10] = "xyz";
    
    strcpy( a+1,b+2 );
    printf("%s\n",a);

    strcat( a,c+1 );

    printf("%s\n",a);
    printf("%s\n",b);
    printf("%s\n",c);

    strcat( a,c+1 );
    printf("%s\n",a);

    return 0;
}

