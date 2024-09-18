
#include "md5.h"

#include <bitset>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

constexpr uint32_t INIT_A = 0x67452301;
constexpr uint32_t INIT_B = 0xefcdab89;
constexpr uint32_t INIT_C = 0x98badcfe;
constexpr uint32_t INIT_D = 0x10325476;

constexpr size_t BITS_PER_BTYE = 8;
constexpr size_t BITS_PER_BLOCK = 512;
constexpr size_t BITS_PER_SUBBLOCK = 32;
constexpr size_t NUM_SUBBLOCKS_PER_BLOCK = BITS_PER_BLOCK / BITS_PER_SUBBLOCK;

constexpr uint32_t K[] = {
        0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,
        0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,0x698098d8,
        0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,
        0xa679438e,0x49b40821,0xf61e2562,0xc040b340,0x265e5a51,
        0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
        0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,
        0xfcefa3f8,0x676f02d9,0x8d2a4c8a,0xfffa3942,0x8771f681,
        0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,0xf6bb4b60,
        0xbebfbc70,0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,
        0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,0xf4292244,
        0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,
        0xffeff47d,0x85845dd1,0x6fa87e4f,0xfe2ce6e0,0xa3014314,
        0x4e0811a1,0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391 };

const uint32_t s[] = { 7,12,17,22,7,12,17,22,7,12,17,22,7,
        12,17,22,5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
        4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,6,10,
        15,21,6,10,15,21,6,10,15,21,6,10,15,21 };

typedef uint32_t subblock;
typedef subblock block[16];

std::string Hex2Str(uint32_t num)
{
    std::string retStr;
    uint32_t reorderNum = 0;
    uint32_t mask = 0xff000000;
    uint32_t leftShift = 0;
    for (size_t i = 0; i < sizeof(num); i++)
    {
        reorderNum |= ((num & mask)) << leftShift;
        if (i == sizeof(num) - 1) break;
        mask >>= 8;
        reorderNum >>= 8;
        leftShift += 8;
    }
    std::stringstream ss;
    ss << std::hex << std::nouppercase << std::setfill('0') << std::setw(2) << reorderNum;
    retStr.append(ss.str());
    return retStr;
}

std::string GetMd5SumFromStr(const std::string& str)
{
    size_t szInBytes = str.length();
    size_t numBits = szInBytes * BITS_PER_BTYE;

    size_t numBlocks;

    if (numBits == 0)
    {
        numBlocks = 1;
    }
    else if (numBits % BITS_PER_BLOCK == 0)
    {
        numBlocks = numBits / BITS_PER_BLOCK;
    }
    else
    {
        numBlocks = numBits / BITS_PER_BLOCK + 1;
        numBlocks += (numBits % BITS_PER_BLOCK) < 448 ? 0 : 1;
    }

    block* paddedBlocks = new block[numBlocks];
    memset(paddedBlocks, 0, numBlocks * sizeof(block));
    memcpy(paddedBlocks, str.data(), szInBytes);

    if (numBits == 0 || numBits % BITS_PER_BLOCK)
    {
        // fill 1
        block& b = paddedBlocks[numBits / BITS_PER_BLOCK];
        subblock& sb = b[(numBits - numBits / BITS_PER_BLOCK * BITS_PER_BLOCK) / BITS_PER_SUBBLOCK];
        uint32_t mask = 0x80 << (numBits % BITS_PER_BLOCK);
        sb |= mask;

        // fill length
        block& lastBlock = paddedBlocks[numBlocks - 1];
        uint64_t* ptr = (uint64_t*)&lastBlock;
        const int lastSubBlkIdx = 7;
        ptr[lastSubBlkIdx] = numBits;
    }

    uint32_t a0 = INIT_A, b0 = INIT_B, c0 = INIT_C, d0 = INIT_D;
    for (size_t i = 0; i < numBlocks; i++)
    {
        block& b = paddedBlocks[i];

        uint32_t A = a0;
        uint32_t B = b0;
        uint32_t C = c0;
        uint32_t D = d0;

        for (size_t j = 0; j < 64; j++)
        {
            uint32_t F, g;
            if (j >= 0 && j <= 15)
            {
                F = (B & C) | ((~B) & D);
                g = j;
            }
            else if (j >= 16 && j <= 31)
            {
                F = (D & B) | ((~D) & C);
                g = (5 * j + 1) % 16;
            }
            else if (j >= 32 && j <= 47)
            {
                F = B ^ C ^ D;
                g = (3 * j + 5) % 16;
            }
            else if (j >= 48 && j <= 63)
            {
                F = C ^ (B | ~D);
                g = (7 * j) % 16;
            }
            F = F + A + K[j] + b[g];
            A = D;
            D = C;
            C = B;
            B = B + (((F) << (s[j])) | ((F) >> (32 - (s[j]))));
        }
        a0 += A;
        b0 += B;
        c0 += C;
        d0 += D;
    }
    delete[] paddedBlocks;
    return Hex2Str(a0).append(Hex2Str(b0)).append(Hex2Str(c0)).append(Hex2Str(d0));
}

std::string GetMd5SumFromFile(const std::string& filePath)
{
    std::ifstream file;
    file.open(filePath, std::ios::in | std::ios::binary);
    std::istreambuf_iterator<char> begin(file), eof;
    std::string str(begin, eof);
    return GetMd5SumFromStr(str);
}