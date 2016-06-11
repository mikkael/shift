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

TEST_CASE( "uint32_t values can be streamed to the buffer in specified locations using the << and the % operator"
         , "[uint32_t]" )
{
	test::test_stream_out_operators<shift::uint32_t, shift::uint32_t, 32>(          1,   0);
	test::test_stream_out_operators<shift::uint32_t, shift::uint32_t, 32>(          1,   1);
	test::test_stream_out_operators<shift::uint32_t, shift::uint32_t, 32>(          1,  16);
	test::test_stream_out_operators<shift::uint32_t, shift::uint32_t, 32>(          1,  27);
	test::test_stream_out_operators<shift::uint32_t, shift::uint32_t, 32>(          1,  28);

	test::test_stream_out_operators<shift::uint32_t, shift::uint32_t, 16>(      12345,   0);
	test::test_stream_out_operators<shift::uint32_t, shift::uint32_t, 16>(      12345,   1);
	test::test_stream_out_operators<shift::uint32_t, shift::uint32_t, 16>(      12345,   8);
	test::test_stream_out_operators<shift::uint32_t, shift::uint32_t, 16>(      12345,  11);
	test::test_stream_out_operators<shift::uint32_t, shift::uint32_t, 16>(      12345,  12);

	test::test_stream_out_operators<shift::uint32_t, shift::uint32_t, 50>(      65535,   0);
	test::test_stream_out_operators<shift::uint32_t, shift::uint32_t, 50>(      65535,   1);
	test::test_stream_out_operators<shift::uint32_t, shift::uint32_t, 50>(      65535,  25);
	test::test_stream_out_operators<shift::uint32_t, shift::uint32_t, 50>(      65535,  45);
	test::test_stream_out_operators<shift::uint32_t, shift::uint32_t, 50>(      65535,  46);

	test::test_stream_out_operators<shift::uint32_t, shift::uint32_t,128>( 123456789,   0);
	test::test_stream_out_operators<shift::uint32_t, shift::uint32_t,128>( 987654321,  64);
	test::test_stream_out_operators<shift::uint32_t, shift::uint32_t,128>(4294967294, 123);
	test::test_stream_out_operators<shift::uint32_t, shift::uint32_t,128>(4294967295, 124);
}

TEST_CASE( "uint32_t values can be streamed out of a source from specified locations using the >> and the % operator"
         , "[uint32_t]")
{
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  32>(         1,   0);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  32>(         1,   1);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  32>(         1,  16);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  32>(         1,  27);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  32>(         1,  28);

	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  32>(         8,   0);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  32>(         8,   1);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  32>(         8,  16);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  32>(         8,  27);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  32>(         8,  28);

	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  32>(       127,   0);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  32>(       127,   1);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  32>(       127,  16);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  32>(       127,  27);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  32>(       127,  28);

	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  32>(       255,   0);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  32>(       255,   1);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  32>(       255,  16);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  32>(       255,  27);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  32>(       255,  28);

	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  16>(     12345,   0);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  16>(     12345,   1);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  16>(     12345,   8);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  16>(     12345,  11);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  16>(     12345,  12);

	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  50>(     65535,   0);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  50>(     65535,   1);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  50>(     65535,  25);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  50>(     65535,  45);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t,  50>(     65535,  46);

	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t, 128>( 123456789,   0);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t, 128>( 987654321,  64);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t, 128>(4294967294, 123);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t, 128>(4294967295, 124);
}

TEST_CASE( "writing a uint32_t does only modify the specified location in the buffer"
         , "[uint32_t]" )
{
	const shift::uint32_t v = 4294967294;
	test::test_only_specified_location_in_buffer_is_modified_on_stream_out(v);
}

TEST_CASE( "when consecutively streaming uint32_t values without specifying the position the values are 'appended' "
           "to the buffer in the order they were streamed"
         , "[uint32_t]")
{
	test::test_consecutive_inserting<shift::uint32_t,shift::uint32_t>(123456789);
}

TEST_CASE( "when consecutively streaming out a source uint32_t values without specifying the position the values are "
           "extracted from the buffer in the order they are contained"
         , "[uint32_t]")
{
	test::test_consecutive_extraction<shift::uint32_t, shift::uint32_t>(987654321);
}

TEST_CASE( "encoding and decoding of uint32_t values with different endiannesses"
         , "[uint32_t]" )
{
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>(         1,  0);
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>(        13,  2);
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>(       255,  1);
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>(       256, 28);
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>(       257, 27);
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>(       258, 16);
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>(       512, 10);
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>(       513, 20);
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>(      1293, 40);
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>(      3456, 50);
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>(      5837, 60);
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>(     16000, 60);
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>(     65000,  0);
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>(     65534,  0);
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>(     65535, 60);
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>(     65536, 60);
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>( 123456789,  0);
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>( 987654321,  0);
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>(4294967294, 60);
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>(4294967295, 60);
}

TEST_CASE( "writing a uint32_t value beyond the capacity of the buffer causes an out_ot_range exception to be thrown"
         , "[uint32_t]" )
{
	test::test_out_of_range_when_inserting_beyond_static_buffer_capacity<shift::uint32_t, 64>(123456789);
}

TEST_CASE( "trying to stream out uint32_t values beyond the size of a source results in an out_of_range exception to be thrown"
         , "[uint32_t]" )
{
	test::test_out_of_range_when_extracting_beyond_static_buffer_capacity<shift::uint32_t>();
}

}} // test
