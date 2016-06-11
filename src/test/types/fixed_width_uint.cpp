#include <catch.hpp>

#include <shift/types/fixed_width_uint.hpp>

namespace test { namespace {

TEST_CASE( "fixed_width_uint<>: default constructed instances have the initial value 0"
         , "fixed_width_uint<>")
{
	shift::fixed_width_uint<unsigned char, 1> v1;
	CHECK(*v1 == 0);

	shift::fixed_width_uint<unsigned short, 2> v2;
	CHECK(*v2 == 0);

	shift::fixed_width_uint<unsigned short, 3> v3;
	CHECK(*v3 == 0);

	shift::fixed_width_uint<unsigned int, 9> v9;
	CHECK(*v9 == 0);

	shift::fixed_width_uint<unsigned int, 31> v31;
	CHECK(*v31 == 0);

	shift::fixed_width_uint<shift::uint64_t, 60> v60;
	CHECK(*v60 == 0);

	shift::fixed_width_uint<shift::uint64_t, 64> v64;
	CHECK(*v64 == 0);
}

TEST_CASE( "fixed_width_uint<>: values < 2^n_bits can be assigned at construction"
         , "fixed_width_uint<>")
{
	const unsigned int n_bits = 5;
	const unsigned int max_value = std::pow(2., static_cast<double>(n_bits)) - 1;
	typedef shift::fixed_width_uint<unsigned char, n_bits> uint_t;

	for (unsigned int i=0; i<=max_value; ++i) {
		uint_t tmp(i);
		CHECK(*tmp == i);
	}

	uint_t tmp(max_value + 1);
	CHECK(*tmp == 0);
}

TEST_CASE( "fixed_width_uint<>: values < 2^n_bits can be assigned to fixed_width_uint<..., n_bits>"
         , "fixed_width_uint<>")
{
	const unsigned int n_bits = 7;
	const unsigned int max_value = std::pow(2., static_cast<double>(n_bits)) - 1;
	typedef shift::fixed_width_uint<unsigned short, n_bits> uint_t;
	uint_t value;

	for (unsigned int i=0; i<=max_value; ++i) {
		value = i;
		CHECK(*value == i);
	}

	value = max_value + 1;
	CHECK(*value == 0);
}

TEST_CASE( "fixed_width_uint<>: values >= 2^n_bits are masked at construction of fixed_width_uint<..., n_bits>"
         , "fixed_width_uint<>")
{
	const unsigned int n_bits = 2;
	const unsigned int max_value = std::pow(2., static_cast<double>(n_bits)) - 1;
	typedef shift::fixed_width_uint<unsigned long, n_bits> uint_t;

	for (int i=0; i<max_value; ++i) {
		uint_t tmp(max_value + 1 + i);
		CHECK(*tmp == i);
	}
}

TEST_CASE( "fixed_width_uint<>: values >= 2^n_bits are masked at assignment to fixed_width_uint<..., n_bits>"
         , "fixed_width_uint<>")
{
	const unsigned int n_bits = 6;
	const unsigned int max_value = std::pow(2., static_cast<double>(n_bits)) - 1;
	typedef shift::fixed_width_uint<unsigned int, n_bits> uint_t;
	uint_t value;

	for (int i=0; i<max_value; ++i) {
		value = max_value + 1 + i;
		CHECK(*value == i);
	}
}

TEST_CASE( "fixed_width_uint<>: assignment operator (value_type)"
         , "fixed_width_uint<>")
{
	const unsigned int n_bits = 4;
	const unsigned int max_value = std::pow(2., static_cast<double>(n_bits)) - 1;
	typedef shift::fixed_width_uint<unsigned short, n_bits> uint_t;
	uint_t value;

	value = 0;
	CHECK(*value == 0);

	value = 1;
	CHECK(*value == 1);

	value = max_value - 1;
	CHECK(*value == max_value - 1);

	value = max_value;
	CHECK(*value == max_value);
}

TEST_CASE( "fixed_width_uint<>: assignment operator (fixed_width_uint<>)"
         , "fixed_width_uint<>")
{
	const unsigned int n_bits = 4;
	const unsigned int max_value = std::pow(2., static_cast<double>(n_bits)) - 1;
	typedef shift::fixed_width_uint<unsigned char, n_bits> uint_t;
	uint_t value;

	value = uint_t(0);
	CHECK(*value == 0);

	value = uint_t(1);
	CHECK(*value == 1);

	value = uint_t(max_value-1);
	CHECK(*value == max_value-1);

	value = uint_t(max_value);
	CHECK(*value == max_value);
}

TEST_CASE( "fixed_width_uint<>: swap(fixed_width_uint<>)"
         , "fixed_width_uint<>")
{
	const unsigned int n_bits = 11;
	typedef shift::fixed_width_uint<unsigned short, n_bits> uint_t;
	const unsigned short v1 = 293, v2 = 117;
	uint_t obj_1(v1);
	uint_t obj_2(v2);

	REQUIRE(*obj_1 == v1);
	REQUIRE(*obj_2 == v2);

	obj_1.swap(obj_2);

	CHECK(*obj_1 == v2);
	CHECK(*obj_2 == v1);
}

TEST_CASE( "fixed_width_uint<>: set"
         , "fixed_width_uint<>")
{
	const unsigned int n_bits = 10;
	const unsigned int max_value = std::pow(2., static_cast<double>(n_bits)) - 1;
	typedef shift::fixed_width_uint<unsigned short, n_bits> uint_t;
	uint_t value;

	value.set(0);
	CHECK(*value == 0);

	value.set(1);
	CHECK(*value == 1);

	value.set(5);
	CHECK(*value == 5);

	value.set(13);
	CHECK(*value == 13);

	value.set(999);
	CHECK(*value == 999);

	value.set(max_value-1);
	CHECK(*value == max_value-1);

	value .set(max_value);
	CHECK(*value == max_value);
}

TEST_CASE( "fixed_width_uint<>: get"
         , "fixed_width_uint<>")
{
	const unsigned int n_bits = 5;
	const unsigned int max_value = std::pow(2., static_cast<double>(n_bits)) - 1;
	typedef shift::fixed_width_uint<unsigned int, n_bits> uint_t;
	uint_t value;

	value.set(0);
	CHECK(value.get() == 0);

	value.set(1);
	CHECK(value.get() == 1);

	value.set(8);
	CHECK(value.get() == 8);

	value.set(17);
	CHECK(value.get() == 17);

	value.set(max_value-1);
	CHECK(value.get() == max_value-1);

	value .set(max_value);
	CHECK(value.get() == max_value);
}

TEST_CASE( "fixed_width_uint<>: dereference operator"
         , "fixed_width_uint<>")
{
	const unsigned int n_bits = 5;
	const unsigned int max_value = std::pow(2., static_cast<double>(n_bits)) - 1;
	typedef shift::fixed_width_uint<unsigned int, n_bits> uint_t;
	uint_t value;

	value = 0;
	CHECK(*value == 0);
	CHECK(*value == value.get());

	value = 1;
	CHECK(*value == 1);
	CHECK(*value == value.get());

	value = 5;
	CHECK(*value == 5);
	CHECK(*value == value.get());

	value = 13;
	CHECK(*value == 13);
	CHECK(*value == value.get());
}

TEST_CASE( "fixed_width_uint<>: comparison operators fixed_width_uint<IntType, SizeBits>, fixed_width_uint<IntType, SizeBits>"
         , "fixed_width_uint<>")
{
	const unsigned int n_bits = 4;
	typedef shift::fixed_width_uint<unsigned int, n_bits> uint3_t;
	const uint3_t value_2(2);
	const uint3_t value_3(3);
	const uint3_t another_value_3(3);
	const uint3_t value_4(4);

	CHECK(    (value_3 == another_value_3));
	CHECK(not (value_2 == value_3));
	CHECK(not (value_2 == value_4));
	CHECK(not (value_3 == value_4));

	CHECK(    (value_3 != value_2));
	CHECK(    (value_3 != value_4));
	CHECK(    (value_2 != value_4));
	CHECK(not (value_3 != another_value_3));

	CHECK(    (value_2 < value_3));
	CHECK(    (value_2 < value_4));
	CHECK(    (value_3 < value_4));
	CHECK(not (value_3 < value_2));
	CHECK(not (value_4 < value_2));
	CHECK(not (value_4 < value_3));

	CHECK(    (value_3 > value_2));
	CHECK(    (value_4 > value_2));
	CHECK(    (value_4 > value_3));
	CHECK(not (value_2 > value_3));
	CHECK(not (value_2 > value_4));
	CHECK(not (value_3 > value_4));

	CHECK(    (value_2 <= value_3));
	CHECK(    (value_3 <= another_value_3));
	CHECK(    (value_2 <= value_4));
	CHECK(    (value_3 <= value_4));
	CHECK(not (value_3 <= value_2));
	CHECK(not (value_4 <= value_2));
	CHECK(not (value_4 <= value_2));

	CHECK(    (value_3 >= value_2));
	CHECK(    (value_3 >= another_value_3));
	CHECK(    (value_4 >= value_2));
	CHECK(    (value_4 >= value_3));
	CHECK(not (value_2 >= value_3));
	CHECK(not (value_2 >= value_4));
	CHECK(not (value_3 >= value_4));
}

TEST_CASE("fixed_width_uint<>: comparison operators fixed_width_uint<IntType, SizeBits>, IntType"
         , "fixed_width_uint<>")
{
	const unsigned int n_bits = 5;
	typedef shift::fixed_width_uint<unsigned short, n_bits> uint3_t;
	const uint3_t value_20(20);
	const uint3_t value_21(21);
	const uint3_t value_22(22);

	CHECK(    (value_21 == 21));
	CHECK(not (value_20 == 21));
	CHECK(not (value_20 == 22));
	CHECK(not (value_21 == 22));

	CHECK(    (value_21 != 20));
	CHECK(    (value_21 != 22));
	CHECK(    (value_20 != 22));
	CHECK(not (value_21 != 21));

	CHECK(    (value_20 < 21));
	CHECK(    (value_20 < 22));
	CHECK(    (value_21 < 22));
	CHECK(not (value_21 < 20));
	CHECK(not (value_22 < 20));
	CHECK(not (value_22 < 21));

	CHECK(    (value_21 > 20));
	CHECK(    (value_22 > 20));
	CHECK(    (value_22 > 21));
	CHECK(not (value_20 > 21));
	CHECK(not (value_20 > 22));
	CHECK(not (value_21 > 22));

	CHECK(    (value_20 <= 21));
	CHECK(    (value_21 <= 21));
	CHECK(    (value_20 <= 22));
	CHECK(    (value_21 <= 22));
	CHECK(not (value_21 <= 20));
	CHECK(not (value_22 <= 20));
	CHECK(not (value_22 <= 20));

	CHECK(    (value_21 >= 20));
	CHECK(    (value_21 >= 21));
	CHECK(    (value_22 >= 20));
	CHECK(    (value_22 >= 21));
	CHECK(not (value_20 >= 21));
	CHECK(not (value_20 >= 22));
	CHECK(not (value_21 >= 22));
}

TEST_CASE("fixed_width_uint<>: comparison operators IntType, fixed_width_uint<IntType, SizeBits>"
         , "fixed_width_uint<>")
{
	const unsigned int n_bits = 2;
	typedef shift::fixed_width_uint<unsigned char, n_bits> uint3_t;
	const uint3_t value_0(0);
	const uint3_t value_1(1);
	const uint3_t value_2(2);

	CHECK(    (1 == value_1));
	CHECK(not (0 == value_1));
	CHECK(not (0 == value_2));
	CHECK(not (1 == value_2));

	CHECK(    (1 != value_0));
	CHECK(    (1 != value_2));
	CHECK(    (0 != value_2));
	CHECK(not (1 != value_1));

	CHECK(    (0 < value_1));
	CHECK(    (0 < value_2));
	CHECK(    (1 < value_2));
	CHECK(not (1 < value_0));
	CHECK(not (2 < value_0));
	CHECK(not (2 < value_1));

	CHECK(    (1 > value_0));
	CHECK(    (2 > value_0));
	CHECK(    (2 > value_1));
	CHECK(not (0 > value_1));
	CHECK(not (0 > value_2));
	CHECK(not (1 > value_2));

	CHECK(    (0 <= value_1));
	CHECK(    (1 <= value_1));
	CHECK(    (0 <= value_2));
	CHECK(    (1 <= value_2));
	CHECK(not (1 <= value_0));
	CHECK(not (2 <= value_0));
	CHECK(not (2 <= value_0));

	CHECK(    (1 >= value_0));
	CHECK(    (1 >= value_1));
	CHECK(    (2 >= value_0));
	CHECK(    (2 >= value_1));
	CHECK(not (0 >= value_1));
	CHECK(not (0 >= value_2));
	CHECK(not (1 >= value_2));
}

TEST_CASE("fixed_width_uint<>: typedefs have the correct number of bits set"
         , "fixed_width_uint<>")
{
	CHECK( (shift::uint1_t ::num_bits ==  1));
	CHECK( (shift::uint2_t ::num_bits ==  2));
	CHECK( (shift::uint3_t ::num_bits ==  3));
	CHECK( (shift::uint4_t ::num_bits ==  4));
	CHECK( (shift::uint5_t ::num_bits ==  5));
	CHECK( (shift::uint6_t ::num_bits ==  6));
	CHECK( (shift::uint7_t ::num_bits ==  7));
	CHECK( (shift::uint9_t ::num_bits ==  9));
	CHECK( (shift::uint10_t::num_bits == 10));
	CHECK( (shift::uint11_t::num_bits == 11));
	CHECK( (shift::uint12_t::num_bits == 12));
	CHECK( (shift::uint13_t::num_bits == 13));
	CHECK( (shift::uint14_t::num_bits == 14));
	CHECK( (shift::uint15_t::num_bits == 15));
}

TEST_CASE("fixed_width_uint<>: the bit mask is correctly calculated"
         , "fixed_width_uint<>")
{
	CHECK( (shift::uint1_t ::mask == std::pow(2.,  1.) - 1));
	CHECK( (shift::uint2_t ::mask == std::pow(2.,  2.) - 1));
	CHECK( (shift::uint3_t ::mask == std::pow(2.,  3.) - 1));
	CHECK( (shift::uint4_t ::mask == std::pow(2.,  4.) - 1));
	CHECK( (shift::uint5_t ::mask == std::pow(2.,  5.) - 1));
	CHECK( (shift::uint6_t ::mask == std::pow(2.,  6.) - 1));
	CHECK( (shift::uint7_t ::mask == std::pow(2.,  7.) - 1));
	CHECK( (shift::uint9_t ::mask == std::pow(2.,  9.) - 1));
	CHECK( (shift::uint10_t::mask == std::pow(2., 10.) - 1));
	CHECK( (shift::uint11_t::mask == std::pow(2., 11.) - 1));
	CHECK( (shift::uint12_t::mask == std::pow(2., 12.) - 1));
	CHECK( (shift::uint13_t::mask == std::pow(2., 13.) - 1));
	CHECK( (shift::uint14_t::mask == std::pow(2., 14.) - 1));
	CHECK( (shift::uint15_t::mask == std::pow(2., 15.) - 1));
}

}} // test
