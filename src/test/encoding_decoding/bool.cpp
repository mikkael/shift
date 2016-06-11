#include <catch.hpp>

#include <iterator>
#include <limits>
#include <iostream>

#include <shift/buffer/static_buffer.hpp>
#include <shift/sink.hpp>
#include <shift/source.hpp>
#include <shift/operator/universal.hpp>

#include <test/utility.hpp>
#include <examples/utility.hpp>


namespace test { namespace {

#define TEST_STREAM_OUT_OP(byte, bit, val, op)                                  \
	{                                                                           \
		const bool value = val;                                                 \
		const shift::buffer_position pos(byte, bit);                            \
		sink op pos op value;                                                   \
		CHECK(((sink.buffer()[pos.byte_index] >> pos.bit_index) & 1) == value); \
	}

#define TEST_EXTRACT_OP(byte, bit, val, op)              \
	{                                                    \
		const bool value = val;                          \
		const shift::buffer_position pos(byte, bit);     \
		buffer[pos.byte_index] = value << pos.bit_index; \
		bool result;                                     \
		source op pos op result;                         \
		CHECK(result == value);                          \
	}

TEST_CASE( "booleans values can be streamed to a sink in specified locations using the << and the % operator"
         , "[bool]" )
{
	const unsigned int buffer_size = 64;
	typedef shift::static_buffer<buffer_size> buffer_t;
	typedef shift::sink<shift::little_endian, buffer_t> sink_t;
	sink_t sink;

	try {
		TEST_STREAM_OUT_OP(0, 4, true , <<);
		TEST_STREAM_OUT_OP(0, 3, true , % );
		TEST_STREAM_OUT_OP(0, 2, false, <<);
		TEST_STREAM_OUT_OP(0, 0, true , % );
		TEST_STREAM_OUT_OP(1, 7, false, <<);
		TEST_STREAM_OUT_OP(3, 6, true , % );
		TEST_STREAM_OUT_OP(5, 5, false, <<);
		TEST_STREAM_OUT_OP(8, 1, true , % );
	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << ", "<< 	e.file() << " line " << e.line();
	}
}

TEST_CASE( "booleans values can be extracted from a sink from specified locations using the >> and the % operator"
         , "[bool]" )
{
	const unsigned int buffer_capacity = 32;
	typedef shift::source <shift::big_endian> source_type;
	shift::byte_type buffer[buffer_capacity];
	source_type source(buffer, buffer_capacity);

	try {
		TEST_EXTRACT_OP(0, 5, true , >>);
		TEST_EXTRACT_OP(0, 0, true , % );
		TEST_EXTRACT_OP(1, 7, false, >>);
		TEST_EXTRACT_OP(1, 4, false, % );
		TEST_EXTRACT_OP(2, 3, true , >>);
		TEST_EXTRACT_OP(3, 1, true , % );
		TEST_EXTRACT_OP(3, 2, false, >>);
		TEST_EXTRACT_OP(8, 6, false, % );
	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << ", "<< 	e.file() << " line " << e.line();
	}
}

TEST_CASE( "writing a bool does only modify the specified location in the buffer"
         , "[bool]" )
{
	const unsigned int buffer_size = 64;
	typedef shift::static_buffer<buffer_size> buffer_t;
	typedef shift::sink<shift::little_endian, buffer_t> sink_t;
	sink_t sink;

	try {
		const bool value = true;
		const shift::buffer_position pos(4, 3);
		sink << pos << value;

		for (unsigned int i=0; i<buffer_size; ++i) {
			if (i == pos.byte_index)
				CHECK(sink.buffer()[i] == value << pos.bit_index);
			else
				CHECK(sink.buffer()[i] == 0);
		}
	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << ", "<< 	e.file() << " line " << e.line();
	}
}

TEST_CASE( "when consecutively streaming booleans without specifying the position the values are 'appended' "
           "to the buffer in the order they were streamed, with each value starting at the next byte, bit "
           "position 7"
         , "[bool]")
{
	const unsigned int buffer_size = 16;
	typedef shift::sink<shift::big_endian, shift::buffer_interface> sink_type;
	shift::byte_type buffer[buffer_size];
	sink_type sink(sink_type::initialization_params(buffer, buffer_size));

	try {

		for (unsigned int i=0; i<buffer_size; ++i)
			sink << static_cast<bool>(i % 2);

		for (unsigned int i=0; i<buffer_size; ++i)
			CHECK(buffer[i] == ((i % 2) << 7));

	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << ", "<< 	e.file() << " line " << e.line();
	}
}

TEST_CASE( "when consecutively streaming out a source booleans without specifying the position the values are "
           "extracted from the buffer in the order they are contained, with each value starting at the next byte, "
           "bit position 7"
         , "[bool]")
{
	const unsigned int buffer_size = 24;
	typedef shift::source<shift::little_endian> source_type;
	shift::byte_type buffer[buffer_size];
	source_type source(buffer, buffer_size);

	try {
		for (unsigned int i=0; i<buffer_size; ++i)
			buffer[i] = ((i % 2) << 7);

		for (unsigned int i=0; i<buffer_size; ++i) {
			bool value;
			source >> value;
			CHECK(value == (i % 2));
		}

//		example::debug_print(std::cout, buffer, buffer_size);

	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << ", "<< 	e.file() << " line " << e.line();
	}
}

TEST_CASE( "writing a boolean beyond the capacity of the buffer causes an out_ot_range exception to be thrown"
           "(static_buffer)"
         , "[bool]" )
{
	const unsigned int buffer_capacity = 16;
	typedef shift::sink <shift::little_endian, shift::static_buffer<buffer_capacity> > sink_type;
	sink_type sink;

	bool value = true;
	CHECK_THROWS_AS(sink << shift::buffer_position(buffer_capacity) << value, const shift::out_of_range&);
}

TEST_CASE( "writing a boolean beyond the capacity of the buffer causes an out_ot_range exception to be thrown"
           "(buffer_interface)"
         , "[bool]" )
{
	const unsigned int buffer_capacity = 16;
	shift::byte_type buffer[buffer_capacity];
	typedef shift::sink <shift::little_endian, shift::buffer_interface > sink_type;
	sink_type sink(sink_type::initialization_params(buffer, buffer_capacity));

	bool value = true;
	CHECK_THROWS_AS(sink << shift::buffer_position(buffer_capacity) << value, const shift::out_of_range&);
}

TEST_CASE( "trying to extract booleans beyond the size of a source results in an out_of_range exception to be thrown"
         , "[bool]" )
{
	const unsigned int buffer_capacity = 32;
	typedef shift::source <shift::big_endian> source_type;
	shift::byte_type buffer[buffer_capacity];
	source_type source(buffer, buffer_capacity);

	bool value;
	CHECK_THROWS_AS(source >> shift::buffer_position(buffer_capacity) >> value, const shift::out_of_range&);
}

}} // test
