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

short int crc16cal(unsigned char *frame, unsigned int size)
{
    short int crc = 0xffff;
    int i;

    short int crcTable[] = {0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7, 0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
                            0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6, 0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
                            0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485, 0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
                            0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4, 0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
                            0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823, 0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
                            0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12, 0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
                            0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41, 0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
                            0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70, 0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
                            0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F, 0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
                            0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E, 0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
                            0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D, 0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
                            0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C, 0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
                            0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB, 0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
                            0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A, 0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
                            0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9, 0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
                            0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8, 0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0};

    for (i = 0; i < size; i++)
    {
        unsigned char temp = (frame[i] ^ (crc >> 8) & 0xff);
        crc = crcTable[temp] ^ (crc << 8);
    }

    crc = ((crc ^ 0xFFFF) & 0x0FFFF);

    return (crc & 0xFFFF);
}

void bitStuffing(unsigned char *frame, int frameLength, unsigned char **encodedFrame, int *encodedLength)
{
    int cnt = 0;            // Counter for consecutive "1"s
    int bitIndex = 128;     // Bit index (128 = 2^7)
    unsigned char byte = 0; // Byte to store the processed bits
    *encodedLength = 0;     // Initialize encoded length

    // Dynamically allocate enough memory for the worst-case scenario
    *encodedFrame = (unsigned char *)calloc(frameLength * 2 + 10, sizeof(unsigned char)); // Adding space for flags and extra bits

    // Add Start flag
    (*encodedFrame)[(*encodedLength)++] = 0x7E;

    // Frame Bit processing (Bit Stuffing)
    for (int i = 0; i < frameLength; i++)
    {
        for (int mask = 128; mask > 0; mask >>= 1)
        { // Iterate through bits
            if (frame[i] & mask)
            { // Current bit is "1"
                byte += bitIndex;
                bitIndex >>= 1; // Shift for next bit position
                if (bitIndex == 0)
                {
                    (*encodedFrame)[(*encodedLength)++] = byte; // Save the byte
                    byte = 0;
                    bitIndex = 128;
                }

                cnt += 1;
                if (cnt == 5)
                {                   // Five consecutive "1"s
                    bitIndex >>= 1; // Add a "0"
                    if (bitIndex == 0)
                    {
                        (*encodedFrame)[(*encodedLength)++] = byte; // Save the byte
                        byte = 0;
                        bitIndex = 128;
                    }
                    cnt = 0; // Reset "1" counter
                }
            }
            else
            {                   // Current bit is "0"
                bitIndex >>= 1; // Shift for next bit position
                if (bitIndex == 0)
                {
                    (*encodedFrame)[(*encodedLength)++] = byte; // Save the byte
                    byte = 0;
                    bitIndex = 128;
                }
                cnt = 0; // Reset "1" counter
            }
        }
    }

    // Add End flag (0x7E)
    bitIndex >>= 1; // Just shifting adds a "0"
    if (bitIndex == 0)
    {
        (*encodedFrame)[(*encodedLength)++] = byte; // Save the byte
        byte = 0;
        bitIndex = 128;
    }

    // Add six "1"s
    for (int i = 0; i < 6; i++)
    {
        byte += bitIndex; // Add "1"
        bitIndex >>= 1;
        if (bitIndex == 0)
        {
            (*encodedFrame)[(*encodedLength)++] = byte; // Save the byte
            byte = 0;
            bitIndex = 128;
        }
    }

    // Add "0"
    bitIndex >>= 1; // Just shifting adds a "0"
    // Add current processing byte even if not complete/full
    (*encodedFrame)[(*encodedLength)++] = byte;

    // Add End flag again
    //(*encodedFrame)[(*encodedLength)++] = 0x7E;
}

unsigned char *data2frame(struct data dt, unsigned int *size)
{
    unsigned int mem = 16 + strlen(dt.payload);
    unsigned char *frame = (unsigned char *)calloc(mem, sizeof(unsigned char));
    int i, j;

    j = 0;
    for (i = 0; i < 6; i++)
    {
        if (dt.dst[j] == 0)
        {
            dt.dst[j] = 0x20; // 0x20 = space
        }

        frame[i] = reverseBits((dt.dst[j] & 0xff) << 1);
        j++;
    }

    // Range of SSID (0-15)
    frame[6] = ((dt.dstSSID & 0x0f) << 1) + 0x60;
    if (dt.iscmd == 1)
    {
        frame[6] += 0x80;
    }

    frame[6] = reverseBits(frame[6]);

    j = 0;
    for (i = 7; i < 13; i++)
    {
        if (dt.src[j] == 0)
        {
            dt.src[j] = 0x20; // 0x20 = space
        }

        frame[i] = reverseBits((dt.src[j] & 0xff) << 1);
        j++;
    }

    frame[13] = ((dt.srcSSID & 0x0f) << 1) + 0x60;
    if (dt.iscmd == 0)
    {
        frame[13] += 0x80;
    }

    frame[13] += 0x01;
    frame[13] = reverseBits(frame[13]);

    frame[14] = reverseBits(dt.control & 0xff);
    frame[15] = reverseBits(dt.pid & 0xff);

    j = 0;
    for (i = 16; i < mem; i++)
    {
        frame[i] = reverseBits(dt.payload[j] & 0xff);
        j++;
    }

    *size = mem;

    return frame;
}

unsigned char *addcrc(unsigned char *frame, unsigned int frame_size, unsigned int *new_frame_size)
{
    unsigned int new_mem = frame_size + 2;
    unsigned char *new_frame = (unsigned char *)calloc(new_mem, sizeof(unsigned char));

    for (unsigned int i = 0; i < frame_size; i++)
    {
        new_frame[i] = frame[i];
    }

    unsigned short int crc = crc16cal(frame, frame_size);
    new_frame[new_mem - 2] = ((crc >> 8) & 0xff);
    new_frame[new_mem - 1] = (crc & 0xff);

    *new_frame_size = new_mem;

    return new_frame;
}

void output(unsigned char *frame, unsigned int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        printf("0x%X ", frame[i]);
    }
}

int main()
{
    struct data dt = {"N7LEM", 0, "NJ7P", 0, 0xF0, 0x03, "The quick brown fox jumps over the lazy dog", 1}; // Data frame
    unsigned int frame_size;
    unsigned char *frame = data2frame(dt, &frame_size);

    printf("\nAX.25 Frame:\n");
    output(frame, frame_size);

    unsigned int new_frame_size;
    unsigned char *new_frame = addcrc(frame, frame_size, &new_frame_size);

    printf("\nAX.25 Frame (FCS):\n");
    output(new_frame, new_frame_size);

    unsigned char *encodeFrame;
    unsigned int encodeFrame_size;
    bitStuffing(new_frame, new_frame_size, &encodeFrame, &encodeFrame_size);

    printf("\nHDLC Frame:\n");
    output(encodeFrame, encodeFrame_size);

    free(frame);
    free(new_frame);
    free(encodeFrame);

    return 0;
}