#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void headAX(char data[], int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        data[i] = data[i] << 1;
        printf("%X ", data[i]);
    }
}

void dataAX(char data[], int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        printf("%X ", data[i]);
    }
}

int main()
{
    // Data
    char source[7] = "N7LEM  ";
    char sourceSSID[2] = "0";
    char dest[7] = "NJ7P  ";
    char destSSID[2] = "0";
    int PID = 240;
    int control = 3;
    char payload[256] = "The quick brown fox jumps over the lazy dog";

    // Length of array
    int nSource = strlen(source);
    int nSourceSSID = strlen(sourceSSID);
    int nDest = strlen(dest);
    int nDestSSID = strlen(destSSID);
    int nPayload = strlen(payload);

    // AX.25 Frame
    headAX(dest, nDest);
    headAX(source, nSource);
    dataAX(payload, nPayload);

    return 0;
}