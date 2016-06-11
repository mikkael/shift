
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_DETAIL_VAR_INT_ENCODING_HPP_
#define SHIFT_DETAIL_VAR_INT_ENCODING_HPP_

#include <shift/types/byte.hpp>
#include <shift/detail/var_uint.hpp>
#include <shift/detail/zig_zag_coding.hpp>
#include <shift/detail/type_traits.hpp>

namespace shift {
namespace detail {

template<bool WithZigZagEncoding>
struct var_int_encoding;

template<>
struct var_int_encoding<true> {
	template<typename SinkType, typename IntType>
	static void write_var_int(SinkType& sink, IntType value) {
		typedef typename zig_zag_encoding_type_traits<IntType>::unsigned_type uint_type;
		detail::write_var_uint(sink, zig_zag_encode<uint_type>(value));
	}
	template<typename SourceType, typename IntType>
	static void read_var_int(SourceType& source, IntType& value) {
		typedef typename zig_zag_encoding_type_traits<IntType>::unsigned_type uint_type;
		uint_type tmp;
		detail::read_var_uint(source, tmp);
		value = zig_zag_decode<IntType>(tmp);
	}
};

template<>
struct var_int_encoding<false> {
	template<typename SinkType, typename IntType>
	static void write_var_int(SinkType& sink, IntType value) {
		detail::write_var_uint(sink, value);
	}
	template<typename SourceType, typename IntType>
	static void read_var_int(SourceType& source, IntType& value) {
		detail::read_var_uint(source, value);
	}
};

template<typename SinkType, typename IntType>
inline void write_var_int(SinkType& sink, IntType value) {
	var_int_encoding<detail::is_signed_integral<IntType>::value >::write_var_int(sink, value);
}

template<typename SourceType, typename IntType>
inline void read_var_int(SourceType& source, IntType& value) {
	detail::var_int_encoding<detail::is_signed_integral<IntType>::value >::read_var_int(source, value);
}

}} // shift::detail

#endif /* SHIFT_DETAIL_VAR_INT_ENCODING_HPP_ */
