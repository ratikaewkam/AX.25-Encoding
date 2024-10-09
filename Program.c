#include <stdio.h>
#include <string.h>

int main()
{
    char source[7] = "123456";
    int n = strlen(source);
    int i;
    printf("%d\n", n);

    for (i = 0; i < n; i++)
    {
        printf("0x%X ", source[i]);
    }

    return 0;
}