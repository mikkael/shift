
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

// var_width_int

template<typename IntType>
struct ovar_int {
	ovar_int(const IntType& value) : value(value){}
	const IntType& value;
};

template<typename IntType>
struct ivar_int{
	ivar_int(IntType& value) : value(value){}
	IntType& value;
};

template<endianness EncodingEndianness, typename BufferType, typename IntType>
sink<EncodingEndianness, BufferType>& operator << (sink<EncodingEndianness, BufferType>& sink_, const ovar_int<IntType>& v) {
	detail::write_var_int(sink_, v.value);
	return sink_;
}

template<endianness EncodingEndianness, typename IntType>
source<EncodingEndianness>& operator >> (source<EncodingEndianness>& source_, const ivar_int<IntType>& v) {
	detail::read_var_int(source_, v.value);
	return source_;
}

template<typename IntType>
inline ovar_int<IntType> var_width_int(const IntType& ref) { return ovar_int<IntType>(ref); }

template<typename IntType>
inline ivar_int<IntType> var_width_int(      IntType& ref) { return ivar_int<IntType>(ref); }

}

#endif /* SHIFT_OPERATOR_VAR_INT_HPP_ */
