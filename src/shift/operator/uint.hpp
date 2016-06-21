
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_OPERATOR_UINT_HPP_
#define SHIFT_OPERATOR_UINT_HPP_

#include <shift/sink.hpp>
#include <shift/source.hpp>
#include <shift/types/fixed_width_uint.hpp>

namespace shift {

template<typename UintType, unsigned int NBits>
struct ouint {
	ouint(const UintType& value) : value(value) {}
	const UintType& value;
};

template<endianness EncodingEndianness, typename BufferType, typename UintType, unsigned int NBits>
sink<EncodingEndianness, BufferType>& operator << (sink<EncodingEndianness, BufferType>& sink_, const ouint<UintType, NBits>& v) {
	return sink_ << fixed_width_uint<UintType, NBits>(v.value);
}

//

template<typename UintType, unsigned int NBits>
struct iuint {
	iuint(UintType& value) : value(value) {}
	UintType& value;
};

template<endianness EncodingEndianness, typename UintType, unsigned int NBits>
source<EncodingEndianness>& operator >> (source<EncodingEndianness>& source_, const iuint<UintType, NBits>& v) {
	fixed_width_uint<UintType, NBits> tmp;
	source_ >> tmp;
	v.value = *tmp;
	return source_;
}

//

template<unsigned int NBits, typename UintType>
inline ouint<UintType, NBits> uint(const UintType& ref) { return ouint<UintType, NBits>(ref); }

template<unsigned int NBits, typename UintType>
inline iuint<UintType, NBits> uint(      UintType& ref) { return iuint<UintType, NBits>(ref); }
}

#endif /* SHIFT_OPERATOR_UINT_HPP_ */
