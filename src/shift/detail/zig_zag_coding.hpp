
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_DETAIL_ZIG_ZAG_CODING_HPP_
#define SHIFT_DETAIL_ZIG_ZAG_CODING_HPP_

namespace shift { namespace detail {

/*
 * using zig zag encoding for variable width signed(!) integers: to make sure that small negative values are
 * also encoded with few bytes, encoding does zig zag forth and back between negative and positive values.
 *
 * +-------------------+------------+
 * | original (signed) | encoded as |
 * +-------------------+------------+
 * |                0  |         0  |
 * |               -1  |         1  |
 * |                1  |         2  |
 * |               -2  |         3  |
 * |                2  |         4  |
 * |               -3  |         5  |
 * |                3  |         6  |
 * |       2147483647  | 4294967294 |
 * |      -2147483648  | 4294967295 |
 * +-------------------+------------+
 */

template<typename UintType, typename IntType>
UintType zig_zag_encode(IntType value) {
	return (value << 1) ^ (value >> (sizeof(IntType) * CHAR_BIT - 1));
}

template<typename IntType, typename UintType>
IntType zig_zag_decode(UintType value) {
	return (value & 1) ? -((value >> 1) + 1) : value >> 1;
}

template<typename SignedIntType> struct zig_zag_encoding_type_traits;
template<> struct zig_zag_encoding_type_traits<char     > { typedef unsigned char      unsigned_type; };
template<> struct zig_zag_encoding_type_traits<short    > { typedef unsigned short     unsigned_type; };
template<> struct zig_zag_encoding_type_traits<int      > { typedef unsigned int       unsigned_type; };
template<> struct zig_zag_encoding_type_traits<long     > { typedef unsigned long      unsigned_type; };
template<> struct zig_zag_encoding_type_traits<long long> { typedef unsigned long long unsigned_type; };

}} // shift::detail

#endif /* SHIFT_DETAIL_ZIG_ZAG_CODING_HPP_ */
