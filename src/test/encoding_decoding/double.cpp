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

TEST_CASE( "double values can be streamed to a sink in specified locations using the << and the % operator"
         , "[double]" )
{
	const double max                    = std::numeric_limits<double>::max();
	const double smallest_representable = std::numeric_limits<double>::min();
	const double inf                    = std::numeric_limits<double>::infinity();
	const double nan                    = std::numeric_limits<double>::quiet_NaN();

	test::test_stream_out_operators<double, shift::uint64_t, 32>(                   0.f,   0);
	test::test_stream_out_operators<double, shift::uint64_t, 32>(                   0.f,   1);
	test::test_stream_out_operators<double, shift::uint64_t, 32>(                   0.f,  16);
	test::test_stream_out_operators<double, shift::uint64_t, 32>(                   0.f,  23);
	test::test_stream_out_operators<double, shift::uint64_t, 32>(                   0.f,  24);

	test::test_stream_out_operators<double, shift::uint64_t, 32>(                   1.f,   0);
	test::test_stream_out_operators<double, shift::uint64_t, 32>(                   1.f,   1);
	test::test_stream_out_operators<double, shift::uint64_t, 32>(                   1.f,  16);
	test::test_stream_out_operators<double, shift::uint64_t, 32>(                   1.f,  23);
	test::test_stream_out_operators<double, shift::uint64_t, 32>(                   1.f,  24);

	test::test_stream_out_operators<double, shift::uint64_t, 16>(        3.14159265359f,   0);
	test::test_stream_out_operators<double, shift::uint64_t, 16>(        3.14159265359f,   1);
	test::test_stream_out_operators<double, shift::uint64_t, 16>(        3.14159265359f,   4);
	test::test_stream_out_operators<double, shift::uint64_t, 16>(        3.14159265359f,   7);
	test::test_stream_out_operators<double, shift::uint64_t, 16>(        3.14159265359f,   8);

	test::test_stream_out_operators<double, shift::uint64_t, 50>(                   max,  0);
	test::test_stream_out_operators<double, shift::uint64_t, 50>(                   max,  1);
	test::test_stream_out_operators<double, shift::uint64_t, 50>(                   max, 25);
	test::test_stream_out_operators<double, shift::uint64_t, 50>(                   max, 41);
	test::test_stream_out_operators<double, shift::uint64_t, 50>(                   max, 42);

	test::test_stream_out_operators<double, shift::uint64_t, 64>(smallest_representable,  0);
	test::test_stream_out_operators<double, shift::uint64_t, 64>(smallest_representable,  1);
	test::test_stream_out_operators<double, shift::uint64_t, 64>(smallest_representable, 32);
	test::test_stream_out_operators<double, shift::uint64_t, 64>(smallest_representable, 55);
	test::test_stream_out_operators<double, shift::uint64_t, 64>(smallest_representable, 56);

	test::test_stream_out_operators<double, shift::uint64_t, 13>(                    inf,  0);
	test::test_stream_out_operators<double, shift::uint64_t, 13>(                    inf,  1);
	test::test_stream_out_operators<double, shift::uint64_t, 13>(                    inf,  2);
	test::test_stream_out_operators<double, shift::uint64_t, 13>(                    inf,  4);
	test::test_stream_out_operators<double, shift::uint64_t, 13>(                    inf,  5);

	test::test_stream_out_operators<double, shift::uint64_t, 50>(                   nan,  0);
	test::test_stream_out_operators<double, shift::uint64_t, 50>(                   nan,  1);
	test::test_stream_out_operators<double, shift::uint64_t, 50>(                   nan, 25);
	test::test_stream_out_operators<double, shift::uint64_t, 50>(                   nan, 41);
	test::test_stream_out_operators<double, shift::uint64_t, 50>(                   nan, 42);
}

TEST_CASE( "double values can be streamed out of a source from specified locations using the >> and the % operator"
         , "[double]")
{
	const double max                    = std::numeric_limits<double>::max();
	const double smallest_representable = std::numeric_limits<double>::min();
	const double inf                    = std::numeric_limits<double>::infinity();

	test::test_stream_in_operators<double, shift::uint64_t, 32>(                  0.f,   0);
	test::test_stream_in_operators<double, shift::uint64_t, 32>(                  0.f,   1);
	test::test_stream_in_operators<double, shift::uint64_t, 32>(                  0.f,  16);
	test::test_stream_in_operators<double, shift::uint64_t, 32>(                  0.f,  23);
	test::test_stream_in_operators<double, shift::uint64_t, 32>(                  0.f,  24);

	test::test_stream_in_operators<double, shift::uint64_t, 32>(                  1.f,   0);
	test::test_stream_in_operators<double, shift::uint64_t, 32>(                  1.f,   1);
	test::test_stream_in_operators<double, shift::uint64_t, 32>(                  1.f,  16);
	test::test_stream_in_operators<double, shift::uint64_t, 32>(                  1.f,  23);
	test::test_stream_in_operators<double, shift::uint64_t, 32>(                  1.f,  24);

	test::test_stream_in_operators<double, shift::uint64_t, 16>(       3.14159265359f,   0);
	test::test_stream_in_operators<double, shift::uint64_t, 16>(       3.14159265359f,   1);
	test::test_stream_in_operators<double, shift::uint64_t, 16>(       3.14159265359f,   4);
	test::test_stream_in_operators<double, shift::uint64_t, 16>(       3.14159265359f,   7);
	test::test_stream_in_operators<double, shift::uint64_t, 16>(       3.14159265359f,   8);

	test::test_stream_in_operators<double, shift::uint64_t, 17>(         0.5772156649f,  0);
	test::test_stream_in_operators<double, shift::uint64_t, 17>(         0.5772156649f,  1);
	test::test_stream_in_operators<double, shift::uint64_t, 17>(         0.5772156649f,  2);
	test::test_stream_in_operators<double, shift::uint64_t, 17>(         0.5772156649f,  7);
	test::test_stream_in_operators<double, shift::uint64_t, 17>(         0.5772156649f,  9);

	test::test_stream_in_operators<double, shift::uint64_t, 50>(                   max,  0);
	test::test_stream_in_operators<double, shift::uint64_t, 50>(                   max,  1);
	test::test_stream_in_operators<double, shift::uint64_t, 50>(                   max, 25);
	test::test_stream_in_operators<double, shift::uint64_t, 50>(                   max, 41);
	test::test_stream_in_operators<double, shift::uint64_t, 50>(                   max, 42);

	test::test_stream_in_operators<double, shift::uint64_t, 64>(smallest_representable,  0);
	test::test_stream_in_operators<double, shift::uint64_t, 64>(smallest_representable,  1);
	test::test_stream_in_operators<double, shift::uint64_t, 64>(smallest_representable, 32);
	test::test_stream_in_operators<double, shift::uint64_t, 64>(smallest_representable, 55);
	test::test_stream_in_operators<double, shift::uint64_t, 64>(smallest_representable, 56);

	test::test_stream_in_operators<double, shift::uint64_t, 19>(                   inf,  0);
	test::test_stream_in_operators<double, shift::uint64_t, 19>(                   inf,  1);
	test::test_stream_in_operators<double, shift::uint64_t, 19>(                   inf,  2);
	test::test_stream_in_operators<double, shift::uint64_t, 19>(                   inf, 10);
	test::test_stream_in_operators<double, shift::uint64_t, 19>(                   inf, 11);
}

TEST_CASE( "writing a double does only modify the specified location in the buffer"
         , "[double]" )
{
	const double v = std::numeric_limits<double>::max();
	test::test_only_specified_location_in_buffer_is_modified_on_stream_out(v);
}


TEST_CASE( "when consecutively streaming double values without specifying the position the values are 'appended' "
           "to the buffer in the order they were streamed"
         , "[double]")
{
	test::test_consecutive_inserting<double, shift::uint64_t>(3.14);
}

TEST_CASE( "when consecutively streaming out a source double values without specifying the position the values are "
           "extracted from the buffer in the order they are contained"
         , "[double]")
{
	test::test_consecutive_extraction<double, shift::uint64_t>(-999.9);
}

TEST_CASE( "encoding and decoding of double values with different endiannesses"
         , "[double]" )
{
	const double max                    = std::numeric_limits<double>::max();
	const double smallest_representable = std::numeric_limits<double>::min();
	const double inf                    = std::numeric_limits<double>::infinity();

	test::test_encoding_with_endianness<double, shift::uint64_t>(-                    1,  0);
	test::test_encoding_with_endianness<double, shift::uint64_t>(-                   13,  1);
	test::test_encoding_with_endianness<double, shift::uint64_t>(        3.14159265359f, 16);
	test::test_encoding_with_endianness<double, shift::uint64_t>(         0.5772156649f, 10);
	test::test_encoding_with_endianness<double, shift::uint64_t>(                   0.f, 20);
	test::test_encoding_with_endianness<double, shift::uint64_t>(                   max, 27);
	test::test_encoding_with_endianness<double, shift::uint64_t>(               max-1.f, 27);
	test::test_encoding_with_endianness<double, shift::uint64_t>(               max/2.f, 27);
	test::test_encoding_with_endianness<double, shift::uint64_t>(                   inf, 27);
	test::test_encoding_with_endianness<double, shift::uint64_t>(smallest_representable, 28);
}

TEST_CASE( "writing a double value beyond the capacity of the buffer causes an out_ot_range exception to be thrown"
         , "[double]" )
{
	test::test_out_of_range_when_inserting_beyond_static_buffer_capacity<double, 64>(3.14);
}

TEST_CASE( "trying to stream out double values beyond the size of a source results in an out_of_range exception to be thrown"
         , "[double]" )
{
	test::test_out_of_range_when_extracting_beyond_static_buffer_capacity<double>();
}

}} // test
