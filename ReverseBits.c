#include <stdio.h>
#include <time.h>

// Sol1
unsigned char reverseBitsV1(unsigned char data)
{
    int i;
    unsigned char a, b = 0x00;

    for (i = 0; i < 8; i++)
    {
        a = data & 0x01;
        a = a << (7 - i);
        b = b | a;
        data = data >> 1;
    }

    return b;
}

// Sol2
unsigned char reverseBitsV2(unsigned char data)
{
    data = ((data & 0xf0) >> 4) | ((data & 0x0f) << 4);
    data = ((data & 0xcc) >> 2) | ((data & 0x33) << 2);
    data = ((data & 0xaa) >> 1) | ((data & 0x55) << 1);

    return data;
}

int main()
{
    clock_t time1, time2;
    int iter = 100000000;
    int i;

    // unsigned char 1 bytes = 8 bits
    unsigned char data = 0xda; // 11011010
    // Reverse bits => 01011011 0x5b
    unsigned char x = reverseBitsV1(data);
    unsigned char y = reverseBitsV2(data);
    printf("0x%x 0x%x\n", data, x);
    printf("0x%x 0x%x\n", data, y);

    time1 = clock();
    for (i = 0; i < iter; i++)
    {
        reverseBitsV1(data);
    }

    time1 = clock() - time1;
    float t1 = ((float)time1 / CLOCKS_PER_SEC);

    time2 = clock();
    for (i = 0; i < iter; i++)
    {
        reverseBitsV2(data);
    }

    time2 = clock() - time2;
    float t2 = ((float)time2 / CLOCKS_PER_SEC);

    printf("Time for v1: %fs\n", t1);
    printf("Time for v2: %fs\n", t2);
    printf("Faster: %f%%", (t1 - t2) * 100 / t1);

    /*
        Time for v1: 1.841000s
        Time for v2: 0.299000s
        Faster: 83.758827%
    */

    return 0;
}