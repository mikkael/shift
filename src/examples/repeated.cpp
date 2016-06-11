#include <iostream>
#include <iterator>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <tr1/functional>

#include <code_sample.hpp>

#include <shift/types/fixed_width_uint.hpp>
#include <shift/sink.hpp>
#include <shift/source.hpp>
#include <shift/buffer/static_buffer.hpp>
#include <shift/operator/repeated.hpp>
#include <shift/operator/universal.hpp>
#include <shift/operator/var_int.hpp>

#include <examples/utility.hpp>
#include <shift/types/var_int.hpp>

using namespace shift;

namespace {

CODE_SAMPLE( repeated
           , "repeated field using a std::vector as source for encoding, decoding also to a std::vector"
           , "repeated", "vector", "std::vector")
{
	typedef std::vector<int> container_type;
	container_type v;
	for (std::size_t i=0; i < 13; ++i ) v.push_back(i);

	typedef sink<little_endian, static_buffer<64> > sink_message_type;
	sink_message_type sink;
	sink.clear();

	try {
		sink << orepeated<uint16_t, container_type::const_iterator>(v.begin(), v.end());
	} catch (out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	typedef source<little_endian> source_type;
	source_type source(sink.buffer(), sink.size());

	container_type deserialized_v;

	try {
		source >> pos(0) >> irepeated<uint16_t, std::back_insert_iterator<container_type> >(std::back_inserter(deserialized_v));
	} catch (out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	example::debug_print(std::cout, sink);
	std::cout << "\ndecoded v: "
	          << example::stream_repeated< container_type::const_iterator
	                                     , container_type::value_type >(deserialized_v.begin(), deserialized_v.end(), ", ");
}

CODE_SAMPLE( repeated
           , "repeated field using a std::vector as source for encoding, decoding to a c array"
           , "repeated", "vector", "std::vector", "array")
{
	typedef std::vector<int> container_type;
	container_type v;
	for (std::size_t i=0; i < 13; ++i ) v.push_back(i);

	typedef sink<little_endian, static_buffer<64> > sink_message_type;
	sink_message_type sink;
	sink.clear();

	try {
		sink << orepeated<uint16_t, container_type::const_iterator>(v.begin(), v.end());
	} catch (out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	typedef source<little_endian> source_type;
	source_type source(sink.buffer(), sink.size());

	unsigned int encoded_size = 0;
	container_type::value_type array[1024];

	try {
		source >> pos(0) >> irepeated<uint16_t, container_type::value_type*>(array, encoded_size);
	} catch (out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	example::debug_print(std::cout, sink);
	std::cout << "\ndecoded v: "
	          << example::stream_repeated< container_type::value_type*
	                                     , container_type::value_type >(array, array + encoded_size, ", ");
}

CODE_SAMPLE( repeated
           , "repeated field using a c-array as source for encoding, decoding also to a c array"
           , "repeated", "array", "uint8_t", "fixed length encoded size")
{
	typedef uint8_t int_type;
	const int_type a[] = {1, 2, 3, 4, 5};
	const std::size_t size = sizeof(a)/sizeof(uint8_t);

	typedef sink<little_endian, static_buffer<64> > sink_message_type;
	sink_message_type sink;
	sink.clear();

	try {
		sink << orepeated<uint32_t, const uint8_t*>(a, a+5);
	} catch (out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	typedef source<little_endian> source_type;
	source_type source(sink.buffer(), sink.size());

	uint8_t  a_deserialized[size];
	unsigned int encoded_size = 0;

	try {
		source >> pos(0) >> irepeated<uint32_t, uint8_t*>(a_deserialized, encoded_size);
	} catch (out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	example::debug_print(std::cout, sink);
	std::cout << "\ndecoded v: "
	          << example::stream_repeated<int_type*, int>(a_deserialized, a_deserialized + encoded_size, ", ");
}

CODE_SAMPLE( repeated
           , "repeated field using a c-array as source for encoding, decoding also to a c array, number of \
              elements (i.e. array size) is encoded with variable length"
           , "repeated", "array", "uint16_t", "variable length encoded size", "var_uint"
           , "big endian", "big_endian", "size encoding")
{
	typedef uint16_t int_type;
	int_type a[5] = {1, 2, 3, 4, 5};
	std::size_t size = sizeof(a)/sizeof(int_type);

	typedef sink<big_endian, static_buffer<64> > sink_message_type;
	sink_message_type sink;
	sink.clear();

	try {
		sink << orepeated<variable_length, int_type*>(a, a+size);
	} catch (out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	typedef source<big_endian> source_type;
	source_type source(sink.buffer(), sink.size());
	int_type a_deserialized[1024];
	unsigned int encoded_size;

	try {
		source >> pos(0) >> irepeated<variable_length, int_type*>(a_deserialized, encoded_size);
	} catch (out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	example::debug_print(std::cout, sink);
	std::cout << "\ndecoded v: "
	          << example::stream_repeated<int_type*, int_type>(a_deserialized, a_deserialized + encoded_size, ", ");
}

CODE_SAMPLE( repeated
           , "repeated field using a c-array as source for encoding, decoding also to a c array, \
              number of elements (i.e. array size) is not encoded, hence needs to be passed to decoding"
           , "repeated", "array", "uint16_t", "no size encoded", "big endian", "big_endian")
{
	typedef uint16_t int_type;
	int_type a[5] = {1, 2, 3, 4, 5};
	const unsigned int size = sizeof(a)/sizeof(int_type);

	typedef sink<big_endian, static_buffer<64> > sink_message_type;
	sink_message_type sink;
	sink.clear();

	try {
		sink << orepeated<no_size_field, int_type*>(a, a+5);
	} catch (out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}


	typedef source<big_endian> source_type;
	source_type source(sink.buffer(), sink.size());

	int_type a_deserialized[1024];

	try {
		source >> irepeated<no_size_field, int_type*>(a_deserialized, size);
	} catch (out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	example::debug_print(std::cout, sink);
	std::cout << "\ndecoded v: "
	          << example::stream_repeated<int_type*, int_type>(a_deserialized, a_deserialized + size, ", ");
}

CODE_SAMPLE( repeated
           , "repeated field using a c-array as source for encoding, decoding also to a c array, \
              number of elements (i.e. array size) is not encoded, hence needs to be passed to decoding"
           , "repeated", "array", "uint8_t", "no size encoded", "static size", "big endian", "big_endian")
{
	typedef uint8_t int_type;
	int_type a[5] = {1, 2, 3, 4, 5};
	const unsigned int size = sizeof(a)/sizeof(int_type);

	typedef sink<big_endian, static_buffer<64> > sink_message_type;
	sink_message_type sink;
	sink.clear();

	try {
		sink << orepeated<no_size_field, int_type*>(a, a+5);
	} catch (out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	typedef source<big_endian> source_type;
	source_type source(sink.buffer(), sink.size());

	int_type a_deserialized[1024];

	try {
		source >> irepeated<static_size<size>, int_type*>(a_deserialized);
	} catch (out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	example::debug_print(std::cout, sink);
	std::cout << "\ndecoded v: "
	          << example::stream_repeated<int_type*, int>(a_deserialized, a_deserialized + size, ", ");
}

CODE_SAMPLE( repeated
           , "repeated field using a std::vector as source for encoding, decoding to a c array"
           , "repeated", "vector", "std::vector", "array", "var_int")
{
	typedef var_int<long> value_type;
	typedef std::vector<value_type> container_type;
	container_type v;
	for (std::size_t i=0; i < 13; ++i ) v.push_back(value_type(i));

	typedef sink<little_endian, static_buffer<64> > sink_message_type;
	sink_message_type sink;
	sink.clear();

	try {
		sink << orepeated<uint16_t, container_type::const_iterator>(v.begin(), v.end());
	} catch (out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	typedef source<little_endian> source_type;
	source_type source(sink.buffer(), sink.size());

	unsigned int encoded_size = 0;
	value_type array[1024];

	try {
		source >> pos(0) >> irepeated<uint16_t, container_type::value_type*>(array, encoded_size);
	} catch (out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	example::debug_print(std::cout, sink);
//	std::cout << "\ndecoded v: "
//	          << example::stream_repeated< container_type::value_type*
//	                                     , container_type::value_type >(array, array + encoded_size, ", ");
}

struct test_struct {
	test_struct() : v1(), v2(){}
	test_struct(uint8_t v1, float v2) : v1(v1), v2(v2){}
	uint8_t v1;
	float          v2;
};

template<endianness EncodingEndianness, typename BufferType>
sink<EncodingEndianness, BufferType>& operator << (sink<EncodingEndianness, BufferType>& sink_, const test_struct& inst) {
	sink_ << inst.v1
	      << inst.v2;
	return sink_;
}

template<endianness EncodingEndianness>
source<EncodingEndianness>& operator >> (source<EncodingEndianness>& source_, test_struct& ref) {
	source_ >> ref.v1
	        >> ref.v2;
	return source_;
}

CODE_SAMPLE( repeated
           , "repeated field using a std::vector as source for encoding, decoding to a c array"
           , "repeated", "vector", "std::vector", "array", "var_int")
{
	typedef test_struct value_type;
	typedef std::vector<value_type> container_type;
	container_type v;
	v.push_back(test_struct(  1,      3.14f    ));
	v.push_back(test_struct( 13,      0.000131f));
	v.push_back(test_struct(169, 192738.011f   ));

	typedef sink<little_endian, static_buffer<64> > sink_message_type;
	sink_message_type sink;
	sink.clear();

	try {
		sink << orepeated<uint16_t, container_type::const_iterator>(v.begin(), v.end());
	} catch (out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	typedef source<little_endian> source_type;
	source_type source(sink.buffer(), sink.size());

	unsigned int encoded_size = 0;
	value_type array[1024];

	try {
		source >> pos(0) >> irepeated<uint16_t, container_type::value_type*>(array, encoded_size);
	} catch (out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	example::debug_print(std::cout, sink);
//	std::cout << "\ndecoded v: "
//	          << example::stream_repeated< container_type::value_type*
//	                                     , container_type::value_type >(array, array + encoded_size, ", ");
}

} // namespace
