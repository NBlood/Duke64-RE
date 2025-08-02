#include "common.h"
#include "duke3d.h"

typedef struct
{
    unsigned long f_0;
    unsigned short f_4;
    unsigned long f_8;
    unsigned short f_c;
} Huffman;

char D_8026ECC0;
char D_8026ECC1;
char* D_8026ECC4;
char* D_8026ECC8;
char* D_8026ECCC;
unsigned int D_8026ECD0;
Huffman D_8026ECD8[16];
Huffman D_8026EDD8[16];
Huffman D_8026EED8[16];


unsigned short func_800700F0(char bits)
{
    unsigned short val;

    val = 0;
    while (bits--)
    {
        if (D_8026ECC1 == 0)
        {
            D_8026ECC0 = *D_8026ECC4++;
            D_8026ECC1 = 8;
        }
        val = val << 1;
        if (D_8026ECC0 & 0x80)
            val++;
    
        D_8026ECC0 *= 2;
        D_8026ECC1 = D_8026ECC1 - 1;
    }
    return val;
}

unsigned short func_800701A0(void)
{
    unsigned short val;

    val = 0;
    if (func_800700F0(1))
    {
        val = func_800700F0(1);
        if (func_800700F0(1))
        {
            val = (func_800700F0(1) + (val * 2)) | 4;
            if (func_800700F0(1) == 0)
                val = func_800700F0(1) + (val * 2);
        } else if (val == 0)
            val = func_800700F0(1) + 2;
    }
    return *D_8026ECC4++ + (val << 8) + 1;
}

unsigned short func_80070274(void)
{
    unsigned short val;

    val = func_800700F0(1) + 4;
    if (func_800700F0(1) == 0)
        return val;
    return ((val-1)<<1) + func_800700F0(1);
}

unsigned short func_800702D4(char arg0)
{
    unsigned short val;
    unsigned short bit;

    val = 0;
    bit = 1;
    while (arg0--)
    {
        if (D_8026ECC1 == 0)
        {
            D_8026ECD0 = (D_8026ECC4[3] << 0x18) | (D_8026ECC4[2] << 0x10) | (D_8026ECC4[1] << 8) | D_8026ECC4[0];
            D_8026ECC4 += 2;
            D_8026ECC1 = 0x10;
        }
        if (D_8026ECD0 & 1)
            val = val | bit;
    
        bit = bit << 1;
        D_8026ECD0 = D_8026ECD0 >> 1;
        D_8026ECC1 = D_8026ECC1 - 1;
    }
    return val;
}

unsigned short func_800703A8(Huffman* h2)
{
    char i;
    Huffman *h;

    i = 0;
    h = h2;

    while (h->f_c == 0 || (((1U << h->f_c) - 1)&D_8026ECD0) != h->f_8)
    {
        h++;
        i++;
    }
    func_800702D4(h->f_c);
    if (i < 2)
        return i;
    return func_800702D4(i-1) | (1 << (i-1));
}

int func_80070488(unsigned int src, char size)
{
    unsigned int val;

    val = 0;
    while (size--)
    {
        val <<= 1;
        if (src & 1)
            val = val | 1;
        src = src >> 1;
    }
    return val;
}

void func_800704DC(Huffman* huff, char size)
{
    Huffman* h;
    unsigned short i;
    unsigned short b;
    unsigned int val;
    unsigned int m;

    b = 1;
    val = 0;
    m = 0x80000000;
    while (b < 17)
    {
        h = huff;
        for (i = 0; i < size; i++)
        {
            if (h->f_c == b)
            {
                h->f_8 = func_80070488(val / m, b);
                val += m;
            }
            h++;
        }
        b++;
        m >>= 1;
    }
}

void func_800705C8(Huffman* h, char cnt)
{
    while(cnt--)
    {
        h->f_0 = 0;
        h->f_4 = 0xFFFF;
        h->f_8 = 0;
        h->f_c = 0;
        h++;
    }
}

void func_80070614(Huffman* huff, char cnt)
{
    char siz;
    char i;
    Huffman* h;

    func_800705C8(huff, cnt);
    siz = func_800702D4(5);
    if (siz == 0)
        return;
    if (siz > 16)
        siz = 16;
    h = huff;
    for (i = 0; i < siz; i++)
    {
        h->f_c = func_800702D4(4);
        h++;
    }
    func_800704DC(huff, siz);
}

void func_800706B8(char* in, char *out)
{
    D_8026ECC4 = in + 18;
    D_8026ECC8 = out;
    D_8026ECCC = out + ((in[4] << 0x18) | (in[5] << 0x10) | (in[6] << 8) | in[7]);
    D_8026ECC1 = 0;
}

unsigned short RNCDecompress2(char* arg0, char* arg1)
{
    char* src;
    unsigned short size;
    unsigned short offset;

    func_800706B8(arg0, arg1);
    func_800700F0(2);
    while (D_8026ECC8 < D_8026ECCC)
    {
l1:
        while (func_800700F0(1) == 0)
        {
            *D_8026ECC8++ = *D_8026ECC4++;
        }
        if (func_800700F0(1) != 0)
        {
            if (func_800700F0(1) == 0)
            {
                size = 2;
                offset = *D_8026ECC4++ + 1;
            }
            else
            {
                if (func_800700F0(1) == 0)
                {
                    size = 3;
                }
                else
                {
                    size = *D_8026ECC4++ + 8;
                    if (size == 8)
                    {
                        goto l2;
                    }
                }
                offset = func_800701A0();
            }
            src = D_8026ECC8 - offset;
            while (size--)
            {
                *D_8026ECC8++ = *src++;
            }
        }
        else
        {
            size = func_80070274();
            if (size == 9)
            {
                size = (func_800700F0(4) * 4) + 12;
                while (size--)
                {
                    *D_8026ECC8++ = *D_8026ECC4++;
                }
            }
            else
            {
                src = D_8026ECC8 - func_800701A0();
                while (size--)
                {
                    *D_8026ECC8++ = *src++;
                }
            }
        }
        goto l1;
l2:
        func_800700F0(1);
    }
    return 0;
}

unsigned short RNCDecompress1(char* arg0, char* arg1)
{
    unsigned short cnt;
    unsigned short size;
    char* ptr;

    func_800706B8(arg0, arg1);
    func_800702D4(2);
    while (D_8026ECC8 < D_8026ECCC)
    {
        func_80070614(D_8026ECD8, 16);
        func_80070614(D_8026EDD8, 16);
        func_80070614(D_8026EED8, 16);
        cnt = func_800702D4(16);
        goto l2;

        do
        {
            ptr = (D_8026ECC8 - func_800703A8(D_8026EDD8)) - 1;
            size = func_800703A8(D_8026EED8) + 2;
            while (size--)
            {
                *D_8026ECC8++ = *ptr++;
            }
l2:
            size = func_800703A8(D_8026ECD8);

            while (size--)
            {
                *D_8026ECC8++ = *D_8026ECC4++;
            }
            D_8026ECD0 = (((D_8026ECC4[2] << 0x10) + (D_8026ECC4[1] << 8) + D_8026ECC4[0]) << D_8026ECC1) + (D_8026ECD0 & ((1 << D_8026ECC1) - 1));
            cnt--;
        } while (cnt);
    }
    return 0;
}

short RNCDecompress(char* in, char* out)
{
    unsigned long len;
    unsigned short status;

    if (in[0] != 'R' || in[1] != 'N' || in[2] != 'C')
        return -1;
    len = (in[4] << 0x18) | (in[5] << 0x10) | (in[6] << 8) | in[7];
    switch (in[3])
    {
        case 0:
            fastmemcpy(out, in + 8, len);
            status = 0;
            break;
        case 1:
            status = RNCDecompress1(in, out);
            break;
        case 2:
            status = RNCDecompress2(in, out);
            break;
        default:
            status = -2;
    }
    if (status != 0)  while(1);
    return status;
}
