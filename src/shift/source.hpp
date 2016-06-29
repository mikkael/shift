
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_SOURCE_HPP_
#define SHIFT_SOURCE_HPP_

#include <utility>

#include <shift/stream_buffer.hpp>
#include <shift/types/byte.hpp>
#include <shift/exception.hpp>
#include <shift/types/fixed_width_uint.hpp>
#include <shift/detail/stream_operator_interface.hpp>
#include <shift/detail/utility.hpp>
#include <shift/detail/endian_reversal.hpp>

namespace shift {

template<endianness EncodingEndianness>
class source : public stream_buffer<EncodingEndianness, input> {
public:

	source(const byte_type* buffer, unsigned int size )
	: buffer_(buffer)
	, size_(size)
	, block_read_func_(get_block_read_func())
	{}

	unsigned int size() const { return size_; }

private:

	typedef stream_buffer<EncodingEndianness, input> base_type;

	template<typename SourceType>
	friend struct detail::istream_operator_interface;

	typedef byte_type* (*block_read_func_type) (const byte_type*, const byte_type*, byte_type*);

	static block_read_func_type get_block_read_func() {
		if(base_type::requires_endianness_conversion())
			return &detail::reverse_copy<const byte_type*, byte_type*>;
		else
			return &std::copy<const byte_type*, byte_type*>;
	}

	byte_type get() {
		SHIFT_THROW_ON_INDEX_OUT_OF_RANGE(base_type::current_position_.byte_index, 0, size_);
		base_type::current_position_.bit_index = 7;
		return buffer_[base_type::current_position_.byte_index++];
	}

	void get(bool& value) {
		SHIFT_THROW_ON_INDEX_OUT_OF_RANGE(base_type::current_position_.byte_index, 0, size_);
		value = buffer_[base_type::current_position_.byte_index++] & (1 << base_type::current_position_.bit_index);
		base_type::current_position_.bit_index   = 7;
	}

	/*
	 * consider endianess
	 */
	void get_block(byte_type* data, unsigned int n_bytes) {
		SHIFT_THROW_ON_INDEX_OUT_OF_RANGE(base_type::current_position_.byte_index + n_bytes - 1, 0, size_);
		(*block_read_func_)( buffer_ + base_type::current_position_.byte_index
		                   , buffer_ + base_type::current_position_.byte_index + n_bytes
		                   , data);
		base_type::current_position_.byte_index += n_bytes;
		base_type::current_position_.bit_index   = 7;
	}

	/*
	 * endianess is not considered
	 */
	std::pair<const byte_type*, const byte_type*> get_array(unsigned int n_bytes) {
		SHIFT_THROW_ON_INDEX_OUT_OF_RANGE(base_type::current_position_.byte_index + n_bytes - 1, 0, size_);
		const unsigned int current_pos = base_type::current_position_.byte_index;
		base_type::current_position_.byte_index += n_bytes;
		base_type::current_position_.bit_index   = 7;
		return std::make_pair( buffer_ + current_pos
		                     , buffer_ + current_pos + n_bytes);
	}

	template<typename IntType, unsigned int NumBits>
	void get(fixed_width_uint<IntType, NumBits>& value){
		IntType result = 0;
		unsigned int n_bits = NumBits;
		while(n_bits > 0){
			if (n_bits > base_type::current_position_.bit_index + 1) {
				const unsigned int n_bits_to_decode = base_type::current_position_.bit_index + 1;
				const unsigned int n_shifts         = n_bits - n_bits_to_decode;
				result = result | static_cast<IntType>((get() & detail::bit_mask::get(n_bits_to_decode))) << n_shifts;
				n_bits -= n_bits_to_decode;
			} else {
				const unsigned int n_shifts = (base_type::current_position_.bit_index + 1) - n_bits;
				result = result | static_cast<IntType>(((get() >> n_shifts) & detail::bit_mask::get(n_bits)));
				n_bits  = 0;
			}
			base_type::current_position_.bit_index = 7;
		}
		value = base_type::requires_endianness_conversion() ? detail::endian_reverse(result) : result;
	}

	const byte_type*     buffer_;
	unsigned int         size_;
	block_read_func_type block_read_func_;
};

template<endianness EncodingEndianness>
source<EncodingEndianness>& operator >> (source<EncodingEndianness>& source_, const buffer_position& pos) {
	typedef stream_buffer<EncodingEndianness, input> stream_type;
	detail::stream_operator_interface<stream_type>::set_position(source_, pos);
	return source_;
}

template<endianness EncodingEndianness, typename IntType, unsigned int NumBits>
source<EncodingEndianness>& operator >> (source<EncodingEndianness>& source_, fixed_width_uint<IntType, NumBits>& value) {
	detail::istream_operator_interface<source<EncodingEndianness> >::get(source_, value);
	return source_;
}

template<endianness EncodingEndianness>
source<EncodingEndianness>& operator >> (source<EncodingEndianness>& source_, typename shift::byte_type& ref) {
	ref = detail::istream_operator_interface<source<EncodingEndianness> >::get(source_);
	return source_;
}

template<endianness EncodingEndianness>
source<EncodingEndianness>& operator >> (source<EncodingEndianness>& source_, bool& value) {
	detail::istream_operator_interface<source<EncodingEndianness> >::get(source_, value);
	return source_;
}

#define DEFINE_SHIFT_EXTRACT_OPERATOR_BASIC_TYPE(type)                                                                                     \
	template<endianness EncodingEndianness>                                                                                                \
	source<EncodingEndianness>& operator >> (source<EncodingEndianness>& source_, type& ref) {                                             \
	typedef shift::byte_type byte_type;                                                                                                    \
	detail::istream_operator_interface<source<EncodingEndianness> >::get_block(source_, reinterpret_cast<byte_type*>(&ref), sizeof(type)); \
	return source_;                                                                                                                        \
}

DEFINE_SHIFT_EXTRACT_OPERATOR_BASIC_TYPE(shift::int8_t  )
DEFINE_SHIFT_EXTRACT_OPERATOR_BASIC_TYPE(shift::uint16_t)
DEFINE_SHIFT_EXTRACT_OPERATOR_BASIC_TYPE(shift::int16_t )
DEFINE_SHIFT_EXTRACT_OPERATOR_BASIC_TYPE(shift::uint32_t)
DEFINE_SHIFT_EXTRACT_OPERATOR_BASIC_TYPE(shift::int32_t )
DEFINE_SHIFT_EXTRACT_OPERATOR_BASIC_TYPE(shift::uint64_t)
DEFINE_SHIFT_EXTRACT_OPERATOR_BASIC_TYPE(shift::int64_t )
DEFINE_SHIFT_EXTRACT_OPERATOR_BASIC_TYPE(float          )
DEFINE_SHIFT_EXTRACT_OPERATOR_BASIC_TYPE(double         )

#undef DEFINE_SHIFT_EXTRACT_OPERATOR_BASIC_TYPE

} // shift

#endif /* SHIFT_SOURCE_HPP_ */
