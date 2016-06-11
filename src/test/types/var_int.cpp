#include <catch.hpp>

#include <shift/types/var_int.hpp>

namespace test { namespace {

#define CHECK_COPY_CONSTRUCTOR(type, value)                   \
	{                                                         \
		shift::var_int<type> v( shift::var_int<type>(value)); \
		CHECK(*v ==  value);                                  \
	}

#define CHECK_CONSTRUCTOR_TAKING_VALUE_TYPE_ARGUMENT(type, value) \
	{                                                             \
		shift::var_int<type> v(value);                            \
		CHECK(*v == value);                                       \
	}

#define CHECK_ASSIGNMENT_TO_VALUE_TYPE(type, value) \
	{                                               \
		shift::var_int<type> v;                     \
		v = value;                                  \
		CHECK(*v == value);                         \
	}

#define CHECK_ASSIGNMENT_TO_VARINT(type, value) \
	{                                           \
		shift::var_int<type> v;                 \
		v = shift::var_int<type>(value);        \
		CHECK(*v == value);                     \
	}

#define CHECK_SWAP(type, v1, v2)       \
	{                                  \
		shift::var_int<type> obj1(v1); \
		shift::var_int<type> obj2(v2); \
		REQUIRE(*obj1 == v1);          \
		REQUIRE(*obj2 == v2);          \
		obj1.swap(obj2);               \
		CHECK(*obj1 == v2);            \
		CHECK(*obj2 == v1);            \
		obj2.swap(obj1);               \
		CHECK(*obj1 == v1);            \
		CHECK(*obj2 == v2);            \
	}

#define CHECK_SET(type, value)  \
	{                           \
		shift::var_int<type> v; \
		REQUIRE(*v == 0);       \
		v.set(value);           \
		CHECK(*v == value);     \
	}

#define CHECK_GET(type, value)         \
	{                                  \
		shift::var_int<type> v(value); \
		REQUIRE(*v == value);          \
		CHECK(v.get() == value);       \
	}

#define CHECK_DEREFERENCE_OP(type, value) \
	{                                     \
		shift::var_int<type> v;           \
		REQUIRE(*v == 0);                 \
		*v =  value;                      \
		CHECK(v.get() == value);          \
	}

#define CHECK_DEREFERENCE_OP_CONST(type, value) \
	{                                           \
		shift::var_int<type> v(value);          \
		REQUIRE(v.get() == value);              \
		CHECK(*v== value);                      \
	}

TEST_CASE( "var_int<>: default constructed instances are initialized with zero"
         , "var_int<>")
{
	{ shift::var_int<unsigned short    > v; CHECK(*v == 0); }
	{ shift::var_int<short             > v; CHECK(*v == 0); }
	{ shift::var_int<unsigned int      > v; CHECK(*v == 0); }
	{ shift::var_int<int               > v; CHECK(*v == 0); }
	{ shift::var_int<unsigned long     > v; CHECK(*v == 0); }
	{ shift::var_int<long              > v; CHECK(*v == 0); }
	{ shift::var_int<unsigned long long> v; CHECK(*v == 0); }
	{ shift::var_int<long long         > v; CHECK(*v == 0); }
}

TEST_CASE( "var_int<>: var_int(const var_int&)"
         , "var_int<>")
{
	CHECK_COPY_CONSTRUCTOR(unsigned short    ,                       0);
	CHECK_COPY_CONSTRUCTOR(short             ,                       1);
	CHECK_COPY_CONSTRUCTOR(unsigned int      ,                   66666);
	CHECK_COPY_CONSTRUCTOR(int               ,                  -12345);
	CHECK_COPY_CONSTRUCTOR(unsigned long     ,                42000000);
	CHECK_COPY_CONSTRUCTOR(long              ,                     111);
	CHECK_COPY_CONSTRUCTOR(unsigned long long, 18446744073709551615ULL);
	CHECK_COPY_CONSTRUCTOR(long long         ,                     -13);
}

TEST_CASE( "var_int<>: var_int(IntType&)"
         , "var_int<>")
{
	CHECK_CONSTRUCTOR_TAKING_VALUE_TYPE_ARGUMENT(unsigned short    ,                       0);
	CHECK_CONSTRUCTOR_TAKING_VALUE_TYPE_ARGUMENT(short             ,                      55);
	CHECK_CONSTRUCTOR_TAKING_VALUE_TYPE_ARGUMENT(unsigned int      ,                 1111111);
	CHECK_CONSTRUCTOR_TAKING_VALUE_TYPE_ARGUMENT(int               ,               -19021978);
	CHECK_CONSTRUCTOR_TAKING_VALUE_TYPE_ARGUMENT(unsigned long     ,                      42);
	CHECK_CONSTRUCTOR_TAKING_VALUE_TYPE_ARGUMENT(long              ,                   32178);
	CHECK_CONSTRUCTOR_TAKING_VALUE_TYPE_ARGUMENT(unsigned long long, 18446744073709551615ULL);
	CHECK_CONSTRUCTOR_TAKING_VALUE_TYPE_ARGUMENT(long long         ,                -6789876);
}

TEST_CASE( "var_int<>: assignment to value_type)"
         , "var_int<>")
{
	CHECK_ASSIGNMENT_TO_VALUE_TYPE(unsigned short    ,                       0);
	CHECK_ASSIGNMENT_TO_VALUE_TYPE(short             ,                   12121);
	CHECK_ASSIGNMENT_TO_VALUE_TYPE(unsigned int      ,                92107863);
	CHECK_ASSIGNMENT_TO_VALUE_TYPE(int               ,               -19021978);
	CHECK_ASSIGNMENT_TO_VALUE_TYPE(unsigned long     ,                      42);
	CHECK_ASSIGNMENT_TO_VALUE_TYPE(long              ,       99001783620273829);
	CHECK_ASSIGNMENT_TO_VALUE_TYPE(unsigned long long, 18446744073709551615ULL);
	CHECK_ASSIGNMENT_TO_VALUE_TYPE(long long         ,         -61282392068238);
}

TEST_CASE( "var_int<>: assignment to var_int<>)"
         , "var_int<>")
{
	CHECK_ASSIGNMENT_TO_VARINT(unsigned short    ,                       0);
	CHECK_ASSIGNMENT_TO_VARINT(short             ,                   13313);
	CHECK_ASSIGNMENT_TO_VARINT(unsigned int      ,              3819271098);
	CHECK_ASSIGNMENT_TO_VARINT(int               ,              -526146789);
	CHECK_ASSIGNMENT_TO_VARINT(unsigned long     ,                      42);
	CHECK_ASSIGNMENT_TO_VARINT(long              ,       73101238210128744);
	CHECK_ASSIGNMENT_TO_VARINT(unsigned long long, 18446744073709551615ULL);
	CHECK_ASSIGNMENT_TO_VARINT(long long         ,       -3419204719102788);
}

TEST_CASE( "var_int<>: swap(var_int<>)"
         , "var_int<>")
{
	CHECK_SWAP(unsigned short    ,                       0,                   1);
	CHECK_SWAP(short             ,                   13313,              -13313);
	CHECK_SWAP(unsigned int      ,              3819271098,            28917897);
	CHECK_SWAP(int               ,              -526146789,              543210);
	CHECK_SWAP(unsigned long     ,                      42,                  42);
	CHECK_SWAP(long              ,       73101238210128744,                  13);
	CHECK_SWAP(unsigned long long, 18446744073709551615ULL, 9999999999999999ULL);
	CHECK_SWAP(long long         ,       -3419204719102788,        330033003300);
}

TEST_CASE( "var_int<>: set(value_type)"
         , "var_int<>")
{
	CHECK_SET(unsigned short    ,                   12211);
	CHECK_SET(short             ,                    8178);
	CHECK_SET(unsigned int      ,                 7180172);
	CHECK_SET(int               ,                  -12345);
	CHECK_SET(unsigned long     ,                42424242);
	CHECK_SET(long              ,           3736353433323);
	CHECK_SET(unsigned long long, 18439732809274839387ULL);
	CHECK_SET(long long         ,             -6318197890);
}

TEST_CASE( "var_int<>: get()"
         , "var_int<>")
{
	CHECK_GET(unsigned short    ,                       0);
	CHECK_GET(short             ,                       1);
	CHECK_GET(unsigned int      ,                   66666);
	CHECK_GET(int               ,                  -12345);
	CHECK_GET(unsigned long     ,                42000000);
	CHECK_GET(long              ,                     111);
	CHECK_GET(unsigned long long, 18446744073709551615ULL);
	CHECK_GET(long long         ,                     -13);
}

TEST_CASE( "var_int<>: dereference operator"
         , "var_int<>")
{
	CHECK_DEREFERENCE_OP(unsigned short    ,                       0);
	CHECK_DEREFERENCE_OP(short             ,                   13313);
	CHECK_DEREFERENCE_OP(unsigned int      ,              3819271098);
	CHECK_DEREFERENCE_OP(int               ,              -526146789);
	CHECK_DEREFERENCE_OP(unsigned long     ,                      42);
	CHECK_DEREFERENCE_OP(long              ,       73101238210128744);
	CHECK_DEREFERENCE_OP(unsigned long long, 18446744073709551615ULL);
	CHECK_DEREFERENCE_OP(long long         ,       -3419204719102788);
}

TEST_CASE( "var_int<>: dereference operator const"
         , "var_int<>")
{
	CHECK_DEREFERENCE_OP_CONST(unsigned short    ,                       1);
	CHECK_DEREFERENCE_OP_CONST(short             ,                  -13313);
	CHECK_DEREFERENCE_OP_CONST(unsigned int      ,              3819271098);
	CHECK_DEREFERENCE_OP_CONST(int               ,              -526146789);
	CHECK_DEREFERENCE_OP_CONST(unsigned long     ,                      42);
	CHECK_DEREFERENCE_OP_CONST(long              ,                      13);
	CHECK_DEREFERENCE_OP_CONST(unsigned long long, 12807347432843747477ULL);
	CHECK_DEREFERENCE_OP_CONST(long long         ,           -330033003300);
}

TEST_CASE( "var_int<>: comparison operators - ( var_int<IntType> const&, var_int<IntType> const&)"
         , "var_int<>")
{
	typedef shift::var_int<unsigned short> varint;
	const varint a(1);
	const varint b(2);
	const varint c(3);

	CHECK(    (a == varint(*a)));
	CHECK(not (a == b));
	CHECK(not (a == c));
	CHECK(not (b == c));

	CHECK(not (a != varint(*a)));
	CHECK(    (a != b));
	CHECK(    (a != c));
	CHECK(    (b != c));

	CHECK(    (a < b));
	CHECK(    (a < c));
	CHECK(    (b < c));
	CHECK(not (c < a));
	CHECK(not (c < b));
	CHECK(not (b < a));

	CHECK(    (b > a));
	CHECK(    (c > a));
	CHECK(    (c > b));
	CHECK(not (a > c));
	CHECK(not (a > b));
	CHECK(not (b > c));

	CHECK(    (a <= b));
	CHECK(    (a <= c));
	CHECK(    (b <= c));
	CHECK(not (c <= a));
	CHECK(not (c <= b));
	CHECK(not (b <= a));
	CHECK(    (a <= varint(*a)));
	CHECK(    (b <= varint(*b)));
	CHECK(    (c <= varint(*c)));

	CHECK(    (b >= a));
	CHECK(    (c >= a));
	CHECK(    (c >= b));
	CHECK(not (a >= c));
	CHECK(not (a >= b));
	CHECK(not (b >= c));
	CHECK(    (a >= varint(*a)));
	CHECK(    (b >= varint(*b)));
	CHECK(    (c >= varint(*c)));
}

TEST_CASE( "var_int<>: comparison operators - ( var_int<IntType> const&, value_type)"
         , "var_int<>")
{
	typedef shift::var_int<int> varint;

	const long a = -333, b = -222, c = -111;
	const varint va(a), vb(b), vc(c);

	CHECK(    (va == a));
	CHECK(not (va == b));
	CHECK(not (va == c));
	CHECK(not (vb == c));

	CHECK(not (va != a));
	CHECK(    (va != b));
	CHECK(    (va != c));
	CHECK(    (vb != c));

	CHECK(    (va < b));
	CHECK(    (va < c));
	CHECK(    (vb < c));
	CHECK(not (vc < a));
	CHECK(not (vc < b));
	CHECK(not (vb < a));

	CHECK(    (vb > a));
	CHECK(    (vc > a));
	CHECK(    (vc > b));
	CHECK(not (va > c));
	CHECK(not (va > b));
	CHECK(not (vb > c));

	CHECK(    (va <= b));
	CHECK(    (va <= c));
	CHECK(    (vb <= c));
	CHECK(not (vc <= a));
	CHECK(not (vc <= b));
	CHECK(not (vb <= a));
	CHECK(    (va <= a));
	CHECK(    (vb <= b));
	CHECK(    (vc <= c));

	CHECK(    (vb >= a));
	CHECK(    (vc >= a));
	CHECK(    (vc >= b));
	CHECK(not (va >= c));
	CHECK(not (va >= b));
	CHECK(not (vb >= c));
	CHECK(    (va >= a));
	CHECK(    (vb >= b));
	CHECK(    (vc >= c));
}

TEST_CASE( "var_int<>: comparison operators - ( value_type, var_int<IntType> const&)"
         , "var_int<>")
{
	typedef shift::var_int<long> varint;

	const long a = 0, b = 1, c = 30000;
	const varint va(a), vb(b), vc(c);

	CHECK(    (a == va));
	CHECK(not (a == vb));
	CHECK(not (a == vc));
	CHECK(not (b == vc));

	CHECK(not (a != va));
	CHECK(    (a != vb));
	CHECK(    (a != vc));
	CHECK(    (b != vc));

	CHECK(    (a < vb));
	CHECK(    (a < vc));
	CHECK(    (b < vc));
	CHECK(not (c < va));
	CHECK(not (c < vb));
	CHECK(not (b < va));

	CHECK(    (b > va));
	CHECK(    (c > va));
	CHECK(    (c > vb));
	CHECK(not (a > vc));
	CHECK(not (a > vb));
	CHECK(not (b > vc));

	CHECK(    (a <= vb));
	CHECK(    (a <= vc));
	CHECK(    (b <= vc));
	CHECK(not (c <= va));
	CHECK(not (c <= vb));
	CHECK(not (b <= va));
	CHECK(    (a <= va));
	CHECK(    (b <= vb));
	CHECK(    (c <= vc));

	CHECK(    (b >= va));
	CHECK(    (c >= va));
	CHECK(    (c >= vb));
	CHECK(not (a >= vc));
	CHECK(not (a >= vb));
	CHECK(not (b >= vc));
	CHECK(    (a >= va));
	CHECK(    (b >= vb));
	CHECK(    (c >= vc));
}

#undef CHECK_COPY_CONSTRUCTOR
#undef CHECK_CONSTRUCTOR_TAKING_VALUE_TYPE_ARGUMENT
#undef CHECK_ASSIGNMENT_TO_VALUE_TYPE
#undef CHECK_ASSIGNMENT_TO_VARINT
#undef CHECK_SWAP
#undef CHECK_SET
#undef CHECK_GET
#undef CHECK_DEREFERENCE_OP
#undef CHECK_DEREFERENCE_OP_CONST(type, value)

}} // test
