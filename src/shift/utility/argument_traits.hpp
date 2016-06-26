
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_UTILITY_ARGUMENT_TRAITS_HPP_
#define SHIFT_UTILITY_ARGUMENT_TRAITS_HPP_

#include <shift/concepts/modes.hpp>

namespace shift {

template<typename Mode, typename ArgumentType>
struct argument_traits;

template<typename ArgumentType>
struct argument_traits<output, ArgumentType> { typedef const ArgumentType& type; };

template<typename ArgumentType>
struct argument_traits<input, ArgumentType>  { typedef       ArgumentType& type; };

template<typename Mode, typename ArgumentType>
typename argument_traits<Mode, ArgumentType>::type ref(ArgumentType& r) {
	typedef typename argument_traits<Mode, ArgumentType>::type T;
	return static_cast<T>(r);
}

}

#endif /* SHIFT_UTILITY_ARGUMENT_TRAITS_HPP_ */
