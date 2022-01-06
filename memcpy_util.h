#pragma once

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

#include <string.h>

/**
 * swap memory in two buffers.
 *
 * @param ptr1 pointer to first buffer.
 * @param ptr2 pointer to second buffer.
 * @param bytes number of bytes to swap.
 */
inline void memswap( void* ptr1, void* ptr2, size_t bytes );

/**
 * copy rect.
 *
 * @note if dst and src overlap, this is undefined and will most likely not do what was expected.
 *
 * @param dst destination buffer where to start the copy
 * @param src source buffer to copy from.
 * @param lines number of lines to copy from src.
 * @param linelen number of bytes in lines to copy from src.
 * @param dststride number of bytes between each row in dst.
 * @param srcstride number of bytes between each row in src.
 *
 * src:             dst:
 * X---+-------+    +-----------+
 * |x  |       | -> |           |
 * |  o|       |    | Y---+     |
 * +---+       |    | |x  |     |
 * |           |    | |  o|     |
 * |           |    | +---+     |
 * +-----------+    +-----------+
 * <-srcstride->    <-dststride->
 *
 * X = src passed to function
 * Y = dst passed to function
 */
inline void* memcpy_rect( void* dst, void* src, size_t lines, size_t linelen, size_t dststride, size_t srcstride );

/**
 * copy rect.
 *
 * @note this is the same operation as memcpy_rect except this is safe where dst and src overlap.
 *
 * @param dst destination buffer where to start the copy
 * @param src source buffer to copy from.
 * @param lines number of lines to copy from src.
 * @param linelen number of bytes in lines to copy from src.
 * @param dststride number of bytes between each row in dst.
 * @param srcstride number of bytes between each row in src.
 *
 * src:             dst:
 * X---+-------+    +-----------+
 * |x  |       | -> |           |
 * |  o|       |    | Y---+     |
 * +---+       |    | |x  |     |
 * |           |    | |  o|     |
 * |           |    | +---+     |
 * +-----------+    +-----------+
 * <-srcstride->    <-dststride->
 *
 * X = src passed to function
 * Y = dst passed to function
 */
inline void* memmove_rect( void* dst, void* src, size_t lines, size_t linelen, size_t dststride, size_t srcstride );

/**
 * copy rect rotated right 90 deg.
 *
 * @param dst destination buffer where to start the copy
 * @param src source buffer to copy from.
 * @param lines number of lines to copy from src.
 * @param linelen number of bytes in lines to copy from src.
 * @param dststride number of bytes between each row in dst.
 * @param srcstride number of bytes between each row in src.
 *
 * src:             dst:
 * X---+-------+    +-----------+
 * |1 2|       | -> |           |
 * |3 4|       |    | Y---+     |
 * +---+       |    | |3 1|     |
 * |           |    | |4 2|     |
 * |           |    | +---+     |
 * +-----------+    +-----------+
 * <-srcstride->    <-dststride->
 *
 * X = src passed to function
 * Y = dst passed to function
 */
inline void* memcpy_rectrotr ( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride );
inline void* memmove_rectrotr( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride );

// TODO: better name
inline void* memmove_rectrotr_x( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride, size_t item_size );

/**
 * copy rect rotated left 90 deg.
 *
 * @param dst destination buffer where to start the copy
 * @param src source buffer to copy from.
 * @param lines number of lines to copy from src.
 * @param linelen number of bytes in lines to copy from src.
 * @param dststride number of bytes between each row in dst.
 * @param srcstride number of bytes between each row in src.
 *
 * src:             dst:
 * X---+-------+    +-----------+
 * |1 2|       | -> |           |
 * |3 4|       |    | Y---+     |
 * +---+       |    | |2 4|     |
 * |           |    | |1 3|     |
 * |           |    | +---+     |
 * +-----------+    +-----------+
 * <-srcstride->    <-dststride->
 *
 * X = src passed to function
 * Y = dst passed to function
 */
inline void* memcpy_rectrotl( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride );

inline void* memmove_rectrotl( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride );

// TODO: better name
// TODO: add unittest when src and dst overlap but is one byte of.
inline void* memmove_rectrotl_x( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride, size_t item_size );

/**
 * copy rect flipped horizontally.
 *
 * @note if dst and src overlap, this is undefined and will most likely not do what was expected.
 *
 * @param dst destination buffer where to start the copy
 * @param src source buffer to copy from.
 * @param lines number of lines to copy from src.
 * @param linelen number of bytes in lines to copy from src.
 * @param dststride number of bytes between each row in dst.
 * @param srcstride number of bytes between each row in src.
 *
 * src:             dst:
 * X---+-------+    +-----------+
 * |123|       | -> |           |
 * |456|       |    | Y---+     |
 * +---+       |    | |456|     |
 * |           |    | |123|     |
 * |           |    | +---+     |
 * +-----------+    +-----------+
 * <-srcstride->    <-dststride->
 *
 * X = src passed to function
 * Y = dst passed to function
 */
inline void* memcpy_rectfliph( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride );

/**
 * move rect flipped horizontally.
 *
 * @note this is the same operation as memcpy_rectfliph except this is safe where dst and src overlap.
 *
 * @param dst destination buffer where to start the copy
 * @param src source buffer to copy from.
 * @param lines number of lines to copy from src.
 * @param linelen number of bytes in lines to copy from src.
 * @param dststride number of bytes between each row in dst.
 * @param srcstride number of bytes between each row in src.
 *
 * src:             dst:
 * X---+-------+    +-----------+
 * |123|       | -> |           |
 * |456|       |    | Y---+     |
 * +---+       |    | |456|     |
 * |           |    | |123|     |
 * |           |    | +---+     |
 * +-----------+    +-----------+
 * <-srcstride->    <-dststride->
 *
 * X = src passed to function
 * Y = dst passed to function
 */
inline void* memmove_rectfliph( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride );

/**
 * copy rect flipped vertically.
 *
 * @note if dst and src overlap, this is undefined and will most likely not do what was expected.
 *
 * @param dst destination buffer where to start the copy
 * @param src source buffer to copy from.
 * @param lines number of lines to copy from src.
 * @param linelen number of bytes in lines to copy from src.
 * @param dststride number of bytes between each row in dst.
 * @param srcstride number of bytes between each row in src.
 *
 * src:             dst:
 * X---+-------+    +-----------+
 * |123|       | -> |           |
 * |456|       |    | Y---+     |
 * +---+       |    | |321|     |
 * |           |    | |654|     |
 * |           |    | +---+     |
 * +-----------+    +-----------+
 * <-srcstride->    <-dststride->
 *
 * X = src passed to function
 * Y = dst passed to function
 */
inline void* memcpy_rectflipv( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride );

/**
 * move rect flipped vertically.
 *
 * @note this is the same operation as memcpy_rectfliph except this is safe where dst and src overlap.
 *
 * @param dst destination buffer where to start the copy
 * @param src source buffer to copy from.
 * @param lines number of lines to copy from src.
 * @param linelen number of bytes in lines to copy from src.
 * @param dststride number of bytes between each row in dst.
 * @param srcstride number of bytes between each row in src.
 *
 * src:             dst:
 * X---+-------+    +-----------+
 * |123|       | -> |           |
 * |456|       |    | Y---+     |
 * +---+       |    | |321|     |
 * |           |    | |654|     |
 * |           |    | +---+     |
 * +-----------+    +-----------+
 * <-srcstride->    <-dststride->
 *
 * X = src passed to function
 * Y = dst passed to function
 */
inline void* memmove_rectflipv( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride );



///////////////////////////////////////////////////////
//                  Implementations                  //
///////////////////////////////////////////////////////

inline void memswap( void* ptr1, void* ptr2, size_t bytes )
{
	// TODO: last time I checked this generated really bad code on MSVC and doing a swap in
	//       'chunks' was a way better way to go for that compiler.

	char* s1 = (char*)ptr1;
	char* s2 = (char*)ptr2;
	for( size_t i = 0; i < bytes; ++i )
	{
		char tmp = s1[i];
		s1[i] = s2[i];
		s2[i] = tmp;
	}
}

inline void* memcpy_rect( void* dst, void* src, size_t lines, size_t linelen, size_t dststride, size_t srcstride )
{
	char* d = (char*)dst;
	char* s = (char*)src;

	for( size_t line = 0; line < lines; ++line )
		memcpy( d + line * dststride, s + line * srcstride, linelen );
	return dst;
}

inline void* memmove_rect( void* dst, void* src, size_t lines, size_t linelen, size_t dststride, size_t srcstride )
{
	char* d = (char*)dst;
	char* s = (char*)src;

	// TODO: what happen if dst/src have different strides and overlap?

	// ... if dst and src do not overlap, use memcpy_rect ...
	if( d < s - ( lines * srcstride ) || d > s + ( lines * srcstride ) )
		return memcpy_rect( dst, src, lines, linelen, dststride, srcstride );

	// ... if dst < src use memmove by line ...
	else if( d >= s + srcstride + linelen )
	{
		// ... memcpy by line, in this case there will be no intersection by line ...
		for( size_t line = 0; line < lines; ++line )
			memcpy( d + line * dststride, s + line * srcstride, linelen );
	}
	else if( d < s )
	{
		for( size_t line = 0; line < lines; ++line )
			memmove( d + line * dststride, s + line * srcstride, linelen );
	}
	else
	{
		// ... if none of the above fits, copy lines backwards ...
		for( int line = (int)lines - 1; line >= 0; --line )
			memmove( d + line * (int)dststride, s + line * (int)srcstride, linelen );
	}
	return dst;
}

inline void* memcpy_rectrotr( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride )
{
	char* d = (char*)dst;
	char* s = (char*)src;

	for( size_t line = 0; line < linecnt; ++line )
		for( size_t byte = 0; byte < linelen; ++byte )
			d[ ( linecnt - line ) + ( dststride * byte ) - 1 ] = s[ line * srcstride + byte ];

	return dst;
}

inline void* memmove_rectrotr( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride )
{
	(void)dststride; // TODO: not used, why?
	char* d = (char*)dst;
	char* s = (char*)src;
	size_t image_end = linecnt * srcstride - 1;
	for( size_t y = 0; y < linecnt / 2; ++y )
	{
		size_t subimage_start = y * srcstride + y;
		size_t subimage_end   = image_end - y * srcstride - y;
		for( size_t x = 0; x < linelen - 1; ++x )
		{
			size_t offset = ( x * srcstride ) + linelen - 1;
			size_t src_p1 = subimage_start + x;
			size_t src_p2 = subimage_start + offset;
			size_t src_p3 = subimage_end - x;
			size_t src_p4 = subimage_end - offset;

			char swap_p1 = s[src_p1];
			char swap_p2 = s[src_p2];
			char swap_p3 = s[src_p3];
			char swap_p4 = s[src_p4];

			d[src_p2] = swap_p1;
			d[src_p3] = swap_p2;
			d[src_p4] = swap_p3;
			d[src_p1] = swap_p4;
		}
		linelen -= 2;
	}

	return dst;
}

inline void* memmove_rectrotr_x( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride, size_t item_size )
{
	(void)dststride; // TODO: not used, why?

	char* d = (char*)dst;
	char* s = (char*)src;
	size_t image_end = linecnt * srcstride - 1;
	for( size_t y = 0; y < linecnt / 2; ++y )
	{
		size_t subimage_start = y * srcstride + y;
		size_t subimage_end   = image_end - y * srcstride - y;
		for( size_t x = 0; x < linelen - 1; ++x )
		{
			size_t offset = ( x * srcstride ) + linelen - 1;
			size_t src_p1 = subimage_start + x;
			size_t src_p2 = subimage_start + offset;
			size_t src_p3 = subimage_end - x;
			size_t src_p4 = subimage_end - offset;

			// TODO: handle bigger items than 16 bytes?
			char swap_p1[16];
			char swap_p2[16];
			char swap_p3[16];
			char swap_p4[16];
			memcpy( swap_p1, &s[src_p1 * item_size], item_size );
			memcpy( swap_p2, &s[src_p2 * item_size], item_size );
			memcpy( swap_p3, &s[src_p3 * item_size], item_size );
			memcpy( swap_p4, &s[src_p4 * item_size], item_size );

			memcpy( &d[src_p2 * item_size], swap_p1, item_size );
			memcpy( &d[src_p3 * item_size], swap_p2, item_size );
			memcpy( &d[src_p4 * item_size], swap_p3, item_size );
			memcpy( &d[src_p1 * item_size], swap_p4, item_size );
		}
		linelen -= 2;
	}

	return dst;
}

inline void* memcpy_rectrotl( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride )
{
	char* d = (char*)dst;
	char* s = (char*)src;

	for( size_t line = 0; line < linecnt; ++line )
		for( size_t byte = 0; byte < linelen; ++byte )
			d[ ( linelen - byte - 1 ) * dststride + line ] = s[ line * srcstride + byte ];

	return dst;
}

inline void* memmove_rectrotl( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride )
{
	(void)dststride; // TODO: not used, why?

	char* d = (char*)dst;
	char* s = (char*)src;
	size_t image_end = linecnt * srcstride - 1;
	for( size_t y = 0; y < linecnt / 2; ++y )
	{
		size_t subimage_start = y * srcstride + y;
		size_t subimage_end   = image_end - y * srcstride - y;
		for( size_t x = 0; x < linelen - 1; ++x )
		{
			size_t offset = ( x * srcstride ) + linelen - 1;
			size_t src_p1 = subimage_start + x;
			size_t src_p2 = subimage_start + offset;
			size_t src_p3 = subimage_end - x;
			size_t src_p4 = subimage_end - offset;

			char swap_p1 = s[src_p1];
			char swap_p2 = s[src_p2];
			char swap_p3 = s[src_p3];
			char swap_p4 = s[src_p4];

			d[src_p4] = swap_p1;
			d[src_p1] = swap_p2;
			d[src_p2] = swap_p3;
			d[src_p3] = swap_p4;
		}
		linelen -= 2;
	}

	return dst;
}

inline void* memmove_rectrotl_x( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride, size_t item_size )
{
	(void)dststride; // TODO: not used, why?

	char* d = (char*)dst;
	char* s = (char*)src;
	size_t image_end = linecnt * srcstride - 1;
	for( size_t y = 0; y < linecnt / 2; ++y )
	{
		size_t subimage_start = y * srcstride + y;
		size_t subimage_end   = image_end - y * srcstride - y;
		for( size_t x = 0; x < linelen - 1; ++x )
		{
			size_t offset = ( x * srcstride ) + linelen - 1;
			size_t src_p1 = subimage_start + x;
			size_t src_p2 = subimage_start + offset;
			size_t src_p3 = subimage_end - x;
			size_t src_p4 = subimage_end - offset;

			// TODO: handle bigger items than 16 bytes?
			char swap_p1[16];
			char swap_p2[16];
			char swap_p3[16];
			char swap_p4[16];
			memcpy( swap_p1, &s[src_p1 * item_size], item_size );
			memcpy( swap_p2, &s[src_p2 * item_size], item_size );
			memcpy( swap_p3, &s[src_p3 * item_size], item_size );
			memcpy( swap_p4, &s[src_p4 * item_size], item_size );

			memcpy( &d[src_p4 * item_size], swap_p1, item_size );
			memcpy( &d[src_p1 * item_size], swap_p2, item_size );
			memcpy( &d[src_p2 * item_size], swap_p3, item_size );
			memcpy( &d[src_p3 * item_size], swap_p4, item_size );
		}
		linelen -= 2;
	}

	return dst;
}

inline void* memcpy_rectfliph( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride )
{
	char* d = (char*)dst;
	char* s = (char*)src;
	for( size_t line = 0; line < linecnt; ++line )
		memcpy( d + ( linecnt - 1 - line ) * dststride, s + ( line * srcstride ), linelen );

	return dst;
}

inline void* memmove_rectfliph( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride )
{
	// TODO: it might be worth doing an overlap-check here and use memcpy_rectfliph if they don't overlap as that is WAY
	//       faster!
	char* d = (char*)dst;
	char* s = (char*)src;
	for( size_t line = 0; line < linecnt / 2; ++line )
		memswap( d + ( linecnt - 1 - line ) * dststride, s + ( line * srcstride ), linelen );

	return dst;
}

inline void* memcpy_rectflipv( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride )
{
	char* d = (char*)dst;
	char* s = (char*)src;
	for( size_t line = 0; line < linecnt; ++line )
	for( size_t byte = 0; byte < linelen; ++byte )
		d[ line * dststride + byte ] = s[ line * srcstride + ( linelen - byte ) - 1 ];
	return dst;
}

inline void* memmove_rectflipv( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride )
{
	char* d = (char*)dst;
	char* s = (char*)src;
	for( size_t line = 0; line < linecnt; ++line )
	for( size_t byte = 0; byte < linelen / 2; ++byte )
	{
		char* curr_d = d + line * dststride + byte;
		char* curr_s = s + line * srcstride + ( linelen - byte ) - 1;

		char tmp = *curr_d;
		*curr_d  = *curr_s;
		*curr_s  = tmp;
	}
	return dst;
}
