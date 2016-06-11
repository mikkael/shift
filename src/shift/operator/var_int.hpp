
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_OPERATOR_VAR_INT_HPP_
#define SHIFT_OPERATOR_VAR_INT_HPP_

#include <shift/source.hpp>
#include <shift/sink.hpp>
#include <shift/detail/var_int.hpp>
#include <shift/types/var_int.hpp>

namespace shift {

// var_int

template<endianness EncodingEndianness, typename BufferType, typename IntType>
sink<EncodingEndianness, BufferType>& operator << (sink<EncodingEndianness, BufferType>& sink_, const var_int<IntType> value) {
	detail::write_var_int(sink_, *value);
	return sink_;
}

template<endianness EncodingEndianness, typename IntType>
source<EncodingEndianness>& operator >> (source<EncodingEndianness>& source_, var_int<IntType>& value) {
	detail::read_var_int(source_, *value);
	return source_;
}

// var_int_2

template<typename IntType>
class var_int_2;

template<endianness EncodingEndianness, typename BufferType, typename IntType>
sink<EncodingEndianness, BufferType>& operator << (sink<EncodingEndianness, BufferType>& sink_, const var_int_2<IntType> v) {
	detail::write_var_int(sink_, v.value);
	return sink_;
}

template<endianness EncodingEndianness, typename IntType>
source<EncodingEndianness>& operator >> (source<EncodingEndianness>& source_, var_int_2<IntType>& v) {
	detail::read_var_int(source_, v.value);
	return source_;
}

}

#endif /* SHIFT_OPERATOR_VAR_INT_HPP_ */
