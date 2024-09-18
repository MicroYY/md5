
#include "md5.h"
#include <iostream>

static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ(expect, actual) \
    test_count++; \
    if ( expect == actual ) { \
        test_pass++; \
    } \
    else { \
        std::cout << "expect: " << expect << " actual: " << actual << "\n"; \
    }

static void test()
{
    EXPECT_EQ("d41d8cd98f00b204e9800998ecf8427e", GetMd5SumFromStr(""));
    EXPECT_EQ("900150983cd24fb0d6963f7d28e17f72", GetMd5SumFromStr("abc"));
    EXPECT_EQ("689de1e396ad9c089ae2b9aaffd6faf7", GetMd5SumFromStr("1234567890123456789012345678901234567890123456789012345678901234567890"));

    std::cout << test_pass << "/" << test_count << " (" << test_pass / test_count << ") passed\n";
}

int main()
{
    test();

    return 0;
}