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

TEST_CASE( "uint64_t values can be streamed to a sink in specified locations using the << and the % operator"
         , "[uint64_t]" )
{
	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 32>(                    1ULL,   0);
	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 32>(                    1ULL,   1);
	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 32>(                    1ULL,  16);
	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 32>(                    1ULL,  23);
	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 32>(                    1ULL,  24);

	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 16>(                12345ULL,   0);
	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 16>(                12345ULL,   1);
	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 16>(                12345ULL,   8);
	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 16>(                12345ULL,   7);
	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 16>(                12345ULL,   8);

	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 50>(                65535ULL,   0);
	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 50>(                65535ULL,   1);
	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 50>(                65535ULL,  25);
	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 50>(                65535ULL,  41);
	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 50>(                65535ULL,  42);

	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 128>(           123456789ULL,   0);
	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 128>(           987654321ULL,  64);
	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 128>(          4294967294ULL, 119);
	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 128>(          4294967295ULL, 120);

	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 128>(10000000000000000000ULL,   0);
	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 128>(18000000000000000000ULL,  64);
	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 128>(18446744073709551614ULL, 119);
	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 128>(18446744073709551615ULL, 120);
}

TEST_CASE( "uint64_t values can be streamed out of a source from specified locations using the >> and the % operator"
         , "[uint64_t]")
{
	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t,32>(                    1ULL,   0);
	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t,32>(                    1ULL,   1);
	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t,32>(                    1ULL,  16);
	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t,32>(                    1ULL,  23);
	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t,32>(                    1ULL,  24);

	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t,16>(                12345ULL,   0);
	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t,16>(                12345ULL,   1);
	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t,16>(                12345ULL,   8);
	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t,16>(                12345ULL,   7);
	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t,16>(                12345ULL,   8);

	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t,50>(                65535ULL,   0);
	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t,50>(                65535ULL,   1);
	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t,50>(                65535ULL,  25);
	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t,50>(                65535ULL,  41);
	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t,50>(                65535ULL,  42);

	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t,128>(           123456789ULL,   0);
	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t,128>(           987654321ULL,  64);
	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t,128>(          4294967294ULL, 119);
	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t,128>(          4294967295ULL, 120);

	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t,128>(10000000000000000000ULL,   0);
	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t,128>(18000000000000000000ULL,  64);
	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t,128>(18446744073709551614ULL, 119);
	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t,128>(18446744073709551615ULL, 120);
}

TEST_CASE( "writing a uint64_t does only modify the specified location in the buffer"
         , "[uint64_t]" )
{
	const shift::uint64_t v = 18446744073709551614ULL;
	test::test_only_specified_location_in_buffer_is_modified_on_stream_out(v);
}

TEST_CASE( "when consecutively streaming uint64_t values without specifying the position the values are 'appended' "
           "to the buffer in the order they were streamed"
         , "[uint64_t]")
{
	test::test_consecutive_inserting<shift::uint64_t, shift::uint64_t>(123456789);
}

TEST_CASE( "when consecutively streaming out a source uint64_t values without specifying the position the values are "
           "extracted from the buffer in the order they are contained"
         , "[uint64_t]")
{
	test::test_consecutive_extraction<shift::uint64_t, shift::uint64_t>(10000000000000000000ULL);
}

TEST_CASE( "encoding and decoding of uint64_t values with different endiannesses"
         , "[uint64_t]" )
{
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(                   1ULL,  0);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(                  13ULL,  2);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(                 255ULL,  1);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(                 256ULL, 28);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(                 257ULL, 27);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(                 258ULL, 16);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(                 512ULL, 10);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(                 513ULL, 20);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(                1293ULL, 40);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(                3456ULL, 50);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(                5837ULL, 56);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(               16000ULL, 56);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(               65000ULL,  0);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(               65534ULL,  0);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(               65535ULL, 56);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(               65536ULL, 56);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(           123456789ULL,  0);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(           987654321ULL,  0);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(          4294967294ULL, 56);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(          4294967295ULL, 56);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(10000000000000000000ULL, 56);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(18000000000000000000ULL, 56);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(18446744073709551614ULL, 56);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(18446744073709551615ULL, 56);
}

TEST_CASE( "writing a uint64_t value beyond the capacity of the buffer causes an out_ot_range exception to be thrown"
         , "[uint64_t]" )
{
	test::test_out_of_range_when_inserting_beyond_static_buffer_capacity<shift::uint64_t, 64>(123456789);
}

TEST_CASE( "trying to stream out uint64_t values beyond the size of a source results in an out_of_range exception to be thrown"
         , "[uint64_t]" )
{
	test::test_out_of_range_when_extracting_beyond_static_buffer_capacity<shift::uint64_t>();
}

}} // test
