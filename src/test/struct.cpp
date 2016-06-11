#include <catch.hpp>

#include <shift/source.hpp>
#include <shift/sink.hpp>
#include <shift/types/fixed_width_uint.hpp>
#include <shift/buffer/static_buffer.hpp>
#include <shift/operator/universal.hpp>
#include <shift/types/cstdint.hpp>

#include <examples/utility.hpp>
#include <test/utility.hpp>

namespace test { namespace {

struct test_struct {

	static const unsigned int size = 9;

	test_struct()
	: t1()
	, t2()
	, t3()
	, t4()
	, t5()
	, t6()
	{}

	test_struct( unsigned int    p1
	           , unsigned int    p2
	           , unsigned int    p3
	           , bool            p4
	           , shift::uint16_t p5
	           , shift::uint32_t p6)
	: t1(p1)
	, t2(p2)
	, t3(p3)
	, t4(p4)
	, t5(p5)
	, t6(p6)
	{}

	shift::uint8_t  t1;
	shift::uint2_t  t2;
	shift::uint3_t  t3;
	bool            t4;
	shift::uint16_t t5;
	shift::uint32_t t6;
};

template<typename MessageType>
MessageType& serialize(MessageType& stream, test_struct& obj) {
	typedef typename MessageType::mode mode;
	stream % shift::buffer_position(0   ) % obj.t1
	       % shift::buffer_position(1, 7) % obj.t2
	       % shift::buffer_position(1, 2) % obj.t3
	       % shift::buffer_position(2, 3) % obj.t4
	       % shift::buffer_position(3   ) % obj.t5
	       % shift::buffer_position(5   ) % obj.t6;
	return stream;
}

template<typename MessageType>
MessageType& operator >> (MessageType& source, test_struct& obj) {
	source >> shift::buffer_position(0   ) >> obj.t1
	       >> shift::buffer_position(1, 7) >> obj.t2
	       >> shift::buffer_position(1, 2) >> obj.t3
	       >> shift::buffer_position(2, 3) >> obj.t4
	       >> shift::buffer_position(3   ) >> obj.t5
	       >> shift::buffer_position(5   ) >> obj.t6;
	return source;
}

template<typename MessageType>
MessageType& operator << (MessageType& sink, const test_struct& obj) {
	sink << shift::buffer_position(0   ) << obj.t1
	     << shift::buffer_position(1, 7) << obj.t2
	     << shift::buffer_position(1, 2) << obj.t3
	     << shift::buffer_position(2, 3) << obj.t4
	     << shift::buffer_position(3   ) << obj.t5
	     << shift::buffer_position(5   ) << obj.t6;
	return sink;
}

TEST_CASE( "overloading of stream out operator for user defined types"
         , "<< user defined type")
{
	const shift::endianness endianness = shift::big_endian;
	typedef shift::static_buffer<test_struct::size> buffer_t;
	typedef shift::sink<endianness, buffer_t> sink_t;

	sink_t sink;

	const shift::uint8_t  t1(127);
	const unsigned int    t2(1);
	const unsigned int    t3(5);
	const bool            t4(false);
	const shift::uint16_t t5(60123);
	const shift::uint32_t t6(40123456);

	test_struct obj(t1, t2, t3, t4, t5, t6);

	try {
		sink << obj;

		CHECK(sink.size() == test_struct::size);
		CHECK(sink.buffer()[0] == t1);
		CHECK(((sink.buffer()[1] >> 6) == t2));
		CHECK(((sink.buffer()[1]  & 7) == t3));
		CHECK(((sink.buffer()[2] >> 3) & 1) == t4);

		const std::vector<shift::byte_type> bytes_t5 = detail::to_bytes(t5);
		detail::check_buffer_content<endianness>(3, bytes_t5, sink.buffer());

		const std::vector<shift::byte_type> bytes_t6 = detail::to_bytes(t6);
		detail::check_buffer_content<endianness>(5, bytes_t6, sink.buffer());
	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << ", "<< 	e.file() << " line " << e.line();
	}

//	example::debug_print(std::cout, sink);
}

TEST_CASE( "overloading of stream in operator for user defined types"
         , ">> user defined type")
{
	const shift::endianness endianness = shift::big_endian;
	typedef shift::source<endianness> source_t;

	shift::byte_type buffer[test_struct::size];

	const shift::uint8_t  t1(99);
	const unsigned int    t2(2);
	const unsigned int    t3(7);
	const bool            t4(true);
	const shift::uint16_t t5(63900);
	const shift::uint32_t t6(40212345);

	buffer[0]  = t1;
	buffer[1]  = t2 << 6;
	buffer[1] |= t3;
	buffer[2]  = t4 << 3;

	detail::copy_to_buffer<endianness>(3, detail::to_bytes(t5), buffer);
	detail::copy_to_buffer<endianness>(5, detail::to_bytes(t6), buffer);

	source_t source(buffer, test_struct::size);
	test_struct obj;

	try {
		source >> obj;

		CHECK((obj.t1 == t1));
		CHECK((obj.t2 == t2));
		CHECK((obj.t3 == t3));
		CHECK((obj.t4 == t4));
		CHECK((obj.t5 == t5));
		CHECK((obj.t6 == t6));
	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << ", "<< 	e.file() << " line " << e.line();
	}
}

TEST_CASE( "overloading of universal operator for user defined types: stream out"
         , "% user defined type")
{
	const shift::endianness endianness = shift::little_endian;
	typedef shift::static_buffer<test_struct::size> buffer_t;
	typedef shift::sink<endianness, buffer_t> sink_t;

	sink_t sink;

	const shift::uint8_t  t1(201);
	const unsigned int    t2(3);
	const unsigned int    t3(6);
	const bool            t4(true);
	const shift::uint16_t t5(601);
	const shift::uint32_t t6(40123);

	test_struct obj(t1, t2, t3, t4, t5, t6);

	try {
		sink % obj;

		CHECK(sink.size() == test_struct::size);
		CHECK(sink.buffer()[0] == t1);
		CHECK(((sink.buffer()[1] >> 6) == t2));
		CHECK(((sink.buffer()[1]  & 7) == t3));
		CHECK(((sink.buffer()[2] >> 3) & 1) == t4);

		const std::vector<shift::byte_type> bytes_t5 = detail::to_bytes(t5);
		detail::check_buffer_content<endianness>(3, bytes_t5, sink.buffer());

		const std::vector<shift::byte_type> bytes_t6 = detail::to_bytes(t6);
		detail::check_buffer_content<endianness>(5, bytes_t6, sink.buffer());
	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << ", "<< 	e.file() << " line " << e.line();
	}

//	example::debug_print(std::cout, sink);
}

TEST_CASE( "overloading of universal operator for user defined types: stream in"
         , "% user defined type")
{
	const shift::endianness endianness = shift::little_endian;
	typedef shift::source<endianness> source_t;

	shift::byte_type buffer[test_struct::size];

	const shift::uint8_t  t1(33);
	const unsigned int    t2(0);
	const unsigned int    t3(4);
	const bool            t4(false);
	const shift::uint16_t t5(12345);
	const shift::uint32_t t6(821365);

	buffer[0]  = t1;
	buffer[1]  = t2 << 6;
	buffer[1] |= t3;
	buffer[2]  = t4 << 3;

	detail::copy_to_buffer<endianness>(3, detail::to_bytes(t5), buffer);
	detail::copy_to_buffer<endianness>(5, detail::to_bytes(t6), buffer);

	source_t source(buffer, test_struct::size);
	test_struct obj;

	try {
		source >> obj;

		CHECK((obj.t1 == t1));
		CHECK((obj.t2 == t2));
		CHECK((obj.t3 == t3));
		CHECK((obj.t4 == t4));
		CHECK((obj.t5 == t5));
		CHECK((obj.t6 == t6));
	} catch (const shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << ", "<< 	e.file() << " line " << e.line();
	}
}

}} // test
