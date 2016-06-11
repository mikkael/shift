#include <catch.hpp>

#include <shift/source.hpp>

#include <test/utility.hpp>

namespace test { namespace {

TEST_CASE( "source: when buffer_position is streamed before a data object, the passed buffer position is used to \
            extract the data from the buffer"
         , "source")
{
	const unsigned int buffer_size = 64;
	shift::byte_type buffer[buffer_size];
	const shift::endianness endianness = shift::big_endian;
	typedef shift::source<endianness> source_t;

	source_t source(buffer, buffer_size);

	try {

		{
			typedef uint8_t value_t;
			const value_t value = 17;
			const shift::buffer_position pos(1);
			detail::copy_to_buffer<endianness>(pos.byte_index, detail::to_bytes(value), buffer);
			value_t result;
			source >> pos >> result;
			CHECK(result == value);
		}

		{
			typedef uint16_t value_t;
			const value_t value = 45000;
			const shift::buffer_position pos(2);
			detail::copy_to_buffer<endianness>(pos.byte_index, detail::to_bytes(value), buffer);
			value_t result;
			source >> pos >> result;
			CHECK(result == value);
		}

		{
			typedef uint32_t value_t;
			const value_t value = 42000000;
			const shift::buffer_position pos(20);
			detail::copy_to_buffer<endianness>(pos.byte_index, detail::to_bytes(value), buffer);
			value_t result;
			source >> pos >> result;
			CHECK(result == value);
		}

		{
			typedef uint32_t value_t;
			const value_t value = 3;
			const shift::buffer_position pos(10);
			detail::copy_to_buffer<endianness>(pos.byte_index, detail::to_bytes(value), buffer);
			value_t result;
			source >> pos >> result;
			CHECK(result == value);
		}

		{
			typedef uint64_t value_t;
			const value_t value = 180000000;
			const shift::buffer_position pos(30);
			detail::copy_to_buffer<endianness>(pos.byte_index, detail::to_bytes(value), buffer);
			value_t result;
			source >> pos >> result;
			CHECK(result == value);
		}

		{
			typedef uint8_t value_t;
			const value_t value = 42;
			const shift::buffer_position pos(38);
			detail::copy_to_buffer<endianness>(pos.byte_index, detail::to_bytes(value), buffer);
			value_t result;
			source >> pos >> result;
			CHECK(result == value);
		}
	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << ", "<< 	e.file() << " line " << e.line();
	}
}

TEST_CASE( "source: when no buffer_positions are streamed, data objects are extracted from the buffer in the order \
            they are contained in the buffer"
         , "source")
{
	const unsigned int buffer_size = 64;
	shift::byte_type buffer[buffer_size];
	const shift::endianness endianness = shift::big_endian;
	typedef shift::source<endianness> source_t;

	source_t source(buffer, buffer_size);

	try {

		unsigned int pos = 0;
		{
			typedef uint8_t value_t;
			const value_t value = 13;
			detail::copy_to_buffer<endianness>(pos, detail::to_bytes(value), buffer);
			value_t result;
			source >> result;
			CHECK(result == value);
			pos += sizeof(value_t);
		}

		{
			typedef uint16_t value_t;
			const value_t value = 12121;
			detail::copy_to_buffer<endianness>(pos, detail::to_bytes(value), buffer);
			value_t result;
			source >> result;
			CHECK(result == value);
			pos += sizeof(value_t);
		}

		{
			typedef uint32_t value_t;
			const value_t value = 29000000;
			detail::copy_to_buffer<endianness>(pos, detail::to_bytes(value), buffer);
			value_t result;
			source >> result;
			CHECK(result == value);
			pos += sizeof(value_t);
		}

		{
			typedef uint32_t value_t;
			const value_t value = 767;
			detail::copy_to_buffer<endianness>(pos, detail::to_bytes(value), buffer);
			value_t result;
			source >> result;
			CHECK(result == value);
			pos += sizeof(value_t);
		}

		{
			typedef uint64_t value_t;
			const value_t value = 180000001;
			detail::copy_to_buffer<endianness>(pos, detail::to_bytes(value), buffer);
			value_t result;
			source >> result;
			CHECK(result == value);
			pos += sizeof(value_t);
		}

		{
			typedef uint8_t value_t;
			const value_t value = 89;
			detail::copy_to_buffer<endianness>(pos, detail::to_bytes(value), buffer);
			value_t result;
			source >> result;
			CHECK(result == value);
			pos += sizeof(value_t);
		}
	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << ", "<< 	e.file() << " line " << e.line();
	}
}

TEST_CASE( "source: data objects can be extracted with the >> and the % operator"
         , "source")
{
	test::test_stream_in_operators<shift::uint8_t , shift::uint8_t ,50>(255,  0);
	test::test_stream_in_operators<shift::uint8_t , shift::uint8_t ,50>(255,  1);
	test::test_stream_in_operators<shift::uint8_t , shift::uint8_t ,50>(255, 25);
	test::test_stream_in_operators<shift::uint8_t , shift::uint8_t ,50>(255, 48);
	test::test_stream_in_operators<shift::uint8_t , shift::uint8_t ,50>(255, 49);

	test::test_stream_in_operators<shift::uint16_t, shift::uint16_t, 16>(12345,  0);
	test::test_stream_in_operators<shift::uint16_t, shift::uint16_t, 16>(12345,  1);
	test::test_stream_in_operators<shift::uint16_t, shift::uint16_t, 16>(12345,  8);
	test::test_stream_in_operators<shift::uint16_t, shift::uint16_t, 16>(12345, 13);
	test::test_stream_in_operators<shift::uint16_t, shift::uint16_t, 16>(12345, 14);

	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t, 128>( 123456789,   0);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t, 128>( 987654321,  64);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t, 128>(4294967294, 123);
	test::test_stream_in_operators<shift::uint32_t, shift::uint32_t, 128>(4294967295, 124);

	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t, 128>(10000000000000000000ULL,   0);
	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t, 128>(18000000000000000000ULL,  64);
	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t, 128>(18446744073709551614ULL, 119);
	test::test_stream_in_operators<shift::uint64_t, shift::uint64_t, 128>(18446744073709551615ULL, 120);
}

TEST_CASE( "source: values are decoded with the specified endianness"
         , "source")
{
	test::test_encoding_with_endianness<shift::uint16_t, shift::uint16_t>(  123, 62);
	test::test_encoding_with_endianness<shift::uint16_t, shift::uint16_t>( 2001,  0);
	test::test_encoding_with_endianness<shift::uint16_t, shift::uint16_t>(10101,  0);
	test::test_encoding_with_endianness<shift::uint16_t, shift::uint16_t>(65534, 62);
	test::test_encoding_with_endianness<shift::uint16_t, shift::uint16_t>(   66,  0);

	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>(  12345678,  0);
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>( 732489889,  0);
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>(      1024, 60);
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>(4294967295, 60);

	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(          4294967296ULL, 56);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(  123123123123123123ULL, 56);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(                 333ULL, 56);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(                2048ULL, 56);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(18446744073709551615ULL, 56);
}

TEST_CASE( "source: out_of_range is thrown when trying to extract from a position beyond the capacity of the buffer"
         , "source" )
{
	const unsigned int buffer_size = 64;
	shift::byte_type buffer[buffer_size];
	const shift::endianness endianness = shift::little_endian;
	typedef shift::source<endianness> source_t;

	source_t source(buffer, buffer_size);

	{
		uint8_t result;
		CHECK_THROWS_AS(source >> shift::buffer_position(buffer_size) >> result, const shift::out_of_range&);
	}

	{
		uint16_t result;
		CHECK_THROWS_AS(source >> shift::buffer_position(buffer_size - 1) >> result, const shift::out_of_range&);
	}

	{
		uint32_t result;
		CHECK_THROWS_AS(source >> shift::buffer_position(buffer_size - 3) >> result, const shift::out_of_range&);
	}

	{
		uint64_t result;
		CHECK_THROWS_AS(source >> shift::buffer_position(buffer_size - 7) >> result, const shift::out_of_range&);
	}
}

}} // test
