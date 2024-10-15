#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Raw data
struct data
{
    char src[7];
    unsigned short int srcSSID;
    char dst[7];
    unsigned short int dstSSID;
    unsigned short int pid;
    unsigned short int control;
    char payload[256];
    unsigned short int iscmd;
    /*
        1: Command
        0: Response
    */
};

// Encode data
struct frame
{
    // To prevent negative values and save memory, use unsigned short int
    unsigned short int src[7];
    unsigned short int srcSSID;
    unsigned short int dst[7];
    unsigned short int dstSSID;
    unsigned short int pid;
    unsigned short int control;
    unsigned short int payload[256];
};

struct frame encode2ax(struct data dt)
{
    struct frame fm;
    int i;

    for (i = 0; i < 6; i++)
    {
        if (dt.src[i] == 0)
        {
            dt.src[i] = 0x20; // 0x20 = space
        }

        if (dt.dst[i] == 0)
        {
            dt.dst[i] = 0x20;
        }

        fm.src[i] = (dt.src[i] & 0xff) << 1;
        fm.dst[i] = (dt.dst[i] & 0xff) << 1;
    }

    for (i = 0; i < 255; i++)
    {
        fm.payload[i] = dt.payload[i] & 0xff;
    }

    // Range of SSID (0-15)
    fm.dstSSID = ((dt.dstSSID & 0x0f) << 1) + 0x60;
    if (dt.iscmd == 1)
    {
        fm.dstSSID += 0x80;
    }

    fm.srcSSID = ((dt.srcSSID & 0x0f) << 1) + 0x60;
    if (dt.iscmd == 0)
    {
        fm.srcSSID += 0x80;
    }

    fm.srcSSID += 0x01;

    fm.pid = dt.pid & 0xff;
    fm.control = dt.control & 0xff;

    return fm;
}

void outputFrame(struct frame fm)
{
    int i;

    for (i = 0; i < 6; i++)
    {
        printf("0x%x ", fm.dst[i]);
    }

    printf("0x%x ", fm.dstSSID);

    for (i = 0; i < 6; i++)
    {
        printf("0x%x ", fm.src[i]);
    }

    printf("0x%x ", fm.srcSSID);
    printf("0x%x ", fm.control);
    printf("0x%x ", fm.pid);

    for (i = 0; i < 255; i++)
    {
        if (fm.payload[i] == 0)
        {
            printf("");
        }
        else
        {
            printf("0x%x ", fm.payload[i]);
        }
    }
}

int main()
{
    struct data dt = {"N7LEM", 0, "NJ7P", 0, 0xf0, 0x03, "The quick brown fox jumps over the lazy dog", 1}; // Data frame
    struct frame fm = encode2ax(dt); // Encode data frame to AX.25 frame
    outputFrame(fm); // Print data in AX.25 frame

    return 0;
}