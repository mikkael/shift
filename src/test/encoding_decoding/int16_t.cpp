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

TEST_CASE( "int16_t values can be streamed to a sink in specified locations using the << and the % operator"
         , "[int16_t]" )
{
	const shift::int16_t max = std::numeric_limits<shift::int16_t>::max();
	const shift::int16_t min = std::numeric_limits<shift::int16_t>::min();

	test::test_stream_out_operators<shift::int16_t, shift::uint16_t, 32>(  min,  0);
	test::test_stream_out_operators<shift::int16_t, shift::uint16_t, 32>(  min,  1);
	test::test_stream_out_operators<shift::int16_t, shift::uint16_t, 32>(  min, 16);
	test::test_stream_out_operators<shift::int16_t, shift::uint16_t, 32>(  min, 29);
	test::test_stream_out_operators<shift::int16_t, shift::uint16_t, 32>(  min, 30);

	test::test_stream_out_operators<shift::int16_t, shift::uint16_t, 32>(-5912,  0);
	test::test_stream_out_operators<shift::int16_t, shift::uint16_t, 32>(-5912,  1);
	test::test_stream_out_operators<shift::int16_t, shift::uint16_t, 32>(-5912, 16);
	test::test_stream_out_operators<shift::int16_t, shift::uint16_t, 32>(-5912, 29);
	test::test_stream_out_operators<shift::int16_t, shift::uint16_t, 32>(-5912, 30);

	test::test_stream_out_operators<shift::int16_t, shift::uint16_t, 32>(    1,  0);
	test::test_stream_out_operators<shift::int16_t, shift::uint16_t, 32>(    1,  1);
	test::test_stream_out_operators<shift::int16_t, shift::uint16_t, 32>(    1, 16);
	test::test_stream_out_operators<shift::int16_t, shift::uint16_t, 32>(    1, 29);
	test::test_stream_out_operators<shift::int16_t, shift::uint16_t, 32>(    1, 30);

	test::test_stream_out_operators<shift::int16_t, shift::uint16_t, 16>(30123,  0);
	test::test_stream_out_operators<shift::int16_t, shift::uint16_t, 16>(30123,  1);
	test::test_stream_out_operators<shift::int16_t, shift::uint16_t, 16>(30123,  8);
	test::test_stream_out_operators<shift::int16_t, shift::uint16_t, 16>(30123, 13);
	test::test_stream_out_operators<shift::int16_t, shift::uint16_t, 16>(30123, 14);

	test::test_stream_out_operators<shift::int16_t, shift::uint16_t, 50>(  max,  0);
	test::test_stream_out_operators<shift::int16_t, shift::uint16_t, 50>(  max,  1);
	test::test_stream_out_operators<shift::int16_t, shift::uint16_t, 50>(  max, 25);
	test::test_stream_out_operators<shift::int16_t, shift::uint16_t, 50>(  max, 47);
	test::test_stream_out_operators<shift::int16_t, shift::uint16_t, 50>(  max, 48);
}

TEST_CASE( "int16_t values can be streamed out of a source from specified locations using the >> and the % operator"
         , "[int16_t]")
{
	const shift::int16_t max = std::numeric_limits<shift::int16_t>::max();
	const shift::int16_t min = std::numeric_limits<shift::int16_t>::min();

	test::test_stream_in_operators<shift::int16_t, shift::uint16_t, 32>(  min,  0);
	test::test_stream_in_operators<shift::int16_t, shift::uint16_t, 32>(  min,  1);
	test::test_stream_in_operators<shift::int16_t, shift::uint16_t, 32>(  min, 16);
	test::test_stream_in_operators<shift::int16_t, shift::uint16_t, 32>(  min, 29);
	test::test_stream_in_operators<shift::int16_t, shift::uint16_t, 32>(  min, 30);

	test::test_stream_in_operators<shift::int16_t, shift::uint16_t, 32>( -921,  0);
	test::test_stream_in_operators<shift::int16_t, shift::uint16_t, 32>( -921,  1);
	test::test_stream_in_operators<shift::int16_t, shift::uint16_t, 32>( -921, 16);
	test::test_stream_in_operators<shift::int16_t, shift::uint16_t, 32>( -921, 29);
	test::test_stream_in_operators<shift::int16_t, shift::uint16_t, 32>( -921, 30);

	test::test_stream_in_operators<shift::int16_t, shift::uint16_t, 32>(    1,  0);
	test::test_stream_in_operators<shift::int16_t, shift::uint16_t, 32>(    1,  1);
	test::test_stream_in_operators<shift::int16_t, shift::uint16_t, 32>(    1, 16);
	test::test_stream_in_operators<shift::int16_t, shift::uint16_t, 32>(    1, 29);
	test::test_stream_in_operators<shift::int16_t, shift::uint16_t, 32>(    1, 30);

	test::test_stream_in_operators<shift::int16_t, shift::uint16_t, 16>(12345,  0);
	test::test_stream_in_operators<shift::int16_t, shift::uint16_t, 16>(12345,  1);
	test::test_stream_in_operators<shift::int16_t, shift::uint16_t, 16>(12345,  8);
	test::test_stream_in_operators<shift::int16_t, shift::uint16_t, 16>(12345, 13);
	test::test_stream_in_operators<shift::int16_t, shift::uint16_t, 16>(12345, 14);

	test::test_stream_in_operators<shift::int16_t, shift::uint16_t, 50>(  max,  0);
	test::test_stream_in_operators<shift::int16_t, shift::uint16_t, 50>(  max,  1);
	test::test_stream_in_operators<shift::int16_t, shift::uint16_t, 50>(  max, 25);
	test::test_stream_in_operators<shift::int16_t, shift::uint16_t, 50>(  max, 47);
	test::test_stream_in_operators<shift::int16_t, shift::uint16_t, 50>(  max, 48);
}

TEST_CASE( "writing a int16_t does only modify the specified location in the buffer"
         , "[int16_t]" )
{
	const shift::int16_t v = 12345;
	test::test_only_specified_location_in_buffer_is_modified_on_stream_out(v);
}

TEST_CASE( "when consecutively streaming int16_t values without specifying the position the values are 'appended' "
           "to the buffer in the order they were streamed"
         , "[int16_t]")
{
	test::test_consecutive_inserting<shift::int16_t, shift::uint16_t>(-42);
}

TEST_CASE( "when consecutively streaming out a source int16_t values without specifying the position the values are "
           "extracted from the buffer in the order they are contained"
         , "[int16_t]")
{
	test::test_consecutive_extraction<shift::int16_t, shift::uint16_t>(-256);
}

TEST_CASE( "encoding and decoding of int16_t values with different endiannesses"
         , "[int16_t]" )
{
	const shift::int16_t max = std::numeric_limits<shift::int16_t>::max();
	const shift::int16_t min = std::numeric_limits<shift::int16_t>::min();

	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(-    1,  0);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(-   13,  1);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(-  255, 28);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(-  256, 27);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(-  257, 16);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(-  258, 10);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(-  512, 20);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(-  513, 40);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(- 1293, 50);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(- 3456, 62);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(- 5837, 62);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(-16000,  0);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(-30000,  0);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>( min+1, 62);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(   min,  0);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(     0,  0);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(     1,  0);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(    13,  1);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(   255, 28);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(   256, 27);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(   257, 16);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(   258, 10);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(   512, 20);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(   513, 40);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(  1293, 50);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(  3456, 62);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(  5837, 62);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>( 16000,  0);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>( 30000,  0);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>( max-1, 62);
	test::test_encoding_with_endianness<shift::int16_t, shift::uint16_t>(   max,  0);
}

TEST_CASE( "writing a int16_t value beyond the capacity of the buffer causes an out_ot_range exception to be thrown"
         , "[int16_t]" )
{
	test::test_out_of_range_when_inserting_beyond_static_buffer_capacity<shift::int16_t, 64>(-12345);
}

TEST_CASE( "trying to stream out int16_t values beyond the size of a source results in an out_of_range exception to be thrown"
         , "[int16_t]" )
{
	test::test_out_of_range_when_extracting_beyond_static_buffer_capacity<shift::int16_t>();
}

}} // test
