
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_DETAIL_VAR_UINT_HPP_
#define SHIFT_DETAIL_VAR_UINT_HPP_

#include <shift/exception.hpp>
#include <shift/types/byte.hpp>
#include <shift/detail/stream_operator_interface.hpp>

namespace shift { namespace detail {

// todo: add documentation: var uints are base 128

template<typename SinkType, typename UintType>
void write_var_uint(SinkType& sink, UintType X) {
	typedef shift::byte_type byte_type;
	do {
		byte_type encoded_byte = X % 128;
		X /= 128;
		if (X > 0)
			encoded_byte = encoded_byte | 128;
		ostream_operator_interface<SinkType>::write(sink, encoded_byte);
	} while (X > 0);
}

template<typename UintType>
UintType max_multiplier_impl() {
	UintType max_multiplier = 1;
	UintType max_value      = std::numeric_limits<UintType>::max() / 128;
	while (max_multiplier * 128 < max_value) max_multiplier *= 128;
	return max_multiplier;
}

template<typename UintType>
UintType max_multiplier() {
	static const UintType max_multiplier = max_multiplier_impl<UintType>();
	return max_multiplier;
}

template<typename SourceType, typename UintType>
void read_var_uint(SourceType& source, UintType& value) {
	typedef shift::byte_type byte_type;
	typedef UintType         value_type;
	value                   = 0;
	value_type multiplier   = 1;
	byte_type  encoded_byte = 0;

	do {
		encoded_byte = istream_operator_interface<SourceType>::get(source);
		value += (encoded_byte & 127) * multiplier;
		multiplier *= 128;
		if (multiplier > max_multiplier<UintType>())
			SHIFT_THROW(malformed_var_uint());
	} while ((encoded_byte & 128) != 0);
}

}} // shift::detail

#endif /* SHIFT_DETAIL_VAR_UINT_HPP_ */
