
#include "md5.h"

#include <bitset>

template<class T>
T Align2Value(T value, size_t alignment)
{
    return ((value + (alignment - 1)) & ~(alignment - 1));
}

constexpr auto INIT_A = 0x67452301;
constexpr auto INIT_B = 0xefcdab89;
constexpr auto INIT_C = 0x98badcfe;
constexpr auto INIT_D = 0x10325476;

constexpr size_t BITS_PER_BTYE  = 8;
constexpr size_t BITS_PER_BLOCK = 512;
constexpr size_t BITS_PER_SUBBLOCK = 32;
constexpr size_t NUM_SUBBLOCKS_PER_BLOCK = BITS_PER_BLOCK / BITS_PER_SUBBLOCK;

typedef uint32_t subblock;
typedef subblock block[16];

std::string GetMd5SumFromStr(const std::string& str)
{
    size_t szInBytes = str.length();
    size_t numBits   = szInBytes * BITS_PER_BTYE;

    size_t numBlocks;
    
    if (numBits % BITS_PER_BLOCK == 0)
    {
        numBlocks = numBits / BITS_PER_BLOCK;
    }
    else
    {
        numBlocks = numBits / BITS_PER_BLOCK + 1;
        numBlocks += (numBits % BITS_PER_BLOCK) < 448 ? 0 : 1;
    }

    //size_t blkSzInByte = BITS_PER_BLOCK / BITS_PER_BTYE * numBlocks;

    block* paddedBlocks = new block[numBlocks];
    memset(paddedBlocks, 0, numBlocks * sizeof(block));
    memcpy(paddedBlocks, str.data(), szInBytes);
    
    if (numBits % BITS_PER_BLOCK)
    {
        // fill 1
        block& b = paddedBlocks[numBits / BITS_PER_BLOCK];
        subblock& sb = b[(numBits - numBits / BITS_PER_BLOCK * BITS_PER_BLOCK) / BITS_PER_SUBBLOCK];
        uint32_t mask = 0x00000001 << (numBits % BITS_PER_BLOCK);
        sb |= mask;

        // fill length
        block& lastBlock = paddedBlocks[numBlocks - 1];
        lastBlock[BITS_PER_BLOCK / BITS_PER_SUBBLOCK - 4] |= numBits & 0xffff000000000000;
        lastBlock[BITS_PER_BLOCK / BITS_PER_SUBBLOCK - 3] |= numBits & 0x0000ffff00000000;
        lastBlock[BITS_PER_BLOCK / BITS_PER_SUBBLOCK - 2] |= numBits & 0x00000000ffff0000;
        lastBlock[BITS_PER_BLOCK / BITS_PER_SUBBLOCK - 1] |= numBits & 0x000000000000ffff;
    }

    for (size_t i = 0; i < numBlocks; i++)
    {
        block& b = paddedBlocks[i];

        for (size_t j = 0; j < 16; j++)
        {

        }
    }


    return std::string();
}

std::string GetMd5SumFromFile(const std::string& filePath)
{
    return std::string();
}