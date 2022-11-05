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

#include "greatest.h"
#include "../memcpy_util.h"

#include <stdint.h>

#define ASSERT_MEMEQ(b1, b2) \
	static_assert(sizeof(b1) == sizeof(b2), ""); \
	GREATEST_ASSERT_MEM_EQ(b1, b2, sizeof(b1));

///////////////////////////////////////////////////////////////
//                          memswap                          //
///////////////////////////////////////////////////////////////
TEST memswap_simple()
{
	const uint8_t expect_a[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
	const uint8_t expect_b[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };

	uint8_t test_a[sizeof(expect_a)];
	uint8_t test_b[sizeof(expect_b)];
	memcpy(test_a, expect_a, sizeof(expect_a));
	memcpy(test_b, expect_b, sizeof(expect_b));

	memswap(test_a, test_b, sizeof(test_a));
	ASSERT_MEMEQ(test_a, expect_b);
	ASSERT_MEMEQ(test_b, expect_a);

	return GREATEST_TEST_RES_PASS;
}

TEST memswap_many_sizes()
{
	size_t max_size = 512;
	uint8_t* expect_a = (uint8_t*)malloc(max_size);
	uint8_t* expect_b = (uint8_t*)malloc(max_size);
	uint8_t* buf_a    = (uint8_t*)malloc(max_size);
	uint8_t* buf_b    = (uint8_t*)malloc(max_size);

	GREATEST_TEST res = GREATEST_TEST_RES_PASS;
	for(size_t i = 0; i < 512; ++i )
	{
		{
			memcpy(buf_a, expect_a, i);
			memcpy(buf_b, expect_b, i);
			memswap(buf_a, buf_b, i);
			ASSERT_MEMEQ(buf_a, expect_b);
			ASSERT_MEMEQ(buf_b, expect_a);
		}

		{
			memcpy(buf_a, expect_a, i);
			memcpy(buf_b, expect_b, i);
			memswap_sse2(buf_a, buf_b, i);
			ASSERT_MEMEQ(buf_a, expect_b);
			ASSERT_MEMEQ(buf_b, expect_a);
		}

		{
			memcpy(buf_a, expect_a, i);
			memcpy(buf_b, expect_b, i);
			memswap_sse2_unroll(buf_a, buf_b, i);
			ASSERT_MEMEQ(buf_a, expect_b);
			ASSERT_MEMEQ(buf_b, expect_a);
		}

		{
			memcpy(buf_a, expect_a, i);
			memcpy(buf_b, expect_b, i);
			memswap_avx(buf_a, buf_b, i);
			ASSERT_MEMEQ(buf_a, expect_b);
			ASSERT_MEMEQ(buf_b, expect_a);
		}

		{
			memcpy(buf_a, expect_a, i);
			memcpy(buf_b, expect_b, i);
			memswap_avx_unroll(buf_a, buf_b, i);
			ASSERT_MEMEQ(buf_a, expect_b);
			ASSERT_MEMEQ(buf_b, expect_a);
		}
	}

	free(expect_a);
	free(expect_b);
	free(buf_a);
	free(buf_b);

	return res;
}

// TODO: add test for memcpy, sse2 and avx-versions, bigger and smaller!

///////////////////////////////////////////////////////////////
//                        memcpy_rect                        //
///////////////////////////////////////////////////////////////

TEST memcpy_rect_simple()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
			 	 	 	 'e', 'f', 'g', 'h',
			 	 	 	 'A', 'B', 'C', 'D',
			 	 	 	 'E', 'F', 'G', 'H' };
	uint8_t expect[] = {  0,   0,   0,   0,
			 	 	 	  0,  'a', 'b',  0,
			 	 	 	  0,  'e', 'f',  0,
			 	 	 	  0,   0,   0,   0, };
	uint8_t dst[sizeof(buffer)] = {0};
	memcpy_rect( &dst[5], buffer, 2, 2, 4, 4 );
	ASSERT_MEMEQ(dst, expect);

    return GREATEST_TEST_RES_PASS;
}

///////////////////////////////////////////////////////////////
//                      memcpy_rectrotr                      //
///////////////////////////////////////////////////////////////

TEST memcpy_rectrotr_full()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
			 	 	 	 'e', 'f', 'g', 'h',
			 	 	 	 'A', 'B', 'C', 'D',
			 	 	 	 'E', 'F', 'G', 'H' };
	uint8_t expect[] = { 'E', 'A', 'e', 'a',
	 	 	 	 	 	 'F', 'B', 'f', 'b',
	 	 	 	 	 	 'G', 'C', 'g', 'c',
	 	 	 	 	 	 'H', 'D', 'h', 'd' };
	uint8_t dst[sizeof(buffer)] = {0};
	memcpy_rectrotr( dst, buffer, 4, 4, 4, 4 );
	ASSERT_MEMEQ(dst, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memcpy_rectrotr_big()
{
	uint8_t buffer[8*8] = {  0,  1,  2,  3,  4,  5,  6,  7,
			 	 	 	  	 8,  9, 10, 11, 12, 13, 14, 15,
							16, 17, 18, 19, 20, 21, 22, 23,
							24, 25, 26, 27, 28, 29, 30, 31,
							32, 33, 34, 35, 36, 37, 38, 39,
							40, 41, 42, 43, 44, 45, 46, 47,
							48, 49, 50, 51, 52, 53, 54, 55,
							56, 57, 58, 59, 60, 61, 62, 63 };
	uint8_t expect[8*8] = { 56, 48, 40, 32, 24, 16,  8,  0,
	                        57, 49, 41, 33, 25, 17,  9,  1,
							58, 50, 42, 34, 26, 18, 10,  2,
							59, 51, 43, 35, 27, 19, 11,  3,
							60, 52, 44, 36, 28, 20, 12,  4,
							61, 53, 45, 37, 29, 21, 13,  5,
							62, 54, 46, 38, 30, 22, 14,  6,
							63, 55, 47, 39, 31, 23, 15,  7 };
	uint8_t dst[sizeof(buffer)] = {0};
	memcpy_rectrotr( dst, buffer, 8, 8, 8, 8 );
	ASSERT_MEMEQ(dst, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memcpy_rectrotr_simple()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
			 	 	 	 'e', 'f', 'g', 'h',
			 	 	 	 'A', 'B', 'C', 'D',
			 	 	 	 'E', 'F', 'G', 'H' };
	uint8_t expect[] = {  0,   0,   0,   0,
			 	 	 	  0,  'e', 'a',  0,
			 	 	 	  0,  'f', 'b',  0,
			 	 	 	  0,   0,   0,   0 };
	uint8_t dst[sizeof(buffer)] = {0};
	memcpy_rectrotr( &dst[5], buffer, 2, 2, 4, 4 );
	ASSERT_MEMEQ(dst, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memcpy_rectrotr_subrect()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
			 	 	 	 'e', 'f', 'g', 'h',
			 	 	 	 'A', 'B', 'C', 'D',
			 	 	 	 'E', 'F', 'G', 'H' };
	uint8_t expect[] = {  0,  'A', 'e',  0,
			 	 	 	  0,  'B', 'f',  0,
			 	 	 	  0,  'C', 'g',  0,
			 	 	 	  0,  'D', 'h',  0 };
	uint8_t dst[sizeof(buffer)] = {0};
	memcpy_rectrotr( &dst[1], &buffer[4], 2, 4, 4, 4 );
	ASSERT_MEMEQ(dst, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memmove_rectrotr_full()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
			 	 	 	 'e', 'f', 'g', 'h',
			 	 	 	 'A', 'B', 'C', 'D',
			 	 	 	 'E', 'F', 'G', 'H' };
	uint8_t expect[] = { 'E', 'A', 'e', 'a',
	 	 	 	 	 	 'F', 'B', 'f', 'b',
	 	 	 	 	 	 'G', 'C', 'g', 'c',
	 	 	 	 	 	 'H', 'D', 'h', 'd' };
	memmove_rectrotr( buffer, buffer, 4, 4, 4, 4 );
	ASSERT_MEMEQ(buffer, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memmove_rectrotr_big()
{
	uint8_t buffer[8*8] = {  0,  1,  2,  3,  4,  5,  6,  7,
							 8,  9, 10, 11, 12, 13, 14, 15,
							16, 17, 18, 19, 20, 21, 22, 23,
							24, 25, 26, 27, 28, 29, 30, 31,
							32, 33, 34, 35, 36, 37, 38, 39,
							40, 41, 42, 43, 44, 45, 46, 47,
							48, 49, 50, 51, 52, 53, 54, 55,
							56, 57, 58, 59, 60, 61, 62, 63 };
	uint8_t expect[8*8] = { 56, 48, 40, 32, 24, 16,  8,  0,
							57, 49, 41, 33, 25, 17,  9,  1,
							58, 50, 42, 34, 26, 18, 10,  2,
							59, 51, 43, 35, 27, 19, 11,  3,
							60, 52, 44, 36, 28, 20, 12,  4,
							61, 53, 45, 37, 29, 21, 13,  5,
							62, 54, 46, 38, 30, 22, 14,  6,
							63, 55, 47, 39, 31, 23, 15,  7 };
	memmove_rectrotr( buffer, buffer, 8, 8, 8, 8 );
	ASSERT_MEMEQ(buffer, expect);

    return GREATEST_TEST_RES_PASS;
}

///////////////////////////////////////////////////////////////
//                      memcpy_rectrotl                      //
///////////////////////////////////////////////////////////////

TEST memcpy_rectrotl_full()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
			 	 	 	 'e', 'f', 'g', 'h',
			 	 	 	 'A', 'B', 'C', 'D',
			 	 	 	 'E', 'F', 'G', 'H' };
	uint8_t expect[] = { 'd', 'h', 'D', 'H',
	 	 	 	 	 	 'c', 'g', 'C', 'G',
	 	 	 	 	 	 'b', 'f', 'B', 'F',
	 	 	 	 	 	 'a', 'e', 'A', 'E' };
	uint8_t dst[sizeof(buffer)] = {0};
	memcpy_rectrotl( dst, buffer, 4, 4, 4, 4 );
	ASSERT_MEMEQ(dst, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memcpy_rectrotl_simple()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
			 	 	 	 'e', 'f', 'g', 'h',
			 	 	 	 'A', 'B', 'C', 'D',
			 	 	 	 'E', 'F', 'G', 'H' };
	uint8_t expect[] = {  0,   0,   0,   0,
			 	 	 	  0,  'b', 'f',  0,
			 	 	 	  0,  'a', 'e',  0,
			 	 	 	  0,   0,   0,   0 };
	uint8_t dst[sizeof(buffer)] = {0};
	memcpy_rectrotl( &dst[5], buffer, 2, 2, 4, 4 );
	ASSERT_MEMEQ(dst, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memcpy_rectrotl_subrect()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
			 	 	 	 'e', 'f', 'g', 'h',
			 	 	 	 'A', 'B', 'C', 'D',
			 	 	 	 'E', 'F', 'G', 'H' };
	uint8_t expect[] = {  0,  'h', 'D',  0,
			 	 	 	  0,  'g', 'C',  0,
			 	 	 	  0,  'f', 'B',  0,
			 	 	 	  0,  'e', 'A',  0 };
	uint8_t dst[sizeof(buffer)] = {0};
	memcpy_rectrotl( &dst[1], &buffer[4], 2, 4, 4, 4 );
	ASSERT_MEMEQ(dst, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memmove_rectrotl_full()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
			 	 	 	 'e', 'f', 'g', 'h',
			 	 	 	 'A', 'B', 'C', 'D',
			 	 	 	 'E', 'F', 'G', 'H' };
    uint8_t expect[] = { 'd', 'h', 'D', 'H',
	 	 	 	 	 	 'c', 'g', 'C', 'G',
	 	 	 	 	 	 'b', 'f', 'B', 'F',
	 	 	 	 	 	 'a', 'e', 'A', 'E' };
	memmove_rectrotl( buffer, buffer, 4, 4, 4, 4 );
	ASSERT_MEMEQ(buffer, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memmove_rectrotl_big()
{
	uint8_t buffer[8*8] = {  0,  1,  2,  3,  4,  5,  6,  7,
							 8,  9, 10, 11, 12, 13, 14, 15,
							16, 17, 18, 19, 20, 21, 22, 23,
							24, 25, 26, 27, 28, 29, 30, 31,
							32, 33, 34, 35, 36, 37, 38, 39,
							40, 41, 42, 43, 44, 45, 46, 47,
							48, 49, 50, 51, 52, 53, 54, 55,
							56, 57, 58, 59, 60, 61, 62, 63 };
	uint8_t expect[8*8] = {  7, 15, 23, 31, 39, 47, 55, 63,
							 6, 14, 22, 30, 38, 46, 54, 62,
							 5, 13, 21, 29, 37, 45, 53, 61,
							 4, 12, 20, 28, 36, 44, 52, 60,
							 3, 11, 19, 27, 35, 43, 51, 59,
							 2, 10, 18, 26, 34, 42, 50, 58,
							 1,  9, 17, 25, 33, 41, 49, 57,
							 0,  8, 16, 24, 32, 40, 48, 56 };

	memmove_rectrotl( buffer, buffer, 8, 8, 8, 8 );
	ASSERT_MEMEQ(buffer, expect);

    return GREATEST_TEST_RES_PASS;
}

///////////////////////////////////////////////////////////////
//                        memmove_rect                       //
///////////////////////////////////////////////////////////////

TEST memmove_rect_simple()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
			 	 	 	 'e', 'f', 'g', 'h',
			 	 	 	 'A', 'B', 'C', 'D',
			 	 	 	 'E', 'F', 'G', 'H' };
	uint8_t expect[] = { 'a', 'b', 'c', 'd',
	 	 	 	 	 	 'e', 'f', 'a', 'b',
	 	 	 	 	 	 'A', 'B', 'e', 'f',
	 	 	 	 	 	 'E', 'F', 'G', 'H' };
	memmove_rect( &buffer[6], buffer, 2, 2, 4, 4 );
	ASSERT_MEMEQ(buffer, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memmove_rect_no_overlap_n()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
			 	 	 	 'e', 'f', 'g', 'h',
			 	 	 	 'A', 'B', 'C', 'D',
			 	 	 	 'E', 'F', 'G', 'H' };

	uint8_t expect[] = { 'a', 'B', 'C', 'd',
	 	 	 	 	 	 'e', 'F', 'G', 'h',
	 	 	 	 	 	 'A', 'B', 'C', 'D',
	 	 	 	 	 	 'E', 'F', 'G', 'H' };
	memmove_rect( &buffer[1], &buffer[9], 2, 2, 4, 4 );
	ASSERT_MEMEQ(buffer, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memmove_rect_overlap_n()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
			 	 	 	 'e', 'f', 'g', 'h',
			 	 	 	 'A', 'B', 'C', 'D',
			 	 	 	 'E', 'F', 'G', 'H' };

	uint8_t expect[] = { 'a', 'f', 'g', 'd',
	 	 	 	 	 	 'e', 'B', 'C', 'h',
	 	 	 	 	 	 'A', 'B', 'C', 'D',
	 	 	 	 	 	 'E', 'F', 'G', 'H' };
	memmove_rect( &buffer[1], &buffer[5], 2, 2, 4, 4 );
	ASSERT_MEMEQ(buffer, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memmove_rect_overlap_s()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
			 	 	 	 'e', 'f', 'g', 'h',
			 	 	 	 'A', 'B', 'C', 'D',
			 	 	 	 'E', 'F', 'G', 'H' };

	uint8_t expect[] = { 'a', 'b', 'c', 'd',
	 	 	 	 	 	 'e', 'f', 'g', 'h',
	 	 	 	 	 	 'A', 'f', 'g', 'D',
	 	 	 	 	 	 'E', 'B', 'C', 'H' };
	memmove_rect( &buffer[9], &buffer[5], 2, 2, 4, 4 );
	ASSERT_MEMEQ(buffer, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memmove_rect_overlap_w()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
			 	 	 	 'e', 'f', 'g', 'h',
			 	 	 	 'A', 'B', 'C', 'D',
			 	 	 	 'E', 'F', 'G', 'H' };

	uint8_t expect[] = { 'a', 'b', 'c', 'd',
						 'f', 'g', 'g', 'h',
						 'B', 'C', 'C', 'D',
	 	 	 	 	 	 'E', 'F', 'G', 'H' };
	memmove_rect( &buffer[4], &buffer[5], 2, 2, 4, 4 );
	ASSERT_MEMEQ(buffer, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memmove_rect_overlap_e()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
			 	 	 	 'e', 'f', 'g', 'h',
			 	 	 	 'A', 'B', 'C', 'D',
			 	 	 	 'E', 'F', 'G', 'H' };

	uint8_t expect[] = { 'a', 'b', 'c', 'd',
	 	 	 	 	 	 'e', 'f', 'f', 'g',
	 	 	 	 	 	 'A', 'B', 'B', 'C',
	 	 	 	 	 	 'E', 'F', 'G', 'H' };
	memmove_rect( &buffer[6], &buffer[5], 2, 2, 4, 4 );
	ASSERT_MEMEQ(buffer, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memmove_rect_overlap_diagnal_nw()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
			 	 	 	 'e', 'f', 'g', 'h',
			 	 	 	 'A', 'B', 'C', 'D',
			 	 	 	 'E', 'F', 'G', 'H' };

	uint8_t expect[] = { 'f', 'g', 'c', 'd',
						 'B', 'C', 'g', 'h',
	 	 	 	 	 	 'A', 'B', 'C', 'D',
	 	 	 	 	 	 'E', 'F', 'G', 'H' };
	memmove_rect( &buffer[0], &buffer[5], 2, 2, 4, 4 );
	ASSERT_MEMEQ(buffer, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memmove_rect_overlap_diagnal_ne()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
			 	 	 	 'e', 'f', 'g', 'h',
			 	 	 	 'A', 'B', 'C', 'D',
			 	 	 	 'E', 'F', 'G', 'H' };

	uint8_t expect[] = { 'a', 'b', 'f', 'g',
	 	 	 	 	 	 'e', 'f', 'B', 'C',
	 	 	 	 	 	 'A', 'B', 'C', 'D',
	 	 	 	 	 	 'E', 'F', 'G', 'H' };
	memmove_rect( &buffer[2], &buffer[5], 2, 2, 4, 4 );
	ASSERT_MEMEQ(buffer, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memmove_rect_overlap_diagnal_sw()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
			 	 	 	 'e', 'f', 'g', 'h',
			 	 	 	 'A', 'B', 'C', 'D',
			 	 	 	 'E', 'F', 'G', 'H' };

	uint8_t expect[] = { 'a', 'b', 'c', 'd',
	 	 	 	 	 	 'e', 'f', 'g', 'h',
	 	 	 	 	 	 'f', 'g', 'C', 'D',
	 	 	 	 	 	 'B', 'C', 'G', 'H' };
	memmove_rect( &buffer[8], &buffer[5], 2, 2, 4, 4 );
	ASSERT_MEMEQ(buffer, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memmove_rect_overlap_diagnal_se()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
			 	 	 	 'e', 'f', 'g', 'h',
			 	 	 	 'A', 'B', 'C', 'D',
			 	 	 	 'E', 'F', 'G', 'H' };

	uint8_t expect[] = { 'a', 'b', 'c', 'd',
	 	 	 	 	 	 'e', 'f', 'g', 'h',
	 	 	 	 	 	 'A', 'B', 'f', 'g',
	 	 	 	 	 	 'E', 'F', 'B', 'C' };
	memmove_rect( &buffer[10], &buffer[5], 2, 2, 4, 4 );
	ASSERT_MEMEQ(buffer, expect);

    return GREATEST_TEST_RES_PASS;
}

///////////////////////////////////////////////////////////////
//                      memcpy_rectfliph                     //
///////////////////////////////////////////////////////////////

TEST memcpy_rectfliph_even()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
						 'e', 'f', 'g', 'h',
						 'A', 'B', 'C', 'D',
						 'E', 'F', 'G', 'H' };

	uint8_t expect[] = { 'E', 'F', 'G', 'H',
						 'A', 'B', 'C', 'D',
						 'e', 'f', 'g', 'h',
						 'a', 'b', 'c', 'd' };

	uint8_t dst[sizeof(buffer)] = {0};
	memcpy_rectfliph( dst, buffer, 4, 4, 4, 4, 1 );
	ASSERT_MEMEQ(dst, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memcpy_rectfliph_uneven()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
						 'e', 'f', 'g', 'h',
						 'i', 'j', 'k', 'l',
						 'A', 'B', 'C', 'D',
						 'E', 'F', 'G', 'H' };

	uint8_t expect[] = { 'E', 'F', 'G', 'H',
						 'A', 'B', 'C', 'D',
						 'i', 'j', 'k', 'l',
						 'e', 'f', 'g', 'h',
						 'a', 'b', 'c', 'd' };

	uint8_t dst[sizeof(buffer)] = {0};
	memcpy_rectfliph( dst, buffer, 5, 4, 4, 4, 1 );
	ASSERT_MEMEQ(dst, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memcpy_rectfliph_subrect()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
						 'e', 'f', 'g', 'h',
						 'i', 'j', 'k', 'l',
						 'A', 'B', 'C', 'D',
						 'E', 'F', 'G', 'H' };

	uint8_t expect[] = { 'i', 'j', 0, 0,
						 'e', 'f', 0, 0,
						 'a', 'b', 0, 0,
						  0,   0,  0, 0,
						  0,   0,  0, 0 };

	uint8_t dst[sizeof(buffer)] = {0};
	memcpy_rectfliph( dst, buffer, 3, 2, 4, 4, 1 );
	ASSERT_MEMEQ(dst, expect);

    return GREATEST_TEST_RES_PASS;
}

///////////////////////////////////////////////////////////////
//                     memmove_rectfliph                     //
///////////////////////////////////////////////////////////////

TEST memmove_rectfliph_even()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
						 'e', 'f', 'g', 'h',
						 'A', 'B', 'C', 'D',
						 'E', 'F', 'G', 'H' };

	uint8_t expect[] = { 'E', 'F', 'G', 'H',
						 'A', 'B', 'C', 'D',
						 'e', 'f', 'g', 'h',
						 'a', 'b', 'c', 'd' };

	memmove_rectfliph( buffer, buffer, 4, 4, 4, 4, 1 );
	ASSERT_MEMEQ(buffer, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memmove_rectfliph_uneven()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
						 'e', 'f', 'g', 'h',
						 'i', 'j', 'k', 'l',
						 'A', 'B', 'C', 'D',
						 'E', 'F', 'G', 'H' };

	uint8_t expect[] = { 'E', 'F', 'G', 'H',
						 'A', 'B', 'C', 'D',
						 'i', 'j', 'k', 'l',
						 'e', 'f', 'g', 'h',
						 'a', 'b', 'c', 'd' };

	memmove_rectfliph( buffer, buffer, 5, 4, 4, 4, 1 );
	ASSERT_MEMEQ(buffer, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memmove_rectfliph_subrect()
{
	// ... flip top left corner ...
	{
		uint8_t buffer[] = { 'a', 'b', 'c', 'd',
							 'e', 'f', 'g', 'h',
							 'i', 'j', 'k', 'l',
							 'A', 'B', 'C', 'D',
							 'E', 'F', 'G', 'H' };

		uint8_t expect[] = { 'i', 'j',   'c', 'd',
							 'e', 'f',   'g', 'h',
							 'a', 'b',   'k', 'l',

							 'A', 'B',   'C', 'D',
							 'E', 'F',   'G', 'H' };

		memmove_rectfliph( buffer, buffer, 3, 2, 4, 4, 1 );
		ASSERT_MEMEQ(buffer, expect);
	}

	{
		uint8_t buffer[] = { 'a', 'b', 'c', 'd',
							 'e', 'f', 'g', 'h',
							 'i', 'j', 'k', 'l',
							 'A', 'B', 'C', 'D',
							 'E', 'F', 'G', 'H',
							 'I', 'J', 'K', 'L' };

		uint8_t expect[] = { 'A', 'B',   'c', 'd',
							 'i', 'j',   'g', 'h',
							 'e', 'f',   'k', 'l',
							 'a', 'b',   'C', 'D',
							 
							 'E', 'F',   'G', 'H',
							 'I', 'J',   'K', 'L' };

		memmove_rectfliph( buffer, buffer, 4, 2, 4, 4, 1 );
		ASSERT_MEMEQ(buffer, expect);
	}

    return GREATEST_TEST_RES_PASS;
}

///////////////////////////////////////////////////////////////
//                      memcpy_rectflipv                     //
///////////////////////////////////////////////////////////////

TEST memcpy_rectflipv_even()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
					     'e', 'f', 'g', 'h',
						 'A', 'B', 'C', 'D',
						 'E', 'F', 'G', 'H' };

	uint8_t expect[] = { 'd', 'c', 'b', 'a',
		     	 	 	 'h', 'g', 'f', 'e',
		     	 	 	 'D', 'C', 'B', 'A',
		     	 	 	 'H', 'G', 'F', 'E' };

	uint8_t dst[sizeof(buffer)] = {0};
	memcpy_rectflipv( dst, buffer, 4, 4, 4, 4, sizeof(uint8_t) );
	ASSERT_MEMEQ(dst, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memcpy_rectflipv_uneven()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd', 'e',
						 'f', 'g', 'h', 'A', 'B',
						 'C', 'D', 'E', 'F', 'G' };

	uint8_t expect[] = { 'e', 'd', 'c', 'b', 'a',
			 	 	 	 'B', 'A', 'h', 'g', 'f',
			 	 	 	 'G', 'F', 'E', 'D', 'C' };

	uint8_t dst[sizeof(buffer)] = {0};
	memcpy_rectflipv( dst, buffer, 3, 5, 5, 5, sizeof(uint8_t) );
	ASSERT_MEMEQ(dst, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memcpy_rectflipv_subrect()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
						 'e', 'f', 'g', 'h',
						 'i', 'j', 'k', 'l',
						 'A', 'B', 'C', 'D',
						 'E', 'F', 'G', 'H' };

	uint8_t expect[] = { 'b', 'a', 0, 0,
						 'f', 'e', 0, 0,
						 'j', 'i', 0, 0,
						  0,   0,  0, 0,
						  0,   0,  0, 0 };

	uint8_t dst[sizeof(buffer)] = {0};
	memcpy_rectflipv( dst, buffer, 
                      3, // linecnt
                      2, // linelen
                      4, // dststride
                      4, // srcstride
					  sizeof(uint8_t) );
	ASSERT_MEMEQ(dst, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memmove_rectflipv_even()
{
    uint8_t buffer[] = { 'a', 'b', 'c', 'd',
					     'e', 'f', 'g', 'h',
						 'A', 'B', 'C', 'D',
						 'E', 'F', 'G', 'H' };

	uint8_t expect[] = { 'd', 'c', 'b', 'a',
		     	 	 	 'h', 'g', 'f', 'e',
		     	 	 	 'D', 'C', 'B', 'A',
		     	 	 	 'H', 'G', 'F', 'E' };

	memmove_rectflipv( buffer, buffer, 4, 4, 4, 4, sizeof(uint8_t) );
	ASSERT_MEMEQ(buffer, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memmove_rectflipv_uneven()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd', 'e',
						 'f', 'g', 'h', 'A', 'B',
						 'C', 'D', 'E', 'F', 'G' };

	uint8_t expect[] = { 'e', 'd', 'c', 'b', 'a',
			 	 	 	 'B', 'A', 'h', 'g', 'f',
			 	 	 	 'G', 'F', 'E', 'D', 'C' };

	memmove_rectflipv( buffer, buffer, 3, 5, 5, 5, sizeof(uint8_t) );
	ASSERT_MEMEQ(buffer, expect);

    return GREATEST_TEST_RES_PASS;
}

TEST memmove_rectflipv_subrect()
{
	uint8_t buffer[] = { 'a', 'b', 'c', 'd',
						 'e', 'f', 'g', 'h',
						 'i', 'j', 'k', 'l',
						 'A', 'B', 'C', 'D',
						 'E', 'F', 'G', 'H' };

	uint8_t expect[] = { 'b', 'a', 'c', 'd',
						 'f', 'e', 'g', 'h',
						 'j', 'i', 'k', 'l',
						 'A', 'B', 'C', 'D',
						 'E', 'F', 'G', 'H' };

	memmove_rectflipv( buffer, buffer, 
                       3, // linecnt
                       2, // linelen
                       4, // dststride
                       4, // srcstride
					   sizeof(uint8_t) );
	ASSERT_MEMEQ(buffer, expect);

    return GREATEST_TEST_RES_PASS;
}

// TODO: tests for some different sizes for memXXX_rectflipv

GREATEST_SUITE( swap )
{
	RUN_TEST( memswap_simple     );
	RUN_TEST( memswap_many_sizes );
}

GREATEST_SUITE( rect )
{
	RUN_TEST( memcpy_rect_simple );

    RUN_TEST( memmove_rect_simple             );
    RUN_TEST( memmove_rect_no_overlap_n       );
    RUN_TEST( memmove_rect_overlap_n          );
    RUN_TEST( memmove_rect_overlap_s          );
    RUN_TEST( memmove_rect_overlap_w          );
    RUN_TEST( memmove_rect_overlap_e          );
    RUN_TEST( memmove_rect_overlap_diagnal_nw );
    RUN_TEST( memmove_rect_overlap_diagnal_ne );
    RUN_TEST( memmove_rect_overlap_diagnal_sw );
    RUN_TEST( memmove_rect_overlap_diagnal_se );
};

GREATEST_SUITE( rectrotr )
{
    RUN_TEST( memcpy_rectrotr_full    );
    RUN_TEST( memcpy_rectrotr_big     );
    RUN_TEST( memcpy_rectrotr_simple  );
    RUN_TEST( memcpy_rectrotr_subrect );

    RUN_TEST( memmove_rectrotr_full   );
    RUN_TEST( memmove_rectrotr_big    );
};

GREATEST_SUITE( rectrotl )
{
    RUN_TEST( memcpy_rectrotl_full    );
    RUN_TEST( memcpy_rectrotl_simple  );
    RUN_TEST( memcpy_rectrotl_subrect );

    RUN_TEST( memmove_rectrotl_full   );
    RUN_TEST( memmove_rectrotl_big    );
};

GREATEST_SUITE( rectfliph )
{
    RUN_TEST( memcpy_rectfliph_even     );
    RUN_TEST( memcpy_rectfliph_uneven   );
    RUN_TEST( memcpy_rectfliph_subrect  );

    RUN_TEST( memmove_rectfliph_even    );
    RUN_TEST( memmove_rectfliph_uneven  );
    RUN_TEST( memmove_rectfliph_subrect );
};

GREATEST_SUITE( rectflipv )
{
    RUN_TEST( memcpy_rectflipv_even    );
    RUN_TEST( memcpy_rectflipv_uneven  );
    RUN_TEST( memcpy_rectflipv_subrect );

    RUN_TEST( memmove_rectflipv_even    );
    RUN_TEST( memmove_rectflipv_uneven  );
    RUN_TEST( memmove_rectflipv_subrect );
};

GREATEST_MAIN_DEFS();

int main( int argc, char **argv )
{
    GREATEST_MAIN_BEGIN();
	RUN_SUITE( swap );
    RUN_SUITE( rect );
    RUN_SUITE( rectrotr );
    RUN_SUITE( rectrotl );
    RUN_SUITE( rectfliph );
    RUN_SUITE( rectflipv );
    GREATEST_MAIN_END();
}
