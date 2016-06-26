
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_UTILITY_CONST_REF_HPP_
#define SHIFT_UTILITY_CONST_REF_HPP_

namespace shift {

template<typename T> const T& const_ref(      T& v) { return static_cast<const T&>(v); }
template<typename T> const T& const_ref(const T& v) { return v; }

}

#endif /* SHIFT_UTILITY_CONST_REF_HPP_ */
