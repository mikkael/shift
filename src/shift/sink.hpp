
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_SINK_HPP_
#define SHIFT_SINK_HPP_

#include <shift/stream_buffer.hpp>
#include <shift/types/fixed_width_uint.hpp>
#include <shift/types/cstdint.hpp>
#include <shift/detail/stream_operator_interface.hpp>
#include <shift/detail/size_encoding.hpp>
#include <shift/detail/utility.hpp>
#include <shift/detail/endian_reversal.hpp>

namespace shift {

template<endianness EncodingEndianness, typename BufferType>
class sink : public stream_buffer<EncodingEndianness, output> {
public:

	typedef BufferType                                  buffer_type;
	typedef typename buffer_type::initialization_params initialization_params;

	sink(const initialization_params& init_params = initialization_params())
	: base_type()
	, size_()
	, buffer_(init_params)
	, block_write_func_(get_block_write_func())
	{
	}

	void clear() {
		buffer_.clear();
		base_type::current_position_.byte_index = 0;
		base_type::current_position_.bit_index  = 0;
		size_                                   = 0;
	}

	unsigned int size() const {
		return size_;
	}

	const byte_type* buffer() const {
		return buffer_.buffer();
	}

private:
	typedef stream_buffer<EncodingEndianness, output> base_type;

	template<typename SinkType>
	friend struct detail::ostream_operator_interface;

	typedef std::size_t (buffer_type::*block_write_func_type) (const byte_type*, std::size_t, std::size_t);

	static block_write_func_type get_block_write_func() {
		if (base_type::requires_endianness_conversion())
			return &buffer_type::reverse_write;
		else
			return &buffer_type::write;
	}

	static inline byte_type inverse_bits(byte_type v) {
		return ~v;
	}

	void write(const byte_type value) {
		base_type::current_position_.byte_index = buffer_.write(value, base_type::current_position_.byte_index);
		base_type::current_position_.bit_index  = 7;
		update_size();
	}

	template<typename IntType, unsigned int NumBits>
	void write(const fixed_width_uint<IntType, NumBits> v) {
		const IntType value = base_type::requires_endianness_conversion() ? detail::endian_reverse(*v) : *v;
		unsigned int n_bits = NumBits;
		while(n_bits > 0){
			if (n_bits > base_type::current_position_.bit_index+1) {
				const unsigned int n_bits_to_encode = base_type::current_position_.bit_index + 1;
				const unsigned int n_shifts         = n_bits - n_bits_to_encode;
				const byte_type byte_value          = (buffer_.at(base_type::current_position_.byte_index) & inverse_bits(detail::bit_mask::get(n_bits_to_encode))) | (value >> n_shifts);
				write(byte_value);
				n_bits -= n_bits_to_encode;
			} else {
				const unsigned int n_shifts = (base_type::current_position_.bit_index + 1) - n_bits;
				const byte_type byte_value  = (buffer_.at(base_type::current_position_.byte_index) & (inverse_bits(detail::bit_mask::get(n_bits) << n_shifts))) | (value << n_shifts);
				write(byte_value);
				n_bits = 0;
			}
			base_type::current_position_.bit_index  = 7;
		}
	}

	void write(bool value) {
		const byte_type byte_value = (buffer_.at(base_type::current_position_.byte_index) & inverse_bits(1 << base_type::current_position_.bit_index)) | ((value & 1) << base_type::current_position_.bit_index);
		write(byte_value);
	}

	/*
	 * considering endianness
	 */
	void write_block(const byte_type* p, const std::size_t n) {

		base_type::current_position_.byte_index = (buffer_.*block_write_func_)(p, base_type::current_position_.byte_index, n);
		base_type::current_position_.bit_index  = 7;
		update_size();
	}

	/*
	 * endianess is not considered
	 */
	void write_array(const byte_type* p, const std::size_t n) {
		base_type::current_position_.byte_index = buffer_.write(p, base_type::current_position_.byte_index, n);
		base_type::current_position_.bit_index  = 7;
		update_size();
	}

	void update_size() {
		size_ = base_type::current_position_.byte_index > size_ ? base_type::current_position_.byte_index : size_;
	}

	unsigned int          size_;
	buffer_type           buffer_;
	block_write_func_type block_write_func_;
};

template<endianness EncodingEndianness, typename BufferType>
sink<EncodingEndianness, BufferType>& operator << (sink<EncodingEndianness, BufferType>& sink, const buffer_position& pos) {
	typedef stream_buffer<EncodingEndianness, output> stream_type;
	detail::stream_operator_interface<stream_type>::set_position(sink, pos);
	return sink;
}

template<endianness EncodingEndianness, typename BufferType, typename IntType, unsigned int NumBits>
sink<EncodingEndianness, BufferType>& operator << (sink<EncodingEndianness, BufferType>& sink_, const fixed_width_uint<IntType, NumBits> value) {
	typedef sink<EncodingEndianness, BufferType> sink_type;
	return detail::ostream_operator_interface<sink_type>::write(sink_, value);
}

template<endianness EncodingEndianness, typename BufferType>
sink<EncodingEndianness, BufferType>& operator << (sink<EncodingEndianness, BufferType>& sink_, bool value) {
	typedef sink<EncodingEndianness, BufferType> sink_type;
	return detail::ostream_operator_interface<sink_type>::write(sink_, value);
}

template<endianness EncodingEndianness, typename BufferType>
sink<EncodingEndianness, BufferType>& operator << (sink<EncodingEndianness, BufferType>& sink_, const byte_type value) {
	typedef sink<EncodingEndianness, BufferType> sink_type;
	return detail::ostream_operator_interface<sink_type>::write(sink_, value);
}

#define DEFINE_SHIFT_INSERT_OPERATOR_BASIC_TYPE(type)                                                             \
	template<endianness EncodingEndianness, typename BufferType>                                                  \
	sink<EncodingEndianness, BufferType>& operator << (sink<EncodingEndianness, BufferType>& sink_, type value) { \
		typedef sink<EncodingEndianness, BufferType> sink_type;                                                   \
		detail::ostream_operator_interface<sink_type>::write_block( sink_                                         \
		                                                          , reinterpret_cast<const byte_type*>(&value)    \
		                                                          , sizeof(type));                                \
		return sink_;                                                                                             \
	}                                                                                                             \

DEFINE_SHIFT_INSERT_OPERATOR_BASIC_TYPE(shift::int8_t  )
DEFINE_SHIFT_INSERT_OPERATOR_BASIC_TYPE(shift::uint16_t)
DEFINE_SHIFT_INSERT_OPERATOR_BASIC_TYPE(shift::int16_t )
DEFINE_SHIFT_INSERT_OPERATOR_BASIC_TYPE(shift::uint32_t)
DEFINE_SHIFT_INSERT_OPERATOR_BASIC_TYPE(shift::int32_t )
DEFINE_SHIFT_INSERT_OPERATOR_BASIC_TYPE(shift::uint64_t)
DEFINE_SHIFT_INSERT_OPERATOR_BASIC_TYPE(shift::int64_t )
DEFINE_SHIFT_INSERT_OPERATOR_BASIC_TYPE(float          )
DEFINE_SHIFT_INSERT_OPERATOR_BASIC_TYPE(double         )

#undef DEFINE_SHIFT_INSERT_OPERATOR_BASIC_TYPE

} // shift

#endif /* SHIFT_SINK_HPP_ */
