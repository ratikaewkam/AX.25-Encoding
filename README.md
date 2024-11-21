# AX.25 Frame and HDLC Encoding Documentation

This program demonstrates the process of generating an AX.25 frame, calculating the Frame Check Sequence (FCS), and performing bit stuffing to produce an HDLC frame. The program simulates the construction of a frame from given data and then encodes it with necessary protocols.

---

### Table of contents
1. [Overview](https://github.com/ratikaewkam/AX.25-Encoding#overview)
2. [Data structure](https://github.com/ratikaewkam/AX.25-Encoding#data-stucture)
3. [Functions](https://github.com/ratikaewkam/AX.25-Encoding#functions)
   - [reverseBits](https://github.com/ratikaewkam/AX.25-Encoding#reversebits)
   - [data2frame](https://github.com/ratikaewkam/AX.25-Encoding#data2frame)
   - [addcrc](https://github.com/ratikaewkam/AX.25-Encoding#addcrc)
   - [crc16cal](https://github.com/ratikaewkam/AX.25-Encoding#crc16cal)
   - [bitStuffing](https://github.com/ratikaewkam/AX.25-Encoding#bitstuffing)
   - [output](https://github.com/ratikaewkam/AX.25-Encoding#output)
5. [Main flow](https://github.com/ratikaewkam/AX.25-Encoding?tab=readme-ov-file#main-flow)
6. [Memory management](https://github.com/ratikaewkam/AX.25-Encoding?tab=readme-ov-file#memory-management)

### Overview

The program simulates the generation of an AX.25 frame, which is commonly used in amateur radio communication. The frame undergoes several transformations:

Frame Creation: Data is packed into an AX.25 frame structure.
FCS (Frame Check Sequence): A CRC16 check is added to the frame to detect transmission errors.
Bit Stuffing: The AX.25 frame is encoded using bit stuffing for HDLC compatibility, ensuring that consecutive bits 1 do not appear more than five times in a row.
The program demonstrates these processes by constructing a sample AX.25 frame, adding a CRC16 FCS, and performing bit stuffing to generate an HDLC frame.

### Data stucture

The data structure used to represent the raw data is:
```c
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
};

```

- src & dst: The source and destination addresses. These are ASCII strings and can hold up to 6 characters.
- srcSSID & dstSSID: The SSID fields, each can range from 0 to 15.
- pid: The protocol identifier that specifies the type of data being transmitted.
- control: A control byte used to signify the type of frame.
- payload: The data being transmitted.
- iscmd: Flag to indicate whether the frame is a command (1) or a response (0).

---

### Functions

#### reverseBits

```c
unsigned char reverseBits(unsigned char byte);
```

This function reverses the bits of a given byte. The input byte is flipped, which is necessary for the AX.25 frame construction.

- **Input**: A single byte of data.
- **Output**: The byte with its bits reversed.

#### data2frame

```c
unsigned char *data2frame(struct data dt, unsigned int *size);
```

This function constructs an AX.25 frame from the provided data. The frame is packed with addresses, SSID, PID, control bytes, and the payload.

- **Input**: A struct data containing all necessary information.
- **Output**: The AX.25 frame and its size.

#### addcrc

```c
unsigned char *addcrc(unsigned char *frame, unsigned int frame_size, unsigned int *new_frame_size);
```

This function calculates and appends the CRC16 checksum to the provided AX.25 frame, resulting in a frame with the FCS (Frame Check Sequence) included.

- **Input**: The original frame and its size.
- **Output**: The frame with the appended CRC16 checksum and its new size.

#### crc16cal

```c
short int crc16cal(unsigned char *frame, unsigned int size);
```
This function computes the CRC16 checksum for a given data frame.

- **Input**: The AX.25 frame and its size.
- **Output**: A 16-bit CRC checksum.

#### bitStuffing

```c
void bitStuffing(unsigned char *frame, int frameLength, unsigned char **encodedFrame, int *encodedLength);
```
Bit stuffing is performed to prevent sequences of 5 consecutive 1s in the data, which are problematic for HDLC and AX.25 frames. After processing, the function encodes the frame and returns the encoded data.

- **Input**: The data frame and its length.
- **Output**: The bit-stuffed frame and its length.

#### output

```c
void output(unsigned char *frame, unsigned int size);
```

This function prints the content of the frame in hexadecimal format, useful for debugging and visualization.

- **Input**: The frame and its size.
- **Output**: Prints the frame in hexadecimal format to the console.

---

### Main flow

1. Create Data Frame:
        - A **struct** data instance is populated with sample data, such as source and destination addresses, payload, and command type.
2. Convert to AX.25 Frame:
        - The **data2frame** function is called to pack the data into an AX.25 frame. This includes converting the addresses and SSID fields, and applying bit reversal.
3. Add CRC16 Checksum:
        - The **addcrc** function is used to calculate and append the CRC16 checksum (FCS) to the frame.
4. Perform Bit Stuffing:
        - The **bitStuffing** function is called to ensure that no more than five consecutive 1 bits appear in the frame, which is a requirement for HDLC compatibility.
5. Output:
        - Finally, the program outputs the AX.25 frame (with FCS) and the HDLC frame to the console in hexadecimal format using the **output** function.

---

### Memory management

Memory for the frames is dynamically allocated using calloc to ensure there is enough space to hold the frame and its encoded versions. After use, memory is freed to prevent memory leaks. Specifically:

- **calloc** is used for frame allocations, initializing the memory to zero to avoid garbage values.
- Memory for frames is allocated in **data2frame**, **addcrc**, and **bitStuffing**. Each function that allocates memory ensures that the frame's size is adjusted appropriately for the next step (e.g., adding the CRC or performing bit stuffing).
- Freeing memory: After all operations are complete, **free()** is called to release the allocated memory.
