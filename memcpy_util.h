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
#include <stdint.h>

// TODO:
// * missing memcpy_rectrot_180() that does a full rotate (better name needed!)
// * need versions taking item-size of all functions, maybe add item size on all ops
//   and specialize them as _1, _2, _4, _8 and _16?
// * go through doc!

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
// TODO: better doc, but this is the same as _rectrotr but with copied chunks being more than one byte, i.e. pixels in an image ;)
//       maybe the original rectrotr-functions that are just on bytes are quite useless as this kind of operation is usually done on
//       items with a size over 1 (read pixels)
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
 * @param linelen number of 'items' in lines to copy from src.
 * @param dststride number of 'items' between each row in dst.
 * @param srcstride number of 'items' between each row in src.
 * @param item_size size of 'atom' in a line in bytes.
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
inline void* memcpy_rectfliph( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride, size_t item_size );

/**
 * move rect flipped horizontally.
 *
 * @note this is the same operation as memcpy_rectfliph except this is safe where dst and src overlap.
 *
 * @param dst destination buffer where to start the copy
 * @param src source buffer to copy from.
 * @param lines number of lines to copy from src.
 * @param linelen number of 'items' in lines to copy from src.
 * @param dststride number of 'items' between each row in dst.
 * @param srcstride number of 'items' between each row in src.
 * @param item_size size of 'atom' in a line in bytes.
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
inline void* memmove_rectfliph( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride, size_t item_size );

/**
 * copy rect flipped vertically.
 *
 * @note if dst and src overlap, this is undefined and will most likely not do what was expected.
 *
 * @param dst destination buffer where to start the copy
 * @param src source buffer to copy from.
 * @param lines number of lines to copy from src.
 * @param linelen number of 'items' in lines to copy from src.
 * @param dststride number of 'items' between each row in dst.
 * @param srcstride number of 'items' between each row in src.
 * @param item_size size of 'atom' in a line in bytes.
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
inline void* memcpy_rectflipv( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride, size_t item_size );

/**
 * move rect flipped vertically.
 *
 * @note this is the same operation as memcpy_rectfliph except this is safe where dst and src overlap.
 *
 * @param dst destination buffer where to start the copy
 * @param src source buffer to copy from.
 * @param lines number of lines to copy from src.
 * @param linelen number of 'items' in lines to copy from src.
 * @param dststride number of 'items' between each row in dst.
 * @param srcstride number of 'items' between each row in src.
 * @param item_size size of 'atom' in a line in bytes.
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
inline void* memmove_rectflipv( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride, size_t item_size );



///////////////////////////////////////////////////////
//                  Implementations                  //
///////////////////////////////////////////////////////

inline void memswap( void* ptr1, void* ptr2, size_t bytes )
{
	// TODO: last time I checked this generated really bad code on MSVC and doing a swap in
	//       'chunks' was a way better way to go for that compiler.

	uint8_t* s1 = (uint8_t*)ptr1;
	uint8_t* s2 = (uint8_t*)ptr2;
	for( size_t i = 0; i < bytes; ++i )
	{
		uint8_t tmp = s1[i];
		s1[i] = s2[i];
		s2[i] = tmp;
	}
}

inline void* memcpy_rect( void* dst, void* src, size_t lines, size_t linelen, size_t dststride, size_t srcstride )
{
	uint8_t* d = (uint8_t*)dst;
	uint8_t* s = (uint8_t*)src;

	for( size_t line = 0; line < lines; ++line )
		memcpy( d + line * dststride, s + line * srcstride, linelen );
	return dst;
}

inline void* memmove_rect( void* dst, void* src, size_t lines, size_t linelen, size_t dststride, size_t srcstride )
{
	uint8_t* d = (uint8_t*)dst;
	uint8_t* s = (uint8_t*)src;

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
	uint8_t* d = (uint8_t*)dst;
	uint8_t* s = (uint8_t*)src;

	for( size_t line = 0; line < linecnt; ++line )
		for( size_t byte = 0; byte < linelen; ++byte )
			d[ ( linecnt - line ) + ( dststride * byte ) - 1 ] = s[ line * srcstride + byte ];

	return dst;
}

inline void* memmove_rectrotr( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride )
{
	(void)dststride; // TODO: not used, why?
	uint8_t* d = (uint8_t*)dst;
	uint8_t* s = (uint8_t*)src;
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

			uint8_t swap_p1 = s[src_p1];
			uint8_t swap_p2 = s[src_p2];
			uint8_t swap_p3 = s[src_p3];
			uint8_t swap_p4 = s[src_p4];

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

	uint8_t* d = (uint8_t*)dst;
	uint8_t* s = (uint8_t*)src;
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
			uint8_t swap_p1[16];
			uint8_t swap_p2[16];
			uint8_t swap_p3[16];
			uint8_t swap_p4[16];
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
	uint8_t* d = (uint8_t*)dst;
	uint8_t* s = (uint8_t*)src;

	for( size_t line = 0; line < linecnt; ++line )
		for( size_t byte = 0; byte < linelen; ++byte )
			d[ ( linelen - byte - 1 ) * dststride + line ] = s[ line * srcstride + byte ];

	return dst;
}

inline void* memmove_rectrotl( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride )
{
	(void)dststride; // TODO: not used, why?

	uint8_t* d = (uint8_t*)dst;
	uint8_t* s = (uint8_t*)src;
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

			uint8_t swap_p1 = s[src_p1];
			uint8_t swap_p2 = s[src_p2];
			uint8_t swap_p3 = s[src_p3];
			uint8_t swap_p4 = s[src_p4];

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

	uint8_t* d = (uint8_t*)dst;
	uint8_t* s = (uint8_t*)src;
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
			uint8_t swap_p1[16];
			uint8_t swap_p2[16];
			uint8_t swap_p3[16];
			uint8_t swap_p4[16];
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

inline void* memcpy_rectfliph( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride, size_t item_size )
{
	// TODO: this is slower than the memmove_-version in non-debug build as GCC inserts a call to memcpy instead of inlining it, making 
	//       memswap() the faster alternative!
	uint8_t* d = (uint8_t*)dst;
	uint8_t* s = (uint8_t*)src;
	const size_t dststride_bytes = dststride * item_size;
	const size_t srcstride_bytes = srcstride * item_size;
	const size_t linelen_bytes   = linelen   * item_size;
	for( size_t line = 0; line < linecnt; ++line )
		memcpy( d + ( linecnt - 1 - line ) * dststride_bytes, s + ( line * srcstride_bytes ), linelen_bytes );

	return dst;
}

inline void* memmove_rectfliph( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride, size_t item_size )
{
	// TODO: it might be worth doing an overlap-check here and use memcpy_rectfliph if they don't overlap as that is WAY
	//       faster!
	uint8_t* d = (uint8_t*)dst;
	uint8_t* s = (uint8_t*)src;
	const size_t dststride_bytes = dststride * item_size;
	const size_t srcstride_bytes = srcstride * item_size;
	const size_t linelen_bytes   = linelen   * item_size;
	for( size_t line = 0; line < linecnt / 2; ++line )
		memswap( d + ( linecnt - 1 - line ) * dststride_bytes, s + ( line * srcstride_bytes ), linelen_bytes );

	return dst;
}

inline void* memcpy_rectflipv( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride, size_t item_size )
{
	// TODO: handle alignment of buffers as the macro casts array to different types.

	#define MEMCPY_RECTFLIPV_GENERIC(type)                                                     \
		{                                                                                      \
			uint8_t* d = (uint8_t*)dst;                                                        \
			uint8_t* s = (uint8_t*)src;                                                        \
			for( size_t line = 0; line < linecnt; ++line )                                     \
			for( size_t item = 0; item < linelen; ++item )                                     \
				d[ line * dststride + item ] = s[ line * srcstride + ( linelen - item ) - 1 ]; \
		}

	switch(item_size)
	{
		case 1: MEMCPY_RECTFLIPV_GENERIC(uint8_t);  break;
		case 2: MEMCPY_RECTFLIPV_GENERIC(uint16_t); break;
		case 4: MEMCPY_RECTFLIPV_GENERIC(uint32_t); break;
		case 8: MEMCPY_RECTFLIPV_GENERIC(uint64_t); break;
		default:
		{
			uint8_t* d = (uint8_t*)dst;
			uint8_t* s = (uint8_t*)src;
			for( size_t line = 0; line < linecnt; ++line )
			for( size_t item = 0; item < linelen; ++item )
				d[ (line * dststride + item) * item_size ] = s[ (line * srcstride + (linelen - item - 1)) * item_size ];
		}
		break;
	}
	return dst;

	#undef MEMCPY_RECTFLIPV_GENERIC
}

inline void* memmove_rectflipv( void* dst, void* src, size_t linecnt, size_t linelen, size_t dststride, size_t srcstride, size_t item_size )
{
	// TODO: handle alignment of buffers as the macro casts array to different types.

	#define MEMMOVE_RECTFLIPV_GENERIC(type)                                   \
		{                                                                     \
			type* d = (type*)dst;                                             \
			type* s = (type*)src;                                             \
			for( size_t line = 0; line < linecnt; ++line )                    \
			for( size_t item = 0; item < linelen / 2; ++item )                \
			{                                                                 \
				type* curr_d = d + line * dststride + item;                   \
				type* curr_s = s + line * srcstride + ( linelen - item ) - 1; \
                                                                              \
				type tmp = *curr_d;                                           \
				*curr_d  = *curr_s;                                           \
				*curr_s  = tmp;                                               \
			}                                                                 \
		}

	switch(item_size)
	{
		case 1: MEMMOVE_RECTFLIPV_GENERIC(uint8_t);  break;
		case 2: MEMMOVE_RECTFLIPV_GENERIC(uint16_t); break;
		case 4: MEMMOVE_RECTFLIPV_GENERIC(uint32_t); break;
		case 8: MEMMOVE_RECTFLIPV_GENERIC(uint64_t); break;
		break;
		default:
		{
			uint8_t* d = (uint8_t*)dst;
			uint8_t* s = (uint8_t*)src;
			for( size_t line = 0; line < linecnt; ++line )
			for( size_t item = 0; item < linelen / 2; ++item )
			{
				uint8_t* curr_d = d + (line * dststride + item) * item_size;
				uint8_t* curr_s = s + (line * srcstride + (linelen - item - 1)) * item_size;

				memswap(curr_d, curr_s, item_size);
			}
		}
		break;
	}
	return dst;

	#undef MEMMOVE_RECTFLIPV_GENERIC
}
