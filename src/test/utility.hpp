#ifndef SHIFT_TEST_UTILITY_HPP_
#define SHIFT_TEST_UTILITY_HPP_

#include <cstring>
#include <iostream>
#include <vector>

#include <catch.hpp>

#include <shift/buffer/static_buffer.hpp>
#include <shift/sink.hpp>
#include <shift/source.hpp>
#include <shift/operator/universal.hpp>

namespace test {
namespace detail {

	template<typename T>
	struct bitify_type;

	template<> struct bitify_type<shift::uint8_t > { typedef shift::uint8_t  type; };
	template<> struct bitify_type<shift::uint16_t> { typedef shift::uint16_t type; };
	template<> struct bitify_type<shift::uint32_t> { typedef shift::uint32_t type; };
	template<> struct bitify_type<shift::uint64_t> { typedef shift::uint64_t type; };
	template<> struct bitify_type<shift::int8_t  > { typedef shift::uint8_t  type; };
	template<> struct bitify_type<shift::int16_t > { typedef shift::uint16_t type; };
	template<> struct bitify_type<shift::int32_t > { typedef shift::uint32_t type; };
	template<> struct bitify_type<shift::int64_t > { typedef shift::uint64_t type; };
	template<> struct bitify_type<float          > { typedef shift::uint32_t type; };
	template<> struct bitify_type<double         > { typedef shift::uint64_t type; };

	/*
	 * returns a vector of bytes, where the MSB is at index 0 in the vector and the
	 * LSB is at the last index.
	 */
	template<typename UintType>
	std::vector<shift::byte_type> to_bytes(UintType value) {
		std::vector<shift::byte_type> bytes;
		const unsigned int n_bytes = sizeof(UintType);
		for (unsigned int i=n_bytes; i>0; --i)
			bytes.push_back( static_cast<shift::uint8_t>(value >> (i-1)*8 ) );
		return bytes;
	}

	template<shift::endianness Endianness>
	void check_buffer_content                      (const unsigned int pos, const std::vector<shift::byte_type>& bytes, const shift::byte_type* buffer);

	template<>
	void check_buffer_content<shift::little_endian>(const unsigned int pos, const std::vector<shift::byte_type>& bytes, const shift::byte_type* buffer);

	template<>
	void check_buffer_content<shift::big_endian   >(const unsigned int pos, const std::vector<shift::byte_type>& bytes, const shift::byte_type* buffer);

	template<typename ValueType, shift::endianness Endianness>
	void check_encoded_content(ValueType value, const shift::byte_type* buffer, unsigned int pos) {
		typedef typename detail::bitify_type<ValueType>::type bitify_type;
		const bitify_type tmp = *reinterpret_cast<const bitify_type*>(&value);
		const std::vector<shift::uint8_t> bytes = test::detail::to_bytes<bitify_type>(tmp);
		detail::check_buffer_content<Endianness>(pos, bytes, buffer);
	}


	template<typename T, typename BitifyType, shift::endianness Endianness, unsigned int BufferSize>
	void test_stream_out_operators(const T value, const unsigned int position) {
		typedef shift::static_buffer<BufferSize>      buffer_type;
		typedef shift::sink <Endianness, buffer_type> sink_type;

		const unsigned int n_bytes = sizeof(T);
		const BitifyType tmp = *reinterpret_cast<const BitifyType*>(&value);
		const std::vector<shift::byte_type> bytes = to_bytes(tmp);

		try {
			sink_type sink;
			sink << shift::buffer_position(position) << value;
			CHECK(sink.size() == position + bytes.size());
			detail::check_buffer_content<Endianness>(position, bytes, sink.buffer());
		} catch (shift::out_of_range& e) {
			CHECK(false);
			std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
		}

		try {
			sink_type sink;
			sink % shift::buffer_position(position) % value;
			CHECK(sink.size() == position + bytes.size());
			detail::check_buffer_content<Endianness>(position, bytes, sink.buffer());
		} catch (shift::out_of_range& e) {
			CHECK(false);
			std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
		}
	}


	template<shift::endianness Endianness>
	void copy_to_buffer                      (unsigned int pos, const std::vector<shift::byte_type>& bytes, shift::byte_type* buffer);

	template<>
	void copy_to_buffer<shift::little_endian>(unsigned int pos, const std::vector<shift::byte_type>& bytes, shift::byte_type* buffer);

	template<>
	void copy_to_buffer<shift::big_endian   >(unsigned int pos, const std::vector<shift::byte_type>& bytes, shift::byte_type* buffer);

	template<typename T, typename BitifyType, shift::endianness Endianness, unsigned int BufferSize>
	void test_stream_in_operators(const T value, const unsigned int position){
		typedef shift::source <Endianness> source_type;
		uint8_t buffer[BufferSize];
		std::memset(buffer, 0, BufferSize);

		const BitifyType tmp = *reinterpret_cast<const BitifyType*>(&value);
		const std::vector<shift::byte_type> bytes = to_bytes(tmp);
		copy_to_buffer<Endianness>(position, bytes, buffer);
		source_type source(buffer, BufferSize);

		try {
			T result;
			source >> shift::buffer_position(position) >> result;
			CHECK( result == value);
		} catch (shift::out_of_range& e) {
			CHECK(false);
			std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
		}

		try {
			T result;
			source % shift::buffer_position(position) % result;
			CHECK( result == value);
		} catch (shift::out_of_range& e) {
			CHECK(false);
			std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
		}
	}

} // detail

void check_all_zero_except(const shift::uint8_t* buffer, const unsigned int size, unsigned int non_zero_start, unsigned int non_zero_end);

void check_all_zero(const shift::uint8_t* buffer, const unsigned int size);


template<typename T>
void test_only_specified_location_in_buffer_is_modified_on_stream_out(T value){
	const unsigned int buffer_capacity = 64;
	typedef shift::static_buffer<buffer_capacity>            buffer_type;
	typedef shift::sink  <shift::little_endian, buffer_type> sink_type;

	const unsigned int n_bytes = sizeof(T);

	sink_type sink;
	const unsigned int pos = 30;
	sink << shift::buffer_position(pos) << value;

	check_all_zero_except(sink.buffer(), buffer_capacity, pos, pos + n_bytes-1);

	CHECK(sink.size() == pos + n_bytes);

	const shift::byte_type* bytes = reinterpret_cast<const shift::byte_type*>(&value);

	for (unsigned int i=0; i<n_bytes; ++i)
		CHECK(sink.buffer()[pos + i] == bytes[i]);
//		CHECK(sink.buffer()[pos + i] == static_cast<shift::uint8_t>(value >> i*8 ));
}

//template<typename UintType>
//void test_encoding_with_endianness(const UintType value, const unsigned int pos) {
//	CAPTURE(value);
//
//	const unsigned int n_bytes = sizeof(UintType);
//	const std::vector<shift::byte_type> bytes = detail::to_bytes(value);
//
//	const unsigned int buffer_capacity = 64;
//	typedef shift::static_buffer<buffer_capacity>            buffer_type;
//	typedef shift::sink  <shift::little_endian, buffer_type> little_endian_sink_type;
//	typedef shift::sink  <shift::big_endian   , buffer_type> big_endian_sink_type;
//
//	typedef shift::source<shift::little_endian>              little_endian_source_type;
//	typedef shift::source<shift::big_endian   >              big_endian_source_type;
//
//	little_endian_sink_type le_sink;
//	big_endian_sink_type    be_sink;
//
//	try {
//		le_sink << shift::buffer_position(pos) << value;
//		be_sink << shift::buffer_position(pos) << value;
//	} catch (const shift::out_of_range& e) {
//		CHECK(false);
//		std::cout << e.what() << " - " << e.file() << " - " << e.line() << std::endl;
//	}
//
//	detail::check_buffer_content<shift::little_endian>(pos, bytes, le_sink.buffer());
//	detail::check_buffer_content<shift::big_endian   >(pos, bytes, be_sink.buffer());
//
//	little_endian_source_type le_source(le_sink.buffer(), le_sink.size());
//	big_endian_source_type    be_source(be_sink.buffer(), be_sink.size());
//
//	UintType result_le;
//	UintType result_be;
//
//	try {
//		le_source >> shift::buffer_position(pos) >> result_le;
//		be_source >> shift::buffer_position(pos) >> result_be;
//	} catch (const shift::out_of_range& e) {
//		CHECK(false);
//		std::cout << e.what() << " - " << e.file() << " - " << e.line() << std::endl;
//	}
//
//	CHECK(result_le == value);
//	CHECK(result_be == value);
//}

template<typename T, typename BitifyType>
void test_encoding_with_endianness(const T value, const unsigned int pos) {
	CAPTURE(value);

	const unsigned int n_bytes = sizeof(T);
	const BitifyType tmp = *reinterpret_cast<const BitifyType*>(&value);
	const std::vector<shift::byte_type> bytes = detail::to_bytes(tmp);

	const unsigned int buffer_capacity = 64;
	typedef shift::static_buffer<buffer_capacity>            buffer_type;
	typedef shift::sink  <shift::little_endian, buffer_type> little_endian_sink_type;
	typedef shift::sink  <shift::big_endian   , buffer_type> big_endian_sink_type;

	typedef shift::source<shift::little_endian>              little_endian_source_type;
	typedef shift::source<shift::big_endian   >              big_endian_source_type;

	little_endian_sink_type le_sink;
	big_endian_sink_type    be_sink;

	try {
		le_sink << shift::buffer_position(pos) << value;
		be_sink << shift::buffer_position(pos) << value;
	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << " - " << e.file() << " - " << e.line() << std::endl;
	}

	detail::check_buffer_content<shift::little_endian>(pos, bytes, le_sink.buffer());
	detail::check_buffer_content<shift::big_endian   >(pos, bytes, be_sink.buffer());

	little_endian_source_type le_source(le_sink.buffer(), le_sink.size());
	big_endian_source_type    be_source(be_sink.buffer(), be_sink.size());

	T result_le;
	T result_be;

	try {
		le_source >> shift::buffer_position(pos) >> result_le;
		be_source >> shift::buffer_position(pos) >> result_be;
	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << " - " << e.file() << " - " << e.line() << std::endl;
	}

	CHECK(result_le == value);
	CHECK(result_be == value);
}


template<typename T, typename BitifyType, unsigned int BufferSize>
void test_stream_out_operators(const T value, const unsigned int position) {
	detail::test_stream_out_operators<T, BitifyType, shift::little_endian, BufferSize>(value, position);
	detail::test_stream_out_operators<T, BitifyType, shift::big_endian   , BufferSize>(value, position);
}

template<typename T, typename BitifyType, unsigned int BufferSize>
void test_stream_in_operators(const T value, const unsigned int position) {
	detail::test_stream_in_operators<T, BitifyType, shift::little_endian, BufferSize>(value, position);
	detail::test_stream_in_operators<T, BitifyType, shift::big_endian   , BufferSize>(value, position);
}

template<typename T, unsigned int BufferSize>
void test_out_of_range_when_inserting_beyond_static_buffer_capacity(const T value) {
	const unsigned int n_bytes = sizeof(T);
	const unsigned int buffer_capacity = BufferSize;
	typedef shift::static_buffer<buffer_capacity> buffer_type;
	typedef shift::sink  <shift::little_endian, buffer_type> sink_type;
	sink_type sink;

	CHECK_THROWS_AS( sink << shift::buffer_position(buffer_capacity) << value, const shift::out_of_range&);

	try {
		sink << shift::buffer_position(buffer_capacity-n_bytes+1) << value;
		CHECK(false);
	} catch (shift::out_of_range& e) {
		check_all_zero(sink.buffer(), buffer_capacity);
		CHECK(true);
		CHECK(e.value() == buffer_capacity);
		CHECK((e.range() == std::make_pair<unsigned int, unsigned int>(0, buffer_capacity)));
	}

	try {
		sink << shift::buffer_position(buffer_capacity) << value;
		CHECK(false);
	} catch (shift::out_of_range& e) {
		check_all_zero(sink.buffer(), buffer_capacity);
		CHECK(true);
		CHECK(e.value() == buffer_capacity + n_bytes - 1);
		CHECK((e.range() == std::make_pair<unsigned int, unsigned int>(0, buffer_capacity)));
	}
}

template<typename UintType, unsigned int BufferSize>
void test_out_of_range_when_inserting_beyond_buffer_interface_capacity(const UintType value) {
	const unsigned int n_bytes = sizeof(UintType);
	const unsigned int buffer_capacity = BufferSize;
	shift::byte_type buffer[buffer_capacity];
	typedef shift::sink  <shift::little_endian, shift::buffer_interface> sink_type;
	sink_type sink(sink_type::initialization_params(buffer, buffer_capacity));

	CHECK_THROWS_AS( sink << shift::buffer_position(buffer_capacity) << static_cast<UintType>(123), const shift::out_of_range&);

	try {
		sink << shift::buffer_position(buffer_capacity-n_bytes+1) << value;
		CHECK(false);
	} catch (shift::out_of_range& e) {
		check_all_zero(sink.buffer(), buffer_capacity);
		CHECK(true);
		CHECK(e.value() == buffer_capacity);
		CHECK((e.range() == std::make_pair<unsigned int, unsigned int>(0, buffer_capacity)));
	}

	try {
		sink << shift::buffer_position(buffer_capacity) << value;
		CHECK(false);
	} catch (shift::out_of_range& e) {
		check_all_zero(sink.buffer(), buffer_capacity);
		CHECK(true);
		CHECK(e.value() == buffer_capacity + n_bytes - 1);
		CHECK((e.range() == std::make_pair<unsigned int, unsigned int>(0, buffer_capacity)));
	}
}


template<typename UintType>
void test_out_of_range_when_extracting_beyond_static_buffer_capacity() {
	const unsigned int buffer_capacity = 32;
	const unsigned int n_bytes = sizeof(UintType);
	typedef shift::source <shift::little_endian> source_type;
	uint8_t buffer[buffer_capacity];
	std::memset(buffer, 255, buffer_capacity);
	source_type source(buffer, buffer_capacity);
	UintType v(0);

	CHECK_THROWS_AS(source >> shift::buffer_position(buffer_capacity) >> v, const shift::out_of_range&);

	try {
		source >> shift::buffer_position(buffer_capacity - n_bytes + 1) >> v;
		CHECK(false);
	} catch (shift::out_of_range& e) {
		CHECK(true);
		CHECK((v == 0));
		CHECK(e.value() == buffer_capacity);
		CHECK((e.range() == std::make_pair<unsigned int, unsigned int>(0, buffer_capacity)));
	}

	try {
		source >> shift::buffer_position(buffer_capacity) >> v;
		CHECK(false);
	} catch (shift::out_of_range& e) {
		CHECK(true);
		CHECK((v == 0));
		CHECK(e.value() == buffer_capacity + n_bytes - 1);
		CHECK((e.range() == std::make_pair<unsigned int, unsigned int>(0, buffer_capacity)));
	}
}

template<typename T, typename BitifyType>
void test_consecutive_extraction(const T start_value) {
	const unsigned int n_bytes = sizeof(T);
	const unsigned int buffer_capacity = n_bytes * 20;
	shift::byte_type buffer[buffer_capacity];

	typedef shift::source<shift::big_endian> source_type;

	for (unsigned int i=0; i<buffer_capacity/n_bytes; ++i) {
		const T v = start_value + i;
		const BitifyType tmp = *reinterpret_cast<const BitifyType*>(&v);
		const std::vector<shift::byte_type> bytes = detail::to_bytes(tmp);
		detail::copy_to_buffer<shift::big_endian>( i*n_bytes, bytes, buffer);
	}

	source_type source(buffer, buffer_capacity);
	for (unsigned int i=0; i<buffer_capacity/n_bytes; ++i) {
		const T expected = start_value + i;
		T result = 0;
		source >> result;
		CHECK(result == expected);
	}
}

template<typename T, typename BitifyType>
void test_consecutive_inserting(const T start_value) {
	const unsigned int n_bytes = sizeof(T);
	const unsigned int buffer_capacity = n_bytes * 20;
	typedef shift::static_buffer<buffer_capacity> buffer_type;
	typedef shift::sink  <shift::little_endian, buffer_type> sink_type;
	sink_type sink;

	for (T i=0; i<buffer_capacity/n_bytes; ++i) {
		const T v = start_value + i;
		sink << v;
		CHECK(sink.size() == (i+1)*n_bytes);
	}

	for (T i=0; i<buffer_capacity/n_bytes; ++i) {
		const T v = start_value + i;
		const BitifyType tmp = *reinterpret_cast<const BitifyType*>(&v);
		const std::vector<shift::byte_type> bytes = detail::to_bytes(tmp);
		detail::check_buffer_content<shift::little_endian>(i*n_bytes, bytes, sink.buffer());
	}
}



} // test

#endif /* SHIFT_TEST_UTILITY_HPP_ */
