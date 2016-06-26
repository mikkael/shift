#include <catch.hpp>

#include <iostream>

#include <shift/sink.hpp>
#include <shift/source.hpp>
#include <shift/buffer/static_buffer.hpp>
#include <shift/operator/universal.hpp>
#include <shift/operator/uint.hpp>
#include <shift/utility/argument_traits.hpp>
#include <shift/detail/static_assert.hpp>

#include <examples/utility.hpp>

namespace test { namespace {

struct t_struct{
	t_struct(shift::uint8_t v1 =0, shift::uint16_t v2=0) : v1(v1), v2(v2){}
	shift::uint8_t  v1;
	shift::uint16_t v2;

	void print() {
		std::cout << "v1: " << (int)v1 << ", v2: " << v2 << std::endl;
	}
};


// depending on the mode of the stream (input for sources / output for sinks) the argument type
// will either be "t_struct&" for sources or "const t_struct&" for sinks

template<typename StreamType>
void serialize( StreamType& stream
              , typename shift::argument_traits<typename StreamType::mode, t_struct>::type arg ) {
	stream % shift::pos(0, 2) % shift::uint<3>(arg.v1)
	       % shift::pos(1   ) % arg.v2;
}


TEST_CASE( "argument_traits<...> free serialize function: this is rather a 'does compile' test"
         , "argument_traits<...>")
{
	typedef shift::sink<shift::little_endian, shift::static_buffer<64> > sink_type;
	sink_type sink;

	t_struct value(2, 513);
	serialize(sink, value);

	typedef shift::source<shift::little_endian> source_type;
	source_type source(sink.buffer(), sink.size());

	t_struct result;
	serialize(source, result);

//	example::debug_print(std::cout, sink);
	CHECK(value.v1 == result.v1);
	CHECK(value.v2 == result.v2);
}

template<typename StreamType>
struct serializer {

	// depending on the mode of the stream (input for sources / output for sinks) the argument type
	// will either be "t_struct&" for sources or "const t_struct&" for sinks
	typedef typename shift::argument_traits<typename StreamType::mode, t_struct>::type arg_type;

	static void serialize(StreamType& stream, arg_type arg) {
		stream % shift::pos(0, 2) % shift::uint<3>(arg.v1)
		       % shift::pos(1   ) % arg.v2;
	}
};

TEST_CASE( "argument_traits<...> test 2: serializer-struct this is rather a 'does compile' test"
         , "argument_traits<...>")
{
	typedef shift::sink<shift::little_endian, shift::static_buffer<64> > sink_type;
	sink_type sink;

	t_struct value(2, 513);
	serializer<sink_type>::serialize(sink, value);

	typedef shift::source<shift::little_endian> source_type;
	source_type source(sink.buffer(), sink.size());

	t_struct result;
	serializer<source_type>::serialize(source, result);

//	example::debug_print(std::cout, sink);
	CHECK(value.v1 == result.v1);
	CHECK(value.v2 == result.v2);
}

template<typename A, typename B> struct is_same       { static const unsigned int value = false; };
template<typename A>             struct is_same<A, A> { static const unsigned int value = true;  };

struct argument_traits_test {

	typedef shift::sink<shift::little_endian, shift::static_buffer<64> > sink_type;
	typedef shift::source<shift::little_endian>                          source_type;

	typedef shift::argument_traits<sink_type::mode  , t_struct>::type arg_type_for_sink;
	typedef shift::argument_traits<source_type::mode, t_struct>::type arg_type_for_source;

	typedef const t_struct& expected_arg_type_for_sinks;    // expecting const ref for sinks
	typedef       t_struct& expected_arg_type_for_sources;  // expecting non const ref for sources

	SHIFT_STATIC_ASSERT( (is_same<arg_type_for_sink, expected_arg_type_for_sinks>::value)
	                   , expecting_const_ref_as_arg_for_sinks);

	SHIFT_STATIC_ASSERT( (is_same<arg_type_for_source, expected_arg_type_for_sources>::value)
	                   , expecting_non_const_ref_as_arg_for_sources);
};

}} // test::unnamed
