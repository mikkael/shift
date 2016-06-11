#include <catch.hpp>

#include <iterator>
#include <limits>

#include <shift/buffer/static_buffer.hpp>

namespace test { namespace {

#define CHECK_OUT_OF_RANGE_IS_THROWN(expr, index, buffer_size)            \
	try {                                                                 \
		expr;                                                             \
		REQUIRE(false);                                                   \
	} catch(const shift::out_of_range& e) {                               \
		CHECK(e.value() == index);                                        \
		CHECK(e.range() == std::make_pair<unsigned int>(0, buffer_size)); \
	}                                                                     \

TEST_CASE( "allocated memory is cleared on construction", "[static_buffer]" ) {
	typedef shift::static_buffer<64> buffer_type;
	buffer_type buffer;

	for (unsigned i=0; i<buffer_type::size; ++i)
		CHECK(buffer.buffer()[i] == 0);
}

TEST_CASE( "buffer is cleared on clear()", "[static_buffer]" ) {
	typedef shift::static_buffer<64> buffer_type;
	buffer_type buffer;

	for (unsigned i=0; i<buffer_type::size; ++i)
		buffer.write(i+1, i);

	for (unsigned i=0; i<buffer_type::size; ++i)
		REQUIRE(buffer.buffer()[i] != 0);

	buffer.clear();

	for (unsigned i=0; i<buffer_type::size; ++i)
		CHECK(buffer.buffer()[i] == 0);
}

TEST_CASE( "single bytes can be written into the buffer", "[static_buffer]") {
	typedef shift::static_buffer<64> buffer_type;
	buffer_type buffer;

	for (unsigned i=0; i<buffer_type::size; ++i)
		buffer.write(i, i);

	for (unsigned i=0; i<buffer_type::size; ++i)
		CHECK(buffer.buffer()[i] == i);
}

TEST_CASE("arrays of bytes can be written into the buffer", "[static_buffer]") {
	typedef shift::static_buffer<64> buffer_type;
	typedef shift::byte_type byte_type;

	buffer_type buffer;

	byte_type array[] = {11, 25, 129, 42, 8};
	const unsigned n = sizeof(array)/sizeof(byte_type);

	const unsigned start_index = 33;
	buffer.write(array, start_index, n);

	for (unsigned i=0; i<n; ++i)
		CHECK(buffer.buffer()[start_index + i] == array[i]);
}

TEST_CASE("arrays of bytes can be written into the buffer in reversed order", "[static_buffer]") {
	typedef shift::static_buffer<64> buffer_type;
	typedef shift::byte_type byte_type;

	buffer_type buffer;

	byte_type array[] = {113, 17, 8, 55, 233, 1};
	const unsigned n = sizeof(array)/sizeof(byte_type);

	const unsigned start_index = buffer_type::size-n-1;
	buffer.reverse_write(array, start_index, n);

	for (unsigned i=0; i<n; ++i)
		CHECK(buffer.buffer()[start_index + i] == array[n-i-1]);
}

TEST_CASE("single bytes can be read from the buffer", "[static_buffer]") {
	typedef shift::static_buffer<64> buffer_type;
	buffer_type buffer;

	for (unsigned i=0; i<buffer_type::size; ++i)
		buffer.write(i, i);

	for (unsigned i=0; i<buffer_type::size; ++i)
		CHECK(buffer.at(i) == i);
}

TEST_CASE("when trying to read a single byte beyond the buffer's size a out_of_range exception is thrown", "[static_buffer]") {
	const unsigned int buffer_size = 64;
	typedef shift::static_buffer<buffer_size> buffer_type;
	buffer_type buffer;

	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.at(buffer_type::size    ), buffer_size    , buffer_size );
	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.at(buffer_type::size + 1), buffer_size + 1, buffer_size );

	const unsigned int invalid_index = std::numeric_limits<unsigned int>::max();
	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.at(invalid_index)        , invalid_index  , buffer_size );
}

TEST_CASE("when trying to write a single byte beyond the buffer's size a out_of_range exception is thrown", "[static_buffer]") {
	const unsigned int buffer_size = 1024;
	typedef shift::static_buffer<buffer_size> buffer_type;
	buffer_type buffer;
	const shift::byte_type value = 0xFF;

	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.write(value, buffer_size       ), buffer_size      , buffer_size );
	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.write(value, buffer_size + 1   ), buffer_size + 1  , buffer_size );
	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.write(value, buffer_size + 222 ), buffer_size + 222, buffer_size );
}

TEST_CASE("when trying to write an array of bytes beyond the buffer's size a out_of_range exception is thrown", "[static_buffer]") {
	const unsigned int buffer_size = 1024;
	typedef shift::static_buffer<buffer_size> buffer_type;
	buffer_type buffer;
	const shift::byte_type values[] = {11, 22, 33, 44, 55, 66};
	const unsigned n = sizeof(values) / sizeof(shift::byte_type);
	CAPTURE(n);

	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.write(values, buffer_size      , n), buffer_size + n       - 1, buffer_size );
	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.write(values, buffer_size + 1  , n), buffer_size + 1 + n   - 1, buffer_size );
	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.write(values, buffer_size + 222, n), buffer_size + 222 + n - 1, buffer_size );

	unsigned invalid_index = buffer_size - n + 1;
	CAPTURE(invalid_index);
	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.write(values, invalid_index    , n), invalid_index + n - 1, buffer_size );

	invalid_index = buffer_size - n + 2;
	CAPTURE(invalid_index);
	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.write(values, invalid_index    , n), invalid_index + n - 1, buffer_size );

	invalid_index = buffer_size - n + 3;
	CAPTURE(invalid_index);
	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.write(values, invalid_index    , n), invalid_index + n - 1, buffer_size );

	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.write(values, 1    , buffer_size+1), buffer_size+1    , buffer_size );
}

TEST_CASE("when trying to write an array of bytes in reversed order beyond the buffer's size a out_of_range exception is thrown", "[static_buffer]") {
	const unsigned int buffer_size = 256;
	typedef shift::static_buffer<buffer_size> buffer_type;
	buffer_type buffer;
	const shift::byte_type values[] = {11, 22, 33, 44, 55, 66, 77, 88};
	const unsigned n = sizeof(values) / sizeof(shift::byte_type);
	CAPTURE(n);

	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.reverse_write(values, buffer_size      , n), buffer_size + n       - 1, buffer_size );
	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.reverse_write(values, buffer_size + 1  , n), buffer_size + 1 + n   - 1, buffer_size );
	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.reverse_write(values, buffer_size + 222, n), buffer_size + 222 + n - 1, buffer_size );

	unsigned invalid_index = buffer_size - n + 1;
	CAPTURE(invalid_index);
	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.reverse_write(values, invalid_index    , n), invalid_index + n - 1, buffer_size );

	invalid_index = buffer_size - n + 2;
	CAPTURE(invalid_index);
	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.reverse_write(values, invalid_index    , n), invalid_index + n - 1, buffer_size );

	invalid_index = buffer_size - n + 3;
	CAPTURE(invalid_index);
	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.reverse_write(values, invalid_index    , n), invalid_index + n - 1, buffer_size );

	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.reverse_write(values, 1    , buffer_size), buffer_size    , buffer_size );
}

#undef CHECK_OUT_OF_RANGE_IS_THROWN

}} // test
