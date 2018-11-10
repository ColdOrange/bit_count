#include "benchmark.h"
#include "bit_count.h"
#include <random>
#include <cassert>

const int N = 10000000;
uint32_t vec32[N];
uint64_t vec64[N];

void init()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dist32;
    std::uniform_int_distribution<uint64_t> dist64;
    for (int i = 0; i < N; ++i)
    {
        vec32[i] = dist32(gen);
        vec64[i] = dist64(gen);

    }
}

void test_equality()
{
#define ASSERT_EQUALITY(i)                  \
assert(bit_count_1(i) == bit_count_2(i));   \
assert(bit_count_1(i) == bit_count_3(i));   \
assert(bit_count_1(i) == bit_count_4(i));   \
assert(bit_count_1(i) == bit_count_5(i));   \

    for (auto i : vec32)
    {
        ASSERT_EQUALITY(i);
    }

    for (auto i : vec64)
    {
        ASSERT_EQUALITY(i);
    }

#undef ASSERT_EQUALITY
}

Benchmarks benchmarks;

BENCHMARK(m1_32, simple_iterating)
{
    for (auto i : vec32)
    {
        bit_count_1(i);
    }
}
BENCHMARK(m2_32, sparse_iterating)
{
    for (auto i : vec32)
    {
        bit_count_2(i);
    }
}
BENCHMARK(m3_32, lookup_table)
{
    for (auto i : vec32)
    {
        bit_count_3(i);
    }
}
BENCHMARK(m4_32, parallel_computing)
{
    for (auto i : vec32)
    {
        bit_count_4(i);
    }
}
BENCHMARK(m5_32, popcount_intrinsic)
{
    for (auto i : vec32)
    {
        bit_count_5(i);
    }
}

BENCHMARK(m1_64, simple_iterating)
{
    for (auto i : vec64)
    {
        bit_count_1(i);
    }
}
BENCHMARK(m2_64, sparse_iterating)
{
    for (auto i : vec64)
    {
        bit_count_2(i);
    }
}
BENCHMARK(m3_64, lookup_table)
{
    for (auto i : vec64)
    {
        bit_count_3(i);
    }
}
BENCHMARK(m4_64, parallel_computing)
{
    for (auto i : vec64)
    {
        bit_count_4(i);
    }
}
BENCHMARK(m5_64, popcount_intrinsic)
{
    for (auto i : vec64)
    {
        bit_count_5(i);
    }
}

int main()
{
    init();
    //test_equality();
    benchmarks.RunBenchmarks();

    return 0;
}

// clang-1000.11.45.5 (macOS 10.13, 2.3 GHz Intel Core i5)
//
//   [m1_32](simple_iterating):   696.63 milliseconds.
//   [m2_32](sparse_iterating):   453.47 milliseconds.
//   [m3_32](lookup_table):       110.44 milliseconds.
//   [m4_32](parallel_computing): 67.55  milliseconds.
//   [m5_32](popcount_intrinsic): 29.70  milliseconds.
//
//   [m1_64](simple_iterating):   1327.91 milliseconds.
//   [m2_64](sparse_iterating):   767.80  milliseconds.
//   [m3_64](lookup_table):       193.34  milliseconds.
//   [m4_64](parallel_computing): 81.05   milliseconds.
//   [m5_64](popcount_intrinsic): 29.44   milliseconds.

// g++ 5.4.0 (ubuntu 16.04, tencent cloud virtual machine with 1 core)
//
//   [m1_32](simple_iterating):   978.15 milliseconds.
//   [m2_32](sparse_iterating):   625.87 milliseconds.
//   [m3_32](lookup_table):       130.06 milliseconds.
//   [m4_32](parallel_computing): 102.38 milliseconds.
//   [m5_32](popcount_intrinsic): 37.30  milliseconds.
//
//   [m1_64](simple_iterating):   1956.74 milliseconds.
//   [m2_64](sparse_iterating):   1106.47 milliseconds.
//   [m3_64](lookup_table):       233.39  milliseconds.
//   [m4_64](parallel_computing): 123.49  milliseconds.
//   [m5_64](popcount_intrinsic): 43.26   milliseconds.
