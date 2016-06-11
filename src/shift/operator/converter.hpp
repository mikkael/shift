
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_OPERATOR_CONVERTER_HPP_
#define SHIFT_OPERATOR_CONVERTER_HPP_

#include <shift/source.hpp>
#include <shift/types/converter.hpp>

namespace shift {

template<endianness EncodingEndianness, typename BufferType, typename DecodedType, typename EncodedType>
sink<EncodingEndianness, BufferType>& operator << (sink<EncodingEndianness, BufferType>& sink_, const converter<DecodedType, EncodedType>& conv_functor) {
	sink_ << conv_functor.encode();
	return sink_;
}

template<endianness EncodingEndianness, typename DecodedType, typename EncodedType>
source<EncodingEndianness>& operator >> (source<EncodingEndianness>& source_, const converter<DecodedType, EncodedType>& conv_functor) {
	EncodedType tmp;
	source_ >> tmp;
	conv_functor.decode(tmp);
	return source_;
}

}

#endif /* SHIFT_OPERATOR_CONVERTER_HPP_ */
