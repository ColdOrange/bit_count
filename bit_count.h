#ifndef BIT_COUNT_H
#define BIT_COUNT_H

#include <cstdint>

//
// method 1: simple iterating
//
template <typename T>
unsigned bit_count_1(T x)
{
    unsigned count = 0;
    while (x)
    {
        count += x & 1u;
        x >>= 1;
    }

    return count;
}

//
// method 2: sparse iterating (iteration count = bit 1 count)
//
template <typename T>
unsigned bit_count_2(T x)
{
    unsigned count = 0;
    while (x)
    {
        ++count;
        x &= x - 1;
    }

    return count;
}

//
// method 3: lookup table
//
template <typename T>
unsigned bit_count_3(T x)
{

#define B2(n) n, n + 1, n + 1, n + 2
#define B4(n) B2(n), B2(n + 1), B2(n + 1), B2(n + 2)
#define B6(n) B4(n), B4(n + 1), B4(n + 1), B4(n + 2)
#define B8(n) B6(n), B6(n + 1), B6(n + 1), B6(n + 2)

    static unsigned lookup_table[256] = { B8(0) };

    unsigned count = 0;
    while (x)
    {
        count += lookup_table[x & 0xff];
        x >>= 8;
    }

#undef B2
#undef B4
#undef B6
#undef B8

    return count;
}

//
// method 4: parallel computing
//
unsigned bit_count_4(uint32_t x)
{
    x = (x & 0x55555555) + ((x >>  1) & 0x55555555);
    x = (x & 0x33333333) + ((x >>  2) & 0x33333333);
    x = (x & 0x0f0f0f0f) + ((x >>  4) & 0x0f0f0f0f);
    x = (x & 0x00ff00ff) + ((x >>  8) & 0x00ff00ff);
    x = (x & 0x0000ffff) + ((x >> 16) & 0x0000ffff);

    return x;
}

unsigned bit_count_4(uint64_t x)
{
    x = (x & 0x5555555555555555) + ((x >>  1) & 0x5555555555555555);
    x = (x & 0x3333333333333333) + ((x >>  2) & 0x3333333333333333);
    x = (x & 0x0f0f0f0f0f0f0f0f) + ((x >>  4) & 0x0f0f0f0f0f0f0f0f);
    x = (x & 0x00ff00ff00ff00ff) + ((x >>  8) & 0x00ff00ff00ff00ff);
    x = (x & 0x0000ffff0000ffff) + ((x >> 16) & 0x0000ffff0000ffff);
    x = (x & 0x00000000ffffffff) + ((x >> 32) & 0x00000000ffffffff);

    return static_cast<unsigned>(x);
}

//
// method 5: popcount intrinsic
//

#include <nmmintrin.h>

unsigned bit_count_5(uint32_t x)
{
    return static_cast<unsigned>(_mm_popcnt_u32(x));
}

unsigned bit_count_5(uint64_t x)
{
    return static_cast<unsigned>(_mm_popcnt_u64(x));
}

#endif // BIT_COUNT_H