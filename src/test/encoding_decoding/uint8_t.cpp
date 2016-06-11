#include <catch.hpp>

#include <iterator>
#include <limits>
#include <iostream>

#include <shift/buffer/static_buffer.hpp>
#include <shift/sink.hpp>
#include <shift/source.hpp>
#include <shift/operator/universal.hpp>

#include <test/utility.hpp>

namespace test { namespace {

TEST_CASE( "uint8_t values can be streamed to a sink in specified locations using the << and the % operator"
         , "[uint8_t]" )
{
	test::test_stream_out_operators<shift::uint8_t, shift::uint8_t,32>(  1,  0);
	test::test_stream_out_operators<shift::uint8_t, shift::uint8_t,32>(  1,  1);
	test::test_stream_out_operators<shift::uint8_t, shift::uint8_t,32>(  1, 16);
	test::test_stream_out_operators<shift::uint8_t, shift::uint8_t,32>(  1, 30);
	test::test_stream_out_operators<shift::uint8_t, shift::uint8_t,32>(  1, 31);

	test::test_stream_out_operators<shift::uint8_t, shift::uint8_t,16>(128,  0);
	test::test_stream_out_operators<shift::uint8_t, shift::uint8_t,16>(128,  1);
	test::test_stream_out_operators<shift::uint8_t, shift::uint8_t,16>(128,  8);
	test::test_stream_out_operators<shift::uint8_t, shift::uint8_t,16>(128, 14);
	test::test_stream_out_operators<shift::uint8_t, shift::uint8_t,16>(128, 15);

	test::test_stream_out_operators<shift::uint8_t, shift::uint8_t,50>(255,  0);
	test::test_stream_out_operators<shift::uint8_t, shift::uint8_t,50>(255,  1);
	test::test_stream_out_operators<shift::uint8_t, shift::uint8_t,50>(255, 25);
	test::test_stream_out_operators<shift::uint8_t, shift::uint8_t,50>(255, 48);
	test::test_stream_out_operators<shift::uint8_t, shift::uint8_t,50>(255, 49);
}

TEST_CASE( "uint8_t values can be streamed out of a source from specified locations using the >> and the % operator"
         , "[uint8_t]")
{
	test::test_stream_in_operators<shift::uint8_t, shift::uint8_t, 32>(  1,  0);
	test::test_stream_in_operators<shift::uint8_t, shift::uint8_t, 32>(  1,  1);
	test::test_stream_in_operators<shift::uint8_t, shift::uint8_t, 32>(  1, 16);
	test::test_stream_in_operators<shift::uint8_t, shift::uint8_t, 32>(  1, 30);
	test::test_stream_in_operators<shift::uint8_t, shift::uint8_t, 32>(  1, 31);

	test::test_stream_in_operators<shift::uint8_t, shift::uint8_t, 16>(123,  0);
	test::test_stream_in_operators<shift::uint8_t, shift::uint8_t, 16>(123,  1);
	test::test_stream_in_operators<shift::uint8_t, shift::uint8_t, 16>(123,  8);
	test::test_stream_in_operators<shift::uint8_t, shift::uint8_t, 16>(123, 14);
	test::test_stream_in_operators<shift::uint8_t, shift::uint8_t, 16>(123, 15);

	test::test_stream_in_operators<shift::uint8_t, shift::uint8_t, 50>(255,  0);
	test::test_stream_in_operators<shift::uint8_t, shift::uint8_t, 50>(255,  1);
	test::test_stream_in_operators<shift::uint8_t, shift::uint8_t, 50>(255, 25);
	test::test_stream_in_operators<shift::uint8_t, shift::uint8_t, 50>(255, 48);
	test::test_stream_in_operators<shift::uint8_t, shift::uint8_t, 50>(255, 49);
}

TEST_CASE( "writing a uint8_t does only modify the specified location in the buffer"
         , "[uint8_t]" )
{
	const shift::uint8_t v = 100;
	test::test_only_specified_location_in_buffer_is_modified_on_stream_out(v);
}

TEST_CASE( "when consecutively streaming uint8_t values without specifying the position the values are 'appended' "
           "to the buffer in the order they were streamed"
         , "[uint8_t]")
{
	test::test_consecutive_inserting<shift::uint8_t, shift::uint8_t>(100);
}

TEST_CASE( "when consecutively streaming out a source uint8_t values without specifying the position the values are "
           "extracted from the buffer in the order they are contained"
         , "[uint8_t]")
{
	test::test_consecutive_extraction<shift::uint8_t, shift::uint8_t>(200);
}

TEST_CASE( "writing a uint8_t value beyond the capacity of the buffer causes an out_ot_range exception to be thrown"
         , "[uint8_t]" )
{
	test::test_out_of_range_when_inserting_beyond_static_buffer_capacity<shift::uint8_t, 64>(128);
}

TEST_CASE( "trying to stream out uint8_t values beyond the size of a source results in an out_of_range exception to be thrown"
         , "[uint8_t]" )
{
	test::test_out_of_range_when_extracting_beyond_static_buffer_capacity<shift::uint8_t>();
}

TEST_CASE( "writing uint8_t values to sources with different encoding endiannesses does not result in different buffer content"
         , "[uint8_t]" )
{
	const unsigned int buffer_capacity = 32;
	typedef shift::static_buffer<buffer_capacity> buffer_type;
	typedef shift::sink  <shift::little_endian, buffer_type> little_endian_sink_type;
	typedef shift::sink  <shift::big_endian   , buffer_type> big_endian_sink_type;

	little_endian_sink_type le_sink;
	big_endian_sink_type    be_sink;

	const uint8_t value    = 99;
	const unsigned int pos = 5;

	le_sink << shift::buffer_position(pos) << value;
	REQUIRE(le_sink.size() == pos + 1);
	REQUIRE(le_sink.buffer()[pos] == value);

	be_sink << shift::buffer_position(pos) << value;
	REQUIRE(be_sink.size() == pos + 1);
	REQUIRE(be_sink.buffer()[pos] == value);

	for (unsigned int i=0; i<buffer_capacity; ++i)
		CHECK(le_sink.buffer()[i] == be_sink.buffer()[i]);
}

}} // test
