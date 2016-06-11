#include <test/utility.hpp>

#include <iostream>

namespace test { namespace detail {

//	template<shift::endianness Endianness>
//	void check_buffer_content(const unsigned int pos, const std::vector<shift::byte_type>& bytes, const shift::byte_type* buffer);

	template<>
	void check_buffer_content<shift::little_endian>(const unsigned int pos, const std::vector<shift::byte_type>& bytes, const shift::byte_type* buffer) {
		const unsigned int n_bytes = bytes.size();
		for (unsigned int i=0; i<n_bytes; ++i)
			CHECK(buffer[pos + i] == bytes[n_bytes-1-i]);
	}

	template<>
	void check_buffer_content<shift::big_endian>(const unsigned int pos, const std::vector<shift::byte_type>& bytes, const shift::byte_type* buffer) {
		const unsigned int n_bytes = bytes.size();
		for (unsigned int i=0; i<n_bytes; ++i)
			CHECK(buffer[pos + i] == bytes[i]);
	}

	template<>
	void copy_to_buffer<shift::little_endian>(unsigned int pos, const std::vector<shift::byte_type>& bytes, shift::byte_type* buffer) {
		for (unsigned int i=0, n_bytes = bytes.size(); i<n_bytes; ++i)
			buffer[pos + i] = bytes[n_bytes - i - 1];
	}

	template<>
	void copy_to_buffer<shift::big_endian>(unsigned int pos, const std::vector<shift::byte_type>& bytes, shift::byte_type* buffer) {
		std::copy(bytes.begin(), bytes.end(), buffer + pos);
	}

} // detail

void check_all_zero_except(const shift::uint8_t* buffer, const unsigned int size, unsigned int non_zero_start, unsigned int non_zero_end) {
	for (unsigned int i=0; i<size; ++i) {
		if (i>=non_zero_start && i <= non_zero_end)
			CHECK(buffer[i] != 0);
		else
			CHECK(buffer[i] == 0);
	}
}

void check_all_zero(const shift::uint8_t* buffer, const unsigned int size) {
	for (unsigned int i=0; i<size; ++i)
		CHECK(buffer[i] == 0);
}


} // test
