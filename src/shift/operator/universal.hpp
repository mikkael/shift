
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_OPERATOR_UNIVERSAL
#define SHIFT_OPERATOR_UNIVERSAL

#include <shift/source.hpp>
#include <shift/sink.hpp>

namespace shift {

template<endianness EncodingEndianness, typename BufferType, typename T>
sink<EncodingEndianness, BufferType>& operator %(sink<EncodingEndianness, BufferType>& sink_, const T& v) {
	return sink_ << v;
}

template<endianness EncodingEndianness, typename T>
source<EncodingEndianness>& operator %(source<EncodingEndianness>& source_, T& v) {
	return source_ >> v;
}

template<endianness EncodingEndianness, typename T>
source<EncodingEndianness>& operator %(source<EncodingEndianness>& source_, const T& v) {
	return source_ >> v;
}

} // shift

#endif /* SHIFT_OPERATOR_UNIVERSAL */
