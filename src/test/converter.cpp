#include <catch.hpp>

#include <cmath>
#include <iostream>

#include <shift/sink.hpp>
#include <shift/source.hpp>
#include <shift/types/cstdint.hpp>
#include <shift/types/converter.hpp>
#include <shift/operator/converter.hpp>
#include <shift/types/combined_converter.hpp>
#include <test/utility.hpp>
#include <examples/utility.hpp>

namespace test { namespace {

template<shift::endianness Endianness, typename ConverterType>
void check_converter_encode_decode( ConverterType&                        converter
                                  , typename ConverterType::decoded_type& value
                                  , typename ConverterType::encoded_type  expected_encoded_result
                                  , typename ConverterType::decoded_type  expected_decoded_result
                                  , unsigned int                          pos) {

	typedef typename ConverterType::encoded_type encoded_type;
	typedef typename ConverterType::decoded_type decoded_type;

	const unsigned int buffer_size = 100000;
	typedef shift::sink  <Endianness, shift::static_buffer<buffer_size> > sink_type;
	typedef shift::source<Endianness>                                     source_type;

//	CAPTURE(value);
//	CAPTURE(expected_decoded_result);
//	CAPTURE(expected_encoded_result);
//	CAPTURE(pos);

	try {
		sink_type sink;
		sink << shift::pos(pos) << converter;

		detail::check_buffer_content<Endianness>(pos, detail::to_bytes<encoded_type>(expected_encoded_result), sink.buffer());

//		example::debug_print(std::cout, sink);

		value = decoded_type();

		source_type source(sink.buffer(), sink.size());
		source >> shift::pos(pos) >> converter;

		CHECK(value == expected_decoded_result);
	} catch (shift::out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	try {
		sink_type sink;
		sink % shift::pos(pos) % converter;

		detail::check_buffer_content<Endianness>(pos, detail::to_bytes<encoded_type>(expected_encoded_result), sink.buffer());

//		example::debug_print(std::cout, sink);

		value = decoded_type();

		source_type source(sink.buffer(), sink.size());
		source % shift::pos(pos) % converter;

		CHECK(value == expected_decoded_result);
	} catch (shift::out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}
}

template<typename DecodedType, typename EncodedType>
void check_scale_converter_encode_decode(const EncodedType value, const DecodedType factor, unsigned int pos) {

	typedef shift::scale_converter<DecodedType, EncodedType> converter_type;

	DecodedType result = value;
	converter_type conv(result, factor);

	const EncodedType expected_encoding_result = value * factor;
	const DecodedType expected_decoding_result = expected_encoding_result / factor;

	check_converter_encode_decode<shift::little_endian>( conv
	                                                   , result
	                                                   , expected_encoding_result
	                                                   , expected_decoding_result
	                                                   , pos);

	check_converter_encode_decode<shift::big_endian   >( conv
	                                                   , result
	                                                   , expected_encoding_result
	                                                   , expected_decoding_result
	                                                   , pos);
}

template<typename DecodedType, typename EncodedType>
void check_offset_converter_encode_decode(const DecodedType value, const DecodedType offset, unsigned int pos) {

	typedef shift::offset_converter<DecodedType, EncodedType> converter_type;
	DecodedType result = value;
	converter_type conv(result, offset);

	const EncodedType expected_encoding_result = value + offset;
	const DecodedType expected_decoding_result = expected_encoding_result - offset;

	check_converter_encode_decode<shift::little_endian>( conv
	                                                   , result
	                                                   , expected_encoding_result
	                                                   , expected_decoding_result
	                                                   , pos);

	check_converter_encode_decode<shift::big_endian   >( conv
	                                                   , result
	                                                   , expected_encoding_result
	                                                   , expected_decoding_result
	                                                   , pos);
}

template<typename ConverterType>
void check_converter_encode_decode( ConverterType&                        conv
                                  , typename ConverterType::decoded_type& value
                                  , typename ConverterType::encoded_type  expected_encoded_value
                                  , unsigned int                          pos) {

	check_converter_encode_decode<shift::little_endian>( conv
	                                                   , value
	                                                   , expected_encoded_value
	                                                   , value
	                                                   , pos);

	check_converter_encode_decode<shift::big_endian   >( conv
	                                                   , value
	                                                   , expected_encoded_value
	                                                   , value
	                                                   , pos);
}

TEST_CASE( "scale_converter::decode"
         , "scale converter, decode")
{
	typedef shift::scale_converter<double, shift::uint16_t> converter_type;

	double result = 0;
	const double factor = 2.5;

	converter_type conv(result, factor);

	uint16_t v = 1;
	conv.decode(v);
	CHECK(result == v / factor);

	v = 10;
	conv.decode(v);
	CHECK(result == v / factor);

	v = 10000;
	conv.decode(v);
	CHECK(result == v / factor);
}

TEST_CASE( "scale_converter::encode"
         , "scale converter, encode")
{
	typedef shift::scale_converter<float, shift::uint32_t> converter_type;

	float value = 0;
	const float factor = 4.;

	converter_type conv(value, factor);

	value = 10.;
	CHECK(conv.encode() == value * factor);

	value = 11.25;
	CHECK(conv.encode() == value * factor);

	value = 73.5;
	CHECK(conv.encode() == value * factor);
}

TEST_CASE( ">> scale_converter, % scale_converter"
         , "extracting data out of a source using the scale_converter")
{
	check_scale_converter_encode_decode<double  , uint8_t >(                      5,        1.5, 33);
	check_scale_converter_encode_decode<double  , uint8_t >(                      5,        2.0,  0);
	check_scale_converter_encode_decode<float   , uint16_t>(                  12345,        1.5,  1);
	check_scale_converter_encode_decode<float   , uint16_t>(                  12345,        2.0, 77);
	check_scale_converter_encode_decode<double  , uint32_t>(                  12345,       3.75, 42);
	check_scale_converter_encode_decode<double  , uint32_t>(                  12345,       2.01, 56);
	check_scale_converter_encode_decode<float   , int32_t >(                 -12345,       3.75,  2);
	check_scale_converter_encode_decode<float   , int32_t >(                 -12345,       2.01, 99);
	check_scale_converter_encode_decode<double  , uint64_t>(10000000000000000000ULL,        1.1, 17);
	check_scale_converter_encode_decode<double  , uint64_t>(10000000000000000000ULL, 0.00000001,  8);
	check_scale_converter_encode_decode<uint16_t, uint32_t>(                  12345,         10,  4);
	check_scale_converter_encode_decode<uint16_t, uint32_t>(                  12345,          2, 20);
}

TEST_CASE( "offset_converter::decode"
         , "offset converter functor")
{
	typedef shift::offset_converter<int, shift::uint8_t> converter_type;
	int result = 0;
	const int offset = -128;

	converter_type conv(result, offset);

	shift::uint8_t value = 0;
	conv.decode(value);
	CHECK(result == value - offset);

	value = 1;
	conv.decode(value);
	CHECK(result == value - offset);

	value = -offset;
	conv.decode(value);
	CHECK(result == value - offset);

	value = 255;
	conv.decode(value);
	CHECK(result == value - offset);
}

TEST_CASE( "offset_converter::encode"
         , "offset converter, encode")
{
	typedef shift::offset_converter<int, shift::int16_t> converter_type;
	int value = 0;
	const int offset = 128;

	converter_type conv(value, offset);

	value = 0;
	CHECK(conv.encode() == value + offset);

	value = -1000;
	CHECK(conv.encode() == value + offset);

	value = 1000;
	CHECK(conv.encode() == value + offset);
}

TEST_CASE( ">> offset_converter, % offset_converter"
         , "extracting data out of a source using the offset converter")
{
	check_offset_converter_encode_decode<int            , shift::uint8_t >(    0,          128,   0);
	check_offset_converter_encode_decode<int            , shift::uint8_t >(    0,         -128,   1);
	check_offset_converter_encode_decode<int            , shift::uint8_t >(  255,         -128, 123);
	check_offset_converter_encode_decode<shift::uint16_t, shift::uint8_t >(  255,          100,  42);
	check_offset_converter_encode_decode<int            , shift::uint16_t>(12345, -(1 << 16)/2, 555);
	check_offset_converter_encode_decode<float          , shift::int32_t>(  55.0,       100000,   3);
}

TEST_CASE( "combined converter (2): scale + offset, encode / decode"
         , "combined converter >> % <<")
{

	typedef shift::scale_converter <double         , shift::uint16_t> scale_c;
	typedef shift::offset_converter<shift::uint16_t, shift::uint16_t> offset_c;

	typedef shift::combined_converter<scale_c, offset_c> combined_c;

	double value;
	const double factor = 10.;
	const shift::uint16_t offset = 100;
	combined_c conv( value
	               , scale_c::params(factor)
	               , offset_c::params(offset));

	value = 25.;
	check_converter_encode_decode( conv
	                             , value
	                             , static_cast<combined_c::encoded_type>(value * factor + offset)
	                             , 0);

	value = 75.;
	check_converter_encode_decode( conv
	                             , value
	                             , static_cast<combined_c::encoded_type>(value * factor + offset)
	                             , 42);

	value = 1.1;
	check_converter_encode_decode( conv
	                             , value
	                             , static_cast<combined_c::encoded_type>(value * factor + offset)
	                             , 17);

	value = 0.2;
	check_converter_encode_decode( conv
	                             , value
	                             , static_cast<combined_c::encoded_type>(value * factor + offset)
	                             , 5);
}

TEST_CASE( "combined converter (3): scale + offset + offset, encode / decode"
         , "combined converter >> % <<")
{
	typedef shift::scale_converter <double         , shift::uint16_t> scale_c;
	typedef shift::offset_converter<shift::uint16_t, shift::uint32_t> offset_c;
	typedef shift::offset_converter<shift::uint32_t, shift::int64_t, shift::int64_t> offset_c_2;

	typedef shift::combined_converter<scale_c, offset_c, offset_c_2> combined_conv_t;

	double value;
	const double factor = 4.;
	const shift::uint32_t offset   =   200;
	const shift::int64_t offset_2  =  -100;

	combined_conv_t conv( value
	                    , scale_c::params(factor)
	                    , offset_c::params(offset)
	                    , offset_c_2::params(offset_2) );

	value = 25.;
	check_converter_encode_decode( conv
	                             , value
	                             , static_cast<combined_conv_t::encoded_type>(value * factor + offset + offset_2)
	                             , 0);

	value = 0.25;
	check_converter_encode_decode( conv
	                             , value
	                             , static_cast<combined_conv_t::encoded_type>(value * factor + offset + offset_2)
	                             , 0);

	value = 1389.5;
	check_converter_encode_decode( conv
	                             , value
	                             , static_cast<combined_conv_t::encoded_type>(value * factor + offset + offset_2)
	                             , 0);

	value = 23.25;
	check_converter_encode_decode( conv
	                             , value
	                             , static_cast<combined_conv_t::encoded_type>(value * factor + offset + offset_2)
	                             , 0);
}

TEST_CASE( "combined converter (4): scale + offset + offset + scale, encode / decode"
         , "combined converter >> % <<")
{
	typedef shift::scale_converter <double         , shift::uint16_t> c1;
	typedef shift::offset_converter<shift::uint16_t, shift::uint32_t> c2;
	typedef shift::offset_converter<shift::uint32_t, shift::int64_t, shift::int64_t > c3;
	typedef shift::scale_converter <shift::int64_t , shift::int16_t, shift::int64_t > c4;

	typedef shift::combined_converter<c1, c2, c3, c4> combined_conv_t;

	double value;
	const double factor            =    5.;
	const shift::uint32_t offset   =   200;
	const shift::int64_t offset_2  =  -100;
	const shift::int64_t factor_2  =     2;

	combined_conv_t conv( value
	                    , c1::params(factor  )
	                    , c2::params(offset  )
	                    , c3::params(offset_2)
	                    , c4::params(factor_2) );

	value = 0.2;
	check_converter_encode_decode( conv
	                             , value
	                             , static_cast<combined_conv_t::encoded_type>((value * factor + offset + offset_2) * factor_2)
	                             , 0);

	value = 1.4;
	check_converter_encode_decode( conv
	                             , value
	                             , static_cast<combined_conv_t::encoded_type>((value * factor + offset + offset_2) * factor_2)
	                             , 0);

	value = 5.;
	check_converter_encode_decode( conv
	                             , value
	                             , static_cast<combined_conv_t::encoded_type>((value * factor + offset + offset_2) * factor_2)
	                             , 0);

	value = 233.2;
	check_converter_encode_decode( conv
	                             , value
	                             , static_cast<combined_conv_t::encoded_type>((value * factor + offset + offset_2) * factor_2)
	                             , 0);
}

TEST_CASE( "combined converter (5): scale + offset + offset + scale + scale, encode / decode"
         , "combined converter >> % <<")
{
	typedef shift::scale_converter <double         , shift::uint16_t> c1;
	typedef shift::offset_converter<shift::uint16_t, shift::uint32_t> c2;
	typedef shift::offset_converter<shift::uint32_t, shift::int64_t, shift::int64_t > c3;
	typedef shift::scale_converter <shift::int64_t , shift::int16_t, shift::int64_t > c4;
	typedef shift::scale_converter <shift::int16_t , shift::int32_t, shift::int32_t > c5;

	typedef shift::combined_converter<c1, c2, c3, c4, c5> combined_conv_t;

	double value;
	const double factor            =    4.;
	const shift::uint32_t offset   =   200;
	const shift::int64_t offset_2  =  -100;
	const shift::int64_t factor_2  =     2;
	const shift::int32_t factor_3  =     3;

	combined_conv_t conv( value
	                    , c1::params(factor  )
	                    , c2::params(offset  )
	                    , c3::params(offset_2)
	                    , c4::params(factor_2)
	                    , c5::params(factor_3) );

	value = 17.25;
	check_converter_encode_decode( conv
	                             , value
	                             , static_cast<combined_conv_t::encoded_type>((value * factor + offset + offset_2) * factor_2 * factor_3)
	                             , 0);

	value = 283.5;
	check_converter_encode_decode( conv
	                             , value
	                             , static_cast<combined_conv_t::encoded_type>((value * factor + offset + offset_2) * factor_2 * factor_3)
	                             , 0);

	value = 50.75;
	check_converter_encode_decode( conv
	                             , value
	                             , static_cast<combined_conv_t::encoded_type>((value * factor + offset + offset_2) * factor_2 * factor_3)
	                             , 0);
}

TEST_CASE( "writing beyond the capacity of the buffer with a converter causes an out_ot_range exception to be thrown"
         , "converter" )
{
	const unsigned int buffer_size = 32;
	typedef shift::sink  <shift::little_endian, shift::static_buffer<buffer_size> > sink_type;

	typedef shift::scale_converter<double, shift::uint32_t> converter_type;

	double value = 1;
	shift::uint32_t factor = 10;
	converter_type conv(value, factor);

	try {
		sink_type sink;
		sink << shift::pos(buffer_size) << conv;
		CHECK(false);
	} catch (shift::out_of_range& e) {
		CHECK(true);
	}

	try {
		sink_type sink;
		sink % shift::pos(buffer_size) % conv;
		CHECK(false);
	} catch (shift::out_of_range& e) {
		CHECK(true);
	}
}

TEST_CASE( "trying to extract with a converter beyond the size of a source results in an out_of_range exception to be thrown"
         , "converter" )
{
	const unsigned int buffer_size = 32;
	shift::byte_type buffer[buffer_size];
	typedef shift::source<shift::little_endian> source_type;
	typedef shift::scale_converter<double, shift::uint32_t> converter_type;

	source_type source(buffer, buffer_size);

	double value = 1;
	shift::uint32_t factor = 10;
	converter_type conv(value, factor);

	try {
		source >> shift::pos(buffer_size) >> conv;
		CHECK(false);
	} catch (shift::out_of_range& e) {
		CHECK(true);
	}

	try {
		source % shift::pos(buffer_size) % conv;
		CHECK(false);
	} catch (shift::out_of_range& e) {
		CHECK(true);
	}
}

enum numbers { zero  = 0,
               one   = 1,
               two   = 2,
               three = 3,
               four  = 4 };

TEST_CASE( "enums and fixed width uint encoding"
         , "[uint, enum, fixed_width_uint]" )
{
	typedef shift::sink<shift::little_endian, shift::static_buffer<64> > sink_type;
	sink_type sink;

	numbers value = three;

	try {
		sink << shift::pos(0, 2) << shift::type_converter<numbers, shift::uint3_t>(value);
		CHECK(sink.buffer()[0] == three);
	} catch (shift::out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	typedef shift::source<shift::little_endian> source_type;
	source_type source(sink.buffer(), sink.size());

	try {
		numbers result = zero;
		source >> shift::pos(0, 2) >> shift::type_converter<numbers, shift::uint3_t>(result);
		CHECK(result == value);
	} catch (shift::out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	//

	sink.clear();
	value = two;
	try {
		sink << shift::pos(0, 4) << shift::type_converter<numbers, shift::uint3_t>(value);
		CHECK(sink.buffer()[0] == value << 2);
	} catch (shift::out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	try {
		numbers result = zero;
		source >> shift::pos(0, 4) >> shift::type_converter<numbers, shift::uint3_t>(result);
		CHECK(result == value);
	} catch (shift::out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

}


}}
