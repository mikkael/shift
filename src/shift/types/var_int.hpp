
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_TYPES_VAR_INT_HPP_
#define SHIFT_TYPES_VAR_INT_HPP_

#include <utility>

#include <shift/detail/static_assert.hpp>
#include <shift/detail/type_traits.hpp>

namespace shift {

template<typename IntType>
class var_int {
public:
	typedef IntType value_type;

	SHIFT_STATIC_ASSERT( shift::detail::is_integral<value_type>::value
	                   , the_provided_type_must_be_an_integral_type);

	         var_int(                 ) : value(0      ) {}
	         var_int(const var_int&  v) : value(v.value) {}
	explicit var_int(IntType v        ) : value(v      ) {}

	var_int& operator=(value_type v        )   { set(v     ); return *this; }
	var_int& operator=(const var_int& other)   { set(*other); return *this; }

	var_int& swap(var_int& other)              { std::swap(value, other.value); return *this; }

	void       set(value_type v)               { std::swap(value, v); }
	value_type get() const                     { return value; }

	inline       value_type& operator*()       { return value; }
	inline const value_type& operator*() const { return value; }

private:
	IntType value;
};

template<typename IntType> inline bool operator == ( var_int<IntType> const& x, var_int<IntType> const& y ) { return *x == *y; }
template<typename IntType> inline bool operator != ( var_int<IntType> const& x, var_int<IntType> const& y ) { return *x != *y; }

template<typename IntType> inline bool operator <  ( var_int<IntType> const& x, var_int<IntType> const& y ) { return *x < *y; }
template<typename IntType> inline bool operator >  ( var_int<IntType> const& x, var_int<IntType> const& y ) { return *x > *y; }

template<typename IntType> inline bool operator <= ( var_int<IntType> const& x, var_int<IntType> const& y ) { return *x <= *y; }
template<typename IntType> inline bool operator >= ( var_int<IntType> const& x, var_int<IntType> const& y ) { return *x >= *y; }

//

template<typename IntType, typename T> inline bool operator == ( var_int<IntType> const& x, T y ) { return *x == y; }
template<typename IntType, typename T> inline bool operator != ( var_int<IntType> const& x, T y ) { return *x != y; }

template<typename IntType, typename T> inline bool operator <  ( var_int<IntType> const& x, T y ) { return *x < y; }
template<typename IntType, typename T> inline bool operator >  ( var_int<IntType> const& x, T y ) { return *x > y; }

template<typename IntType, typename T> inline bool operator <= ( var_int<IntType> const& x, T y ) { return *x <= y; }
template<typename IntType, typename T> inline bool operator >= ( var_int<IntType> const& x, T y ) { return *x >= y; }

//

template<typename T, typename IntType> inline bool operator == ( T x, var_int<IntType> const& y ) { return x == *y; }
template<typename T, typename IntType> inline bool operator != ( T x, var_int<IntType> const& y ) { return x != *y; }

template<typename T, typename IntType> inline bool operator <  ( T x, var_int<IntType> const& y ) { return x < *y; }
template<typename T, typename IntType> inline bool operator >  ( T x, var_int<IntType> const& y ) { return x > *y; }

template<typename T, typename IntType> inline bool operator <= ( T x, var_int<IntType> const& y ) { return x <= *y; }
template<typename T, typename IntType> inline bool operator >= ( T x, var_int<IntType> const& y ) { return x >= *y; }

}

#endif /* SHIFT_VAR_INT_HPP_ */
