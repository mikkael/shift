
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_DETAIL_BIT_MASK_HPP_
#define SHIFT_DETAIL_BIT_MASK_HPP_

#include <shift/types/byte.hpp>
#include <shift/exception.hpp>

namespace shift  { namespace detail {

template<unsigned int NumBits, typename T>
struct bit_mask_all_bits;

template<unsigned int NumBits, typename T>
struct bit_mask_all_bits       { static const T value =  ( T(1) << NumBits-1) + bit_mask_all_bits<NumBits-1, T>::value; };

template<typename T>
struct bit_mask_all_bits<1, T> { static const T value = 1; };

template<typename T>
struct bit_mask_all_bits<0, T> { static const T value = 0; };

struct bit_mask {
	static byte_type get(byte_type num_bits) {
		SHIFT_THROW_ON_INDEX_OUT_OF_RANGE(num_bits, 1, 9);
		static const byte_type bit_masks[8] = { 1, 3, 7, 15, 31, 63, 127, 255};
		return bit_masks[num_bits-1];
	}
};

}} // shift::detail

#endif /* SHIFT_DETAIL_BIT_MASK_HPP_ */
