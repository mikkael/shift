#include <catch.hpp>

#include <shift/detail/endian_reversal.hpp>

namespace test { namespace {

TEST_CASE( "endian_reverse: uint8_t"
         , "[endian_reverse]" )
{
	const shift::uint8_t value = 123;
	CHECK(shift::detail::endian_reverse(value) == value);
}

TEST_CASE( "endian_reverse: uint16_t"
         , "[endian_reverse]" )
{
	shift::uint16_t value = 258;
	shift::uint16_t reversed = shift::detail::endian_reverse(value);
	CHECK(reversed == 513);
	CHECK( shift::detail::endian_reverse(reversed) == value );

	value = 0x0100;
	reversed = shift::detail::endian_reverse(value);
	CHECK( reversed == 0x0001 );
	CHECK( shift::detail::endian_reverse(reversed) == value );

	value = 0x0101;
	reversed = shift::detail::endian_reverse(value);
	CHECK( reversed == 0x0101);
	CHECK( shift::detail::endian_reverse(reversed) == value );

	value = 0xA356;
	reversed = shift::detail::endian_reverse(value);
	CHECK( reversed == 0x56A3);
	CHECK( shift::detail::endian_reverse(reversed) == value );

	uint8_t* p_value    = reinterpret_cast<shift::uint8_t*>(&value   );
	uint8_t* p_reversed = reinterpret_cast<shift::uint8_t*>(&reversed);

	CHECK(p_value[0] == p_reversed[1]);
	CHECK(p_value[1] == p_reversed[0]);
}

TEST_CASE( "endian_reverse: uint32_t"
         , "[endian_reverse]" )
{
	shift::uint32_t value    = 31123321;
	shift::uint32_t reversed = shift::detail::endian_reverse(value);

	uint8_t* p_value    = reinterpret_cast<uint8_t*>(&value   );
	uint8_t* p_reversed = reinterpret_cast<uint8_t*>(&reversed);

	REQUIRE(p_value[0] == p_reversed[3]);
	REQUIRE(p_value[1] == p_reversed[2]);
	REQUIRE(p_value[2] == p_reversed[1]);
	REQUIRE(p_value[3] == p_reversed[0]);

	value    = 0x12A0C345;
	reversed = shift::detail::endian_reverse(value);
	REQUIRE(reversed == 0x45C3A012);
	CHECK( shift::detail::endian_reverse(reversed) == value );

	value    = 0x01000000;
	reversed = shift::detail::endian_reverse(value);
	REQUIRE(reversed == 0x00000001);
	CHECK( shift::detail::endian_reverse(reversed) == value );
}

TEST_CASE( "endian_reverse: uint64_t"
         , "[endian_reverse]" )
{
	shift::uint64_t value    = 13200726100627020021ULL;
	shift::uint64_t reversed = shift::detail::endian_reverse(value);

	uint8_t* p_value    = reinterpret_cast<uint8_t*>(&value   );
	uint8_t* p_reversed = reinterpret_cast<uint8_t*>(&reversed);

	REQUIRE(p_value[0] == p_reversed[7]);
	REQUIRE(p_value[1] == p_reversed[6]);
	REQUIRE(p_value[2] == p_reversed[5]);
	REQUIRE(p_value[3] == p_reversed[4]);
	REQUIRE(p_value[4] == p_reversed[3]);
	REQUIRE(p_value[5] == p_reversed[2]);
	REQUIRE(p_value[6] == p_reversed[1]);
	REQUIRE(p_value[7] == p_reversed[0]);

	value    = 0x12A0C345D922FE8AULL;
	reversed = shift::detail::endian_reverse(value);
	REQUIRE(reversed == 0x8AFE22D945C3A012);
	CHECK( shift::detail::endian_reverse(reversed) == value );

	value    = 0x0102030405060708ULL;
	reversed = shift::detail::endian_reverse(value);
	REQUIRE(reversed == 0x0807060504030201ULL);
	CHECK( shift::detail::endian_reverse(reversed) == value );
}

}} // test::anonymous
