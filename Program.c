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

void srcSSID_AX(char data[], int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        data[i] = data[i] << 1;
        data[i]  = data[i] | 1;
        printf("%X ", data[i]);
    }
}

void destSSID_AX(char data[], int n)
{
    int i;
    
    for (i = 0; i < n; i++)
    {
        data[i] = data[i] << 1;
        data[i]  = data[i] | 128;
        printf("%X ", data[i]);
    }
}

void pinAX(int pin)
{
    printf("%X ", pin);
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
    char source[7] = "N7LEM ";
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

    // AX.25 Frame -> Type Command

    headAX(dest, nDest);
    destSSID_AX(destSSID, nDestSSID);
    headAX(source, nSource);
    srcSSID_AX(sourceSSID, nSourceSSID);
    pinAX(control);
    pinAX(PID);
    dataAX(payload, nPayload);

    return 0;
}