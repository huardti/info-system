#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    for(int i = 0 ; ; ++i)
    {
        if(i % 100000 == 0) printf("%d\n", i);
    }
    return EXIT_SUCCESS;
}