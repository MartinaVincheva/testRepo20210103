#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
int read_signals(float *signal);
int calcK(char *elements, int elementsCount);
int calcC(char *elements, int elementsCount);
int convert_bitStream(int *bitstream, int sizeBitstream, uint8_t *tokens);
int bitsToBytes(uint8_t *tokens, int tokensCount, uint8_t *outBytes);
void convertBarcode(uint8_t *inBytes, int inBytesCount, char *code);
void validateAndPrintResult(char *code);
int main(void)
{
    int size;
    float signal;
    int i = 0;
    scanf("%d", &size);
    int scaned[size];
    while (i < size)
    {
        scanf("%f", &signal);
        scaned[i] = read_signals(&signal);
        i++;
    }
    uint8_t tokens[(size + 1) / 2];
    int tokensCount = convert_bitStream(scaned, size, tokens);
    uint8_t outBytes[(sizeof(tokens) + 4) / 5];
    int bytesCount = bitsToBytes(tokens, tokensCount, outBytes);
    char code[sizeof(outBytes)];
    convertBarcode(outBytes, bytesCount, code);
    validateAndPrintResult(code);
    return 0;
}

void validateAndPrintResult(char *code)
{
    int codeLength = strlen(code);
    int c = calcC(code, codeLength - 2);
    int k = calcK(code, codeLength - 2);
    printf("c=%d\n", c);
    printf("k=%d\n", k);
    printf("myCode = %s,%c,%c\n", code, code[codeLength - 2], code[codeLength - 1]);
    if (codeLength < 2)
    {
        printf("Oooops 1 \n");
        exit(-1);
    }
    if (c != code[codeLength - 2] - '0')
    {
        printf("Oooops c \n");
        exit(-1);
    }
    if (k != code[codeLength - 1] - '0')
    {
        printf("Oooops k \n");
        exit(-2);
    }
    printf("OK\n");
}

int read_signals(float *signal)
{
    return ((int)round(*signal));
}

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

static int getWeight(char element)
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

static int countX(int *bitstream, int sizeBitstream, int x)
{
    int count = 0;

    while ((sizeBitstream - count > 0) && (*bitstream++ == x))
    {
        count++;
    }
    return count;
}

int convert_bitStream(int *bitstream, int sizeBitstream, uint8_t *tokens)
{
    int count = 0;
    uint8_t *token = tokens;

    for (; sizeBitstream > 0;)
    {
        // skip ones
        count = countX(bitstream, sizeBitstream, 1);
        bitstream += count;
        sizeBitstream -= count;
        //  count zeroes
        count = countX(bitstream, sizeBitstream, 0);
        bitstream += count;
        sizeBitstream -= count;

        if (count == 1)
        {
            *token++ = 0;
        }
        else if (count > 1)
        {
            *token++ = 1;
        }
    }
    return (token - tokens);
}

static uint8_t collectForwardSymbol(uint8_t *data)
{
    return (data[0] << 4 | data[1] << 3 | data[2] << 2 | data[3] << 1 | data[4]);
}

static uint8_t collectReverseSymbol(uint8_t *data)
{
    return (data[4] << 4 | data[3] << 3 | data[2] << 2 | data[1] << 1 | data[0]);
}

#define BC_START_STOP (0x06)
#define SYM_WIDTH_IN_BITS (5)
int bitsToBytes(uint8_t *tokens, int tokensCount, uint8_t *outBytes)
{
    uint8_t *p = outBytes;
    int index = 0;
    uint8_t symbol;

    // Adjust tokens count
    // ???? tokensCount += tokensCount % 5;
    if (tokensCount < SYM_WIDTH_IN_BITS)
    {
        return (0);
    }
    symbol = collectForwardSymbol(&tokens[index]);
    if (symbol != BC_START_STOP)
    {
        index = tokensCount - SYM_WIDTH_IN_BITS;
        // Maybe reversed reader
        symbol = collectReverseSymbol(&tokens[index]);
        if (symbol != BC_START_STOP)
        {
            // No valid start/stop found
            return (0);
        }
        do
        {
            *p++ = symbol;
            index -= SYM_WIDTH_IN_BITS;
            tokensCount -= SYM_WIDTH_IN_BITS;
            symbol = collectReverseSymbol(&tokens[index]);
        } while (tokensCount >= SYM_WIDTH_IN_BITS);
    }
    else
    {
        // Found start/stop char, proceed with next bytes
        do
        {
            *p++ = symbol;
            index += SYM_WIDTH_IN_BITS;
            tokensCount -= SYM_WIDTH_IN_BITS;
            symbol = collectForwardSymbol(&tokens[index]);
        } while (tokensCount >= SYM_WIDTH_IN_BITS);
    }
    return (p - outBytes);
}
