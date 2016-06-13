#include <catch.hpp>

#include <iterator>
#include <limits>
#include <iostream>
#include <vector>
#include <list>

#include <shift/buffer/static_buffer.hpp>
#include <shift/sink.hpp>
#include <shift/source.hpp>
#include <shift/operator/repeated.hpp>
#include <shift/operator/universal.hpp>
#include <shift/types/var_int.hpp>

#include <test/utility.hpp>
#include <examples/utility.hpp>

namespace test { namespace {

template<typename SizeType, typename ValueType, shift::endianness Endianness>
void check_size_encoding(unsigned int n, unsigned int start_pos) {

	typedef SizeType size_type;
	typedef ValueType value_type;
	const unsigned int buffer_size = 100000;

	typedef std::vector<value_type> container_type;
	container_type v;
	for (std::size_t i=0; i<n; ++i) v.push_back(ValueType());

	typedef shift::sink<Endianness, shift::static_buffer<buffer_size> > sink_message_type;
	sink_message_type sink;

	try {
		sink << shift::buffer_position(start_pos)
		     << shift::orepeated<size_type, typename container_type::const_iterator>(v.begin(), v.end());

		const std::vector<shift::byte_type> encoded_size = detail::encoded_size_checker<SizeType, Endianness>::check(n, sink.buffer(), start_pos);
		CHECK(sink.size() == start_pos + encoded_size.size() + n * sizeof(ValueType));

	} catch (shift::out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}
}

template<typename SizeType, shift::endianness Endianness>
struct size_encoder {
	static unsigned int encode(unsigned int n, shift::byte_type* buffer, unsigned int start_pos) {
		const std::vector<shift::byte_type> bytes = test::detail::to_bytes<SizeType>(n);
		test::detail::copy_to_buffer<Endianness>(start_pos, bytes, buffer);
		return bytes.size();
	}
};

template<shift::endianness Endianness>
struct size_encoder<shift::variable_length, Endianness> {
	static unsigned int encode(unsigned int n, shift::byte_type* buffer, unsigned int start_pos) {
		const std::vector<shift::byte_type> bytes = detail::var_width_encoded_bytes(n);
		std::copy(bytes.begin(), bytes.end(), buffer + start_pos);
		return bytes.size();
	}
};

template<unsigned int N, shift::endianness Endianness>
struct size_encoder<shift::static_size<N>, Endianness> {
	static unsigned int encode(unsigned int n, shift::byte_type* buffer, unsigned int start_pos) {
		return 0;
	}
};

template<typename SizeType, typename ValueType, shift::endianness Endianness>
void check_size_decoding(unsigned int n, unsigned int start_pos) {

	typedef std::vector<ValueType> container_type;
	typedef shift::source<Endianness> source_type;

	const unsigned int buffer_size = 100000;
	shift::byte_type buffer[buffer_size];
	source_type source(buffer, buffer_size);

	size_encoder<SizeType, Endianness>::encode(n, buffer, start_pos);

	try {
		container_type v;
		source >> shift::buffer_position(start_pos)
		       >> shift::irepeated<SizeType, std::back_insert_iterator<container_type> >(std::back_inserter(v));
		CHECK(v.size() == n);
	} catch (shift::out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	try {
		container_type v;
		source % shift::buffer_position(start_pos)
		       % shift::irepeated<SizeType, std::back_insert_iterator<container_type> >(std::back_inserter(v));

		CHECK(v.size() == n);
	} catch (shift::out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}
}

template<typename ValueType, shift::endianness Endianness, typename IteratorType>
void check_encoded_content( IteratorType begin
                          , IteratorType end
                          , const shift::byte_type* buffer
                          , unsigned int start_pos) {
	unsigned int i=0;
	for (IteratorType it=begin; it!=end; ++it) {
		const unsigned int buffer_pos = start_pos + i * sizeof(ValueType);
		detail::check_encoded_content<ValueType, Endianness>(*it, buffer, buffer_pos);
		++i;
	}
}

template<typename SizeType, typename ValueType, shift::endianness Endianness, typename SinkType, typename IteratorType>
void check_encoded_content(SinkType& sink, IteratorType begin, IteratorType end, unsigned int start_pos) {
	const unsigned long n = std::distance(begin, end);

	const std::vector<shift::byte_type> encoded_size = detail::encoded_size_checker<SizeType, Endianness>::check(n, sink.buffer(), start_pos);
	CHECK(sink.size() == start_pos + encoded_size.size() + n * sizeof(ValueType));

	check_encoded_content<ValueType, Endianness>( begin
	                                            , end
	                                            , sink.buffer()
	                                            , start_pos + encoded_size.size() );
}

template<typename SizeType, typename ValueType, shift::endianness Endianness, typename IteratorType>
void check_encoding(unsigned int start_pos, IteratorType begin, IteratorType end) {

	typedef SizeType size_type;
	typedef ValueType value_type;
	typedef std::vector<value_type> container_type;
	const unsigned int buffer_size = 100000;
	typedef shift::sink<Endianness, shift::static_buffer<buffer_size> > sink_message_type;
	const unsigned int n = std::distance(begin, end);
	sink_message_type sink;

	try {
		sink << shift::buffer_position(start_pos)
		     << shift::orepeated<size_type, IteratorType>(begin, end);
		check_encoded_content<SizeType, ValueType, Endianness>(sink, begin, end, start_pos);
	} catch (shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	try {
		sink % shift::buffer_position(start_pos)
		     % shift::orepeated<size_type, IteratorType>(begin, end);
		check_encoded_content<SizeType, ValueType, Endianness>(sink, begin, end, start_pos);
	} catch (shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}
}

template<typename ValueType, shift::endianness Endianness, typename IteratorType>
void place_content_in_buffer(IteratorType begin, IteratorType end, shift::byte_type* buffer, unsigned int start_pos){
	typedef typename detail::bitify_type<ValueType>::type BitifyType;
	unsigned int i=0;
	for (IteratorType it=begin; it!=end; ++it) {
		const unsigned int buffer_pos = start_pos + i * sizeof(ValueType);
		const ValueType value = *it;
		const BitifyType tmp = *reinterpret_cast<const BitifyType*>(&value);
		const std::vector<shift::uint8_t> bytes = test::detail::to_bytes(tmp);
		test::detail::copy_to_buffer<Endianness>(buffer_pos, bytes, buffer);
		++i;
	}
}

template<typename SizeType, typename ValueType, shift::endianness Endianness, typename IteratorType>
void check_decoding(unsigned int start_pos, IteratorType begin, IteratorType end) {

	typedef std::vector<ValueType> container_type;

	const unsigned int buffer_size = 100000;
	shift::byte_type buffer[buffer_size];
	std::memset(buffer, 0, buffer_size);
	const unsigned int n = std::distance(begin, end);
	typedef shift::source<Endianness> source_type;

	unsigned int n_bytes_encodes_size = size_encoder<SizeType, Endianness>::encode(n, buffer, start_pos);

	place_content_in_buffer<ValueType, Endianness>( begin
	                                              , end
	                                              , buffer
	                                              , start_pos + n_bytes_encodes_size );

	source_type source(buffer, buffer_size);

	try {
		container_type results;

		source >> shift::buffer_position(start_pos)
		       >> shift::irepeated<SizeType, std::back_insert_iterator<container_type> >(std::back_inserter(results));

		CHECK(results.size() == n);
		IteratorType source = begin;
		for (typename container_type::iterator it = results.begin(), end = results.end(); it!=end; ++it) {
			CHECK(*it == *source);
			++source;
		}
	} catch (shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	try {
		container_type results;

		source % shift::buffer_position(start_pos)
		       % shift::irepeated<SizeType, std::back_insert_iterator<container_type> >(std::back_inserter(results));

		CHECK(results.size() == n);
		IteratorType source = begin;
		for (typename container_type::iterator it = results.begin(), end = results.end(); it!=end; ++it) {
			CHECK(*it == *source);
			++source;
		}
	} catch (shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}
}

#define DEFINE_ARRAY(value_type, name, size_name, ...)\
	value_type name[] = { __VA_ARGS__ };\
	const unsigned int size_name = sizeof(name)/sizeof(value_type)

#define DEFINE_STD_VECTOR(value_type, name, size, ...)\
	std::vector<value_type> name;\
	value_type tmp[] = { __VA_ARGS__ };\
	const unsigned int size = sizeof(tmp)/sizeof(value_type);\
	for (unsigned int i=0; i<size; ++i)\
		name.push_back(tmp[i]);\

TEST_CASE( "the size of repeated fields is correctly encoded for fixed width size, variable width size and no size"
         , "[repeated]" )
{
	check_size_encoding<shift::uint8_t        , shift::uint8_t , shift::little_endian>(  0,  33);
	check_size_encoding<shift::uint8_t        , float          , shift::big_endian   >(100, 100);
	check_size_encoding<shift::uint8_t        , double         , shift::little_endian>( 33,  42);
	check_size_encoding<shift::uint8_t        , shift::uint32_t, shift::big_endian   >(255,  10);

	check_size_encoding<shift::uint16_t       , shift::uint16_t, shift::little_endian>(  0,   0);
	check_size_encoding<shift::uint16_t       , shift::uint32_t, shift::big_endian   >( 13, 500);
	check_size_encoding<shift::uint16_t       , bool           , shift::little_endian>(256,  10);
	check_size_encoding<shift::uint16_t       , shift::uint64_t, shift::big_endian   >(799,  42);
	check_size_encoding<shift::uint16_t       , shift::uint8_t , shift::little_endian>(std::numeric_limits<shift::uint16_t>::max(),  42);

	check_size_encoding<shift::uint32_t       , float          , shift::big_endian   >( 99,   1);
	check_size_encoding<shift::uint32_t       , shift::uint8_t , shift::little_endian>( 89, 600);
	check_size_encoding<shift::uint32_t       , bool           , shift::big_endian   >(256,  10);
	check_size_encoding<shift::uint32_t       , double         , shift::little_endian>(799,  42);

	check_size_encoding<shift::variable_length, shift::uint64_t, shift::little_endian>(  0,  33);
	check_size_encoding<shift::variable_length, float          , shift::big_endian   >(100, 100);
	check_size_encoding<shift::variable_length, double         , shift::little_endian>( 33,  42);
	check_size_encoding<shift::variable_length, shift::uint32_t, shift::big_endian   >(255,  10);
	check_size_encoding<shift::variable_length, shift::uint16_t, shift::little_endian>(  0,   0);
	check_size_encoding<shift::variable_length, shift::uint32_t, shift::big_endian   >( 13, 500);
	check_size_encoding<shift::variable_length, bool           , shift::little_endian>(256,  10);
	check_size_encoding<shift::variable_length, double         , shift::big_endian   >(799,  42);
	check_size_encoding<shift::variable_length, shift::uint8_t , shift::little_endian>(16384, 0);
	check_size_encoding<shift::variable_length, shift::uint8_t , shift::big_endian   >(33333, 0);
	check_size_encoding<shift::variable_length, shift::uint8_t , shift::little_endian>(std::numeric_limits<shift::uint16_t>::max(),  42);

	check_size_encoding<shift::no_size_field  , shift::uint16_t, shift::little_endian>(  1,  33);
	check_size_encoding<shift::no_size_field  , float          , shift::big_endian   >(100, 100);
	check_size_encoding<shift::no_size_field  , double         , shift::little_endian>( 33,  42);
	check_size_encoding<shift::no_size_field  , shift::uint32_t, shift::big_endian   >(255,  10);
}

TEST_CASE( "the number of repeated values encoded with a fixed width unsigned integer can be decoded"
         , "[repeated]" )
{
	const unsigned int maxui16 = std::numeric_limits<shift::uint16_t>::max();

	check_size_decoding<shift::uint8_t           , shift::uint8_t , shift::little_endian>(      0,   33);
	check_size_decoding<shift::uint8_t           , float          , shift::big_endian   >(    100,  100);
	check_size_decoding<shift::uint8_t           , double         , shift::little_endian>(     33,   42);
	check_size_decoding<shift::uint8_t           , shift::uint32_t, shift::big_endian   >(    255,   10);
	check_size_decoding<shift::uint16_t          , shift::uint16_t, shift::little_endian>(      0,    0);
	check_size_decoding<shift::uint16_t          , shift::uint32_t, shift::big_endian   >(     13,  500);
	check_size_decoding<shift::uint16_t          , bool           , shift::little_endian>(    256,   10);
	check_size_decoding<shift::uint16_t          , shift::uint64_t, shift::big_endian   >(    799,   42);
	check_size_decoding<shift::uint16_t          , shift::uint8_t , shift::little_endian>(maxui16,   42);
	check_size_decoding<shift::uint32_t          , float          , shift::big_endian   >(     99,    1);
	check_size_decoding<shift::uint32_t          , shift::uint8_t , shift::little_endian>(     89,  600);
	check_size_decoding<shift::uint32_t          , bool           , shift::big_endian   >(    256,   10);
	check_size_decoding<shift::uint32_t          , double         , shift::little_endian>(    799,   42);

	check_size_decoding<shift::variable_length   , shift::uint8_t , shift::little_endian>(      0,   33);
	check_size_decoding<shift::variable_length   , float          , shift::big_endian   >(    100,  100);
	check_size_decoding<shift::variable_length   , double         , shift::little_endian>(     33,   42);
	check_size_decoding<shift::variable_length   , shift::uint32_t, shift::big_endian   >(    255,   10);
	check_size_decoding<shift::variable_length   , shift::uint16_t, shift::little_endian>(      0,    0);
	check_size_decoding<shift::variable_length   , shift::uint32_t, shift::big_endian   >(     13,  500);
	check_size_decoding<shift::variable_length   , bool           , shift::little_endian>(    256,   10);
	check_size_decoding<shift::variable_length   , double         , shift::big_endian   >(    799,   42);
	check_size_decoding<shift::variable_length   , shift::uint8_t , shift::little_endian>(  16384,    0);
	check_size_decoding<shift::variable_length   , shift::uint8_t , shift::big_endian   >(  33333,    0);

	check_size_decoding<shift::static_size<    0>, shift::uint8_t , shift::little_endian>(      0,   33);
	check_size_decoding<shift::static_size<100>  , float          , shift::big_endian   >(    100,  100);
	check_size_decoding<shift::static_size<33>   , double         , shift::little_endian>(     33,   42);
	check_size_decoding<shift::static_size<255>  , shift::uint32_t, shift::big_endian   >(    255,   10);
	check_size_decoding<shift::static_size<   15>, float          , shift::big_endian   >(     15, 1024);
	check_size_decoding<shift::static_size<  333>, double         , shift::little_endian>(    333,   79);
	check_size_decoding<shift::static_size<  255>, shift::uint32_t, shift::big_endian   >(    255,    3);
	check_size_decoding<shift::static_size<    1>, shift::uint16_t, shift::little_endian>(      1,    0);
	check_size_decoding<shift::static_size<   13>, shift::uint32_t, shift::big_endian   >(     13,   90);
	check_size_decoding<shift::static_size<  256>, bool           , shift::little_endian>(    256,  800);
	check_size_decoding<shift::static_size<  391>, double         , shift::big_endian   >(    391,   53);
	check_size_decoding<shift::static_size<16384>, shift::uint8_t , shift::little_endian>(  16384,    0);
	check_size_decoding<shift::static_size<33333>, shift::uint8_t , shift::big_endian   >(  33333,    0);
	check_size_decoding<shift::static_size<65535>, shift::uint8_t , shift::little_endian>(  65535,    7);
}

TEST_CASE( "repeated values can be streamed to a sink starting at specified location using the << and the % operator, "
         , "[repeated]" )
{
	{
		DEFINE_ARRAY(shift::uint8_t, array, size, 99, 0, 0, 13, 42, 57, 255, 139, 19, 91, 8, 2, 0, 1);
		check_encoding<shift::uint8_t        , shift::uint8_t, shift::little_endian>(13, array, array + size);
		check_encoding<shift::variable_length, shift::uint8_t, shift::big_endian   >(13, array, array + size);
		check_encoding<shift::no_size_field  , shift::uint8_t, shift::little_endian>(13, array, array + size);
	}

	{
		DEFINE_ARRAY(shift::uint32_t, array, size, 129, 2001, 1, 15, 255, 65000, 48, 129, 5, 0, 0, 7, 18003);
		check_encoding<shift::uint32_t       , shift::uint32_t, shift::big_endian   >(500, array, array + size);
		check_encoding<shift::variable_length, shift::uint32_t, shift::little_endian>(500, array, array + size);
		check_encoding<shift::no_size_field  , shift::uint32_t, shift::big_endian   >(500, array, array + size);
	}

	{
		DEFINE_ARRAY( float , array , size
		            , 129.f, 2001.f, 3.14f, 15.1515f, 42.12f, 0.0650001f, 433.2940f, 12.89f
		            , 123.4567890f, 0.0f, 0.001f, 7.77f, 18003.f);
		check_encoding<shift::uint16_t       , float, shift::little_endian>(31, array, array + size);
		check_encoding<shift::variable_length, float, shift::big_endian   >(31, array, array + size);
		check_encoding<shift::no_size_field  , float, shift::little_endian>(31, array, array + size);
	}

	{
		DEFINE_ARRAY( double
		            , array
		            , size
		            , 129.f, 2001.f, 3.14f, 15.1515f, 42.12f, 0.0650001f, 433.2940f, 12.89f
		            , 123.4567890f, 0.0f, 0.001f, 7.77f, 18003.f);
		check_encoding<shift::uint64_t       , double, shift::big_endian   >(127, array, array + size);
		check_encoding<shift::variable_length, double, shift::little_endian>(127, array, array + size);
		check_encoding<shift::no_size_field  , double, shift::big_endian   >(127, array, array + size);
	}

	{
		const double values[] = { 3.14159265359f, 0.5772156649f, 2.71828f, 1.61803f };
		const unsigned int n_values = sizeof(values)/sizeof(double);
		const unsigned int n = 155;
		double array [155];
		for (unsigned int i=0; i<n; ++i) array[i] = values[i % n_values];
		check_encoding<shift::uint16_t       , double, shift::little_endian>(13, array, array + n);
		check_encoding<shift::variable_length, double, shift::big_endian   >(13, array, array + n);
		check_encoding<shift::no_size_field  , double, shift::little_endian>(13, array, array + n);
	}

	{
		DEFINE_STD_VECTOR(shift::uint8_t, vec, size, 129, 2, 1, 15, 255, 33, 48, 129, 5, 0);
		check_encoding<shift::uint8_t        , shift::uint8_t, shift::little_endian>(13, vec.begin(), vec.end());
		check_encoding<shift::variable_length, shift::uint8_t, shift::big_endian   >(13, vec.begin(), vec.end());
		check_encoding<shift::no_size_field  , shift::uint8_t, shift::little_endian>(13, vec.begin(), vec.end());
	}

	{
		std::vector<shift::uint8_t> vec;
		for (unsigned int i=0; i<16385; ++i) vec.push_back(i % 256);
		check_encoding<shift::uint32_t       , shift::uint8_t, shift::little_endian>(13, vec.begin(), vec.end());
		check_encoding<shift::variable_length, shift::uint8_t, shift::little_endian>(13, vec.begin(), vec.end());
		check_encoding<shift::no_size_field  , shift::uint8_t, shift::little_endian>(13, vec.begin(), vec.end());
	}
}

TEST_CASE( "repeated values can be extracted from a source starting at specified location using the << and the % operator"
         , "[repeated]" )
{
	{
		DEFINE_ARRAY(shift::uint8_t, array, size, 99, 0, 0, 13, 42, 57, 255, 139, 19, 91, 8, 2, 0, 1);

		check_decoding<shift::uint8_t          , shift::uint8_t, shift::little_endian>(13, array, array + size);
		check_decoding<shift::variable_length  , shift::uint8_t, shift::little_endian>(13, array, array + size);
		check_decoding<shift::static_size<size>, shift::uint8_t, shift::little_endian>(13, array, array + size);
	}

	{
		DEFINE_ARRAY(shift::uint32_t, array, size, 129, 2001, 1, 15, 255, 65000, 48, 129, 5, 0, 0, 7, 18003);

		check_decoding<shift::uint32_t         , shift::uint32_t, shift::big_endian>(500, array, array + size);
		check_decoding<shift::variable_length  , shift::uint32_t, shift::big_endian>(500, array, array + size);
		check_decoding<shift::static_size<size>, shift::uint32_t, shift::big_endian>(500, array, array + size);
	}

	{
		DEFINE_ARRAY( float
		            , array
		            , size
		            , 129.f, 2001.f, 3.14f, 15.1515f, 42.12f, 0.0650001f, 433.2940f, 12.89f
		            , 123.4567890f, 0.0f, 0.001f, 7.77f, 18003.f);

		check_decoding<shift::uint16_t         , float, shift::little_endian>(31, array, array + size);
		check_decoding<shift::variable_length  , float, shift::little_endian>(31, array, array + size);
		check_decoding<shift::static_size<size>, float, shift::little_endian>(31, array, array + size);
	}

	{
		DEFINE_ARRAY( double
		            , array
		            , size
		            , 129.f, 2001.f, 3.14f, 15.1515f, 42.12f, 0.0650001f, 433.2940f, 12.89f
		            , 123.4567890f, 0.0f, 0.001f, 7.77f, 18003.f);

		check_decoding<shift::uint64_t         , double, shift::big_endian>(31, array, array + size);
		check_decoding<shift::variable_length  , double, shift::big_endian>(31, array, array + size);
		check_decoding<shift::static_size<size>, double, shift::big_endian>(31, array, array + size);
	}

	{
		DEFINE_STD_VECTOR(shift::uint16_t, vec, size, 129, 2001, 1, 15, 255, 65000, 48, 129, 5, 0, 0, 7, 18003);

		check_decoding<shift::uint16_t         , shift::uint16_t, shift::little_endian>(500, vec.begin(), vec.end());
		check_decoding<shift::variable_length  , shift::uint16_t, shift::little_endian>(500, vec.begin(), vec.end());
		check_decoding<shift::static_size<size>, shift::uint16_t, shift::little_endian>(500, vec.begin(), vec.end());
	}

	{
		DEFINE_STD_VECTOR( float
		                 , vec
		                 , size
		                 , 129.f, 2001.f, 3.14f, 15.1515f, 42.12f, 0.0650001f, 433.2940f, 12.89f
		                 , 123.4567890f, 0.0f, 0.001f, 7.77f, 18003.f);

		check_decoding<shift::uint32_t         , float, shift::big_endian>(31, vec.begin(), vec.end());
		check_decoding<shift::variable_length  , float, shift::big_endian>(31, vec.begin(), vec.end());
		check_decoding<shift::static_size<size>, float, shift::big_endian>(31, vec.begin(), vec.end());
	}

	{
		const double values[] = { 3.14159265359f, 0.5772156649f, 2.71828f, 1.61803f };
		const unsigned int n_values = sizeof(values)/sizeof(double);
		const unsigned int size = 155;
		double array [size];
		for (unsigned int i=0; i<size; ++i) array[i] = values[i % n_values];

		check_decoding<shift::uint8_t          , double, shift::little_endian>(13, array, array + size);
		check_decoding<shift::variable_length  , double, shift::little_endian>(13, array, array + size);
		check_decoding<shift::static_size<size>, double, shift::little_endian>(13, array, array + size);
	}

	{
		std::vector<shift::uint8_t> vec;
		const unsigned int size = 16385;
		for (unsigned int i=0; i<size; ++i) vec.push_back(i % 256);

		check_decoding<shift::uint32_t         , shift::uint8_t, shift::little_endian>(13, vec.begin(), vec.end());
		check_decoding<shift::variable_length  , shift::uint8_t, shift::little_endian>(13, vec.begin(), vec.end());
		check_decoding<shift::static_size<size>, shift::uint8_t, shift::little_endian>(13, vec.begin(), vec.end());
	}
}

TEST_CASE( "when writing repeated values to a stream, only the specified range in the buffer is modified"
         , "[repeated]" )
{
	typedef std::vector<shift::uint8_t> container_type;
	container_type v;
	const unsigned int n           = 13;
	const unsigned int start_pos   = 20;
	const unsigned int buffer_size = 64;
	for (std::size_t i=0; i<n; ++i) v.push_back(i+1);

	typedef shift::sink<shift::little_endian, shift::static_buffer<buffer_size> > sink_message_type;
	sink_message_type sink;

	try {
		sink << shift::buffer_position(start_pos)
		     << shift::orepeated<shift::uint8_t, container_type::const_iterator>(v.begin(), v.end());
		test::check_all_zero_except(sink.buffer(), buffer_size, start_pos, start_pos + n);
	} catch (shift::out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}
}

TEST_CASE( "writing repeated values beyond the capacity of the buffer causes an out_ot_range exception to be thrown"
         , "[repeated]" )
{
	typedef shift::uint16_t value_type;
	typedef std::vector<value_type> container_type;
	container_type v;
	const unsigned int n           = 10;
	const unsigned int buffer_size = 64;
	const unsigned int start_pos   = 64 - n * sizeof(value_type);
	for (std::size_t i=0; i<n; ++i) v.push_back(i+1);

	typedef shift::sink<shift::little_endian, shift::static_buffer<buffer_size> > sink_type;
	sink_type sink;

	try {
		sink << shift::buffer_position(start_pos)
		     << shift::orepeated<shift::uint8_t, container_type::const_iterator>(v.begin(), v.end());
		CHECK(false);
	} catch (shift::out_of_range& e) {
		CHECK(true);
	}
}

TEST_CASE( "trying to stream out repeated values beyond the size of a source results in an out_of_range exception to be thrown"
         , "[repeated]" )
{
	typedef shift::uint32_t value_type;
	typedef std::vector<value_type> container_type;
	container_type v;
	const unsigned int n           = 10;
	const unsigned int buffer_size = 64;
	const unsigned int start_pos   = 64 - n * sizeof(value_type);
	shift::byte_type buffer[buffer_size];

	typedef shift::source<shift::big_endian> source_type;
	source_type source(buffer, buffer_size);

	try {
		source >> shift::buffer_position(start_pos)
		       >> shift::irepeated<shift::uint8_t, std::back_insert_iterator<container_type> >(std::back_inserter(v));
		CHECK(false);
	} catch (shift::out_of_range& e) {
		CHECK(true);
	}
}

#undef DEFINE_ARRAY
#undef DEFINE_STD_VECTOR

}} // test
