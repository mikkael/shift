#include <catch.hpp>

#include <iterator>
#include <limits>
#include <iostream>
#include <vector>
#include <list>

#include <shift/buffer/static_buffer.hpp>
#include <shift/sink.hpp>
#include <shift/source.hpp>
#include <shift/operator/string.hpp>
#include <shift/operator/universal.hpp>
#include <shift/types/var_int.hpp>

#include <test/utility.hpp>
#include <examples/utility.hpp>

namespace test { namespace {

template<typename SizeType, shift::endianness Endianness>
void check_encoded_content(const std::string& str, const shift::byte_type* buffer, const unsigned int start_pos, unsigned int size) {

	const unsigned int n = str.length();

	// check encoded size
	const std::vector<shift::byte_type> encoded_size = detail::encoded_size_checker<SizeType, Endianness>::check(n, buffer, start_pos);

	// check encoded content
	for (unsigned int i=0; i < n; ++i)
		str[i] == buffer[start_pos + encoded_size.size() + i];

	// check size
	CHECK(size == start_pos + encoded_size.size() + n);
}

template<typename SizeType, shift::endianness Endianness>
void check_string_encoding(const std::string& str, unsigned int start_pos) {
	const unsigned int buffer_size = 10000;
	typedef shift::static_buffer<buffer_size> buffer_type;
	typedef shift::sink<Endianness, buffer_type> sink_type;

	const unsigned int n = str.length();
	REQUIRE(start_pos + n < buffer_size);

	try {
		sink_type sink;
		sink << shift::pos(start_pos) << shift::ostring<SizeType>(str);

		check_encoded_content<SizeType, Endianness>(str, sink.buffer(), start_pos, sink.size());

//		example::debug_print(std::cout, sink);
	} catch (shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	try {
		sink_type sink;
		sink % shift::pos(start_pos) % shift::ostring<SizeType>(str);

		check_encoded_content<SizeType, Endianness>(str, sink.buffer(), start_pos, sink.size());
	} catch (shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}
}

template<shift::endianness Endianness>
void check_std_string_encoding(const std::string& str, unsigned int start_pos) {
	const unsigned int buffer_size = 10000;
	typedef shift::static_buffer<buffer_size> buffer_type;
	typedef shift::sink<Endianness, buffer_type> sink_type;

	const unsigned int n = str.length();
	REQUIRE(start_pos + n < buffer_size);

	try {
		sink_type sink;
		sink << shift::pos(start_pos) << str;

		check_encoded_content<shift::uint16_t, Endianness>(str, sink.buffer(), start_pos, sink.size());

//		example::debug_print(std::cout, sink);
	} catch (shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	try {
		sink_type sink;
		sink % shift::pos(start_pos) % str;

		check_encoded_content<shift::uint16_t, Endianness>(str, sink.buffer(), start_pos, sink.size());
	} catch (shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}
}

template<typename SourceType, typename SizeType>
struct decoder {
	static void decode(SourceType& source, unsigned int start_pos, std::string& result, unsigned int /*length*/) {
		source >> shift::buffer_position(start_pos)
		       >> shift::istring<SizeType>(result);
	}
	static void decode_universal(SourceType& source, unsigned int start_pos, std::string& result, unsigned int /*length*/) {
		source % shift::buffer_position(start_pos)
		       % shift::istring<SizeType>(result);
	}
};

template<typename SourceType>
struct decoder<SourceType, shift::no_size_field> {
	static void decode(SourceType& source, unsigned int start_pos, std::string& result, unsigned int length) {
		source >> shift::buffer_position(start_pos)
		       >> shift::istring<shift::no_size_field>(result, length);
	}
	static void decode_universal(SourceType& source, unsigned int start_pos, std::string& result, unsigned int length) {
		source % shift::buffer_position(start_pos)
		       % shift::istring<shift::no_size_field>(result, length);
	}
};

template<typename SizeType, shift::endianness Endianness>
void check_decoding(const std::string& str, unsigned int start_pos) {

	const unsigned int buffer_size = 100000;
	shift::byte_type buffer[buffer_size];
	std::memset(buffer, 0, buffer_size);
	const unsigned int n = str.length();
	typedef shift::source<Endianness> source_type;

	unsigned int n_bytes_encoded_size = detail::size_encoder<SizeType, Endianness>::encode(n, buffer, start_pos);
	std::memcpy(buffer + start_pos + n_bytes_encoded_size, str.c_str(), n);

	source_type source(buffer, buffer_size);

	try {
		std::string result;
		decoder<source_type, SizeType>::decode(source, start_pos, result, str.length());
		CHECK(result.length() == n);
		CHECK(result == str);
	} catch (shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	try {
		std::string result;
		decoder<source_type, SizeType>::decode_universal(source, start_pos, result, str.length());
		CHECK(result.length() == n);
		CHECK(result == str);
	} catch (shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}
}

TEST_CASE( "strings can be streamed to a sink starting at specified location using the << and the % operator, "
         , "[string, ostring, encoding]" )
{
	check_string_encoding<shift::uint8_t        , shift::little_endian>("hello, world", 0);
	check_string_encoding<shift::uint8_t        , shift::big_endian   >("hello, world", 0);
	check_string_encoding<shift::uint8_t        , shift::little_endian>("hello, world", 3);
	check_string_encoding<shift::uint8_t        , shift::big_endian   >("hello, world", 3);
	check_string_encoding<shift::uint16_t       , shift::little_endian>("hello, world", 0);
	check_string_encoding<shift::uint16_t       , shift::big_endian   >("hello, world", 0);
	check_string_encoding<shift::uint16_t       , shift::little_endian>("hello, world", 3);
	check_string_encoding<shift::uint16_t       , shift::big_endian   >("hello, world", 3);
	check_string_encoding<shift::uint32_t       , shift::little_endian>("hello, world", 0);
	check_string_encoding<shift::uint32_t       , shift::big_endian   >("hello, world", 0);
	check_string_encoding<shift::uint32_t       , shift::little_endian>("hello, world", 3);
	check_string_encoding<shift::uint32_t       , shift::big_endian   >("hello, world", 3);
	check_string_encoding<shift::uint64_t       , shift::little_endian>("hello, world", 0);
	check_string_encoding<shift::uint64_t       , shift::big_endian   >("hello, world", 0);
	check_string_encoding<shift::uint64_t       , shift::little_endian>("hello, world", 3);
	check_string_encoding<shift::uint64_t       , shift::big_endian   >("hello, world", 3);
	check_string_encoding<shift::variable_length, shift::little_endian>("hello, world", 0);
	check_string_encoding<shift::variable_length, shift::big_endian   >("hello, world", 0);
	check_string_encoding<shift::variable_length, shift::little_endian>("hello, world", 3);
	check_string_encoding<shift::variable_length, shift::big_endian   >("hello, world", 3);
	check_string_encoding<shift::no_size_field  , shift::little_endian>("hello, world", 0);
	check_string_encoding<shift::no_size_field  , shift::big_endian   >("hello, world", 0);
	check_string_encoding<shift::no_size_field  , shift::little_endian>("hello, world", 3);
	check_string_encoding<shift::no_size_field  , shift::big_endian   >("hello, world", 3);

	const std::string test_string =
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit.Fusce facilisis tellus quis scelerisque "
		"eleifend. Sed consequat luctus lacus sed lobortis. Pellentesque eget libero eros. Pellentesque at "
		"augue lacinia, malesuada odio quis, scelerisque purus. Fusce sit amet tortor ut augue eleifend "
		"sodales. Mauris volutpat imperdiet euismod. Curabitur et aliquet est. Nunc sed laoreet neque, at "
		"placerat mauris. Duis blandit egestas massa, a ultricies augue vulputate ac.";

	check_string_encoding<shift::uint16_t       , shift::little_endian>(test_string,   0);
	check_string_encoding<shift::uint16_t       , shift::big_endian   >(test_string,   0);
	check_string_encoding<shift::uint16_t       , shift::little_endian>(test_string, 333);
	check_string_encoding<shift::uint16_t       , shift::big_endian   >(test_string, 333);
	check_string_encoding<shift::uint32_t       , shift::little_endian>(test_string,   0);
	check_string_encoding<shift::uint32_t       , shift::big_endian   >(test_string,   0);
	check_string_encoding<shift::uint32_t       , shift::little_endian>(test_string, 333);
	check_string_encoding<shift::uint32_t       , shift::big_endian   >(test_string, 333);
	check_string_encoding<shift::uint64_t       , shift::little_endian>(test_string,   0);
	check_string_encoding<shift::uint64_t       , shift::big_endian   >(test_string,   0);
	check_string_encoding<shift::uint64_t       , shift::little_endian>(test_string, 333);
	check_string_encoding<shift::uint64_t       , shift::big_endian   >(test_string, 333);
	check_string_encoding<shift::variable_length, shift::little_endian>(test_string,   0);
	check_string_encoding<shift::variable_length, shift::big_endian   >(test_string,   0);
	check_string_encoding<shift::variable_length, shift::little_endian>(test_string, 333);
	check_string_encoding<shift::variable_length, shift::big_endian   >(test_string, 333);
	check_string_encoding<shift::no_size_field  , shift::little_endian>(test_string,   0);
	check_string_encoding<shift::no_size_field  , shift::big_endian   >(test_string,   0);
	check_string_encoding<shift::no_size_field  , shift::little_endian>(test_string, 333);
	check_string_encoding<shift::no_size_field  , shift::big_endian   >(test_string, 333);
}

TEST_CASE( "std::strings can be streamed to a sink starting at specified location using the << and the % operator, "
         , "[std::string, ostring, encoding]" )
{
	check_std_string_encoding<shift::little_endian>("hello, world", 3);
	check_std_string_encoding<shift::big_endian   >("hello, world", 3);

	const std::string test_string =
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit.Fusce facilisis tellus quis scelerisque "
		"eleifend. Sed consequat luctus lacus sed lobortis. Pellentesque eget libero eros. Pellentesque at "
		"augue lacinia, malesuada odio quis, scelerisque purus. Fusce sit amet tortor ut augue eleifend "
		"sodales. Mauris volutpat imperdiet euismod. Curabitur et aliquet est. Nunc sed laoreet neque, at "
		"placerat mauris. Duis blandit egestas massa, a ultricies augue vulputate ac.";

	check_std_string_encoding<shift::little_endian>(test_string,   0);
	check_std_string_encoding<shift::big_endian   >(test_string,   0);
}

TEST_CASE( "strings can be extracted from a source starting at specified location using the << and the % operator"
         , "[string, decoding]" )
{
	check_decoding<shift::uint8_t         , shift::little_endian>("hello, world", 0);
	check_decoding<shift::uint8_t         , shift::big_endian   >("hello, world", 0);
	check_decoding<shift::uint8_t         , shift::little_endian>("hello, world", 3);
	check_decoding<shift::uint8_t         , shift::big_endian   >("hello, world", 3);
	check_decoding<shift::uint16_t        , shift::little_endian>("hello, world", 0);
	check_decoding<shift::uint16_t        , shift::big_endian   >("hello, world", 0);
	check_decoding<shift::uint16_t        , shift::little_endian>("hello, world", 3);
	check_decoding<shift::uint16_t        , shift::big_endian   >("hello, world", 3);
	check_decoding<shift::uint32_t        , shift::little_endian>("hello, world", 0);
	check_decoding<shift::uint32_t        , shift::big_endian   >("hello, world", 0);
	check_decoding<shift::uint32_t        , shift::little_endian>("hello, world", 3);
	check_decoding<shift::uint32_t        , shift::big_endian   >("hello, world", 3);
	check_decoding<shift::uint64_t        , shift::little_endian>("hello, world", 0);
	check_decoding<shift::uint64_t        , shift::big_endian   >("hello, world", 0);
	check_decoding<shift::uint64_t        , shift::little_endian>("hello, world", 3);
	check_decoding<shift::uint64_t        , shift::big_endian   >("hello, world", 3);
	check_decoding<shift::variable_length , shift::little_endian>("hello, world", 0);
	check_decoding<shift::variable_length , shift::big_endian   >("hello, world", 0);
	check_decoding<shift::variable_length , shift::little_endian>("hello, world", 3);
	check_decoding<shift::variable_length , shift::big_endian   >("hello, world", 3);
	check_decoding<shift::no_size_field   , shift::little_endian>("hello, world", 0);
	check_decoding<shift::no_size_field   , shift::big_endian   >("hello, world", 0);
	check_decoding<shift::no_size_field   , shift::little_endian>("hello, world", 3);
	check_decoding<shift::no_size_field   , shift::big_endian   >("hello, world", 3);
	check_decoding<shift::static_size<12> , shift::little_endian>("hello, world", 0);
	check_decoding<shift::static_size<12> , shift::big_endian   >("hello, world", 0);
	check_decoding<shift::static_size<12> , shift::little_endian>("hello, world", 3);
	check_decoding<shift::static_size<12> , shift::big_endian   >("hello, world", 3);

	const std::string test_string =
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit.Fusce facilisis tellus quis scelerisque "
		"eleifend. Sed consequat luctus lacus sed lobortis. Pellentesque eget libero eros. Pellentesque at "
		"augue lacinia, malesuada odio quis, scelerisque purus. Fusce sit amet tortor ut augue eleifend "
		"sodales. Mauris volutpat imperdiet euismod. Curabitur et aliquet est. Nunc sed laoreet neque, at "
		"placerat mauris. Duis blandit egestas massa, a ultricies augue vulputate ac.";

	check_decoding<shift::uint16_t        , shift::little_endian>(test_string,   0);
	check_decoding<shift::uint16_t        , shift::big_endian   >(test_string,   0);
	check_decoding<shift::uint16_t        , shift::little_endian>(test_string, 333);
	check_decoding<shift::uint16_t        , shift::big_endian   >(test_string, 333);
	check_decoding<shift::uint32_t        , shift::little_endian>(test_string,   0);
	check_decoding<shift::uint32_t        , shift::big_endian   >(test_string,   0);
	check_decoding<shift::uint32_t        , shift::little_endian>(test_string, 333);
	check_decoding<shift::uint32_t        , shift::big_endian   >(test_string, 333);
	check_decoding<shift::uint64_t        , shift::little_endian>(test_string,   0);
	check_decoding<shift::uint64_t        , shift::big_endian   >(test_string,   0);
	check_decoding<shift::uint64_t        , shift::little_endian>(test_string, 333);
	check_decoding<shift::uint64_t        , shift::big_endian   >(test_string, 333);
	check_decoding<shift::variable_length , shift::little_endian>(test_string,   0);
	check_decoding<shift::variable_length , shift::big_endian   >(test_string,   0);
	check_decoding<shift::variable_length , shift::little_endian>(test_string, 333);
	check_decoding<shift::variable_length , shift::big_endian   >(test_string, 333);
	check_decoding<shift::no_size_field   , shift::little_endian>(test_string,   0);
	check_decoding<shift::no_size_field   , shift::big_endian   >(test_string,   0);
	check_decoding<shift::no_size_field   , shift::little_endian>(test_string, 333);
	check_decoding<shift::no_size_field   , shift::big_endian   >(test_string, 333);
	check_decoding<shift::static_size<462>, shift::little_endian>(test_string,   0);
	check_decoding<shift::static_size<462>, shift::big_endian   >(test_string,   0);
	check_decoding<shift::static_size<462>, shift::little_endian>(test_string, 333);
	check_decoding<shift::static_size<462>, shift::big_endian   >(test_string, 333);
}

TEST_CASE( "when writing a string to a stream, only the specified range in the buffer is modified"
         , "[string, encoding]" )
{
	const unsigned int start_pos   = 3;
	const unsigned int buffer_size = 64;
	const std::string str          = "hello, world! hello, universe!";

	typedef shift::sink<shift::little_endian, shift::static_buffer<buffer_size> > sink_type;
	sink_type sink;

	try {
		sink << shift::buffer_position(start_pos)
		     << shift::ostring<shift::uint8_t>(str);
		test::check_all_zero_except(sink.buffer(), buffer_size, start_pos, start_pos + str.length());

	} catch (shift::out_of_range& e) {
		CHECK(false);
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

//	example::debug_print(std::cout, sink);
}

TEST_CASE( "writing a string beyond the capacity of the buffer causes an out_ot_range exception to be thrown"
         , "[string, encoding]" )
{
	const std::string str = "The quick brown fox jumps over the lazy dog";
	const unsigned int buffer_size = 64;
	const unsigned int start_pos   = 64 - str.length();

	typedef shift::sink<shift::little_endian, shift::static_buffer<buffer_size> > sink_type;
	sink_type sink;

	try {
		sink << shift::buffer_position(start_pos)
		     << shift::ostring<shift::uint8_t>(str);
		CHECK(false);
	} catch (shift::out_of_range& e) {
		CHECK(true);
	}

	try {
		sink % shift::buffer_position(start_pos)
		     % shift::ostring<shift::uint8_t>(str);
		CHECK(false);
	} catch (shift::out_of_range& e) {
		CHECK(true);
	}
}

TEST_CASE( "trying to read a string beyond the size of a source results in an out_of_range exception to be thrown"
         , "[string, decoding]" )
{
	const unsigned int buffer_size   = 64;
	const unsigned int string_length = 20;
	const unsigned int start_pos     = buffer_size - string_length + 1;

	shift::byte_type buffer[buffer_size];

	typedef shift::source<shift::big_endian> source_type;
	source_type source(buffer, buffer_size);
	std::string result;

	try {
		source >> shift::buffer_position(start_pos)
		       >> shift::istring<shift::static_size<string_length> >(result);
		CHECK(false);
	} catch (shift::out_of_range& e) {
		CHECK(true);
		CHECK(result.size() == 0);
	}

	result = "";
	try {
		source % shift::buffer_position(start_pos)
		       % shift::istring<shift::static_size<string_length> >(result);
		CHECK(false);
	} catch (shift::out_of_range& e) {
		CHECK(true);
		CHECK(result.size() == 0);
	}
}

}}
