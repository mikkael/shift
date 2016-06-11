
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_STREAM_OPERATOR_INTERFACE_HPP_
#define SHIFT_STREAM_OPERATOR_INTERFACE_HPP_

#include <utility>

#include <shift/types/byte.hpp>
#include <shift/concepts/endianness.hpp>
#include <shift/buffer_position.hpp>
#include <shift/types/fixed_width_uint.hpp>

namespace shift { namespace detail {

template<typename StreamType>
struct stream_operator_interface {
	typedef StreamType stream_type;
	inline static void set_position(stream_type& stream, const buffer_position& position) {
		stream.set_position(position);
	}
};

template<typename SinkType>
struct ostream_operator_interface {

	typedef SinkType sink_type;
	typedef shift::byte_type byte_type;

	template<typename IntType, unsigned int NumBits>
	inline static sink_type& write(sink_type& sink, const fixed_width_uint<IntType, NumBits> value) {
		sink.write(value);
		return sink;
	}

	inline static sink_type& write(sink_type& sink, const byte_type value) {
		sink.write(value);
		return sink;
	}

	inline static sink_type& write(sink_type& sink, bool value) {
		sink.write(value);
		return sink;
	}

	inline static sink_type& write_block(sink_type& sink, const byte_type* p, const std::size_t n) {
		sink.write_block(p, n);
		return sink;
	}

	inline static sink_type& write_array(sink_type& sink, const byte_type* p, const std::size_t n) {
		sink.write_array(p, n);
		return sink;
	}
};

template<typename SourceType>
struct istream_operator_interface {

	typedef SourceType source_type;
	typedef shift::byte_type byte_type;

	template<typename IntType, unsigned int NumBits>
	inline static void get(source_type& source, fixed_width_uint<IntType, NumBits>& value) {
		source.get(value);
	}

	inline static byte_type get(source_type& source) {
		return source.get();
	}

	inline static void get(source_type& source, bool& value) {
		source.get(value);
	}

	inline static void get_block(source_type& source, byte_type* data, unsigned int n_bytes) {
		source.get_block(data, n_bytes);
	}

	inline static std::pair<const byte_type*, const byte_type*> get_array(source_type& source, unsigned int n_bytes) {
		return source.get_array(n_bytes);
	}
};

} } // shift::detail

#endif /* SHIFT_STREAM_OPERATOR_INTERFACE_HPP_ */
