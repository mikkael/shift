
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_DETAIL_ENDIANNESS_HPP_
#define SHIFT_DETAIL_ENDIANNESS_HPP_

#include <shift/types/cstdint.hpp>
#include <shift/concepts/endianness.hpp>

namespace shift { namespace detail {

struct endianness_impl {

	static bool is_little_endian() {
		static const bool is_little_endian_ = is_little_endian_impl();
		return is_little_endian_;
	}

	static bool is_big_endian() {
		static const bool is_big_endian_ = is_big_endian_impl();
		return is_big_endian_;
	}

	static endianness system_endianness() {
		static const endianness sys_endianness = is_little_endian() ? little_endian : big_endian;
		return sys_endianness;
	}

private:

	static bool is_little_endian_impl() {
		uint16_t v = 256;
		uint8_t* p = reinterpret_cast<uint8_t*>(&v);
		return p[1] == 1;
	}

	static bool is_big_endian_impl() {
		uint16_t v = 1;
		uint8_t* p = reinterpret_cast<uint8_t*>(&v);
		return p[1] == 1;
	}
};

}} // shift::detail

#endif /* SHIFT_DETAIL_ENDIANNESS_HPP_ */
