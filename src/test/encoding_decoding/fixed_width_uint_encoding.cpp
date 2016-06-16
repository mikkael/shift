#include <catch.hpp>

#include <climits>

#include <shift/source.hpp>
#include <shift/sink.hpp>
#include <shift/types/fixed_width_uint.hpp>
#include <shift/buffer/static_buffer.hpp>
#include <shift/operator/universal.hpp>
#include <shift/operator/uint.hpp>

#include <test/utility.hpp>
#include <examples/utility.hpp>

namespace test { namespace {

template<typename UintType>
UintType get_bits(const shift::byte_type* buffer, const shift::buffer_position& pos, unsigned int n_bits)
{
	UintType result = 0;

	unsigned int byte_pos = pos.byte_index;
	unsigned int bit_pos  = pos.bit_index;
	unsigned int bit = n_bits-1;

	for (unsigned int i=0; i<n_bits; ++i, --bit) {
		const UintType tmp = (buffer[byte_pos] >> bit_pos) & 1;
		result |= tmp << bit;
		if (bit_pos == 0) {
			bit_pos = 7;
			++byte_pos;
		} else {
			--bit_pos;
		}
	}
	return result;
}

template<typename UintType, unsigned int NBits>
void test_encode_decode_fixed_width_uint(const UintType value, const shift::buffer_position& pos)
{
	REQUIRE(value < (1 << NBits));

	typedef shift::static_buffer<64> buffer_t;
	typedef shift::sink<shift::little_endian, buffer_t> sink_t;
	typedef shift::source<shift::little_endian> source_t;
	typedef shift::fixed_width_uint<UintType, NBits> uint_t;

	try {
		sink_t sink;
		uint_t v(value);
		sink << pos << v;
		CHECK(get_bits<UintType>(sink.buffer(), pos, NBits) == value);

		uint_t result;
		source_t source(sink.buffer(), sink.size());
		source >> pos >> result;
		CHECK(*result == value);
	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << " "<< 	e.file() << " line " << e.line()
		          << " value: " << value << " pos: " << pos.byte_index << ", " << pos.bit_index << std::endl;
	}

	try {
		sink_t sink;
		uint_t v(value);
		sink % pos % v;
		CHECK(get_bits<UintType>(sink.buffer(), pos, NBits) == value);

		uint_t result;
		source_t source(sink.buffer(), sink.size());
		source % pos % result;
		CHECK(*result == value);
	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << " "<< 	e.file() << " line " << e.line()
		          << " value: " << value << " pos: " << pos.byte_index << ", " << pos.bit_index << std::endl;
	}
}

template<typename UintType, unsigned int NBits>
void test_encode_decode_uint_wrapper(const UintType value, const shift::buffer_position& pos)
{
	REQUIRE(value < (1 << NBits));

	typedef shift::static_buffer<64> buffer_t;
	typedef shift::sink<shift::little_endian, buffer_t> sink_t;
	typedef shift::source<shift::little_endian> source_t;
	typedef shift::fixed_width_uint<UintType, NBits> uint_t;

	try {
		sink_t sink;
		sink << pos << shift::ouint<UintType, NBits>(value);
		CHECK(get_bits<UintType>(sink.buffer(), pos, NBits) == value);

		UintType result;
		source_t source(sink.buffer(), sink.size());
		source >> pos >> shift::iuint<UintType, NBits>(result);
		CHECK(result == value);
	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << " "<< 	e.file() << " line " << e.line()
		          << " value: " << value << " pos: " << pos.byte_index << ", " << pos.bit_index << std::endl;
	}

	try {
		sink_t sink;
		sink << pos << shift::uint<NBits>(value);
		CHECK(get_bits<UintType>(sink.buffer(), pos, NBits) == value);

		UintType result;
		source_t source(sink.buffer(), sink.size());
		source >> pos >> shift::uint<NBits>(result);
		CHECK(result == value);
	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << " "<< 	e.file() << " line " << e.line()
		          << " value: " << value << " pos: " << pos.byte_index << ", " << pos.bit_index << std::endl;
	}

	try {
		sink_t sink;
		sink % pos % shift::ouint<UintType, NBits>(value);
		CHECK(get_bits<UintType>(sink.buffer(), pos, NBits) == value);

		UintType result;
		source_t source(sink.buffer(), sink.size());
		source % pos % shift::iuint<UintType, NBits>(result);
		CHECK(result == value);
	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << " "<< 	e.file() << " line " << e.line()
		          << " value: " << value << " pos: " << pos.byte_index << ", " << pos.bit_index << std::endl;
	}

	try {
		sink_t sink;
		sink % pos % shift::uint<NBits>(value);
		CHECK(get_bits<UintType>(sink.buffer(), pos, NBits) == value);

		UintType result;
		source_t source(sink.buffer(), sink.size());
		source % pos % shift::uint<NBits>(result);
		CHECK(result == value);
	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << " "<< 	e.file() << " line " << e.line()
		          << " value: " << value << " pos: " << pos.byte_index << ", " << pos.bit_index << std::endl;
	}
}

template<typename T>
T max_v(T n_bits) { return ( 1 << n_bits) - 1; }

template<typename UintType, unsigned int NumBits, typename SinkType>
void check_stream_out(SinkType& sink, UintType v, const shift::buffer_position& pos) {
	typedef shift::fixed_width_uint<UintType, NumBits> uint_t;
	const uint_t value(v);
	sink << value;
	const typename uint_t::value_type result = get_bits<typename uint_t::value_type>(sink.buffer(), pos, uint_t::num_bits);
	CHECK(result == *value);
}

template<typename UintType, unsigned int NumBits, typename SinkType, typename SourceType>
void check_stream_in(SinkType& sink, SourceType& source, UintType v, const shift::buffer_position& pos) {
	typedef shift::fixed_width_uint<UintType, NumBits> uint_t;
	const uint_t value(v);
	sink << value;
	const typename uint_t::value_type encoded = get_bits<typename uint_t::value_type>(sink.buffer(), pos, uint_t::num_bits);
	REQUIRE(encoded == *value);

	uint_t result;
	source >> result;
	CHECK((value == result));
}

TEST_CASE( "fixed_width_uint<>: encoding and decoding"
         , "fixed_width_uint<> can be streamed to a sink in specified locations using the << and the % operator"
           "fixed_width_uint<> can be streamed out of a source from specified locations using the >> and the % operator")
{
	test_encode_decode_fixed_width_uint<unsigned char,   2>(        0, shift::buffer_position( 0, 2));
	test_encode_decode_fixed_width_uint<unsigned char,   2>(        1, shift::buffer_position(17, 1));
	test_encode_decode_fixed_width_uint<unsigned char,   2>(        2, shift::buffer_position(38, 7));
	test_encode_decode_fixed_width_uint<unsigned char,   2>(max_v( 2), shift::buffer_position(63, 5));

	test_encode_decode_fixed_width_uint<unsigned char,   5>(        0, shift::buffer_position( 0, 3));
	test_encode_decode_fixed_width_uint<unsigned char,   5>(        5, shift::buffer_position(10, 4));
	test_encode_decode_fixed_width_uint<unsigned char,   5>(       17, shift::buffer_position(33, 6));
	test_encode_decode_fixed_width_uint<unsigned char,   5>(max_v( 5), shift::buffer_position(63, 7));

	test_encode_decode_fixed_width_uint<unsigned short, 11>(      999, shift::buffer_position( 0, 1));
	test_encode_decode_fixed_width_uint<unsigned short, 11>(        3, shift::buffer_position(17, 0));
	test_encode_decode_fixed_width_uint<unsigned short, 11>(       15, shift::buffer_position(23, 6));
	test_encode_decode_fixed_width_uint<unsigned short, 11>(max_v(11), shift::buffer_position(62, 2));

	test_encode_decode_fixed_width_uint<unsigned short, 13>(     7777, shift::buffer_position( 0, 1));
	test_encode_decode_fixed_width_uint<unsigned short, 13>(       55, shift::buffer_position( 5, 0));
	test_encode_decode_fixed_width_uint<unsigned short, 13>(        7, shift::buffer_position(45, 3));
	test_encode_decode_fixed_width_uint<unsigned short, 13>(max_v(13), shift::buffer_position(62, 4));

	test_encode_decode_fixed_width_uint<unsigned int,   19>(        0, shift::buffer_position( 0, 3));
	test_encode_decode_fixed_width_uint<unsigned int,   19>(   333333, shift::buffer_position(29, 0));
	test_encode_decode_fixed_width_uint<unsigned int,   19>(    12345, shift::buffer_position(33, 7));
	test_encode_decode_fixed_width_uint<unsigned int,   19>(max_v(19), shift::buffer_position(61, 2));

	test_encode_decode_fixed_width_uint<unsigned int,   27>(        3, shift::buffer_position( 0, 0));
	test_encode_decode_fixed_width_uint<unsigned int,   27>(   917291, shift::buffer_position(7,  2));
	test_encode_decode_fixed_width_uint<unsigned int,   27>(  1234567, shift::buffer_position(56, 6));
	test_encode_decode_fixed_width_uint<unsigned int,   27>(max_v(27), shift::buffer_position(60, 2));

	//

	test_encode_decode_uint_wrapper    <unsigned char,   2>(        0, shift::buffer_position( 0, 2));
	test_encode_decode_uint_wrapper    <unsigned char,   2>(        1, shift::buffer_position(17, 1));
	test_encode_decode_uint_wrapper    <unsigned char,   2>(        2, shift::buffer_position(38, 7));
	test_encode_decode_uint_wrapper    <unsigned char,   2>(max_v( 2), shift::buffer_position(63, 5));

	test_encode_decode_uint_wrapper    <unsigned char,   5>(        0, shift::buffer_position( 0, 3));
	test_encode_decode_uint_wrapper    <unsigned char,   5>(        5, shift::buffer_position(10, 4));
	test_encode_decode_uint_wrapper    <unsigned char,   5>(       17, shift::buffer_position(33, 6));
	test_encode_decode_uint_wrapper    <unsigned char,   5>(max_v( 5), shift::buffer_position(63, 7));

	test_encode_decode_uint_wrapper    <unsigned short, 11>(      999, shift::buffer_position( 0, 1));
	test_encode_decode_uint_wrapper    <unsigned short, 11>(        3, shift::buffer_position(17, 0));
	test_encode_decode_uint_wrapper    <unsigned short, 11>(       15, shift::buffer_position(23, 6));
	test_encode_decode_uint_wrapper    <unsigned short, 11>(max_v(11), shift::buffer_position(62, 2));

	test_encode_decode_uint_wrapper    <unsigned short, 13>(     7777, shift::buffer_position( 0, 1));
	test_encode_decode_uint_wrapper    <unsigned short, 13>(       55, shift::buffer_position( 5, 0));
	test_encode_decode_uint_wrapper    <unsigned short, 13>(        7, shift::buffer_position(45, 3));
	test_encode_decode_uint_wrapper    <unsigned short, 13>(max_v(13), shift::buffer_position(62, 4));

	test_encode_decode_uint_wrapper    <unsigned int,   19>(        0, shift::buffer_position( 0, 3));
	test_encode_decode_uint_wrapper    <unsigned int,   19>(   333333, shift::buffer_position(29, 0));
	test_encode_decode_uint_wrapper    <unsigned int,   19>(    12345, shift::buffer_position(33, 7));
	test_encode_decode_uint_wrapper    <unsigned int,   19>(max_v(19), shift::buffer_position(61, 2));

	test_encode_decode_uint_wrapper    <unsigned int,   27>(        3, shift::buffer_position( 0, 0));
	test_encode_decode_uint_wrapper    <unsigned int,   27>(   917291, shift::buffer_position(7,  2));
	test_encode_decode_uint_wrapper    <unsigned int,   27>(  1234567, shift::buffer_position(56, 6));
	test_encode_decode_uint_wrapper    <unsigned int,   27>(max_v(27), shift::buffer_position(60, 2));
}

TEST_CASE( "writing a fixed_width_uint<> does only modify the specified location in the buffer"
         , "[fixed_width_uint<>]" )
{
	const unsigned int buffer_size = 16;
	const unsigned int n_bits = 11;
	typedef shift::static_buffer<buffer_size> buffer_t;
	typedef shift::sink<shift::little_endian, buffer_t> sink_t;
	typedef shift::fixed_width_uint<unsigned short, n_bits> uint_t;

	try {
		sink_t sink;
		uint_t v(max_v(11));
		shift::buffer_position pos(3, 1);
		sink << pos << v;

//		example::debug_print(std::cout, sink);

		unsigned int n_bits_total = buffer_size * CHAR_BIT;
		const unsigned int start_bit = pos.byte_index * CHAR_BIT + ( 7 - pos.bit_index);

		for (unsigned int i=0; i<n_bits_total; ++i) {
			const shift::byte_type tmp = ((sink.buffer()[i/CHAR_BIT]) >> (7 - (i%8))) & 1;
			if ( i >= start_bit && i < start_bit + n_bits)
				CHECK(tmp);
			else
				CHECK(not tmp);
		}

	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << ", "<< 	e.file() << " line " << e.line();
	}
}

TEST_CASE( "when consecutively streaming fixed_width_uint<> values without specifying the position the values are 'appended' "
           "to the buffer in the order they were streamed, with each value starting at the next byte"
         , "[fixed_width_uint<>]")
{
	const unsigned int buffer_size = 32;
	typedef shift::static_buffer<buffer_size> buffer_t;
	typedef shift::sink<shift::little_endian, buffer_t> sink_t;

	sink_t sink;

	try {
		check_stream_out<unsigned short, 11, sink_t>(sink,    367, shift::buffer_position( 0, 7));
		check_stream_out<unsigned char ,  3, sink_t>(sink,      3, shift::buffer_position( 2, 7));
		check_stream_out<unsigned int  , 27, sink_t>(sink, 192891, shift::buffer_position( 3, 7));
		check_stream_out<unsigned char ,  5, sink_t>(sink,     18, shift::buffer_position( 7, 7));
		check_stream_out<unsigned short, 14, sink_t>(sink,   1234, shift::buffer_position( 8, 7));
		check_stream_out<unsigned long , 43, sink_t>(sink, 979797, shift::buffer_position(10, 7));
		check_stream_out<unsigned int  , 25, sink_t>(sink,   1164, shift::buffer_position(16, 7));
	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << ", "<< 	e.file() << " line " << e.line();
	}

//	example::debug_print(std::cout, sink);
}

TEST_CASE( "when consecutively streaming out a source fixed_width_uint<> values without specifying the position the values are "
           "extracted from the buffer in the order they are contained, with each value starting at the next byte"
         , "[fixed_width_uint<>]")
{
	const unsigned int buffer_size = 32;
	typedef shift::static_buffer<buffer_size> buffer_t;
	typedef shift::sink<shift::little_endian, buffer_t> sink_t;
	typedef shift::source<shift::little_endian> source_t;

	sink_t sink;
	source_t source(sink.buffer(), buffer_size);

	try {
		check_stream_in<unsigned short, 11, sink_t>(sink, source,     265, shift::buffer_position( 0, 7));
		check_stream_in<unsigned char ,  3, sink_t>(sink, source,       6, shift::buffer_position( 2, 7));
		check_stream_in<unsigned int  , 27, sink_t>(sink, source,  115321, shift::buffer_position( 3, 7));
		check_stream_in<unsigned char ,  5, sink_t>(sink, source,      27, shift::buffer_position( 7, 7));
		check_stream_in<unsigned short, 14, sink_t>(sink, source,    2001, shift::buffer_position( 8, 7));
		check_stream_in<unsigned long , 43, sink_t>(sink, source, 4912731, shift::buffer_position(10, 7));
		check_stream_in<unsigned int  , 25, sink_t>(sink, source,      11, shift::buffer_position(16, 7));
	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << ", "<< 	e.file() << " line " << e.line();
	}

//	example::debug_print(std::cout, sink);
}

TEST_CASE( "writing a fixed_width_uint<> value beyond the capacity of the buffer causes an out_ot_range exception to be thrown"
         , "[fixed_width_uint<>]" )
{
	typedef shift::fixed_width_uint<unsigned char, 6> uint_t;
	const unsigned int buffer_capacity = 32;
	typedef shift::static_buffer<buffer_capacity> buffer_type;
	typedef shift::sink  <shift::little_endian, buffer_type> sink_type;
	sink_type sink;
	uint_t v;

	CHECK_THROWS_AS( sink << shift::buffer_position(buffer_capacity) << v, const shift::out_of_range&);
	CHECK_THROWS_AS( sink %  shift::buffer_position(buffer_capacity) %  v, const shift::out_of_range&);
}

TEST_CASE( "trying to stream out fixed_width_uint<> values beyond the size of a source results in an out_of_range exception to be thrown"
         , "[fixed_width_uint<>]" )
{
	typedef shift::fixed_width_uint<unsigned short, 13> uint_t;
	const unsigned int buffer_capacity = 32;
	typedef shift::source <shift::little_endian> source_type;
	uint8_t buffer[buffer_capacity];
	source_type source(buffer, buffer_capacity);
	uint_t v;

	CHECK_THROWS_AS(source >> shift::buffer_position(buffer_capacity) >> v, const shift::out_of_range&);
	CHECK_THROWS_AS(source %  shift::buffer_position(buffer_capacity) %  v, const shift::out_of_range&);
}

TEST_CASE( "writing fixed_width_uint<> values to sources with different encoding endiannesses does not result in different buffer content"
         , "[fixed_width_uint<>]" )
{
	typedef shift::fixed_width_uint<unsigned short, 12> uint_t;
	const unsigned int buffer_capacity = 32;
	typedef shift::static_buffer<buffer_capacity> buffer_type;
	typedef shift::sink<shift::little_endian, buffer_type> le_sink_type;
	typedef shift::sink<shift::big_endian   , buffer_type> be_sink_type;
	le_sink_type le_sink;
	be_sink_type be_sink;
	uint_t v(687);
	const shift::buffer_position pos(11, 2);

	le_sink << pos << v;
	be_sink << pos << v;

	for (unsigned int i=0; i<buffer_capacity; ++i)
		CHECK(le_sink.buffer()[i] == be_sink.buffer()[i]);
}

}} // test
