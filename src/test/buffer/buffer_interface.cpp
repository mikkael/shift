#include <catch.hpp>

#include <iterator>
#include <limits>

#include <shift/buffer/buffer_interface.hpp>

namespace test { namespace {

typedef  shift::byte_type byte_type;

#define DEFINE_BUFFER(size)                                                  \
	byte_type allocated[size];                                               \
	typedef shift::buffer_interface buffer_type;                             \
	const unsigned int buffer_size = size;                                   \
	buffer_type buffer(buffer_type::initialization_params(allocated, size))  \

#define DEFINE_ARRAY(...)                                                    \
	const shift::byte_type values[] = { __VA_ARGS__ };                 \
	const unsigned n = sizeof(values) / sizeof(shift::byte_type);      \
	CAPTURE(n);                                                              \

#define CHECK_OUT_OF_RANGE_IS_THROWN(expr, index, buffer_size)               \
	try {                                                                    \
		expr;                                                                \
		REQUIRE(false);                                                      \
	} catch(const shift::out_of_range& e) {                                  \
		CHECK(true);                                                         \
		CHECK(e.value() == index);                                           \
		CHECK(e.range() == std::make_pair<unsigned int>(0, buffer_size  ));  \
	}                                                                        \

TEST_CASE( "buffer_interface: allocated memory is cleared on construction"
         , "[buffer_interface]" )
{
	DEFINE_BUFFER(64);

	for (unsigned i=0; i<buffer.size(); ++i)
		CHECK(buffer.buffer()[i] == 0);
}

TEST_CASE( "buffer_interface: not_initialized exception is thrown if NULL pointer is passed to constructor"
         , "[buffer_interface]" )
{
	CHECK_THROWS_AS(shift::buffer_interface(shift::buffer_interface::initialization_params(NULL, 0)), const shift::not_initialized&);
}

TEST_CASE( "buffer_interface: buffer is cleared on clear()"
         , "[buffer_interface]" )
{
	DEFINE_BUFFER(64);

	for (unsigned i=0; i<buffer.size(); ++i)
		buffer.write(i+1, i);

	for (unsigned i=0; i<buffer.size(); ++i)
		REQUIRE(buffer.buffer()[i] != 0);

	buffer.clear();

	for (unsigned i=0; i<buffer.size(); ++i)
		CHECK(buffer.buffer()[i] == 0);
}

TEST_CASE( "buffer_interface: single bytes can be written into the buffer"
         , "[buffer_interface]")
{
	DEFINE_BUFFER(64);

	for (unsigned i=0; i<buffer.size(); ++i)
		buffer.write(i, i);

	for (unsigned i=0; i<buffer.size(); ++i)
		CHECK(buffer.buffer()[i] == i);
}

TEST_CASE( "buffer_interface: arrays of bytes can be written into the buffer"
         , "[buffer_interface]")
{
	DEFINE_BUFFER(64);

	DEFINE_ARRAY(11, 25, 129, 42, 8);

	const unsigned start_index = 33;
	buffer.write(values, start_index, n);

	for (unsigned i=0; i<n; ++i)
		CHECK(buffer.buffer()[start_index + i] == values[i]);
}

TEST_CASE( "buffer_interface: arrays of bytes can be written into the buffer in reversed order"
         , "[buffer_interface]")
{
	DEFINE_BUFFER(64);
	DEFINE_ARRAY(113, 17, 8, 55, 233, 1);

	const unsigned start_index = buffer.size()-n-1;
	buffer.reverse_write(values, start_index, n);

	for (unsigned i=0; i<n; ++i)
		CHECK(buffer.buffer()[start_index + i] == values[n-i-1]);
}

TEST_CASE( "buffer_interface: single bytes can be read from the buffer"
         , "[buffer_interface]")
{
	DEFINE_BUFFER(64);

	for (unsigned i=0; i<buffer.size(); ++i)
		buffer.write(i, i);

	for (unsigned i=0; i<buffer.size(); ++i)
		CHECK(buffer.at(i) == i);
}

TEST_CASE( "buffer_interface: when trying to read a single byte beyond the buffer's size an "
           "out_of_range exception is thrown"
         , "[buffer_interface]")
{
	DEFINE_BUFFER(64);

	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.at(buffer.size()    ), buffer_size    , buffer_size );
	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.at(buffer.size() + 1), buffer_size + 1, buffer_size );

	const unsigned int invalid_index = std::numeric_limits<unsigned int>::max();
	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.at(invalid_index)        , invalid_index  , buffer_size );
}

TEST_CASE( "buffer_interface: when trying to write a single byte beyond the buffer's size a out_of_range "
           "exception is thrown"
         , "[buffer_interface]")
{
	DEFINE_BUFFER(64);

	const shift::byte_type value = 0xFF;

	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.write(value, buffer_size       ), buffer_size      , buffer_size );
	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.write(value, buffer_size + 1   ), buffer_size + 1  , buffer_size );
	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.write(value, buffer_size + 222 ), buffer_size + 222, buffer_size );
}

TEST_CASE( "buffer_interface: when trying to write an array of bytes beyond the buffer's size a out_of_range "
           "exception is thrown"
         , "[buffer_interface]")
{
	DEFINE_BUFFER(1024);
	DEFINE_ARRAY(11, 22, 33, 44, 55 ,66);

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

	CHECK_OUT_OF_RANGE_IS_THROWN( buffer.write(values, 1    , buffer_size+1), buffer_size + 2 -1    , buffer_size );
}

TEST_CASE( "buffer_interface: when trying to write an array of bytes in reversed order beyond the buffer's "
           "size an out_of_range exception is thrown"
         , "[buffer_interface]")
{
	DEFINE_BUFFER(1024);
	DEFINE_ARRAY(11, 22, 33, 44, 55, 66, 77, 88);

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

#undef DEFINE_BUFFER
#undef DEFINE_ARRAY
#undef CHECK_OUT_OF_RANGE_IS_THROWN

}} // test
