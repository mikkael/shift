
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef SHIFT_BUFFER_STATIC_BUFFER_HPP_
#define SHIFT_BUFFER_STATIC_BUFFER_HPP_

#include <shift/types/byte.hpp>
#include <shift/buffer/buffer_interface.hpp>

namespace shift {

template<unsigned int Size>
class static_buffer : public buffer_interface {
public:

	static const unsigned int size = Size;

	struct initialization_params {};

	static_buffer(initialization_params params = initialization_params())
	: buffer_interface(buffer_interface::initialization_params(buffer_, size))
	{
	}

private:
	byte_type buffer_[Size];
};

} // shift

#endif /* SHIFT_BUFFER_STATIC_BUFFER_HPP_ */
