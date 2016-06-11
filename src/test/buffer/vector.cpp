#include <catch.hpp>

#include <iterator>
#include <limits>

#include <shift/buffer/vector.hpp>

namespace test { namespace {

typedef  shift::byte_type byte_type;

#define DEFINE_BUFFER(name, initial_capacity)                                \
	typedef shift::vector buffer_type;                                       \
	buffer_type name(buffer_type::initialization_params(initial_capacity))   \

#define DEFINE_ARRAY(...)                                                    \
	const shift::byte_type values[] = { __VA_ARGS__ };                       \
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

TEST_CASE( "dynamic_vector: allocated memory is cleared on construction"
         , "[buffer_interface]" )
{
	DEFINE_BUFFER(vec, 64);
	for (unsigned i=0; i<vec.size(); ++i)
		CHECK(vec.buffer()[i] == 0);
}

TEST_CASE( "dynamic_vector: buffer is cleared on clear()"
         , "[buffer_interface]" )
{
	DEFINE_BUFFER(vec, 64);

	for (unsigned i=0; i<vec.size(); ++i)
		vec.write(i+1, i);

	for (unsigned i=0; i<vec.size(); ++i)
		REQUIRE(vec.buffer()[i] != 0);

	vec.clear();

	for (unsigned i=0; i<vec.size(); ++i)
		CHECK(vec.buffer()[i] == 0);
}

TEST_CASE( "dynamic_vector: single bytes can be written into the buffer"
         , "[buffer_interface]")
{
	DEFINE_BUFFER(vec, 64);

	for (unsigned i=0; i<vec.size(); ++i)
		vec.write(i, i);

	for (unsigned i=0; i<vec.size(); ++i)
		CHECK(vec.buffer()[i] == i);
}

TEST_CASE( "dynamic_vector: arrays of bytes can be written into the buffer"
         , "[buffer_interface]")
{
	DEFINE_BUFFER(vec, 64);

	DEFINE_ARRAY(11, 25, 129, 42, 8);

	const unsigned start_index = 33;
	vec.write(values, start_index, n);

	for (unsigned i=0; i<n; ++i)
		CHECK(vec.buffer()[start_index + i] == values[i]);
}

TEST_CASE( "dynamic_vector: arrays of bytes can be written into the buffer in reversed order"
         , "[buffer_interface]")
{
	DEFINE_BUFFER(vec, 256);
	DEFINE_ARRAY(113, 17, 8, 55, 233, 1);

	const unsigned start_index = vec.size()-n-1;
	vec.reverse_write(values, start_index, n);

	for (unsigned i=0; i<n; ++i)
		CHECK(vec.buffer()[start_index + i] == values[n-i-1]);
}

TEST_CASE( "dynamic_vector: single bytes can be read from the buffer"
         , "[buffer_interface]")
{
	DEFINE_BUFFER(vec, 128);

	for (unsigned i=0; i<vec.size(); ++i)
		vec.write(i, i);

	for (unsigned i=0; i<vec.size(); ++i)
		CHECK(vec.at(i) == i);
}

TEST_CASE( "dynamic_vector: when trying to read a single byte beyond the buffer's size an "
           "out_of_range exception is thrown"
         , "[buffer_interface]")
{
	const unsigned int buffer_size = 64;
	DEFINE_BUFFER(vec, 64);

	CHECK_OUT_OF_RANGE_IS_THROWN( vec.at(buffer_size    ), buffer_size    , buffer_size );
	CHECK_OUT_OF_RANGE_IS_THROWN( vec.at(buffer_size + 1), buffer_size + 1, buffer_size );

	const unsigned int invalid_index = std::numeric_limits<unsigned int>::max();
	CHECK_OUT_OF_RANGE_IS_THROWN( vec.at(invalid_index)        , invalid_index  , buffer_size );
}


TEST_CASE( "buffer_interface: when trying to write a single byte beyond the buffer's size the buffer is dynamically extended "
         , "[buffer_interface]")
{
	DEFINE_BUFFER(vec, 64);

	const shift::byte_type value = 0xFF;

	const unsigned int pos = 100000;
	CHECK_NOTHROW( vec.write(value, pos) );
	CHECK(vec.size() == pos + 1);
}

TEST_CASE( "buffer_interface: when trying to write multiple bytes beyond the buffer's size the buffer is dynamically "
           "extended"
         , "[buffer_interface]")
{
	DEFINE_BUFFER(vec, 256);
	DEFINE_ARRAY(11, 22, 33, 44, 55 ,66);

	const unsigned int pos = 50000;
	CHECK_NOTHROW( vec.write(values, pos, n) );
	CHECK(vec.size() == pos + n + 1);
}

TEST_CASE( "buffer_interface: when trying to write multiple bytes in reversed order beyond the buffer's size the buffer "
           "is dynamically extended"
         , "[buffer_interface]")
{
	DEFINE_BUFFER(vec, 256);
	DEFINE_ARRAY(11, 22, 33, 44, 55 ,66);

	const unsigned int pos = 50000;
	CHECK_NOTHROW( vec.reverse_write(values, pos, n) );
	CHECK(vec.size() == pos + n + 1);
}

#undef DEFINE_BUFFER
#undef DEFINE_ARRAY
#undef CHECK_OUT_OF_RANGE_IS_THROWN

}} // test
