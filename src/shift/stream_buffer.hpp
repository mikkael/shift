
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_STREAM_BUFFER_HPP_
#define SHIFT_STREAM_BUFFER_HPP_

#include <shift/concepts/endianness.hpp>
#include <shift/concepts/modes.hpp>
#include <shift/buffer_position.hpp>
#include <shift/detail/stream_operator_interface.hpp>
#include <shift/detail/endianness.hpp>

namespace shift {

template<endianness EncodingEndianness, typename Mode>
class stream_buffer {
public:

	typedef Mode mode;

	static endianness endianness_type()                { return EncodingEndianness; }
	static bool       requires_endianness_conversion() { static const bool result = ! (endianness_type() == detail::endianness_impl::system_endianness());
	                                                     return result;                                                                              }

protected:
	template<typename StreamType>
	friend struct detail::stream_operator_interface;

	stream_buffer() : current_position_(0, 7)          {}

	void set_position(const buffer_position& position) { current_position_ = position; }

	buffer_position current_position_;
};

} // shift

#endif /* SHIFT_STREAM_BUFFER_HPP_ */
