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
    for(size_t i = 0; i < size; ++i)
        arr[i] = (uint8_t)rand();
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

UBENCH_EX(swap, small)
{
	uint8_t b1[16];
    uint8_t b2[16];
    fill_with_random_data(b1);
    fill_with_random_data(b2);

	UBENCH_DO_BENCHMARK()
	{
		memswap(b1, b2, sizeof(b1));
        UBENCH_DO_NOTHING(b1);
	}
}

UBENCH_EX(swap, big)
{
    const size_t BUF_SZ = 2048 * 2048;
	uint8_t* b1 = alloc_random_buffer<uint8_t>(BUF_SZ);
    uint8_t* b2 = alloc_random_buffer<uint8_t>(BUF_SZ);

	UBENCH_DO_BENCHMARK()
	{
		memswap(b1, b2, BUF_SZ);
	}

    free(b1);
    free(b2);
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
