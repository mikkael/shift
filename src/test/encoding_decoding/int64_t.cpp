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

TEST_CASE( "int64_t values can be streamed to a sink in specified locations using the << and the % operator"
         , "[int64_t]" )
{
	const shift::int64_t max = std::numeric_limits<shift::int64_t>::max();
	const shift::int64_t min = std::numeric_limits<shift::int64_t>::min();

	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 32>(  min,  0);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 32>(  min,  1);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 32>(  min, 16);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 32>(  min, 23);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 32>(  min, 24);

	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 32>(min/2,  0);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 32>(min/2,  1);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 32>(min/2, 16);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 32>(min/2, 23);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 32>(min/2, 24);

	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 32>(-5912,  0);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 32>(-5912,  1);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 32>(-5912, 16);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 32>(-5912, 23);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 32>(-5912, 24);

	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 32>(    1,  0);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 32>(    1,  1);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 32>(    1, 16);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 32>(    1, 23);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 32>(    1, 24);

	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 16>(30123,  0);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 16>(30123,  1);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 16>(30123,  3);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 16>(30123,  7);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 16>(30123,  8);

	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 50>(max/2,  0);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 50>(max/2,  1);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 50>(max/2, 25);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 50>(max/2, 41);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 50>(max/2, 42);

	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 50>(  max,  0);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 50>(  max,  1);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 50>(  max, 25);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 50>(  max, 41);
	test::test_stream_out_operators<shift::int64_t, shift::uint64_t, 50>(  max, 42);
}

TEST_CASE( "int64_t values can be streamed out of a source from specified locations using the >> and the % operator"
         , "[int64_t]")
{
	const shift::int64_t max = std::numeric_limits<shift::int64_t>::max();
	const shift::int64_t min = std::numeric_limits<shift::int64_t>::min();

	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 32>(  min,  0);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 32>(  min,  1);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 32>(  min, 16);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 32>(  min, 23);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 32>(  min, 24);

	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 32>(min/2,  0);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 32>(min/2,  1);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 32>(min/2, 16);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 32>(min/2, 23);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 32>(min/2, 24);

	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 32>( -921,  0);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 32>( -921,  1);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 32>( -921, 16);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 32>( -921, 23);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 32>( -921, 24);

	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 32>(    1,  0);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 32>(    1,  1);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 32>(    1, 16);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 32>(    1, 23);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 32>(    1, 24);

	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 16>(12345,  0);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 16>(12345,  1);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 16>(12345,  3);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 16>(12345,  7);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 16>(12345,  8);

	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 50>(max/2,  0);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 50>(max/2,  1);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 50>(max/2, 25);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 50>(max/2, 41);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 50>(max/2, 42);

	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 50>(  max,  0);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 50>(  max,  1);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 50>(  max, 25);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 50>(  max, 41);
	test::test_stream_in_operators<shift::int64_t, shift::uint64_t, 50>(  max, 42);
}

TEST_CASE( "writing a int64_t does only modify the specified location in the buffer"
         , "[int64_t]" )
{
	const shift::int64_t v = std::numeric_limits<shift::int64_t>::max();
	test::test_only_specified_location_in_buffer_is_modified_on_stream_out(v);
}

TEST_CASE( "when consecutively streaming int64_t values without specifying the position the values are 'appended' "
           "to the buffer in the order they were streamed"
         , "[int64_t]")
{
	test::test_consecutive_inserting<shift::int64_t, shift::uint64_t>(-7);
}

TEST_CASE( "when consecutively streaming out a source int64_t values without specifying the position the values are "
           "extracted from the buffer in the order they are contained"
         , "[int64_t]")
{
	test::test_consecutive_extraction<shift::int64_t, shift::uint64_t>(-232323);
}

TEST_CASE( "encoding and decoding of int64_t values with different endiannesses"
         , "[int64_t]" )
{
	const shift::int64_t max = std::numeric_limits<shift::int64_t>::max();
	const shift::int64_t min = std::numeric_limits<shift::int64_t>::min();

	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(      -    1LL,  0);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(      -   13LL,  1);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(      -  255LL, 28);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(      -  256LL, 27);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(      -  257LL, 16);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(      -  258LL, 10);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(      -  512LL, 20);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(      -  513LL, 40);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(      - 1293LL, 50);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(      - 3456LL, 56);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(      - 5837LL, 56);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(      -16000LL,  0);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(      -30000LL,  0);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(     -300000LL,  0);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(-81239478367LL,  0);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(         min/2,  0);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(         min+1, 56);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(           min,  0);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(           0LL,  0);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(           1LL,  0);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(          13LL,  1);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(         255LL, 28);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(         256LL, 27);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(         257LL, 16);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(         258LL, 10);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(         512LL, 20);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(         513LL, 40);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(        1293LL, 50);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(        3456LL, 56);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(        5837LL, 56);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(       16000LL,  0);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(       30000LL,  0);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(      300000LL,  0);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>( 81239478367LL,  0);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(         max/2,  0);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(         max-1, 56);
	test::test_encoding_with_endianness<shift::int64_t, shift::uint64_t>(           max,  0);
}

TEST_CASE( "writing a int64_t value beyond the capacity of the buffer causes an out_ot_range exception to be thrown"
         , "[int64_t]" )
{
	test::test_out_of_range_when_inserting_beyond_static_buffer_capacity<shift::int64_t, 64>(-123456789000);
}

TEST_CASE( "trying to stream out int64_t values beyond the size of a source results in an out_of_range exception to be thrown"
         , "[int64_t]" )
{
	test::test_out_of_range_when_extracting_beyond_static_buffer_capacity<shift::int64_t>();
}

}} // test
