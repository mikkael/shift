
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_BUFFER_VECTOR_HPP_
#define SHIFT_BUFFER_VECTOR_HPP_

#include <vector>
#include <shift/exception.hpp>
#include <shift/types/byte.hpp>
#include <shift/types/byte.hpp>
#include <shift/detail/utility.hpp>

namespace shift {

class vector {
public:

	typedef std::vector<byte_type> container_type;
	static const std::size_t default_capacity = 128;

	struct initialization_params {
		initialization_params(                    ) : capacity(default_capacity) {}
		initialization_params(std::size_t capacity) : capacity(capacity        ) {}
		std::size_t capacity;
	};

	explicit vector( initialization_params params = initialization_params(default_capacity))
	: initial_capacity(params.capacity   )
	, buffer_         (params.capacity, 0)
	{ }

	const byte_type* buffer() const {
		return &buffer_[0];
	}

	void clear() {
		buffer_.clear();
	}

	unsigned int size() const {
		return buffer_.size();
	}

	std::size_t write(byte_type value, std::size_t current_index) {
		resize_if_required(current_index);
		buffer_[current_index] = value;
		return ++current_index;
	}

	const byte_type& at(unsigned int current_index) {
		SHIFT_THROW_ON_INDEX_OUT_OF_RANGE(current_index, 0, buffer_.size());
		return buffer_.at(current_index);
	}

	std::size_t write(const byte_type* p, std::size_t current_index, std::size_t n) {
		resize_if_required(current_index + n);
		container_type::iterator it = buffer_.begin();
		std::advance(it, current_index);
		std::copy(p, p+n, it);
		return current_index + n;
	}

	std::size_t reverse_write(const byte_type* p, std::size_t current_index, std::size_t n) {
		resize_if_required(current_index + n);
		container_type::iterator it = buffer_.begin();
		std::advance(it, current_index);
		detail::reverse_copy(p, p+n, it);
		return current_index + n;
	}

private:

	void resize_if_required(unsigned int index) {
		if (index >= buffer_.size())
			buffer_.resize(std::max<unsigned int>(buffer_.size() + initial_capacity / 2, index + 1));
	}

	const std::size_t initial_capacity;
	container_type    buffer_;
};

} // shift

#endif /* SHIFT_BUFFER_VECTOR_HPP_ */
