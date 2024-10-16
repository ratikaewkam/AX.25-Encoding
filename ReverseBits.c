#include <stdio.h>

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
    // short int 2 bytes = 16 bits
    unsigned char data = 0xda; // 11011010
    // Reverse bits => 01011011 0x5b
    unsigned char x = reverseBitsV1(data);
    unsigned char y = reverseBitsV2(data);
    printf("0x%x 0x%x\n", data, x);
    printf("0x%x 0x%x\n", data, y);

    return 0;
}