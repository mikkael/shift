
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_DETAIL_TYPE_TRAITS_HPP_
#define SHIFT_DETAIL_TYPE_TRAITS_HPP_

namespace shift { namespace detail {

template<typename T> struct is_signed_integral;

template<> struct is_signed_integral<  signed char     > { static const int value = true ; };
template<> struct is_signed_integral<  signed short    > { static const int value = true ; };
template<> struct is_signed_integral<  signed int      > { static const int value = true ; };
template<> struct is_signed_integral<  signed long     > { static const int value = true ; };
template<> struct is_signed_integral<  signed long long> { static const int value = true ; };

template<> struct is_signed_integral<unsigned char     > { static const int value = false; };
template<> struct is_signed_integral<unsigned short    > { static const int value = false; };
template<> struct is_signed_integral<unsigned int      > { static const int value = false; };
template<> struct is_signed_integral<unsigned long     > { static const int value = false; };
template<> struct is_signed_integral<unsigned long long> { static const int value = false; };

template<typename T> struct is_unsigned_integral;

template<> struct is_unsigned_integral<  signed char     > { static const int value = false; };
template<> struct is_unsigned_integral<  signed short    > { static const int value = false; };
template<> struct is_unsigned_integral<  signed int      > { static const int value = false; };
template<> struct is_unsigned_integral<  signed long     > { static const int value = false; };
template<> struct is_unsigned_integral<  signed long long> { static const int value = false; };

template<> struct is_unsigned_integral<unsigned char     > { static const int value = true ; };
template<> struct is_unsigned_integral<unsigned short    > { static const int value = true ; };
template<> struct is_unsigned_integral<unsigned int      > { static const int value = true ; };
template<> struct is_unsigned_integral<unsigned long     > { static const int value = true ; };
template<> struct is_unsigned_integral<unsigned long long> { static const int value = true ; };

template<typename T> struct is_integral;

template<> struct is_integral<  signed char     > { static const int value = true; };
template<> struct is_integral<  signed short    > { static const int value = true; };
template<> struct is_integral<  signed int      > { static const int value = true; };
template<> struct is_integral<  signed long     > { static const int value = true; };
template<> struct is_integral<  signed long long> { static const int value = true; };

template<> struct is_integral<unsigned char     > { static const int value = true; };
template<> struct is_integral<unsigned short    > { static const int value = true; };
template<> struct is_integral<unsigned int      > { static const int value = true; };
template<> struct is_integral<unsigned long     > { static const int value = true; };
template<> struct is_integral<unsigned long long> { static const int value = true; };

}} // shift::detail

#endif /* SHIFT_DETAIL_TYPE_TRAITS_HPP_ */
