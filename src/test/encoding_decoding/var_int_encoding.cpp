#include <catch.hpp>

#include <shift/detail/var_int.hpp>
#include <shift/sink.hpp>
#include <shift/source.hpp>
#include <shift/types/var_int.hpp>
#include <shift/operator/var_int.hpp>
#include <shift/buffer/static_buffer.hpp>

#include <examples/utility.hpp>

namespace test { namespace {

typedef shift::buffer_position pos;

template<typename UintType, typename IntType>
UintType encode(IntType v) {
	return (v < 0) ? -v*2-1 : v*2;
}

template<typename IntType, typename UintType>
IntType decode(UintType v) {
	return (v%2) ? -(v/2+1) : v/2;
}

TEST_CASE( "max multiplier for reading variable length uints"
         , "var_uint")
{
	const shift::uint32_t v_32 = shift::detail::max_multiplier<shift::uint32_t>();
	CHECK(v_32 == 128 * 128 * 128);

	const shift::uint64_t v_64 = shift::detail::max_multiplier<shift::uint64_t>();
	CHECK(v_64 == static_cast<shift::uint64_t>(std::pow(128., 8.)));
}

TEST_CASE( "zig zag encoding of signed integers"
         , "[var_int]" )
{
	int array[] = { 0, -1, 1, -2, 2, -3, 3, -4, 4, -227, 227,  -1233, 1233, -5983, 5983, -179356, 179356, -318367, 318367, 2147483647, -2147483648 };
	const unsigned int size = sizeof(array)/sizeof(int);

	for(std::size_t i=0; i<size; ++i) {
		CHECK( encode<unsigned int>(array[i]) == shift::detail::zig_zag_encode<unsigned int>(array[i]));
	}
}

TEST_CASE( "zig zag decoding of signed integers"
         , "[var_int]" )
{
	unsigned int array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 227, 228,  1233, 1234, 5983, 5984, 179356, 179357, 318367, 318368, 2147483646, 2147483647 };
	const unsigned int size = sizeof(array)/sizeof(int);

	for(std::size_t i=0; i<size; ++i) {
		CHECK( decode<int>(array[i]) == shift::detail::zig_zag_decode<int>(array[i]));
	}
}

TEST_CASE( "variable length encoding and decoding of unsigned integers"
         , "[var_uint]")
{
	typedef shift::sink  <shift::little_endian, shift::static_buffer<64> > sink_type;
	typedef shift::source<shift::little_endian>                            source_type;
	typedef shift::var_int<unsigned long long>                             uint_type;

	sink_type sink;

	// 8

	unsigned long long v = 8;
	uint_type value(v);
	REQUIRE(*value == v);

	sink << pos(0) << value;

	CHECK(sink.size() == 1);
	CHECK(sink.buffer()[0] == 8);

	*value = 0;
	REQUIRE(*value == 0);
	source_type source(sink.buffer(), sink.size());
	source >> pos(0) >> value;

	CHECK(*value == v);
	sink.clear();

	// 127

	v = 127;
	value = v;
	REQUIRE(*value == v);
	sink << pos(0) << value;

	CHECK(sink.size() == 1);
	CHECK(sink.buffer()[0] == 127);

	*value = 0;
	REQUIRE(*value == 0);
	source = source_type(sink.buffer(), sink.size());
	source >> pos(0) >> value;

	CHECK(*value == v);

	sink.clear();

	// 128 = 1 * 128

	v = 128;
	value = v;
	REQUIRE(*value == v);
	sink << pos(0) << value;

	CHECK(sink.size() == 2);
	CHECK(sink.buffer()[0] == 128);
	CHECK(sink.buffer()[1] ==   1);

	*value = 0;
	REQUIRE(*value == 0);
	source = source_type(sink.buffer(), sink.size());
	source >> pos(0) >> value;

	CHECK(*value == v);

	sink.clear();

	// 256 = 2 * 128

	v = 256;
	value = v;
	REQUIRE(*value == v);
	sink << pos(0) << value;

	CHECK(sink.size() == 2);
	CHECK(sink.buffer()[0] == 128);
	CHECK(sink.buffer()[1] ==   2);

	*value = 0;
	REQUIRE(*value == 0);
	source = source_type(sink.buffer(), sink.size());
	source >> pos(0) >> value;

	CHECK(*value == v);

	sink.clear();

	// 257 = 2 * 128 + 1

	v= 257;
	value = v;
	REQUIRE(*value == v);
	sink << pos(0) << value;

	CHECK(sink.size() == 2);
	CHECK(sink.buffer()[0] == 128 + 1);
	CHECK(sink.buffer()[1] ==       2);

	*value = 0;
	REQUIRE(*value == 0);
	source = source_type(sink.buffer(), sink.size());
	source >> pos(0) >> value;

	CHECK(*value == v);

	sink.clear();

	// 793 = 6 * 128 + 25

	v = 793;
	value = v;
	REQUIRE(*value == v);
	sink << pos(0) << value;

	CHECK(sink.size() == 2);
	CHECK(sink.buffer()[0] == 128 + 25);
	CHECK(sink.buffer()[1] ==        6);

	*value = 0;
	REQUIRE(*value == 0);
	source = source_type(sink.buffer(), sink.size());
	source >> pos(0) >> value;

	CHECK(*value == v);

	sink.clear();

	// 25385 = 128 * 128 + 70 * 128 + 41

	v = 25385;
	value = v;
	REQUIRE(*value == v);
	sink << pos(0) << value;

	CHECK(sink.size() == 3);
	CHECK(sink.buffer()[0] == 128 + 41);
	CHECK(sink.buffer()[1] == 128 + 70);
	CHECK(sink.buffer()[2] ==        1);

	*value = 0;
	REQUIRE(*value == 0);
	source = source_type(sink.buffer(), sink.size());
	source >> pos(0) >> value;

	CHECK(*value == v);

	sink.clear();

	// 23 115 431 = 11 * 128^3 + 2*128^2 + 109*128 + 39

	v = 23115431;
	value = v;
	REQUIRE(*value == v);
	sink << pos(0) << value;

	CHECK(sink.size() == 4);
	CHECK(sink.buffer()[0] == 128 +  39);
	CHECK(sink.buffer()[1] == 128 + 109);
	CHECK(sink.buffer()[2] == 128 +   2);
	CHECK(sink.buffer()[3] ==        11);

	*value = 0;
	REQUIRE(*value == 0);
	source = source_type(sink.buffer(), sink.size());
	source >> pos(0) >> value;

	CHECK(*value == v);
	sink.clear();

	// 789 145 708 = 2*128^4 + 120*128^3 + 37*128^2 + 80*128 + 108

	v = 789145708;
	value = v;
	REQUIRE(*value == v);
	sink << pos(0) << value;

	CHECK(sink.size() == 5);
	CHECK(sink.buffer()[0] == 128 + 108);
	CHECK(sink.buffer()[1] == 128 +  80);
	CHECK(sink.buffer()[2] == 128 +  37);
	CHECK(sink.buffer()[3] == 128 + 120);
	CHECK(sink.buffer()[4] ==         2);

	*value = 0;
	REQUIRE(*value == 0);
	source = source_type(sink.buffer(), sink.size());
	source >> pos(0) >> value;

	CHECK(*value == v);
	sink.clear();
}

TEST_CASE( "malformed_var_uint is thrown when trying to read a malformed variable length unsigned integer"
         , "[var_uint]")
{
	const unsigned int buffer_size = 20;
	shift::byte_type buffer[buffer_size];

	for(unsigned int i=0; i<9; ++i)
		buffer[i] = 129;

	shift::var_int<shift::uint64_t> value;
	shift::source<shift::little_endian> source(buffer, buffer_size);

	try {
		source >> shift::pos(0) >> value;
		CHECK(false);
	} catch(const shift::malformed_var_uint& e) {
		CHECK(true);
	} catch(const shift::out_of_range& e) {
		CHECK(false);
	}
}

TEST_CASE( "variable length encoding and decoding of signed integers"
         , "[var_int]")
{
	typedef shift::sink  <shift::little_endian, shift::static_buffer<64> > sink_type;
	typedef shift::source<shift::little_endian>                            source_type;
	typedef shift::var_int<long long>                                     int_type;

	sink_type sink;

	// -4
	// zig zag encoded -> 7

	int_type value(-4);
	sink << pos(0) << value;

	CHECK(sink.size() == 1);
	CHECK(sink.buffer()[0] == 7);

	value = 0;
	source_type source(sink.buffer(), sink.size());
	source >> pos(0) >> value;

	CHECK(*value == -4);

	sink.clear();

	// 4
	// zig zag encoded -> 8

	value = 4;
	sink << pos(0) << value;

	CHECK(sink.size() == 1);
	CHECK(sink.buffer()[0] == 8);

	value = 0;
	source = source_type(sink.buffer(), sink.size());
	source >> pos(0) >> value;

	CHECK(*value == 4);

	sink.clear();

	// -13
	// zig zag encoded -> 25

	value = -13;
	sink << pos(0) << value;

	CHECK(sink.size() == 1);
	CHECK(sink.buffer()[0] == 25);

	value = 0;
	source = source_type(sink.buffer(), sink.size());
	source >> pos(0) >> value;

	CHECK(*value == -13);

	sink.clear();

	// 13
	// zig zag encoded -> 26

	value = 13;
	sink << pos(0) << value;

	CHECK(sink.size() == 1);
	CHECK(sink.buffer()[0] == 26);

	value = 0;
	source = source_type(sink.buffer(), sink.size());
	source >> pos(0) >> value;

	CHECK(*value == 13);

	sink.clear();

	// -60
	// zig zag encoded: 119

	value = -60;
	sink << pos(0) << value;

	CHECK(sink.size() == 1);
	CHECK(sink.buffer()[0] == 119);

	value = 0;
	source = source_type(sink.buffer(), sink.size());
	source >> pos(0) >> value;

	CHECK(*value == -60);

	sink.clear();

	// 60
	// zig zag encoded: 120

	value = 60;
	sink << pos(0) << value;

	CHECK(sink.size() == 1);
	CHECK(sink.buffer()[0] == 120);

	value = 0;
	source = source_type(sink.buffer(), sink.size());
	source >> pos(0) >> value;

	CHECK(*value == 60);

	sink.clear();

	// -187
	// zig zag encoded: 373
	// 373 = 2*128 + 117

	value = -187;
	sink << pos(0) << value;

	CHECK(sink.size() == 2);
	CHECK(sink.buffer()[0] == 128 + 117);
	CHECK(sink.buffer()[1] ==         2);

	value = 0;
	source = source_type(sink.buffer(), sink.size());
	source >> pos(0) >> value;

	CHECK(*value == -187);

	sink.clear();

	// 187
	// zig zag encoded: 374
	// 374 = 2*128 + 118

	value = 187;
	sink << pos(0) << value;

	CHECK(sink.size() == 2);
	CHECK(sink.buffer()[0] == 128 + 118);
	CHECK(sink.buffer()[1] ==         2);

	value = 0;
	source = source_type(sink.buffer(), sink.size());
	source >> pos(0) >> value;

	CHECK(*value == 187);

	sink.clear();

	// -33597
	// zig zag encoded = 67193
	// 67193 = 4 * 128 * 128 + 12 * 128 + 121

	value = -33597;
	sink << pos(0) << value;

	CHECK(sink.size() == 3);
	CHECK(sink.buffer()[0] == 128 + 121);
	CHECK(sink.buffer()[1] == 128 +  12);
	CHECK(sink.buffer()[2] ==         4);

	value = 0;
	source = source_type(sink.buffer(), sink.size());
	source >> pos(0) >> value;

	CHECK(*value == -33597);

	sink.clear();

	// 33597
	// zig zag encoded = 67194
	// 67194 = 4 * 128 * 128 + 12 * 128 + 122

	value = 33597;
	sink << pos(0) << value;

	CHECK(sink.size() == 3);
	CHECK(sink.buffer()[0] == 128 + 122);
	CHECK(sink.buffer()[1] == 128 +  12);
	CHECK(sink.buffer()[2] ==         4);

	value = 0;
	source = source_type(sink.buffer(), sink.size());
	source >> pos(0) >> value;

	CHECK(*value == 33597);

	sink.clear();

	// -17878060
	// zig zag encoded: 35756119
	// 35756119 = 17 * 128^3 + 6 * 128^2 + 48*128 + 87

	value = -17878060;
	sink << pos(0) << value;

	CHECK(sink.size() == 4);
	CHECK(sink.buffer()[0] == 128 + 87);
	CHECK(sink.buffer()[1] == 128 + 48);
	CHECK(sink.buffer()[2] == 128 +  6);
	CHECK(sink.buffer()[3] ==       17);

	value = 0;
	source = source_type(sink.buffer(), sink.size());
	source >> pos(0) >> value;

	CHECK(*value == -17878060);

	sink.clear();

	// 17878060
	// zig zag encoded: 35756120
	// 35756119 = 17 * 128^3 + 6 * 128^2 + 48*128 + 88

	value = 17878060;
	sink << pos(0) << value;

	CHECK(sink.size() == 4);
	CHECK(sink.buffer()[0] == 128 + 88);
	CHECK(sink.buffer()[1] == 128 + 48);
	CHECK(sink.buffer()[2] == 128 +  6);
	CHECK(sink.buffer()[3] ==       17);

	value = 0;
	source = source_type(sink.buffer(), sink.size());
	source >> pos(0) >> value;

	CHECK(*value == 17878060);

	sink.clear();
}

}} // test
