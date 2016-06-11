#include <catch.hpp>

#include <iostream>
#include <cstring>

#include <shift/sink.hpp>
#include <shift/source.hpp>
#include <shift/operator/noop.hpp>
#include <shift/operator/universal.hpp>

#include <test/utility.hpp>

namespace test { namespace {

TEST_CASE( "noop does not alter the state of a sink, << and %"
         , "noop, <<, %" )
{
	const unsigned int buffer_capacity = 32;
	typedef shift::static_buffer<buffer_capacity> buffer_type;
	typedef shift::sink  <shift::little_endian, buffer_type> sink_type;

	sink_type sink;

	try {
		REQUIRE(sink.size() == 0);
		sink << shift::noop();
		CHECK(sink.size() == 0);
		for (unsigned int i=0; i<buffer_capacity; ++i)
			sink.buffer()[i] == 0;
	} catch (shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	try {
		REQUIRE(sink.size() == 0);
		sink % shift::noop();
		CHECK(sink.size() == 0);
		for (unsigned int i=0; i<buffer_capacity; ++i)
			sink.buffer()[i] == 0;
	} catch (shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}
}

TEST_CASE( "noop does not alter the state of a source, >> and %"
         , "noop, >>, %" )
{
	typedef shift::source<shift::little_endian> source_type;

	const unsigned int buffer_capacity = 32;
	shift::byte_type buffer[buffer_capacity];
	std::memset(buffer, 0, buffer_capacity);
	buffer[0] = 1;

	try {
		source_type source(buffer, buffer_capacity);
		shift::uint8_t v=0;

		source >> shift::noop();
		source >> v;

		CHECK(v == 1);
	} catch (shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	try {
		source_type source(buffer, buffer_capacity);
		shift::uint8_t v=0;

		source % shift::noop();
		source % v;

		CHECK(v == 1);
	} catch (shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}
}

}}
