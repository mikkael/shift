
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_BUFFER_POSITION_HPP_
#define SHIFT_BUFFER_POSITION_HPP_

#include <cstddef>

namespace shift {

struct buffer_position {
	         buffer_position(                                 ) : byte_index(    ), bit_index(   ) {}
	explicit buffer_position(std::size_t byte                 ) : byte_index(byte), bit_index(   ) {}
	         buffer_position(std::size_t byte, std::size_t bit) : byte_index(byte), bit_index(bit) {}
	std::size_t byte_index;
	std::size_t bit_index;
};

typedef buffer_position pos;

}

#endif /* SHIFT_BUFFER_POSITION_HPP_ */
