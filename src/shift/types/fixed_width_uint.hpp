
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_TYPES_FIXED_WIDTH_UINT_HPP_
#define SHIFT_TYPES_FIXED_WIDTH_UINT_HPP_

#include <utility>
#include <climits>

#include <shift/types/cstdint.hpp>
#include <shift/detail/bit_mask.hpp>
#include <shift/detail/type_traits.hpp>
#include <shift/detail/static_assert.hpp>

namespace shift {

template<typename IntType, unsigned int SizeBits>
class fixed_width_uint {
public:
	typedef IntType value_type;
	static const unsigned int num_bits = SizeBits;
	static const value_type   mask     = detail::bit_mask_all_bits<SizeBits, value_type>::value;

	SHIFT_STATIC_ASSERT( shift::detail::is_unsigned_integral<value_type>::value
	                   , the_provided_type_must_be_unsigned_integral);

	SHIFT_STATIC_ASSERT( (SizeBits > 0)
	                   , the_number_of_bits_must_not_be_zero);

	SHIFT_STATIC_ASSERT( (sizeof(value_type) * CHAR_BIT >= SizeBits)
	                   , the_provided_type_must_be_large_enough_to_hold_the_requested_number_of_bits);

	         fixed_width_uint(                             ) : value(0          ) {}
	         fixed_width_uint(const fixed_width_uint& other) : value(other.value) {}
	explicit fixed_width_uint(value_type v                 ) : value(v & mask   ) {}

	fixed_width_uint& operator=(value_type v                 ) { set(v     ); return *this; }
	fixed_width_uint& operator=(const fixed_width_uint& other) { set(*other); return *this; }

	fixed_width_uint& swap(fixed_width_uint& other)            { std::swap(value, other.value); return *this; }

	void       set(value_type v)                               { value = v & mask; }
	value_type get() const                                     { return value; }

	inline const value_type& operator*() const                 { return value; }

private:
	value_type value;
};

//

template<typename IntType, unsigned int SizeBits> inline bool operator == ( fixed_width_uint<IntType, SizeBits> const& x, fixed_width_uint<IntType, SizeBits> const& y ) { return *x == *y; }
template<typename IntType, unsigned int SizeBits> inline bool operator != ( fixed_width_uint<IntType, SizeBits> const& x, fixed_width_uint<IntType, SizeBits> const& y ) { return *x != *y; }

template<typename IntType, unsigned int SizeBits> inline bool operator <  ( fixed_width_uint<IntType, SizeBits> const& x, fixed_width_uint<IntType, SizeBits> const& y ) { return *x < *y; }
template<typename IntType, unsigned int SizeBits> inline bool operator >  ( fixed_width_uint<IntType, SizeBits> const& x, fixed_width_uint<IntType, SizeBits> const& y ) { return *x > *y; }

template<typename IntType, unsigned int SizeBits> inline bool operator <= ( fixed_width_uint<IntType, SizeBits> const& x, fixed_width_uint<IntType, SizeBits> const& y ) { return *x <= *y; }
template<typename IntType, unsigned int SizeBits> inline bool operator >= ( fixed_width_uint<IntType, SizeBits> const& x, fixed_width_uint<IntType, SizeBits> const& y ) { return *x >= *y; }

//

template<typename IntType, unsigned int SizeBits, typename IntegralType> inline bool operator == ( fixed_width_uint<IntType, SizeBits> const& x, IntegralType y ) { return *x == y; }
template<typename IntType, unsigned int SizeBits, typename IntegralType> inline bool operator != ( fixed_width_uint<IntType, SizeBits> const& x, IntegralType y ) { return *x != y; }

template<typename IntType, unsigned int SizeBits, typename IntegralType> inline bool operator <  ( fixed_width_uint<IntType, SizeBits> const& x, IntegralType y ) { return *x < y; }
template<typename IntType, unsigned int SizeBits, typename IntegralType> inline bool operator >  ( fixed_width_uint<IntType, SizeBits> const& x, IntegralType y ) { return *x > y; }

template<typename IntType, unsigned int SizeBits, typename IntegralType> inline bool operator <= ( fixed_width_uint<IntType, SizeBits> const& x, IntegralType y ) { return *x <= y; }
template<typename IntType, unsigned int SizeBits, typename IntegralType> inline bool operator >= ( fixed_width_uint<IntType, SizeBits> const& x, IntegralType y ) { return *x >= y; }

//

template<typename IntegralType, typename IntType, unsigned int SizeBits> inline bool operator == ( IntegralType x, fixed_width_uint<IntType, SizeBits> const& y ) { return x == *y; }
template<typename IntegralType, typename IntType, unsigned int SizeBits> inline bool operator != ( IntegralType x, fixed_width_uint<IntType, SizeBits> const& y ) { return x != *y; }

template<typename IntegralType, typename IntType, unsigned int SizeBits> inline bool operator <  ( IntegralType x, fixed_width_uint<IntType, SizeBits> const& y ) { return x < *y; }
template<typename IntegralType, typename IntType, unsigned int SizeBits> inline bool operator >  ( IntegralType x, fixed_width_uint<IntType, SizeBits> const& y ) { return x > *y; }

template<typename IntegralType, typename IntType, unsigned int SizeBits> inline bool operator <= ( IntegralType x, fixed_width_uint<IntType, SizeBits> const& y ) { return x <= *y; }
template<typename IntegralType, typename IntType, unsigned int SizeBits> inline bool operator >= ( IntegralType x, fixed_width_uint<IntType, SizeBits> const& y ) { return x >= *y; }

//

typedef fixed_width_uint<shift::uint8_t ,  1> uint1_t ;
typedef fixed_width_uint<shift::uint8_t ,  2> uint2_t ;
typedef fixed_width_uint<shift::uint8_t ,  3> uint3_t ;
typedef fixed_width_uint<shift::uint8_t ,  4> uint4_t ;
typedef fixed_width_uint<shift::uint8_t ,  5> uint5_t ;
typedef fixed_width_uint<shift::uint8_t ,  6> uint6_t ;
typedef fixed_width_uint<shift::uint8_t ,  7> uint7_t ;

typedef fixed_width_uint<shift::uint16_t,  9> uint9_t ;
typedef fixed_width_uint<shift::uint16_t, 10> uint10_t;
typedef fixed_width_uint<shift::uint16_t, 11> uint11_t;
typedef fixed_width_uint<shift::uint16_t, 12> uint12_t;
typedef fixed_width_uint<shift::uint16_t, 13> uint13_t;
typedef fixed_width_uint<shift::uint16_t, 14> uint14_t;
typedef fixed_width_uint<shift::uint16_t, 15> uint15_t;

} // shift

#endif /* SHIFT_TYPES_FIXED_WIDTH_UINT_HPP_ */
