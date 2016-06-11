#include <catch.hpp>

#include <shift/buffer/static_buffer.hpp>
#include <shift/sink.hpp>

#include <test/utility.hpp>

namespace test { namespace {

TEST_CASE( "sink: when buffer_position is streamed before a data object, the passed buffer position is used to \
            place the data within the buffer"
         , "sink")
{
	const unsigned int buffer_size = 64;
	typedef shift::static_buffer<buffer_size> buffer_t;
	const shift::endianness endianness = shift::big_endian;
	typedef shift::sink<endianness, buffer_t> sink_t;

	sink_t sink;

	try {

		{
			const shift::buffer_position pos(0);
			const shift::uint16_t value = 2001;
			sink << pos << value;
			detail::check_buffer_content<endianness>(pos.byte_index, detail::to_bytes(value), sink.buffer());
		}

		{
			const shift::buffer_position pos(10);
			const shift::uint8_t value = 13;
			sink << pos << value;
			detail::check_buffer_content<endianness>(pos.byte_index, detail::to_bytes(value), sink.buffer());
		}

		{
			const shift::buffer_position pos(5);
			const shift::uint32_t value = 3333333;
			sink << pos << value;
			detail::check_buffer_content<endianness>(pos.byte_index, detail::to_bytes(value), sink.buffer());
		}
		{
			const shift::buffer_position pos(15);
			const shift::uint64_t value = 1212121212;
			sink << pos << value;
			detail::check_buffer_content<endianness>(pos.byte_index, detail::to_bytes(value), sink.buffer());
		}

	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << ", "<< 	e.file() << " line " << e.line();
	}
}

TEST_CASE( "sink: when no buffer_positions are streamed, data objects are placed within the buffer in the order \
            they were streamed"
         , "sink")
{
	const unsigned int buffer_size = 64;
	typedef shift::static_buffer<buffer_size> buffer_t;
	const shift::endianness endianness = shift::little_endian;
	typedef shift::sink<endianness, buffer_t> sink_t;

	sink_t sink;

	try {

		{
			const shift::uint16_t value = 2001;
			sink << value;
			detail::check_buffer_content<endianness>(0, detail::to_bytes(value), sink.buffer());
		}

		{
			const shift::uint8_t value = 13;
			sink << value;
			detail::check_buffer_content<endianness>(2, detail::to_bytes(value), sink.buffer());
		}

		{
			const shift::uint32_t value = 3333333;
			sink << value;
			detail::check_buffer_content<endianness>(3, detail::to_bytes(value), sink.buffer());
		}
		{
			const shift::uint64_t value = 1212121212;
			sink << value;
			detail::check_buffer_content<endianness>(7, detail::to_bytes(value), sink.buffer());
		}

		{
			const shift::uint8_t value = 5;
			sink << value;
			detail::check_buffer_content<endianness>(15, detail::to_bytes(value), sink.buffer());
		}

	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << ", "<< 	e.file() << " line " << e.line();
	}
}

TEST_CASE( "sink: the size of the buffer content is correctly updated"
         , "...")
{
	const unsigned int buffer_size = 64;
	typedef shift::static_buffer<buffer_size> buffer_t;
	typedef shift::sink<shift::little_endian, buffer_t> sink_t;

	try {
		sink_t sink;

		sink << shift::buffer_position(0) << static_cast<shift::uint8_t>(5);
		CHECK(sink.size() == 1);

		sink << shift::buffer_position(20) << static_cast<shift::uint16_t>(32000);
		CHECK(sink.size() == 22);

		sink << shift::buffer_position(22) << static_cast<shift::uint8_t>(3);
		CHECK(sink.size() == 23);

		sink << shift::buffer_position(23) << static_cast<shift::uint32_t>(42);
		CHECK(sink.size() == 27);

		sink << shift::buffer_position(27) << static_cast<shift::uint64_t>(16000000);
		CHECK(sink.size() == 35);

		sink << shift::buffer_position(10) << static_cast<shift::uint8_t>(10);
		CHECK(sink.size() == 35);

		sink << shift::buffer_position(40) << static_cast<shift::uint8_t>(42);
		CHECK(sink.size() == 41);
	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << ", "<< 	e.file() << " line " << e.line();
	}
}

TEST_CASE( "sink: data objects can be streamed with the << and the % operator"
         , "sink")
{
	test::test_stream_out_operators<shift::uint8_t , shift::uint8_t , 32>(1,  0);
	test::test_stream_out_operators<shift::uint8_t , shift::uint8_t , 32>(1,  1);
	test::test_stream_out_operators<shift::uint8_t , shift::uint8_t , 32>(1, 16);
	test::test_stream_out_operators<shift::uint8_t , shift::uint8_t , 32>(1, 30);
	test::test_stream_out_operators<shift::uint8_t , shift::uint8_t , 32>(1, 31);

	test::test_stream_out_operators<shift::uint16_t, shift::uint16_t, 16>(12345,  0);
	test::test_stream_out_operators<shift::uint16_t, shift::uint16_t, 16>(12345,  1);
	test::test_stream_out_operators<shift::uint16_t, shift::uint16_t, 16>(12345,  8);
	test::test_stream_out_operators<shift::uint16_t, shift::uint16_t, 16>(12345, 13);
	test::test_stream_out_operators<shift::uint16_t, shift::uint16_t, 16>(12345, 14);

	test::test_stream_out_operators<shift::uint32_t, shift::uint32_t, 128>( 123456789,   0);
	test::test_stream_out_operators<shift::uint32_t, shift::uint32_t, 128>( 987654321,  64);
	test::test_stream_out_operators<shift::uint32_t, shift::uint32_t, 128>(4294967294, 123);
	test::test_stream_out_operators<shift::uint32_t, shift::uint32_t, 128>(4294967295, 124);

	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 128>(10000000000000000000ULL,   0);
	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 128>(18000000000000000000ULL,  64);
	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 128>(18446744073709551614ULL, 119);
	test::test_stream_out_operators<shift::uint64_t, shift::uint64_t, 128>(18446744073709551615ULL, 120);
}

TEST_CASE( "sink: values are encoded with the specified endianness"
         , "sink")
{
	test::test_encoding_with_endianness<shift::uint16_t, shift::uint16_t>( 5837, 62);
	test::test_encoding_with_endianness<shift::uint16_t, shift::uint16_t>(16000,  0);
	test::test_encoding_with_endianness<shift::uint16_t, shift::uint16_t>(65000,  0);
	test::test_encoding_with_endianness<shift::uint16_t, shift::uint16_t>(65534, 62);
	test::test_encoding_with_endianness<shift::uint16_t, shift::uint16_t>(65535,  0);

	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>( 123456789,  0);
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>( 987654321,  0);
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>(4294967294, 60);
	test::test_encoding_with_endianness<shift::uint32_t, shift::uint32_t>(4294967295, 60);

	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(          4294967295ULL, 56);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(10000000000000000000ULL, 56);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(18000000000000000000ULL, 56);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(18446744073709551614ULL, 56);
	test::test_encoding_with_endianness<shift::uint64_t, shift::uint64_t>(18446744073709551615ULL, 56);
}

TEST_CASE( "sink: on clear() buffer content is cleared  and size is reset"
         , "...")
{
	const unsigned int buffer_size = 64;
	typedef shift::static_buffer<buffer_size> buffer_t;
	typedef shift::sink<shift::little_endian, buffer_t> sink_t;

	try {
		sink_t sink;

		sink << shift::buffer_position( 0) << static_cast<shift::uint8_t >(5);
		sink << shift::buffer_position(20) << static_cast<shift::uint16_t>(16161);
		sink << shift::buffer_position(22) << static_cast<shift::uint8_t >(33);
		sink << shift::buffer_position(23) << static_cast<shift::uint32_t>(27);
		sink << shift::buffer_position(27) << static_cast<shift::uint64_t>(16000001);
		sink << shift::buffer_position(10) << static_cast<shift::uint8_t >(127);
		sink << shift::buffer_position(40) << static_cast<shift::uint8_t >(43);

		REQUIRE(sink.buffer()[ 0] != 0);
		REQUIRE(sink.buffer()[20] != 0);
		REQUIRE(sink.buffer()[22] != 0);
		REQUIRE(sink.buffer()[23] != 0);
		REQUIRE(sink.buffer()[27] != 0);
		REQUIRE(sink.buffer()[10] != 0);
		REQUIRE(sink.buffer()[40] != 0);

		sink.clear();
		CHECK(sink.size() == 0);
		for (unsigned int i=0; i<buffer_size; ++i)
			CHECK(sink.buffer()[i] == 0);

	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << ", "<< 	e.file() << " line " << e.line();
	}
}

TEST_CASE( "sink: out_of_range is thrown when trying to 'stream' to a position beyond the capacity of the buffer \
            (static_buffer)"
         , "sink" )
{
	test::test_out_of_range_when_inserting_beyond_static_buffer_capacity<shift::uint8_t , 64>(128);
	test::test_out_of_range_when_inserting_beyond_static_buffer_capacity<shift::uint16_t, 64>(60000);
	test::test_out_of_range_when_inserting_beyond_static_buffer_capacity<shift::uint32_t, 64>(4000000);
	test::test_out_of_range_when_inserting_beyond_static_buffer_capacity<shift::uint64_t, 64>(18000000);
}

TEST_CASE( "sink: out_of_range is thrown when trying to 'stream' to a position beyond the capacity of the buffer \
            (buffer_interface)"
         , "sink")
{
	test_out_of_range_when_inserting_beyond_buffer_interface_capacity   <shift::uint8_t , 64>(128);
	test_out_of_range_when_inserting_beyond_buffer_interface_capacity   <shift::uint16_t, 64>(60000);
	test_out_of_range_when_inserting_beyond_buffer_interface_capacity   <shift::uint32_t, 64>(4000000);
	test_out_of_range_when_inserting_beyond_buffer_interface_capacity   <shift::uint64_t, 64>(18000000);
}

}} // test
