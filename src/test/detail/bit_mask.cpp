#include <catch.hpp>
#include <shift/detail/bit_mask.hpp>

namespace test { namespace {

using shift::detail::bit_mask_all_bits;

unsigned int get_bit_mask(unsigned int n_bits) {
	unsigned int result = 0;
	for (unsigned i=0; i<n_bits; ++i)
		result |= 1 << i;
	return result;
}

#define CHECK_BITMASK(n_bits)                                                     \
	{                                                                             \
		const unsigned int value = bit_mask_all_bits<n_bits, unsigned int>::value;\
		CHECK( value == get_bit_mask(n_bits));                                    \
	}

TEST_CASE( "test bit_mask_all_bits for different number of bits"
         , "[bit_mask]" )
{
	CHECK_BITMASK(1);
	CHECK_BITMASK(2);
	CHECK_BITMASK(3);

	CHECK_BITMASK(7);
	CHECK_BITMASK(8);
	CHECK_BITMASK(9);

	CHECK_BITMASK(23);
	CHECK_BITMASK(27);

	CHECK_BITMASK(31);
	CHECK_BITMASK(32);
}

TEST_CASE( "bit_mask (1-8 bits)"
         , "[bit_mask]")
{
	CHECK(shift::detail::bit_mask::get(1) == 1);
	CHECK(shift::detail::bit_mask::get(2) == 1 + 2);
	CHECK(shift::detail::bit_mask::get(3) == 1 + 2 + 4);
	CHECK(shift::detail::bit_mask::get(4) == 1 + 2 + 4  + 8);
	CHECK(shift::detail::bit_mask::get(5) == 1 + 2 + 4  + 8 + 16);
	CHECK(shift::detail::bit_mask::get(6) == 1 + 2 + 4  + 8 + 16 + 32);
	CHECK(shift::detail::bit_mask::get(7) == 1 + 2 + 4  + 8 + 16 + 32 + 64);
	CHECK(shift::detail::bit_mask::get(8) == 1 + 2 + 4  + 8 + 16 + 32 + 64 + 128);
}

TEST_CASE( "bit_mask throws if number of bits outside valid range (1-8)"
         , "[bit_mask]")
{
	CHECK_THROWS_AS(shift::detail::bit_mask::get(0), const shift::out_of_range&);
	CHECK_THROWS_AS(shift::detail::bit_mask::get(9), const shift::out_of_range&);
}

#undef CHECK_BITMASK

}} // namespace test

