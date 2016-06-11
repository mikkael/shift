
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_BUFFER_BUFFER_INTERFACE_HPP_
#define SHIFT_BUFFER_BUFFER_INTERFACE_HPP_

#include <cstring>
#include <shift/types/byte.hpp>
#include <shift/exception.hpp>
#include <shift/detail/utility.hpp>

namespace shift {

class buffer_interface {
public:

	struct initialization_params {
		initialization_params(                                      ) : p_buffer(NULL    ), size(0   ) {}
		initialization_params(byte_type* p_buffer, unsigned int size) : p_buffer(p_buffer), size(size) {}
		byte_type*   p_buffer;
		unsigned int size;
	};

	explicit buffer_interface(initialization_params parameters = initialization_params())
	: buffer_(parameters.p_buffer)
	, size_(parameters.size)
	{
		if (buffer_ == NULL) SHIFT_THROW(not_initialized("buffer_interface::buffer_interface() : NULL pointer passed as buffer"));
		clear();
	}

	const unsigned size() const {
		return size_;
	}

	const byte_type* buffer() const {
		return buffer_;
	}

	void clear() {
		std::memset(reinterpret_cast<void*>(buffer_), 0, size_);
	}

	const byte_type& at(unsigned int current_index) const {
		SHIFT_THROW_ON_INDEX_OUT_OF_RANGE(current_index, 0, size_);
		return buffer_[current_index];
	}

	std::size_t write(byte_type value, unsigned int current_index) {
		SHIFT_THROW_ON_INDEX_OUT_OF_RANGE(current_index, 0, size_);
		buffer_[current_index] = value;
		return ++current_index;
	}

	std::size_t write(const byte_type* p, std::size_t current_index, std::size_t n) {
		SHIFT_THROW_ON_INDEX_OUT_OF_RANGE(current_index + n - 1, 0, size_);
		std::memcpy(&buffer_[current_index], p, n);
		return current_index + n;
	}

	std::size_t reverse_write(const byte_type* p, std::size_t current_index, std::size_t n) {
		SHIFT_THROW_ON_INDEX_OUT_OF_RANGE(current_index + n - 1, 0, size_);
		detail::reverse_copy(p, p+n, buffer_+current_index);
		return current_index + n;
	}

private:
	byte_type* buffer_;
	const unsigned int size_;
};

} // shift

#endif /* SHIFT_BUFFER_BUFFER_INTERFACE_HPP_ */
