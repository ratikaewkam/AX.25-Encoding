#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Raw data
struct data
{
    char src[7];
    unsigned char srcSSID;
    char dst[7];
    unsigned char dstSSID;
    unsigned char pid;
    unsigned char control;
    char payload[256];
    unsigned char iscmd;
    /*
        1: Command
        0: Response
    */
};

unsigned char reverseBits(unsigned char byte)
{
    byte = ((byte & 0xf0) >> 4) | ((byte & 0x0f) << 4);
    byte = ((byte & 0xcc) >> 2) | ((byte & 0x33) << 2);
    byte = ((byte & 0xaa) >> 1) | ((byte & 0x55) << 1);

    return byte;
}

unsigned char *data2frame(struct data dt, unsigned int *size)
{
    unsigned int mem = 17 + strlen(dt.payload);
    unsigned char *frame = (unsigned char *)malloc(mem * sizeof(unsigned char));
    int i, j;

    frame[0] = 0x7e;

    j = 0;
    for (i = 1; i < 7; i++)
    {
        if (dt.dst[j] == 0)
        {
            dt.dst[j] = 0x20; // 0x20 = space
        }

        frame[i] = reverseBits((dt.dst[j] & 0xff) << 1);

        j++;
    }

    // Range of SSID (0-15)
    frame[7] = ((dt.dstSSID & 0x0f) << 1) + 0x60;
    if (dt.iscmd == 1)
    {
        frame[7] += 0x80;
    }

    frame[7] = reverseBits(frame[7]);

    j = 0;
    for (i = 8; i < 14; i++)
    {
        if (dt.src[j] == 0)
        {
            dt.src[j] = 0x20; // 0x20 = space
        }

        frame[i] = reverseBits((dt.src[j] & 0xff) << 1);

        j++;
    }

    frame[14] = ((dt.srcSSID & 0x0f) << 1) + 0x60;
    if (dt.iscmd == 0)
    {
        frame[14] += 0x80;
    }

    frame[14] += 0x01;
    frame[14] = reverseBits(frame[14]);

    frame[15] = reverseBits(dt.control & 0xff);
    frame[16] = reverseBits(dt.pid & 0xff);

    j = 0;
    for (i = 17; i < mem; i++)
    {
        frame[i] = reverseBits(dt.payload[j] & 0xff);
        j++;
    }

    *size = mem;

    return frame;
}

void output(unsigned char *frame, unsigned int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        printf("0x%x ", frame[i]);
    }
}

int main()
{
    struct data dt = {"N7LEM", 0, "NJ7P", 0, 0xf0, 0x03, "The quick brown fox jumps over the lazy dog", 1}; // Data frame
    unsigned int size;
    unsigned char *frame = data2frame(dt, &size);
    output(frame, size);

    free(frame);

    return 0;
}