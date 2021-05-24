#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>

static const char convertionTable[32] = {
    ' ', // 0
    '0', // 1 -> 0x01
    ' ', // 2
    '7', // 3 -> 0x03
    '-', // 4 -> 0x04
    '4', // 5 -> 0x05
    's', // 6 -> 0x06
    ' ', // 7
    ' ', // 8
    '2', // 9 -> 0x09
    ' ', // A
    ' ', // B
    '6', // C -> 0x0C
    ' ', // D
    ' ', // E
    ' ', // F
    '9', // 10 -> 0x10
    '1', // 11 -> 0x11
    '8', // 12 -> 0x12
    ' ', // 13
    '5', // 14 -> 0x14
    ' ', // 15
    ' ', // 16
    ' ', // 17
    ' ', // 18
    '3', // 19 -> 0x19
    ' ', // 1A
    ' ', // 1B
    ' ', // 1C
    ' ', // 1D
    ' ', // 1E
    ' ', // 1F
};
// 123-4552
// 00110 10001 01001 11001 00100 00101 10100 10100 01001 00110

int getWeight(char element)
{
    if (element == '-')
    {
        return 10;
    }
    if (isdigit(element))
    {
        return element - '0';
    }
    return 0;
}

int calcK(char *elements, int elementsCount)
{
    int sum = 0;
    for (int i = 1; i <= elementsCount + 1; i++)
    {
        sum += ((elementsCount - i + 1) % 9 + 1) * getWeight(elements[i - 1]);
    }
    return (sum % 11);
}

int calcC(char *elements, int elementsCount)
{
    int sum = 0;
    for (int i = 1; i <= elementsCount; i++)
    {
        sum += ((elementsCount - i) % 10 + 1) * getWeight(elements[i - 1]);
    }
    return (sum % 11);
}

void convertBarcode(uint8_t *inBytes, int inBytesCount, char *code)
{
    char *p = code;
    if ('s' != convertionTable[inBytes[0]])
    {
        *p = '\0';
        return;
    }
    for (int i = 1; i < inBytesCount - 1; i++)
    {
        *p++ = convertionTable[inBytes[i]];
    }
    *p = '\0';
}

int main(void)
{
    uint8_t inBytes[] = {0b00110, 0b10001, 0b01001, 0b11001, 0b00100, 0b00101, 0b10100, 0b10100, 0b01001, 0b00110};
    char code[200];
    convertBarcode(inBytes, sizeof(inBytes), code);
    int codeLength = strlen(code);
    int c = calcC(code, 6);
    int k = calcK(code, 6);
    printf("c=%d\n", c);
    printf("k=%d\n", k);
    printf("myCode = %s,%c,%c\n", code, code[codeLength - 2], code[codeLength - 1]);
    if (codeLength < 2)
    {
        printf("Oooops 1 \n");
    }
    if (c != code[codeLength - 2] - '0')
    {
        printf("Oooops c \n");
        return -1;
    }
    if (k != code[codeLength - 1] - '0')
    {
        printf("Oooops k \n");
        return -2;
    }
    printf("OK\n");
    return 0;
}