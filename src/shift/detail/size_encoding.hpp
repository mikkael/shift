
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_DETAIL_SIZE_ENCODING_HPP_
#define SHIFT_DETAIL_SIZE_ENCODING_HPP_

#include <shift/concepts/size_tags.hpp>
#include <shift/detail/var_uint.hpp>

namespace shift { namespace detail {

template<typename SizeType>
struct size_encoder {
	template<typename SinkType>
	static unsigned int encode(SinkType& sink, unsigned int size) {
		const SizeType length = size;
		sink << length;
		return size;
	}
};

template<>
struct size_encoder<variable_length> {
	template<typename SinkType>
	static unsigned int encode(SinkType& sink, unsigned int size) {
		detail::write_var_uint(sink, size);
		return size;
	}
};

template<>
struct size_encoder<no_size_field> {
	template<typename SinkType>
	static unsigned int encode(SinkType& sink, unsigned int size) {
		return size;
	}
};

// size_decoder

template<typename SizeType>
struct size_decoder {
	template<typename SourceType>
	static unsigned int decode_size(SourceType& source) {
		SizeType size = 0;
		source >> size;
		return size;
	}
};

template<>
struct size_decoder<variable_length> {
	template<typename SourceType>
	static unsigned int decode_size(SourceType& source) {
		unsigned int size = 0;
		detail::read_var_uint(source, size);
		return size;
	}
};

}} // shift::detail

#endif /* SHIFT_DETAIL_SIZE_ENCODING_HPP_ */
