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

TEST_CASE( "float values can be streamed to a sink in specified locations using the << and the % operator"
         , "[float]" )
{
	const float max                    = std::numeric_limits<float>::max();
	const float smallest_representable = std::numeric_limits<float>::min();
	const float inf                    = std::numeric_limits<float>::infinity();
	const float nan                    = std::numeric_limits<float>::quiet_NaN();

	test::test_stream_out_operators<float, shift::uint32_t, 32>(                   0.f,   0);
	test::test_stream_out_operators<float, shift::uint32_t, 32>(                   0.f,   1);
	test::test_stream_out_operators<float, shift::uint32_t, 32>(                   0.f,  16);
	test::test_stream_out_operators<float, shift::uint32_t, 32>(                   0.f,  27);
	test::test_stream_out_operators<float, shift::uint32_t, 32>(                   0.f,  28);

	test::test_stream_out_operators<float, shift::uint32_t, 32>(                   1.f,   0);
	test::test_stream_out_operators<float, shift::uint32_t, 32>(                   1.f,   1);
	test::test_stream_out_operators<float, shift::uint32_t, 32>(                   1.f,  16);
	test::test_stream_out_operators<float, shift::uint32_t, 32>(                   1.f,  27);
	test::test_stream_out_operators<float, shift::uint32_t, 32>(                   1.f,  28);

	test::test_stream_out_operators<float, shift::uint32_t, 16>(        3.14159265359f,   0);
	test::test_stream_out_operators<float, shift::uint32_t, 16>(        3.14159265359f,   1);
	test::test_stream_out_operators<float, shift::uint32_t, 16>(        3.14159265359f,   8);
	test::test_stream_out_operators<float, shift::uint32_t, 16>(        3.14159265359f,  11);
	test::test_stream_out_operators<float, shift::uint32_t, 16>(        3.14159265359f,  12);

	test::test_stream_out_operators<float, shift::uint32_t, 50>(                   max,  0);
	test::test_stream_out_operators<float, shift::uint32_t, 50>(                   max,  1);
	test::test_stream_out_operators<float, shift::uint32_t, 50>(                   max, 25);
	test::test_stream_out_operators<float, shift::uint32_t, 50>(                   max, 45);
	test::test_stream_out_operators<float, shift::uint32_t, 50>(                   max, 46);

	test::test_stream_out_operators<float, shift::uint32_t, 64>(smallest_representable,  0);
	test::test_stream_out_operators<float, shift::uint32_t, 64>(smallest_representable,  1);
	test::test_stream_out_operators<float, shift::uint32_t, 64>(smallest_representable, 32);
	test::test_stream_out_operators<float, shift::uint32_t, 64>(smallest_representable, 59);
	test::test_stream_out_operators<float, shift::uint32_t, 64>(smallest_representable, 60);

	test::test_stream_out_operators<float, shift::uint32_t, 8>(                    inf,  0);
	test::test_stream_out_operators<float, shift::uint32_t, 8>(                    inf,  1);
	test::test_stream_out_operators<float, shift::uint32_t, 8>(                    inf,  2);
	test::test_stream_out_operators<float, shift::uint32_t, 8>(                    inf,  3);
	test::test_stream_out_operators<float, shift::uint32_t, 8>(                    inf,  4);

	test::test_stream_out_operators<float, shift::uint32_t, 50>(                   nan,  0);
	test::test_stream_out_operators<float, shift::uint32_t, 50>(                   nan,  1);
	test::test_stream_out_operators<float, shift::uint32_t, 50>(                   nan, 25);
	test::test_stream_out_operators<float, shift::uint32_t, 50>(                   nan, 45);
	test::test_stream_out_operators<float, shift::uint32_t, 50>(                   nan, 46);
}

TEST_CASE( "float values can be streamed out of a source from specified locations using the >> and the % operator"
         , "[float]")
{
	const float max                    = std::numeric_limits<float>::max();
	const float smallest_representable = std::numeric_limits<float>::min();
	const float inf                    = std::numeric_limits<float>::infinity();

	test::test_stream_in_operators<float, shift::uint32_t, 32>(                   0.f,   0);
	test::test_stream_in_operators<float, shift::uint32_t, 32>(                   0.f,   1);
	test::test_stream_in_operators<float, shift::uint32_t, 32>(                   0.f,  16);
	test::test_stream_in_operators<float, shift::uint32_t, 32>(                   0.f,  27);
	test::test_stream_in_operators<float, shift::uint32_t, 32>(                   0.f,  28);

	test::test_stream_in_operators<float, shift::uint32_t, 32>(                   1.f,   0);
	test::test_stream_in_operators<float, shift::uint32_t, 32>(                   1.f,   1);
	test::test_stream_in_operators<float, shift::uint32_t, 32>(                   1.f,  16);
	test::test_stream_in_operators<float, shift::uint32_t, 32>(                   1.f,  27);
	test::test_stream_in_operators<float, shift::uint32_t, 32>(                   1.f,  28);

	test::test_stream_in_operators<float, shift::uint32_t, 16>(        3.14159265359f,   0);
	test::test_stream_in_operators<float, shift::uint32_t, 16>(        3.14159265359f,   1);
	test::test_stream_in_operators<float, shift::uint32_t, 16>(        3.14159265359f,   8);
	test::test_stream_in_operators<float, shift::uint32_t, 16>(        3.14159265359f,  11);
	test::test_stream_in_operators<float, shift::uint32_t, 16>(        3.14159265359f,  12);

	test::test_stream_in_operators<float, shift::uint32_t, 8>(          0.5772156649f,  0);
	test::test_stream_in_operators<float, shift::uint32_t, 8>(          0.5772156649f,  1);
	test::test_stream_in_operators<float, shift::uint32_t, 8>(          0.5772156649f,  2);
	test::test_stream_in_operators<float, shift::uint32_t, 8>(          0.5772156649f,  3);
	test::test_stream_in_operators<float, shift::uint32_t, 8>(          0.5772156649f,  4);

	test::test_stream_in_operators<float, shift::uint32_t, 50>(                   max,  0);
	test::test_stream_in_operators<float, shift::uint32_t, 50>(                   max,  1);
	test::test_stream_in_operators<float, shift::uint32_t, 50>(                   max, 25);
	test::test_stream_in_operators<float, shift::uint32_t, 50>(                   max, 45);
	test::test_stream_in_operators<float, shift::uint32_t, 50>(                   max, 46);

	test::test_stream_in_operators<float, shift::uint32_t, 64>(smallest_representable,  0);
	test::test_stream_in_operators<float, shift::uint32_t, 64>(smallest_representable,  1);
	test::test_stream_in_operators<float, shift::uint32_t, 64>(smallest_representable, 32);
	test::test_stream_in_operators<float, shift::uint32_t, 64>(smallest_representable, 59);
	test::test_stream_in_operators<float, shift::uint32_t, 64>(smallest_representable, 60);

	test::test_stream_in_operators<float, shift::uint32_t, 8>(                    inf,  0);
	test::test_stream_in_operators<float, shift::uint32_t, 8>(                    inf,  1);
	test::test_stream_in_operators<float, shift::uint32_t, 8>(                    inf,  2);
	test::test_stream_in_operators<float, shift::uint32_t, 8>(                    inf,  3);
	test::test_stream_in_operators<float, shift::uint32_t, 8>(                    inf,  4);
}

TEST_CASE( "writing a float does only modify the specified location in the buffer"
         , "[float]" )
{
	const float v = std::numeric_limits<float>::max();
	test::test_only_specified_location_in_buffer_is_modified_on_stream_out(v);
}


TEST_CASE( "when consecutively streaming float values without specifying the position the values are 'appended' "
           "to the buffer in the order they were streamed"
         , "[float]")
{
	test::test_consecutive_inserting<float, shift::uint32_t>(3.14);
}

TEST_CASE( "when consecutively streaming out a source float values without specifying the position the values are "
           "extracted from the buffer in the order they are contained"
         , "[float]")
{
	test::test_consecutive_extraction<float, shift::uint32_t>(-999.9);
}

TEST_CASE( "encoding and decoding of float values with different endiannesses"
         , "[float]" )
{
	const float max                    = std::numeric_limits<float>::max();
	const float smallest_representable = std::numeric_limits<float>::min();
	const float inf                    = std::numeric_limits<float>::infinity();

	test::test_encoding_with_endianness<float, shift::uint32_t>(-                    1,  0);
	test::test_encoding_with_endianness<float, shift::uint32_t>(-                   13,  1);
	test::test_encoding_with_endianness<float, shift::uint32_t>(        3.14159265359f, 16);
	test::test_encoding_with_endianness<float, shift::uint32_t>(         0.5772156649f, 10);
	test::test_encoding_with_endianness<float, shift::uint32_t>(                   0.f, 20);
	test::test_encoding_with_endianness<float, shift::uint32_t>(                   max, 27);
	test::test_encoding_with_endianness<float, shift::uint32_t>(               max-1.f, 27);
	test::test_encoding_with_endianness<float, shift::uint32_t>(               max/2.f, 27);
	test::test_encoding_with_endianness<float, shift::uint32_t>(                   inf, 27);
	test::test_encoding_with_endianness<float, shift::uint32_t>(smallest_representable, 28);
}

TEST_CASE( "writing a float value beyond the capacity of the buffer causes an out_ot_range exception to be thrown"
         , "[float]" )
{
	test::test_out_of_range_when_inserting_beyond_static_buffer_capacity<float, 64>(3.14);
}

TEST_CASE( "trying to stream out float values beyond the size of a source results in an out_of_range exception to be thrown"
         , "[float]" )
{
	test::test_out_of_range_when_extracting_beyond_static_buffer_capacity<float>();
}


}} // test
