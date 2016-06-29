
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_DETAIL_ENDIAN_REVERSAL_HPP_
#define SHIFT_DETAIL_ENDIAN_REVERSAL_HPP_

#include <shift/types/cstdint.hpp>

namespace shift {
namespace detail {

// uint8

inline shift::uint8_t endian_reverse(shift::uint8_t v) {
	return v;
}

// uint16

inline shift::uint16_t endian_reverse(shift::uint16_t v) {
	return (v >> 8) | (v << 8);
}

// uint32

inline shift::uint32_t endian_reverse(shift::uint32_t v) {
	return  v               << 24 |  v               >> 24 |
	       (v & 0x0000FF00) << 8  | (v & 0x00FF0000) >> 8;
}

// uint64

inline shift::uint64_t endian_reverse(shift::uint64_t v) {
	return  v                          << 56 |  v                          >> 56 |
	       (v & 0x000000000000FF00ULL) << 40 | (v & 0x00FF000000000000ULL) >> 40 |
	       (v & 0x0000000000FF0000ULL) << 24 | (v & 0x0000FF0000000000ULL) >> 24 |
	       (v & 0x00000000FF000000ULL) << 8  | (v & 0x000000FF00000000ULL) >> 8;
}

}} // shift::detail

#endif /* SHIFT_DETAIL_ENDIAN_REVERSAL_HPP_ */
