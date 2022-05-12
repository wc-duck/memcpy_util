/*
 Header containing utility functions for copying memory in different ways.

 Most functions exist in a memcpy_- and a memmove_-version where the move
 versions trade performance for being able to work within overlapping
 memory regions.

 version 1.0, January, 2021

 Copyright (C) 2021- Fredrik Kihlander

 This software is provided 'as-is', without any express or implied
 warranty.  In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

 1. The origin of this software must not be misrepresented; you must not
 claim that you wrote the original software. If you use this software
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.
 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original software.
 3. This notice may not be removed or altered from any source distribution.

 Fredrik Kihlander
 */

#include "ubench.h"
#include "../memcpy_util.h"

#include <stdlib.h>
#include <stdint.h>

static void fill_with_random_data(uint8_t *arr, size_t size)
{
    memset(arr, rand(), size);
}

template<typename T, size_t ARR_SIZE>
static void fill_with_random_data(T(&arr)[ARR_SIZE])
{
    fill_with_random_data((uint8_t*)arr, ARR_SIZE * sizeof(T));
}

template<typename T>
T* alloc_random_buffer(size_t item_cnt)
{
    T* buff = (T*)malloc(item_cnt * sizeof(T));
    fill_with_random_data((uint8_t*)buff, item_cnt * sizeof(T));
    return buff;
}

///////////////////////////////////////////////////////////////
//                          memswap                          //
///////////////////////////////////////////////////////////////

UBENCH_NOINLINE void memswap_default_noinline    (void* ptr1, void* ptr2, size_t s) { memswap            (ptr1, ptr2, s); }
UBENCH_NOINLINE void memswap_generic_noinline    (void* ptr1, void* ptr2, size_t s) { memswap_generic    (ptr1, ptr2, s); }
UBENCH_NOINLINE void memswap_memcpy_noinline     (void* ptr1, void* ptr2, size_t s) { memswap_memcpy     (ptr1, ptr2, s); }
UBENCH_NOINLINE void memswap_memcpy_ptr_noinline (void* ptr1, void* ptr2, size_t s) { memswap_memcpy_ptr (ptr1, ptr2, s); }
UBENCH_NOINLINE void memswap_sse2_noinline       (void* ptr1, void* ptr2, size_t s) { memswap_sse2       (ptr1, ptr2, s); }
UBENCH_NOINLINE void memswap_sse2_unroll_noinline(void* ptr1, void* ptr2, size_t s) { memswap_sse2_unroll(ptr1, ptr2, s); }
UBENCH_NOINLINE void memswap_avx_noinline        (void* ptr1, void* ptr2, size_t s) { memswap_avx        (ptr1, ptr2, s); }
UBENCH_NOINLINE void memswap_avx_unroll_noinline (void* ptr1, void* ptr2, size_t s) { memswap_avx_unroll (ptr1, ptr2, s); }

#include <algorithm>
UBENCH_NOINLINE void memswap_std_swap_ranges_noinline(void* ptr1, void* ptr2, size_t s) { std::swap_ranges((uint8_t*)ptr1, (uint8_t*)ptr1 + s, (uint8_t*)ptr2); }
UBENCH_NOINLINE void memswap_memcpy_only_noinline    (void* ptr1, void* ptr2, size_t s) { memcpy(ptr1, ptr2, s); }

struct memswap_small
{
    uint8_t b1[16];
    uint8_t b2[16];
};

UBENCH_NOINLINE void* clear_cache_alloc() { return alloc_random_buffer<uint8_t>(32*1024*1024); }
UBENCH_NOINLINE void  clear_cache()       { free(clear_cache_alloc()); }

#define BENCH_MEMSWAP_SMALL(TYPE)                          \
    UBENCH_EX(memswap_small, TYPE)                         \
    {                                                      \
        uint8_t b1[16];                                    \
        uint8_t b2[16];                                    \
        fill_with_random_data(b1);                         \
        fill_with_random_data(b2);                         \
        clear_cache();                                     \
                                                           \
        UBENCH_DO_BENCHMARK()                              \
        {                                                  \
            memswap_##TYPE##_noinline(b1, b2, sizeof(b1)); \
        }                                                  \
    }

#define BENCH_MEMSWAP_BIG(TYPE)                             \
    UBENCH_EX(memswap_big, TYPE)                            \
    {                                                       \
        const size_t BUF_SZ = 4 * 1024 * 1024;              \
        uint8_t* b1 = alloc_random_buffer<uint8_t>(BUF_SZ); \
        uint8_t* b2 = alloc_random_buffer<uint8_t>(BUF_SZ); \
        clear_cache();                                      \
        UBENCH_DO_BENCHMARK()                               \
        {                                                   \
            memswap_##TYPE##_noinline(b1, b2, BUF_SZ);      \
        }                                                   \
        free(b1);                                           \
        free(b2);                                           \
    }

BENCH_MEMSWAP_SMALL(default)
BENCH_MEMSWAP_SMALL(generic)
BENCH_MEMSWAP_SMALL(memcpy)
BENCH_MEMSWAP_SMALL(memcpy_ptr)
BENCH_MEMSWAP_SMALL(sse2)
BENCH_MEMSWAP_SMALL(sse2_unroll)
BENCH_MEMSWAP_SMALL(avx)
BENCH_MEMSWAP_SMALL(avx_unroll)

BENCH_MEMSWAP_SMALL(std_swap_ranges)
BENCH_MEMSWAP_SMALL(memcpy_only)

BENCH_MEMSWAP_BIG(default)
BENCH_MEMSWAP_BIG(generic)
BENCH_MEMSWAP_BIG(memcpy)
BENCH_MEMSWAP_BIG(memcpy_ptr)
BENCH_MEMSWAP_BIG(sse2)
BENCH_MEMSWAP_BIG(sse2_unroll)
BENCH_MEMSWAP_BIG(avx)
BENCH_MEMSWAP_BIG(avx_unroll)

BENCH_MEMSWAP_BIG(std_swap_ranges)
BENCH_MEMSWAP_BIG(memcpy_only)

UBENCH_EX(memswap_stack, test)
{
    char buff1[16*1024];
    char buff2[16*1024];
	UBENCH_DO_BENCHMARK()
    {
        memswap_sse2_noinline(buff1, buff2, sizeof(buff1));
        UBENCH_DO_NOTHING(buff1);
        UBENCH_DO_NOTHING(buff2);
    }
}

///////////////////////////////////////////////////////////////
//                        memcpy_rect                        //
///////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
//                      memcpy_rectfliph                     //
///////////////////////////////////////////////////////////////

#define BENCH_MEMCPY_RECTFLIPH_SIZE(TYPE, LINE_CNT, LINE_LEN)  \
	TYPE* b1 = alloc_random_buffer<TYPE>(LINE_CNT * LINE_LEN); \
    TYPE* b2 = alloc_random_buffer<TYPE>(LINE_CNT * LINE_LEN); \
                                                               \
	UBENCH_DO_BENCHMARK()                                      \
	{                                                          \
		UBENCH_DO_NOTHING(                                     \
            memcpy_rectfliph(b2, b1,                           \
                             LINE_CNT, LINE_LEN,               \
                             LINE_LEN, LINE_LEN,               \
                             sizeof(b1[0]))                    \
        );                                                     \
	}                                                          \
                                                               \
    free(b1);                                                  \
    free(b2);

UBENCH_EX(memcpy_rectfliph, uint8_t)  { BENCH_MEMCPY_RECTFLIPH_SIZE( uint8_t, 2048, 2048); }
UBENCH_EX(memcpy_rectfliph, uint16_t) { BENCH_MEMCPY_RECTFLIPH_SIZE(uint16_t, 1024, 2048); }
UBENCH_EX(memcpy_rectfliph, uint32_t) { BENCH_MEMCPY_RECTFLIPH_SIZE(uint32_t, 1024, 1024); }
UBENCH_EX(memcpy_rectfliph, uint64_t) { BENCH_MEMCPY_RECTFLIPH_SIZE(uint64_t,  512, 1024); }
// TODO: benchmark "uneven size items!"


///////////////////////////////////////////////////////////////
//                      memmove_rectfliph                    //
///////////////////////////////////////////////////////////////

#define BENCH_MEMMOVE_RECTFLIPH_SIZE(TYPE, LINE_CNT, LINE_LEN) \
	TYPE* b1 = alloc_random_buffer<TYPE>(LINE_CNT * LINE_LEN); \
                                                               \
	UBENCH_DO_BENCHMARK()                                      \
	{                                                          \
        UBENCH_DO_NOTHING(                                     \
		    memmove_rectfliph(b1, b1,                          \
                              LINE_CNT, LINE_LEN,              \
                              LINE_LEN, LINE_LEN,              \
                              sizeof(b1[0]))                   \
        );                                                     \
	}                                                          \
    free(b1);

UBENCH_EX(memmove_rectfliph, uint8_t)  { BENCH_MEMMOVE_RECTFLIPH_SIZE( uint8_t, 2048, 2048);  }
UBENCH_EX(memmove_rectfliph, uint16_t) { BENCH_MEMMOVE_RECTFLIPH_SIZE(uint16_t, 1024, 2048); }
UBENCH_EX(memmove_rectfliph, uint32_t) { BENCH_MEMMOVE_RECTFLIPH_SIZE(uint32_t, 1024, 1024); }
UBENCH_EX(memmove_rectfliph, uint64_t) { BENCH_MEMMOVE_RECTFLIPH_SIZE(uint64_t,  512, 1024); }
// TODO: benchmark "uneven size items!"

///////////////////////////////////////////////////////////////
//                      memcpy_rectfliph                     //
///////////////////////////////////////////////////////////////

#define BENCH_MEMCPY_RECTFLIPV_SIZE(TYPE, LINE_CNT, LINE_LEN)   \
	TYPE* b1 = alloc_random_buffer<TYPE>(LINE_CNT * LINE_LEN);  \
    TYPE* b2 = alloc_random_buffer<TYPE>(LINE_CNT * LINE_LEN);  \
                                                                \
	UBENCH_DO_BENCHMARK()                                       \
	{                                                           \
		UBENCH_DO_NOTHING(                                      \
            memcpy_rectflipv(b2, b1,                            \
                             LINE_CNT, LINE_LEN,                \
                             LINE_LEN, LINE_LEN,                \
                             sizeof(b1[0]))                     \
        );                                                      \
	}                                                           \
                                                                \
    free(b1);                                                   \
    free(b2);

UBENCH_EX(memcpy_rectflipv, uint8_t)  { BENCH_MEMCPY_RECTFLIPV_SIZE( uint8_t, 2048, 2048);  }
UBENCH_EX(memcpy_rectflipv, uint16_t) { BENCH_MEMCPY_RECTFLIPV_SIZE(uint16_t, 1024, 2048); }
UBENCH_EX(memcpy_rectflipv, uint32_t) { BENCH_MEMCPY_RECTFLIPV_SIZE(uint32_t, 1024, 1024); }
UBENCH_EX(memcpy_rectflipv, uint64_t) { BENCH_MEMCPY_RECTFLIPV_SIZE(uint64_t,  512, 1024); }
// TODO: benchmark "uneven size items!"

///////////////////////////////////////////////////////////////
//                      memmove_rectfliph                    //
///////////////////////////////////////////////////////////////

#define BENCH_MEMMOVE_RECTFLIPV_SIZE(TYPE, LINE_CNT, LINE_LEN) \
	TYPE* b1 = alloc_random_buffer<TYPE>(LINE_CNT * LINE_LEN); \
                                                               \
	UBENCH_DO_BENCHMARK()                                      \
	{                                                          \
        UBENCH_DO_NOTHING(                                     \
		    memmove_rectflipv(b1, b1,                          \
                              LINE_CNT, LINE_LEN,              \
                              LINE_LEN, LINE_LEN,              \
                              sizeof(b1[0]))                   \
        );                                                     \
	}                                                          \
    free(b1);

UBENCH_EX(memmove_rectflipv, uint8_t)  { BENCH_MEMMOVE_RECTFLIPV_SIZE( uint8_t, 2048, 2048);  }
UBENCH_EX(memmove_rectflipv, uint16_t) { BENCH_MEMMOVE_RECTFLIPV_SIZE(uint16_t, 1024, 2048); }
UBENCH_EX(memmove_rectflipv, uint32_t) { BENCH_MEMMOVE_RECTFLIPV_SIZE(uint32_t, 1024, 1024); }
UBENCH_EX(memmove_rectflipv, uint64_t) { BENCH_MEMMOVE_RECTFLIPV_SIZE(uint64_t,  512, 1024); }
// TODO: benchmark "uneven size items!"


UBENCH_STATE();

int main(int argc, const char *const argv[])
{
#if defined(__clang__)
    printf("Clang\n");
#elif defined(__GNUC__)
    printf("GCC\n");
#endif
    srand(1337);
    return ubench_main(argc, argv);
}
