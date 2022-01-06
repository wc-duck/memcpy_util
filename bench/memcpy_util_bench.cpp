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

template<size_t ARR_SIZE>
static void fill_with_random_data(uint8_t(&arr)[ARR_SIZE])
{
    fill_with_random_data(arr, ARR_SIZE);
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
	}
}

UBENCH_EX(swap, big)
{
    const size_t BUF_SZ = 8 * 1024;
	uint8_t* b1 = (uint8_t*)malloc(BUF_SZ);
    uint8_t* b2 = (uint8_t*)malloc(BUF_SZ);
    fill_with_random_data(b1, BUF_SZ);
    fill_with_random_data(b2, BUF_SZ);

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

UBENCH_EX(memcpy_rectfliph, big)
{
    const size_t LINE_CNT = 2048;
    const size_t LINE_LEN = 2048;
    const size_t BUF_SZ = LINE_CNT * LINE_LEN;
	uint8_t* b1 = (uint8_t*)malloc(BUF_SZ);
    uint8_t* b2 = (uint8_t*)malloc(BUF_SZ);
    fill_with_random_data(b1, BUF_SZ);

	UBENCH_DO_BENCHMARK()
	{
		memcpy_rectfliph(b2, b1, 
                         LINE_CNT, LINE_LEN,
                         LINE_LEN, LINE_LEN);
	}

    free(b1);
    free(b2);
}


///////////////////////////////////////////////////////////////
//                      movecpy_rectfliph                    //
///////////////////////////////////////////////////////////////

UBENCH_EX(memmove_rectfliph, big)
{
    const size_t LINE_CNT = 2048;
    const size_t LINE_LEN = 2048;
    const size_t BUF_SZ = LINE_CNT * LINE_LEN;
	uint8_t* b1 = (uint8_t*)malloc(BUF_SZ);
    fill_with_random_data(b1, BUF_SZ);

	UBENCH_DO_BENCHMARK()
	{
		memmove_rectfliph(b1, b1, 
                          LINE_CNT, LINE_LEN,
                          LINE_LEN, LINE_LEN);
	}

    free(b1);
}

UBENCH_STATE();

int main(int argc, const char *const argv[])
{
    srand(1337);
    return ubench_main(argc, argv);
}
